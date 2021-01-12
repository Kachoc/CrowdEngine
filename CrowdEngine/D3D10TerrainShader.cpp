#include "D3D10TerrainShader.h"
#include "D3D10Renderer.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;


//
//	TerrainGeometryShader
//


ShaderFile::ptr			TerrainGeometryShader::ms_shaderFile(NULL);
InputLayoutPtr			TerrainGeometryShader::ms_vertexLayout(NULL);

Status TerrainGeometryShader::Load(ShaderFile::ptr effect)
{
	ms_shaderFile = effect;

	D3D10_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	unsigned int numElements = 1;

	EffectTechniquePtr terrainTech = ms_shaderFile->GetTechnique("TerrainTech");
	D3D10_PASS_DESC passDesc;
	terrainTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = g_d3dDevice->CreateInputLayout(vertexLayoutDesc, numElements, passDesc.pIAInputSignature,
								passDesc.IAInputSignatureSize, ms_vertexLayout.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("D3D10::TerrainGeometryShader::Load : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}

Status TerrainGeometryShader::Unload()
{
	ms_shaderFile = NULL;
	ms_vertexLayout = NULL;

	return Status::Success;
}

TerrainGeometryShader::TerrainGeometryShader(const Parameters &param) : Crowd::TerrainGeometryShader(param)
{
	Status status;

	mv_terrainTech = ms_shaderFile->GetTechnique("TerrainTech");
	mv_objectCBuffer = ms_shaderFile->CreateConstantBuffer("Terrain", sizeof(TerrainShaderData), &status);
	if(!status) {
		throw Exception("TerrainGeometryShader() : " + status.asString());
	}
	mv_heightMapVar = ms_shaderFile->GetShaderResourceVariable("g_HeightMap");
	mv_diffuseSpecVar = ms_shaderFile->GetShaderResourceVariable("g_DiffuseSpecularTexture");
}

TerrainGeometryShader::~TerrainGeometryShader()
{

}

Status TerrainGeometryShader::Apply(unsigned int pass)
{
	TerrainShaderData data;
	Matrix::Transpose(data.PositionMatrix, m_positionMatrix);
	data.Diffuse = m_parameters.Diffuse;
	data.HeightConstant = m_parameters.HeightConstant;
	data.HeightMultiplier = m_parameters.HeightMultiplier;
	data.SquareWidth = m_parameters.SquareWidth;
	data.SquareUVWidth = m_parameters.SquareUVWidth;
	mv_objectCBuffer->SetData(&data);

	if(m_parameters.HeightMap) {
		mv_heightMapVar->SetResource(((D3D10::Texture::ptr&)m_parameters.HeightMap)->GetShaderResourceView());
	}
	if(m_parameters.DiffuseSpecularTexture) {
		mv_diffuseSpecVar->SetResource(((D3D10::Texture::ptr&)m_parameters.DiffuseSpecularTexture)->GetShaderResourceView());
	}
	
	g_d3dDevice->IASetInputLayout(ms_vertexLayout);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_d3dDevice->RSSetState(NULL);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	HRESULT hr = mv_terrainTech->GetPassByIndex(0)->Apply(0);

	return Status::Success;
}




//
//	TerrainDepthShader
//


ShaderFile::ptr			TerrainDepthShader::ms_shaderFile(NULL);
InputLayoutPtr			TerrainDepthShader::ms_vertexLayout(NULL);

Status TerrainDepthShader::Load(ShaderFile::ptr terrainShadowFile)
{
	ms_shaderFile = terrainShadowFile;

	D3D10_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	unsigned int numElements = 1;

	EffectTechniquePtr terrainTech = ms_shaderFile->GetTechnique("TerrainDepthTech");
	D3D10_PASS_DESC passDesc;
	terrainTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = g_d3dDevice->CreateInputLayout(vertexLayoutDesc, numElements, passDesc.pIAInputSignature,
								passDesc.IAInputSignatureSize, ms_vertexLayout.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("D3D10::TerrainDepthShader::Load : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}

Status TerrainDepthShader::Unload()
{
	ms_shaderFile = NULL;
	ms_vertexLayout = NULL;

	return Status::Success;
}

TerrainDepthShader::TerrainDepthShader(const Parameters &param) : Crowd::TerrainDepthShader(param)
{
	Status status;

	mv_terrainTech = ms_shaderFile->GetTechnique("TerrainDepthTech");
	mv_objectCBuffer = ms_shaderFile->CreateConstantBuffer("Terrain", sizeof(TerrainShaderData), &status);
	if(!status) {
		throw Exception("TerrainDepthShader() : " + status.asString());
	}
	mv_heightMapVar = ms_shaderFile->GetShaderResourceVariable("g_HeightMap");
}

TerrainDepthShader::~TerrainDepthShader()
{

}

Status TerrainDepthShader::Apply(unsigned int pass)
{
	TerrainShaderData data;
	Matrix::Transpose(data.PositionMatrix, m_positionMatrix);
	data.Diffuse = m_parameters.Diffuse;
	data.HeightConstant = m_parameters.HeightConstant;
	data.HeightMultiplier = m_parameters.HeightMultiplier;
	data.SquareWidth = m_parameters.SquareWidth;
	data.SquareUVWidth = m_parameters.SquareUVWidth;
	mv_objectCBuffer->SetData(&data);

	if(m_parameters.HeightMap) {
		mv_heightMapVar->SetResource(((D3D10::Texture::ptr&)m_parameters.HeightMap)->GetShaderResourceView());
	}
	
	g_d3dDevice->IASetInputLayout(ms_vertexLayout);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_d3dDevice->RSSetState(NULL);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	HRESULT hr = mv_terrainTech->GetPassByIndex(0)->Apply(0);

	return Status::Success;
}

} // namespace Crowd

} // namespace D3D10