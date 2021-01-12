#include "D3D10ShadersLibrary.h"
#include "D3D10Renderer.h"
#include "D3D10PostProcess.h"
#include "D3D10Shaders.h"
#include "D3D10Passes.h"
#include "D3D10LightScene.h"

#include "D3D10TerrainShader.h"


namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

//
//	ShaderLibrary
//
ShaderLibrary::ShaderLibrary()
{
	
}

ShaderLibrary::~ShaderLibrary()
{
	//LambertShader::Unload();
	//TerrainGeometryShader::Unload();
	//TerrainDepthShader::Unload();
	//LightScene::Unload();
}

Status ShaderLibrary::LoadStandardShaders()
{
	ShaderFile::ptr file;
	Status status;
	HRESULT hr;

	//
	//	Global
	//	
	m_worldShader = ShaderFile::Load("World.fx", true, NULL, &status);
	if(!status) {
		return status;
	}
	m_worldEffect = m_worldShader->GetEffect();

	m_worldConstantBuffer = m_worldEffect->GetConstantBufferByName("World");

	D3D10_BUFFER_DESC objectBufferDesc;
	objectBufferDesc.ByteWidth = sizeof(WorldShaderInfo);
	objectBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	objectBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	objectBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	objectBufferDesc.MiscFlags = 0;

	hr = g_d3dDevice->CreateBuffer(&objectBufferDesc, NULL, m_worldBuffer.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("ShadersLibrary::LoadStandardShaders() : ") +GetD3DXErrorString(hr));
	}
	hr = m_worldConstantBuffer->SetConstantBuffer(m_worldBuffer);
	if(FAILED(hr)) {
		return Status(String("ShadersLibrary::LoadStandardShaders() : ") +GetD3DXErrorString(hr));
	}


	//
	//	LambertShader
	//
	m_standardShader = ShaderFile::Load("Standard.fx", false, m_worldShader, &status);
	if(!status) {
		return status;
	}

	LambertShader::Load(m_standardShader);

	//
	//	TerrainGeometryShader
	//
	file = ShaderFile::Load("..\\CrowdEngine\\Terrain.fx", false, m_worldShader, &status);
	if(!status) {
		return status;
	}

	TerrainGeometryShader::Load(file);

	//
	//	Light Scene
	//

	status = LightScene::Load(m_worldShader);
	if(!status) {
		return status;
	}

	//
	//	PostProcess
	//
	status = PostProcess::Load(m_worldShader);
	if(!status) {
		return status;
	}

	status = ((PostProcess&) PostProcess::GetInstance()).Initialize();
	if(!status) {
		return status;
	}

	return status;
}

void ShaderLibrary::SetWorldShaderInfo(const WorldShaderInfo &info)
{
	WorldShaderInfo *data = NULL;
	HRESULT hr = m_worldBuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &data);
	if(FAILED(hr)) {
		throw Exception(String("ShaderLibrary::SetWorldShaderInfo() : ") +GetD3DXErrorString(hr));
	}

	*data = info;

	m_worldBuffer->Unmap();
}

} // namespace D3D10

} // namespace Crowd