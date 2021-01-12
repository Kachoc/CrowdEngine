#include "D3D10Shaders.h"
#include "D3D10ShaderFile.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;


//
//	LambertShader
//


ShaderFile::ptr			LambertShader::ms_shaderFile(NULL);
InputLayoutPtr			LambertShader::ms_vertexLayout(NULL);

Status LambertShader::Load(ShaderFile::ptr effect)
{
	ms_shaderFile = effect;

	D3D10_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};

	unsigned int numElements = 3;

	EffectTechniquePtr lambertTech = ms_shaderFile->GetTechnique("LambertTech");
	D3D10_PASS_DESC passDesc;
	lambertTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = g_d3dDevice->CreateInputLayout(vertexLayoutDesc, numElements, passDesc.pIAInputSignature,
								passDesc.IAInputSignatureSize, ms_vertexLayout.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("D3D10::LambertShader::Load : ") + GetD3DXErrorString(hr));
	}

	//ms_effect->CreateConstantBuffer("Object", sizeof(LambertShader::Data));

	return Status::Success;
}

Status LambertShader::Unload()
{
	ms_shaderFile = NULL;
	ms_vertexLayout = NULL;

	return Status::Success;
}

LambertShader::LambertShader(const Color &diffuse, Crowd::Texture::ptr diffuseSpecularTexture)
{
	m_materialDiffuse = diffuse;
	m_diffuseSpecularTexture  = diffuseSpecularTexture;

	mv_lambertTech = ms_shaderFile->GetTechnique("LambertTech");
	mv_objectCBuffer = ms_shaderFile->CreateConstantBuffer("Object", sizeof(LambertShader::Data));
	mv_diffuseSpecularTexVar = ms_shaderFile->GetShaderResourceVariable("g_DiffuseSpecularTexture");
}

LambertShader::~LambertShader()
{

}

Status LambertShader::Apply(unsigned int pass)
{
	Data data;
	Matrix::Transpose(data.PositionMatrix, m_positionMatrix);
	data.Diffuse = m_materialDiffuse;
	mv_objectCBuffer->SetData(&data);

	if(m_diffuseSpecularTexture) {
		mv_diffuseSpecularTexVar->SetResource(((Texture::ptr&)m_diffuseSpecularTexture)->GetShaderResourceView());
	}
	
	g_d3dDevice->IASetInputLayout(ms_vertexLayout);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_d3dDevice->RSSetState(NULL);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	HRESULT hr = mv_lambertTech->GetPassByIndex(0)->Apply(0);

	return Status::Success;
}


//
//	DepthShader
//
ShaderFile::ptr		DepthShader::m_shaderFile(NULL);
InputLayoutPtr			DepthShader::m_vertexLayout(NULL);

DepthShader::DepthShader()
{
	m_depthTech = m_shaderFile->GetTechnique("ComputeShadowMap");

	Status status;
	m_objectCBuffer = m_shaderFile->CreateConstantBuffer("Object", sizeof(DepthShader::Data), &status);
	if(!status) {
		throw Exception(String("LambertShader::Apply() : ") + status.asString());
	}
}

DepthShader::~DepthShader()
{

}

DepthShader::ptr DepthShader::Create(const Resource::CreationInfo *info)
{
	return new DepthShader;
}

Status DepthShader::Load(ShaderFile::ptr shaderFile)
{
	m_shaderFile = shaderFile;

	D3D10_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};
	/*D3D10_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
	};*/

	unsigned int numElements = 1;

	EffectTechniquePtr depthTech = m_shaderFile->GetTechnique("ComputeShadowMap");
	D3D10_PASS_DESC passDesc;
	depthTech->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = g_d3dDevice->CreateInputLayout(vertexLayoutDesc, numElements, passDesc.pIAInputSignature,
								passDesc.IAInputSignatureSize, m_vertexLayout.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("D3D10::DepthShader::Load : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}

Status DepthShader::Unload()
{
	m_shaderFile = NULL;
	m_vertexLayout = NULL;

	return Status::Success;
}

Status DepthShader::Apply(unsigned int pass)
{
	Status status;

	Data data;
	Matrix::Transpose(data.PositionMatrix, m_positionMatrix);
	status = m_objectCBuffer->SetData(&data);
	if(!status) {
		throw Exception(String("LambertShader::Apply() : ") + status.asString());
	}
	
	g_d3dDevice->IASetInputLayout(m_vertexLayout);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_d3dDevice->RSSetState(NULL);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	m_depthTech->GetPassByIndex(0)->Apply(0);

	return Status::Success;
}

} // namespace D3D10

} // namespace Crowd