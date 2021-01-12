#ifndef CRWD_D3D10_TERRAIN_SHADERS_H_INCLUDED
#define CRWD_D3D10_TERRAIN_SHADERS_H_INCLUDED

#include "D3DEntry.h"
#include "D3D10ShaderFile.h"
#include "D3D10Textures.h"

#include "TerrainShader.h"

namespace Crowd
{

namespace D3D10
{

struct TerrainShaderData
{
	Matrix		PositionMatrix;
	Color		Diffuse;
	float		HeightConstant;
	float		HeightMultiplier;
	float		SquareWidth;
	float		SquareUVWidth;
};

//
//	TerrainGeometryShader
//
class TerrainGeometryShader : public Crowd::TerrainGeometryShader
{
	public:
	TerrainGeometryShader(const Parameters &param);
	virtual ~TerrainGeometryShader();

	static Status Load(ShaderFile::ptr shader);
	static Status Unload();

	virtual Status Apply(unsigned int pass);

	protected:
	static ShaderFile::ptr				ms_shaderFile;
	static InputLayoutPtr				ms_vertexLayout;

	EffectTechniquePtr					mv_terrainTech;

	ConstantBuffer::ptr					mv_objectCBuffer;

	EffectShaderResourceVariablePtr		mv_heightMapVar;
	EffectShaderResourceVariablePtr		mv_diffuseSpecVar;
};

//
//	TerrainDepthShader
//
class TerrainDepthShader : public Crowd::TerrainDepthShader
{
	public:
	TerrainDepthShader(const Parameters &param);
	virtual ~TerrainDepthShader();

	static Status Load(ShaderFile::ptr terrainShadowFile);
	static Status Unload();

	virtual Status Apply(unsigned int pass);

	protected:
	static ShaderFile::ptr				ms_shaderFile;
	static InputLayoutPtr				ms_vertexLayout;

	EffectTechniquePtr					mv_terrainTech;

	ConstantBuffer::ptr					mv_objectCBuffer;

	EffectShaderResourceVariablePtr		mv_heightMapVar;
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_TERRAIN_SHADERS_H_INCLUDED