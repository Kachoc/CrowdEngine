#ifndef CRWD_D3D10_SHADERS_H_INCLUDED
#define CRWD_D3D10_SHADERS_H_INCLUDED

#include "D3DEntry.h"
#include "StandardShaders.h"

#include "D3D10ShaderFile.h"

#include "D3D10Textures.h"

namespace Crowd
{

namespace D3D10
{

//
//	LambertShader
//
class LambertShader : public Crowd::LambertShader
{
	public:
	LambertShader(const Color &diffuse, Crowd::Texture::ptr diffuseSpecularTexture);
	virtual ~LambertShader();

	static LambertShader::ptr Create(const Resource::CreationInfo *info);
	static Status Load(ShaderFile::ptr shader);
	static Status Unload();

	virtual Status Apply(unsigned int pass);

	struct Data
	{
		Matrix		PositionMatrix;
		Color		Diffuse;
	};

	protected:
	static ShaderFile::ptr				ms_shaderFile;
	static InputLayoutPtr				ms_vertexLayout;

	EffectTechniquePtr					mv_lambertTech;

	ConstantBuffer::ptr					mv_objectCBuffer;

	EffectShaderResourceVariablePtr		mv_diffuseSpecularTexVar;
};


//
//	DepthShader
//
class DepthShader : public Crowd::DepthShader
{
	public:
	DepthShader();
	virtual ~DepthShader();

	static DepthShader::ptr Create(const Resource::CreationInfo *info);
	static Status Load(ShaderFile::ptr shader);
	static Status Unload();

	virtual Status Apply(unsigned int pass);

	struct Data
	{
		Matrix		PositionMatrix;
	};

	protected:
	static ShaderFile::ptr			m_shaderFile;
	static InputLayoutPtr			m_vertexLayout;

	ConstantBuffer::ptr				m_objectCBuffer;

	EffectTechniquePtr				m_depthTech;
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_SHADERS_H_INCLUDED