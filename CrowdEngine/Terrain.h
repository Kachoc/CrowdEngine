#ifndef CRWD_TERRAIN_H_INCLUDED
#define CRWD_TERRAIN_H_INCLUDED

#include "Grid.h"
#include "GeometryScene.h"
#include "LightScene.h"

namespace Crowd
{

class Terrain : public Interface
{
	public:
	// Constructor
	// - numGridsX : number of grids the terrain must have in width
	// - numGridsY : number of grids the terrain must have in height
	// - numSquaresPerGrid : number of squares a grid must have in with (a grid has the same height and width)
	// - squareWidth : width of a square in a grid
	// - squareUVWidth : width of a square in a grid in terms of UV space
	// - numLOAD : number of levels of details used
	// The size of a terrain (in engine units) is: (numGridsX*numSquaresPerGrid*squareWidth) per (numGridsY*numSquaresPerGrid*squareWidth).
	CRWD_EXPORT Terrain(GeometryScene::ptr geoScene, LightScene::ptr lightScene, unsigned int numGridsX, unsigned int numGridsY, unsigned int numSquaresPerGrid, float squareWidth, float squareUVWidth, unsigned int numLOAD);

	CRWD_EXPORT void Update(const Camera &camera);

	protected:
	GeometryScene::ptr				m_geoScene;
	LightScene::ptr					m_lightScene;

	std::vector<Grid::ptr>			m_grids;
	std::vector<SGEntity::ptr>		m_fields;

}; // class Terrain

} // namespace Crowd

#endif // CRWD_TERRAIN_H_INCLUDED