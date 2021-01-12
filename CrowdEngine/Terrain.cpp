#include "Terrain.h"

namespace Crowd
{

Terrain::Terrain(GeometryScene::ptr geoScene, LightScene::ptr lightScene, unsigned int numGridsX, unsigned int numGridsY, unsigned int numSquaresPerGrid, float squareWidth, float squareUVWidth, unsigned int numLOAD)
{
	m_geoScene = geoScene;
	m_lightScene = lightScene;


}

void Terrain::Update(const Camera &camera)
{
	
}

} // namespace Crowd