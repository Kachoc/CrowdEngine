#ifndef CRWD_STANDARDSHADERS_H_INCLUDED
#define CRWD_STANDARDSHADERS_H_INCLUDED

#include "ShadersLibrary.h"
#include "CEString.h"
#include "CRWDMath.h"
#include "Camera.h"
#include "Textures.h"

#include <map>

namespace Crowd
{

//
//	CustomShader
//
//	Permet la création de shaders personnalisés, non intégrés à la bibliothèque standard
//
class CustomShader : public Shader
{
	public:
	typedef SmartPtr<CustomShader, IPol> ptr;

	CRWD_EXPORT virtual void SetMatrix(const String &name, const Matrix &matrix);
	CRWD_EXPORT virtual void SetColor(const String &name, const Color &color);
	CRWD_EXPORT virtual void SetResource(const String &name, Resource::ptr resource);

	CRWD_EXPORT virtual Matrix GetMatrix(const String &name);
	CRWD_EXPORT virtual Color GetColor(const String &name);
	CRWD_EXPORT virtual Resource::ptr GetResource(const String &name);

	protected:
	std::map<String, Matrix>			m_matrices;
	std::map<String, Color>				m_colors;
	std::map<String, Resource::ptr>		m_resources;
};


//
//	LambertShader
//
//	Shader de rendu classique.
//
class LambertShader : public MeshShader
{
	public:
	typedef SmartPtr<LambertShader, IPol> ptr;

	CRWD_EXPORT LambertShader();

	CRWD_EXPORT static LambertShader *Create(const Color &diffuse, Texture::ptr diffuseSpecularTexture);

	struct CreationInfo
	{
		Color			Diffuse;
		Texture::ptr	DiffuseSpecularTexture;
	};

	CRWD_EXPORT virtual void SetMaterialDiffuse(const Color &diffuse); // RGB et Specular
	CRWD_EXPORT virtual void SetMaterialDiffSpecTexture(Texture::ptr texture);

	protected:
	Color			m_materialDiffuse;
	Texture::ptr	m_diffuseSpecularTexture;
};


//
//	DepthShader
//
//	Pour la création de shadow maps.
//
class DepthShader : public MeshShader
{
	public:
	typedef SmartPtr<DepthShader, IPol> ptr;

	CRWD_EXPORT static DepthShader *Create();

	DepthShader() {}

};

} // namespace Crowd

#endif // CRWD_STANDARDSHADERS_H_INCLUDED