#ifndef CRWD_D3D10_SHADER_FILE_H_INCLUDED
#define CRWD_D3D10_SHADER_FILE_H_INCLUDED

#include "D3DEntry.h"
#include "Resources.h"
#include "ShadersLibrary.h"

namespace Crowd
{

namespace D3D10
{

class ConstantBuffer : public Crowd::Resource
{
	public:
	typedef SmartPtr<ConstantBuffer, IPol> ptr;

	ConstantBuffer(BufferPtr buffer, EffectConstantBuffer *bufferVar);
	~ConstantBuffer();

	Status SetData(const void *data);

	protected:
	BufferPtr				m_buffer;
	EffectConstantBuffer	*m_bufferVar;
	unsigned int			m_size;
};

class ShaderFile : public Crowd::ShaderFile
{
	public:
	typedef SmartPtr<ShaderFile, IPol> ptr;

	virtual EffectPtr		GetEffect();
	virtual EffectPoolPtr	GetEffectPool();

	static ShaderFile *ShaderFile::Load(const String fileName, bool isParent, Crowd::ShaderFile *parent, Status *status = NULL);

	// Crée un buffer permettant d'accéder à un constant buffer de l'objet Effect
	// constantBufferName : nom du constant buffer
	// sizeInBytes : taille du constant buffer
	ConstantBuffer::ptr CreateConstantBuffer(const String &constantBufferName, unsigned int sizeInBytes, Status *status = NULL);

	EffectMatrixVariable *GetMatrixVariable(const String &name);
	EffectScalarVariable *GetScalarVariable(const String &name);
	EffectShaderVariable *GetShaderVariable(const String &name);
	EffectVectorVariable *GetVectorVariable(const String &name);
	EffectTechnique *GetTechnique(const String &name);
	EffectShaderResourceVariable *GetShaderResourceVariable(const String &name);

	protected:
	ShaderFile(EffectPtr effect, EffectPoolPtr effectPool);
	virtual ~ShaderFile();

	protected:
	EffectPtr			m_effect;
	EffectPoolPtr		m_effectPool;

	std::map<String, ConstantBuffer>		m_constantBuffers;
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_SHADER_FILE_H_INCLUDED