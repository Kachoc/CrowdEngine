#include "D3D10ShaderFile.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr g_d3dDevice;


//
//	ConstantBuffer
//
ConstantBuffer::ConstantBuffer(BufferPtr buffer, EffectConstantBuffer *bufferVar)
{
	m_buffer = buffer;
	m_bufferVar = bufferVar;

	D3D10_BUFFER_DESC desc;
	m_buffer->GetDesc(&desc);
	m_size = desc.ByteWidth;
}

ConstantBuffer::~ConstantBuffer()
{

}

Status ConstantBuffer::SetData(const void *data)
{
	if(data)
	{
		char *mappedData = NULL;
		HRESULT hr = m_buffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**) &mappedData);

		char *cData = (char*) data;
		for(unsigned int i = 0; i < m_size; i++)
		{
			mappedData[i] = cData[i];
		}

		m_buffer->Unmap();
	}

	HRESULT hr = m_bufferVar->SetConstantBuffer(m_buffer);
	if(FAILED(hr)) {
		return Status(String("SetConstantBufferData : ") + GetResourceName() + String(" : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}


//
//	ShaderFile
//
ShaderFile::ShaderFile(EffectPtr effect, EffectPoolPtr effectPool)
{
	m_effect = effect;
	m_effectPool = effectPool;
}

ShaderFile::~ShaderFile()
{

}

EffectPtr ShaderFile::GetEffect()
{
	return m_effect;
}

EffectPoolPtr ShaderFile::GetEffectPool()
{
	return m_effectPool;
}

ConstantBuffer::ptr ShaderFile::CreateConstantBuffer(const String &constantBufferName, unsigned int sizeInBytes, Status *status)
{
	EffectConstantBuffer *cBufferVar = m_effect->GetConstantBufferByName(constantBufferName.asChar());

	if(!cBufferVar->IsValid()) {
		if(status) { *status = Status(String("ShaderFile : ") + GetResourceName() + String(" : ") + constantBufferName + String(" n'est pas un constant buffer : ")); }
		return NULL;
	}

	BufferPtr buffer;
	D3D10_BUFFER_DESC objectBufferDesc;
	objectBufferDesc.ByteWidth = sizeInBytes;
	objectBufferDesc.Usage = D3D10_USAGE_DYNAMIC;
	objectBufferDesc.BindFlags = D3D10_BIND_CONSTANT_BUFFER;
	objectBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	objectBufferDesc.MiscFlags = 0;

	HRESULT hr = g_d3dDevice->CreateBuffer(&objectBufferDesc, NULL, buffer.GetPtrP());
	if(FAILED(hr)) {
		if(status) { *status = Status(String("ShaderFile : ") + GetResourceName() + String(" : Création du ConstantBuffer : ") + constantBufferName + String(" : ") + GetD3DXErrorString(hr)); }
		return NULL;
	}

	ConstantBuffer::ptr nConstantBuffer = new ConstantBuffer(buffer, cBufferVar);
	nConstantBuffer->SetResourceName(constantBufferName);
	return nConstantBuffer;
}

EffectMatrixVariable *ShaderFile::GetMatrixVariable(const String &name)
{
	EffectMatrixVariable *var = m_effect->GetVariableByName(name.asChar())->AsMatrix();
	if(!var->IsValid()) {
		throw Exception(String("ShaderFile : ") + GetResourceName() + String(" : GetMatrixVariable : ") + name);
	}
	return var;
}

EffectScalarVariable *ShaderFile::GetScalarVariable(const String &name)
{
	EffectScalarVariable *var = m_effect->GetVariableByName(name.asChar())->AsScalar();
	if(!var->IsValid()) {
		throw Exception(String("ShaderFile : ") + GetResourceName() + String(" : GetScalarVariable : ") + name);
	}
	return var;
}

EffectShaderVariable *ShaderFile::GetShaderVariable(const String &name)
{
	EffectShaderVariable *var = m_effect->GetVariableByName(name.asChar())->AsShader();
	if(!var->IsValid()) {
		throw Exception(String("ShaderFile : ") + GetResourceName() + String(" : GetShaderVariable : ") + name);
	}
	return var;
}

EffectVectorVariable *ShaderFile::GetVectorVariable(const String &name)
{
	EffectVectorVariable *var = m_effect->GetVariableByName(name.asChar())->AsVector();
	if(!var->IsValid()) {
		throw Exception(String("ShaderFile : ") + GetResourceName() + String(" : GetVectorVariable : ") + name);
	}
	return var;
}

EffectTechnique *ShaderFile::GetTechnique(const String &name)
{
	EffectTechnique *tech = m_effect->GetTechniqueByName(name.asChar());
	if(!tech->IsValid()) {
		throw Exception(String("ShaderFile : ") + GetResourceName() + String(" : GetTechnique : ") + name);
	}
	return tech;
}

EffectShaderResourceVariable *ShaderFile::GetShaderResourceVariable(const String &name)
{
	EffectShaderResourceVariable *var = m_effect->GetVariableByName(name.asChar())->AsShaderResource();
	if(!var->IsValid()) {
		throw Exception(String("ShaderFile : ") + GetResourceName() + String(" : GetShaderResourceVariable : ") + name);
	}
	return var;
}

ShaderFile *ShaderFile::Load(const String fileName, bool isParent, Crowd::ShaderFile *parent, Status *status)
{
	EffectPtr			effect;
	EffectPoolPtr		effectPool;
	UndefBufferPtr		errors;
	UINT fxFlags = 0;
	HRESULT hr;

	String absFileName = ShaderLibrary::GetShadersPath() + fileName;

	if(parent) {
		EffectPoolPtr		parentEffectPool;
		parentEffectPool = ((D3D10::ShaderFile*) parent)->GetEffectPool();
		fxFlags |= D3D10_EFFECT_COMPILE_CHILD_EFFECT;

		#if defined(_DEBUG) | defined(DEBUG)
		hr = D3DX10CreateEffectFromFile(absFileName.asChar(), NULL, NULL, "fx_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_ENABLE_STRICTNESS,
													fxFlags, g_d3dDevice, parentEffectPool, NULL, effect.GetPtrP(), errors.GetPtrP(), NULL);
		#else
		hr = D3DX10CreateEffectFromFile(absFileName.asChar(), NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS,
													fxFlags, g_d3dDevice, parentEffectPool, NULL, effect.GetPtrP(), errors.GetPtrP(), NULL);
		#endif

		if(FAILED(hr)) {
			if(status) {  if(errors) { *status = Status((char*) errors->GetBufferPointer()); } else { *status = Status::Failure; } }
		}
	}
	else if(isParent)
	{
		#if defined(_DEBUG) | defined(DEBUG)
		hr = D3DX10CreateEffectPoolFromFile(absFileName.asChar(), NULL, NULL, "fx_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_ENABLE_STRICTNESS,
										    fxFlags, g_d3dDevice, NULL, effectPool.GetPtrP(), errors.GetPtrP(), NULL);
		#else
		hr = D3DX10CreateEffectPoolFromFile(absFileName.asChar(), NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS,
										    fxFlags, g_d3dDevice, NULL, effectPool.GetPtrP(), errors.GetPtrP(), NULL);
		#endif
	
		if(FAILED(hr)) {
			if(status) {  if(errors) { *status = Status((char*) errors->GetBufferPointer()); } else { *status = Status::Failure; } }
		}
		else {
			effect = effectPool->AsEffect();
			effect->AddRef();
		}
	}
	else
	{
		#if defined(_DEBUG) | defined(DEBUG)
		hr = D3DX10CreateEffectFromFile(absFileName.asChar(), NULL, NULL, "fx_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_ENABLE_STRICTNESS,
													0, g_d3dDevice, NULL, NULL, effect.GetPtrP(), errors.GetPtrP(), NULL);
		#else
		hr = D3DX10CreateEffectFromFile(absFileName.asChar(), NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS,
													0, g_d3dDevice, NULL, NULL, effect.GetPtrP(), errors.GetPtrP(), NULL);
		#endif

		if(FAILED(hr)) {
			if(status) {  if(errors) { *status = Status((char*) errors->GetBufferPointer()); } else { *status = Status::Failure; } }
		}
	}
	
	if(FAILED(hr))
	{
		return NULL;
	}
	else
	{
		ShaderFile *shaderFile = new ShaderFile(effect, effectPool);
		shaderFile->SetResourceName(fileName);
		return shaderFile;
	}
}

} // namespace D3D10

} // namespace Crowd