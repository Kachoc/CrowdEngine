#include "D3D10PointLight.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

ShaderFile::ptr		PointLight::m_pointLightShaderFile;
ShaderFile::ptr		PointLight::m_shadowMapShaderFile;

/*Interface *PointLight::Create(const Resource::CreationInfo *creationParam)
{
	return new PointLight;
}*/

Status PointLight::Load(ShaderFile::ptr lightPass, ShaderFile::ptr shadowMapFile)
{
	Status status;
	m_pointLightShaderFile = ShaderFile::Load("PointLight.fx", false, lightPass, &status);
	if(!status) {
		return status;
	}


	m_shadowMapShaderFile = shadowMapFile;

	return Status::Success;
}

Status PointLight::Unload()
{
	m_pointLightShaderFile = NULL;
	m_shadowMapShaderFile = NULL;

	return Status::Success;
}

PointLight::PointLight(const Vector4 &position, const Color &diffuseSpec) : Crowd::PointLight(position, diffuseSpec)
{
	Status status;
	m_renderPointLight = m_pointLightShaderFile->GetTechnique("RenderPointLight");
	m_renderShadowPointLight  = m_pointLightShaderFile->GetTechnique("RenderShadowPointLight");

	m_shadowMapsArrayResVar = m_pointLightShaderFile->GetShaderResourceVariable("PointLightShadowMapsArray");

	m_lightCBuffer = m_pointLightShaderFile->CreateConstantBuffer("PointLightBuffer", sizeof(PointLight::PointLightData));

	D3D10_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D10_BLEND_DESC));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.BlendEnable[0] = true;
	blendDesc.SrcBlend = D3D10_BLEND_ONE;
	blendDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.DestBlend = D3D10_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	blendDesc.BlendOp = D3D10_BLEND_OP_ADD;
	blendDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	blendDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	for(unsigned int i = 0; i < 8; i++) { blendDesc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL; }

	HRESULT hr = g_d3dDevice->CreateBlendState(&blendDesc, m_blendState.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("PointLight::PointLight() : ") +GetD3DXErrorString(hr));
	}

	D3D10_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.FillMode = D3D10_FILL_SOLID;
	rasterDesc.CullMode = D3D10_CULL_NONE;
	hr = g_d3dDevice->CreateRasterizerState(&rasterDesc, m_rasterizerState.GetPtrP());///////////////////////////////////////////
	if(FAILED(hr)) {
		throw Exception(String("PointLight::PointLight() : ") +GetD3DXErrorString(hr));
	}
}

void PointLight::Draw(const RenderInfo &info, const Camera &camera)
{
	g_d3dDevice->IASetInputLayout(NULL);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	PointLightData lightData;
	lightData.Position = m_position;
	lightData.DiffuseSpec = m_diffuseSpec;
	m_lightCBuffer->SetData(&lightData);

	g_d3dDevice->OMSetBlendState(m_blendState, NULL, 0xffffffff);

	if(m_shadowMapsDesc.Use)
	{
		m_shadowMapsArrayResVar->SetResource(m_shadowMapsArrayResView);

		ShadowPointLightData shadowData;
		for(unsigned int i = 0; i < 6; i++)
		{
			Matrix::Transpose(shadowData.ShadowViewProjMatrices[i], m_shadowMapsCameras[i].GetViewProj());
		}

		PerspectiveCamera::Parameters cameraParams;
		m_shadowMapsCameras[0].GetParameters(cameraParams);

		shadowData.ShadowNearPlane = cameraParams.nearZ;
		shadowData.ShadowFarPlane = cameraParams.farZ;

		m_shadowViewProjBuffer->SetData(&shadowData);

		m_renderShadowPointLight->GetPassByIndex(0)->Apply(0);
		g_d3dDevice->Draw(4, 0);
	}
	else
	{
		m_renderPointLight->GetPassByIndex(0)->Apply(0);
		g_d3dDevice->Draw(4, 0);
	}
}

Status PointLight::UseShadowMaps(unsigned int resolution, float projMinDist, float projMaxDist, unsigned int shadowMapSceneIndice, bool use)
{
	HRESULT hr;

	if(m_shadowMapsDesc.Resolution != resolution || use == false) // On détruit les buffers et autres objets
	{
		for(unsigned int i = 0; i < 6; i++) { m_shadowMapsConstantBuffer[i] = NULL; }
		m_shadowMapsArray = NULL;
		m_shadowMapsArrayResView = NULL;

		m_shadowDepthBuffer = NULL;
		m_shadowDepthBufferView = NULL;

		m_shadowViewProjBuffer = NULL;

		//m_shadowMapRenderTarget = NULL;
		//m_shadowMapRenderTargetView = NULL;

		if(use == false) {
			//m_shadowMapsDesc.ShadowMapSceneIndice = shadowMapSceneIndice;
			//m_shadowMapsDesc.ProjMinDist = projMinDist;
			//m_shadowMapsDesc.ProjMaxDist = projMaxDist;
			m_shadowMapsDesc.Use = use;

			for(unsigned int i = 0; i < 6; i++) { m_shadowMapsConstantBuffer[i] = NULL; }

			// ComputeProjectionMatrices();

			return Status::Success;
		}
	}
	else if(m_shadowMapsDesc.Resolution == resolution && m_shadowMapsDesc.Use == use) // Shadow maps déjà construites
	{
		m_shadowMapsDesc.ShadowMapSceneIndice = shadowMapSceneIndice;
		m_shadowMapsDesc.ProjMinDist = projMinDist;
		m_shadowMapsDesc.ProjMaxDist = projMaxDist;

		ComputeProjectionMatrices();
		ComputeViewMatrices();

		return Status::Success;
	}

	m_shadowMapsDesc.Resolution = resolution;
	m_shadowMapsDesc.ShadowMapSceneIndice = shadowMapSceneIndice;
	m_shadowMapsDesc.ProjMinDist = projMinDist;
	m_shadowMapsDesc.ProjMaxDist = projMaxDist;
	m_shadowMapsDesc.Use = use;

	ComputeProjectionMatrices();
	ComputeViewMatrices();

	Status status;
	// Constant buffer
	for(unsigned int i = 0; i < 6; i++)
	{
		m_shadowMapsConstantBuffer[i] = m_shadowMapShaderFile->CreateConstantBuffer("Shadow", sizeof(LightScene::ShadowMapInfo), &status);
		if(!status)
		{
			status.addFront("PointLight::UseShadowMaps : ");
			return status;
		}
	}
	m_shadowViewProjBuffer = m_pointLightShaderFile->CreateConstantBuffer("ShadowMaps", sizeof(ShadowPointLightData), &status);
	if(!status)
	{
		status.addFront("PointLight::UseShadowMaps : ");
		return status;
	}

	//
	//	Texture array
	//
	D3D10_TEXTURE2D_DESC desc;
	desc.Width = resolution;
	desc.Height = resolution;
	desc.MipLevels = 1;
	desc.ArraySize = 6; // Texture cube
	desc.Format = DXGI_FORMAT_R32_FLOAT;// DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE; // | D3D10_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_shadowMapsArray.GetPtrP());
	if(FAILED(hr)) throw Exception(String("PointLight::UseShadowMaps : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateShaderResourceView(m_shadowMapsArray, NULL, m_shadowMapsArrayResView.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("PointLight::UseShadowMaps : ") + GetD3DXErrorString(hr));
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

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_shadowDepthBuffer.GetPtrP());
	if(FAILED(hr)) throw Exception(String("LightScene::CreateSunShadowMaps : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateDepthStencilView(m_shadowDepthBuffer, NULL, m_shadowDepthBufferView.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("LightScene::CreateSunShadowMaps : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}

void PointLight::ComputeShadowMaps(const RenderInfo &info, const Camera &camera, std::vector<GeometryScene::ptr> &scene)
{
	if(!m_shadowMapsDesc.Use) {
		return;
	}

	D3D10_VIEWPORT viewport;
	viewport.Width = m_shadowMapsDesc.Resolution;
	viewport.Height = m_shadowMapsDesc.Resolution;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_d3dDevice->RSSetViewports(1, &viewport);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	for(unsigned int i = 0; i < 6; i++)
	{
		LightScene::ShadowMapInfo data;
		Matrix::Transpose(data.LightViewProjMatrix, m_shadowMapsCameras[i].GetViewProj());
		m_shadowMapsConstantBuffer[i]->SetData(&data);

		g_d3dDevice->ClearDepthStencilView(m_shadowDepthBufferView, D3D10_CLEAR_DEPTH, 1.0f, 0);

		g_d3dDevice->OMSetRenderTargets(0, NULL, m_shadowDepthBufferView);
		g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

		scene[m_shadowMapsDesc.ShadowMapSceneIndice]->Draw(info, m_shadowMapsCameras[i]);

		g_d3dDevice->CopySubresourceRegion(m_shadowMapsArray, D3D10CalcSubresource(0, i, 1), 0, 0, 0, m_shadowDepthBuffer, D3D10CalcSubresource(0, 0, 1), NULL);
	}
}

} // namespace D3D10

} // namespace Crowd