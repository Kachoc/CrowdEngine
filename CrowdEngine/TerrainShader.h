#ifndef CRWD_TERRAIN_SHADER_H_INCLUDED
#define CRWD_TERRAIN_SHADER_H_INCLUDED

#include "ShadersLibrary.h"
#include "CRWDMath.h"
#include "Camera.h"
#include "Textures.h"

namespace Crowd
{

//
//	LambertShader
//
//	Shader de rendu classique.
//
class TerrainShader : public MeshShader
{
	public:
	typedef SmartPtr<TerrainShader, IPol> ptr;

	struct Parameters
	{
		Color			Diffuse;
		Texture::ptr	HeightMap;
		Texture::ptr	DiffuseSpecularTexture;

		float			HeightConstant;
		float			HeightMultiplier;
		float			SquareWidth;
		float			SquareUVWidth;
		int				TerrainWidth;
	};

	CRWD_EXPORT TerrainShader(const Parameters &param);

	CRWD_EXPORT virtual void SetParameters(const Parameters &parameters);

	CRWD_EXPORT virtual void SetMaterialDiffuse(const Color &diffuse); // RGB et Specular
	CRWD_EXPORT virtual void SetHeightMap(Texture::ptr heightMap);

	protected:
	Parameters		m_parameters;
};

class TerrainGeometryShader : public TerrainShader
{
	public:
	typedef SmartPtr<TerrainShader, IPol> ptr;

	CRWD_EXPORT TerrainGeometryShader(const Parameters &param);

	CRWD_EXPORT static TerrainGeometryShader *Create(const Parameters &param);
};

class TerrainDepthShader : public TerrainShader
{
	public:
	typedef SmartPtr<TerrainShader, IPol> ptr;

	CRWD_EXPORT TerrainDepthShader(const Parameters &param);

	CRWD_EXPORT static TerrainDepthShader *Create(const Parameters &param);
};

} // namespace Crowd

#endif // CRWD_TERRAIN_SHADER_H_INCLUDED