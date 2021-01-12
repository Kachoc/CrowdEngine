#include "D3D10LightScene.h"
#include "D3D10Shaders.h"
#include "D3D10Renderer.h"

#include "D3D10PointLight.h"
#include "D3D10TerrainShader.h"

#include "Camera.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

ShaderFile::ptr							LightScene::ms_lightPass;
ShaderFile::ptr							LightScene::ms_renderSun;
ShaderFile::ptr							LightScene::ms_shadowMap;
ShaderFile::ptr							LightScene::ms_terrainShadow;

ShaderFile::ptr	g_pointLightEffect;

/*Interface *LightScene::Create(const Resource::CreationInfo *creationParam)
{
	return new LightScene(((CreationInfo*)creationParam)->BoundingBox);
}*/

Status LightScene::Load(ShaderFile::ptr worldShader)
{
	ShaderFile::ptr shaderFile;
	EffectPtr effect;

	Status status;
	ms_lightPass = ShaderFile::Load("LightPass.fx", true, NULL, &status);
	if(!status) {
		return status;
	}

	ms_renderSun = ShaderFile::Load("RenderSun.fx", false, ms_lightPass, &status);
	if(!status) {
		return status;
	}
	SunPass::Load(ms_renderSun->GetEffect()); /////////

	//
	//	Shader depth
	//
	ms_shadowMap = ShaderFile::Load("ShadowMap.fx", false, ms_lightPass, &status);
	if(!status) {
		return status;
	}
	DepthShader::Load(ms_shadowMap);

	status = PointLight::Load(ms_lightPass, ms_shadowMap);
	if(!status) {
		return status;
	}

	//
	//	TerrainShadow
	//
	ms_terrainShadow = ShaderFile::Load("..\\CrowdEngine\\TerrainShadow.fx", false, ms_lightPass, &status);
	if(!status) {
		return status;
	}

	TerrainDepthShader::Load(ms_terrainShadow);

	return Status::Success;
}

Status LightScene::Unload()
{
	ms_lightPass = NULL;
	ms_renderSun = NULL;
	ms_shadowMap = NULL;
	ms_terrainShadow = NULL;

	TerrainDepthShader::Unload();
	DepthShader::Unload();
	PointLight::Unload();

	return Status::Success;
}

void LightScene::SetWorldShaderInfo(const WorldShaderInfo &info)
{
	WorldShaderInfo *data = NULL;
	HRESULT hr = m_worldBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &data);
	if(FAILED(hr)) {
		throw Exception(String("ShaderLibrary::SetWorldShaderInfo() : ") +GetD3DXErrorString(hr));
	}

	*data = info;

	m_worldBuffer->Unmap();
}

Status LightScene::CreateSunShadowMaps(unsigned int resolution, unsigned int numShadowMaps, const std::vector<float> &epsilons, const unsigned int *shadowScenesIndices)
{
	HRESULT hr;

	m_sunShadowMapEpsilons = epsilons;
	m_shadowMapsResolution = resolution;
	m_numShadowMaps = numShadowMaps;
	m_shadowMapsLinking.resize(numShadowMaps); // Le rendu de la shadowMap d'indice i se fait au moyen de la shadowScene d'indice m_shadowMapsLinking[i]
	if(shadowScenesIndices)
	{
		for(unsigned int i = 0; i < numShadowMaps; i++)
		{
			m_shadowMapsLinking[i] = shadowScenesIndices[i];
		}
	}
	else
	{
		for(unsigned int i = 0; i < numShadowMaps; i++)
		{
			m_shadowMapsLinking[i] = i;
		}
	}

	//
	//	Clear
	//
	m_shadowMapArray = NULL;
	ms_shadowMapArrayResView = NULL;

	ms_shadowDepthBuffer = NULL;
	ms_shadowDepthBufferView = NULL;

	//
	//	Texture array
	//
	D3D10_TEXTURE2D_DESC desc;
	desc.Width = resolution;
	desc.Height = resolution;
	desc.MipLevels = 1;
	desc.ArraySize = numShadowMaps;
	desc.Format = DXGI_FORMAT_R32_FLOAT;// DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE; // | D3D10_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_shadowMapArray.GetPtrP());
	if(FAILED(hr)) throw Exception(String("LightScene::CreateSunShadowMaps : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateShaderResourceView(m_shadowMapArray, NULL, ms_shadowMapArrayResView.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::CreateSunShadowMaps : ") + GetD3DXErrorString(hr));
	}

	//
	//	Depth buffer pour le rendu
	//
	desc.Width = resolution;
	desc.Height = resolution;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, ms_shadowDepthBuffer.GetPtrP());
	if(FAILED(hr)) throw Exception(String("LightScene::CreateSunShadowMaps : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateDepthStencilView(ms_shadowDepthBuffer, NULL, ms_shadowDepthBufferView.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::CreateSunShadowMaps : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}

void LightScene::RenderSunShadowMaps(const RenderInfo &info, const std::vector<OrthographicCamera> &cameras)
{
	D3D10_VIEWPORT viewport;
	viewport.Width = m_shadowMapsResolution;
	viewport.Height = m_shadowMapsResolution;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_d3dDevice->RSSetViewports(1, &viewport);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	HRESULT hr = ms_shadowConstantBuffer->SetConstantBuffer(ms_shadowBuffer);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::RenderSunShadowMap : ") +GetD3DXErrorString(hr));
	}

	for(unsigned int i = 0; i < m_numShadowMaps; i++)
	{
		ShadowMapInfo *data;
		hr = ms_shadowBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &data);
		if(FAILED(hr)) {
			throw Exception(String("LightScene::RenderSunShadowMap : ") +GetD3DXErrorString(hr));
		}

		OrthographicCamera::Parameters cameraParams;
		cameras[i].GetParameters(cameraParams);

		Matrix::Transpose(data->LightViewProjMatrix, cameras[i].GetViewProj());

		ms_shadowBuffer->Unmap();

		g_d3dDevice->ClearDepthStencilView(ms_shadowDepthBufferView, D3D10_CLEAR_DEPTH, 1.0f, 0);

		g_d3dDevice->OMSetRenderTargets(0, NULL, ms_shadowDepthBufferView);
		g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

		m_shadowScenes[m_shadowMapsLinking[i]]->Draw(info, cameras[i]);

		g_d3dDevice->CopySubresourceRegion(m_shadowMapArray, D3D10CalcSubresource(0, i, 1), 0, 0, 0, ms_shadowDepthBuffer, D3D10CalcSubresource(0, 0, 1), NULL);
	}
	m_sunCameras = cameras;
}
	
void LightScene::RenderSun(const RenderInfo &info, const Camera &camera)
{
	HRESULT hr;

	g_d3dDevice->IASetInputLayout(NULL);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// Actualisation des variables de shader
	hr = ms_diffuseBufferView->SetResource(info.diffuseResView);
			if(FAILED(hr)) { Logger::Error("LightScene::Process"); return; }
	hr = ms_normalBufferView->SetResource(info.normalResView);
			if(FAILED(hr)) { Logger::Error("LightScene::Process"); return; }
	hr = ms_depthBufferView->SetResource(info.depthResView);
			if(FAILED(hr)) { Logger::Error("LightScene::Process"); return; }

	hr = ms_shadowMapArrayResVar->SetResource(ms_shadowMapArrayResView);
			if(FAILED(hr)) { Logger::Error("LightScene::Process"); return; }
	
	SunShadowMapsInfo *sunShadowMapsData;
	hr = ms_shadowMapsBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &sunShadowMapsData);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::RenderSun() : ") +GetD3DXErrorString(hr));
	}
	for(unsigned int i = 0; i < m_numShadowMaps; i++)
	{
		Matrix::Transpose(sunShadowMapsData->ShadowMapViewProjMatrices[i], m_sunCameras[i].GetViewProj());
		//sunShadowMapsData->Epsilons[i] = m_sunShadowMapEpsilons[i];
	}
	sunShadowMapsData->NumShadowMapsLevels = m_numShadowMaps;
	sunShadowMapsData->Epsilon = m_sunShadowMapEpsilons[0]; ////

	ms_shadowMapsBuffer->Unmap();


	SunShaderInfo *data;
	hr = ms_sunBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &data);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::RenderSun() : ") +GetD3DXErrorString(hr));
	}

	data->SunDirection = Vector4(m_sunDirection, 0.0f);
	data->SunDiffuse = m_sunDiffuse;
	data->SunAmbient = m_sunAmbient;
	data->SunSpecular = m_sunSpecular;

	data->SkyColor = m_SkyColor;
	data->HorizonColor = m_HorizonColor;
	data->SkyBlendExponent = m_SkyBlendExponent;
	data->SunRadiusExponent = m_SunRadiusExponent;
	data->SunRadiusMultiplier = m_SunRadiusMultiplier;

	ms_sunBuffer->Unmap();

	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	ms_renderSunTech->GetPassByIndex(0)->Apply(0);
	g_d3dDevice->Draw(4, 0);
}

LightScene::LightScene(const AABB &boundingBox) : Crowd::LightScene(boundingBox)
{
	HRESULT hr;
	D3D10_BUFFER_DESC bufferDesc;
	
	ms_sun = ms_renderSun->GetEffect()->GetConstantBufferByName("Sun");

	bufferDesc.ByteWidth = sizeof(SunShaderInfo);
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	hr = g_d3dDevice->CreateBuffer(&bufferDesc, NULL, ms_sunBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}
	hr = ms_sun->SetConstantBuffer(ms_sunBuffer);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}
	
	ms_renderSunTech = ms_renderSun->GetTechnique("RenderSun");

	///
	EffectPtr lightPassEffect = ms_lightPass->GetEffect();

	ms_normalBufferView = ms_lightPass->GetShaderResourceVariable("NormalBuf");
	ms_diffuseBufferView = ms_lightPass->GetShaderResourceVariable("DiffuseBuf");
	ms_depthBufferView = ms_lightPass->GetShaderResourceVariable("DepthBuf");


	ms_world = lightPassEffect->GetConstantBufferByName("World");

	D3D10_BUFFER_DESC objectBufferDesc;
	objectBufferDesc.ByteWidth = sizeof(WorldShaderInfo);
	objectBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	objectBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	objectBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	objectBufferDesc.MiscFlags = 0;

	hr = g_d3dDevice->CreateBuffer(&objectBufferDesc, NULL, m_worldBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}
	hr = ms_world->SetConstantBuffer(m_worldBuffer);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}



	//
	//	Shadow map
	//
	ms_shadowConstantBuffer = ms_shadowMap->GetEffect()->GetConstantBufferByName("Shadow");

	bufferDesc.ByteWidth = sizeof(LightScene::ShadowMapInfo);
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	hr = g_d3dDevice->CreateBuffer(&bufferDesc, NULL, ms_shadowBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}
	hr = ms_shadowConstantBuffer->SetConstantBuffer(ms_shadowBuffer);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}

	ms_shadowMapsConstantBuffer = ms_renderSun->GetEffect()->GetConstantBufferByName("ShadowMaps");

	ms_shadowMapArrayResVar = ms_renderSun->GetShaderResourceVariable("SunShadowMapsArray");

	bufferDesc.ByteWidth = sizeof(SunShadowMapsInfo);
	bufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	hr = g_d3dDevice->CreateBuffer(&bufferDesc, NULL, ms_shadowMapsBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}
	hr = ms_shadowMapsConstantBuffer->SetConstantBuffer(ms_shadowMapsBuffer);
	if(FAILED(hr)) {
		throw Exception(String("LightScene::LightScene() : ") +GetD3DXErrorString(hr));
	}
}

LightScene::~LightScene()
{

}

} // namespace D3D10

} // namespace Crowd