#include "TerrainShader.h"
#include "D3D10TerrainShader.h"

namespace Crowd
{

//
//	TerrainShader
//
TerrainShader::TerrainShader(const Parameters &param) : m_parameters(param)
{

}

void TerrainShader::SetParameters(const Parameters &parameters)
{
	m_parameters = parameters;
}

void TerrainShader::SetMaterialDiffuse(const Color &diffuse)
{
	m_parameters.Diffuse = diffuse;
}

void TerrainShader::SetHeightMap(Texture::ptr heightMap)
{
	m_parameters.HeightMap = heightMap;
}


//
//	TerrainGeometryShader
//
TerrainGeometryShader::TerrainGeometryShader(const Parameters &param) : TerrainShader(param)
{

}

TerrainGeometryShader *TerrainGeometryShader::Create(const Parameters &param)
{
	return new D3D10::TerrainGeometryShader(param);
}


//
//	TerrainDepthShader
//
TerrainDepthShader::TerrainDepthShader(const Parameters &param) : TerrainShader(param)
{

}

TerrainDepthShader *TerrainDepthShader::Create(const Parameters &param)
{
	return new D3D10::TerrainDepthShader(param);
}

} // namespace Crowd