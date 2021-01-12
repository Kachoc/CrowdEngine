#ifndef CRWD_D3D10_SHADERS_LIBRARY_H_INCLUDED
#define CRWD_D3D10_SHADERS_LIBRARY_H_INCLUDED

#include "D3DEntry.h"
#include "ShadersLibrary.h"

#include "D3D10ShaderFile.h"

namespace Crowd
{

namespace D3D10
{

class ShaderLibrary : public Crowd::ShaderLibrary
{
	friend class Crowd::ShaderLibrary;

	public:
	virtual Status LoadStandardShaders();

	void SetWorldShaderInfo(const WorldShaderInfo &info);

	protected:
	ShaderLibrary();
	virtual ~ShaderLibrary();

	protected:
	ShaderFile::ptr				m_standardShader;
	ShaderFile::ptr				m_worldShader; // Contient un cbuffer partagé avec les informations de caméra, position... Est utilisé par la majorité des shaders.
	EffectPtr					m_worldEffect;

	BufferPtr					m_worldBuffer;
	EffectConstantBufferPtr		m_worldConstantBuffer;
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_SHADERS_LIBRARY_H_INCLUDED