#include "Grid.h"
#include "D3D10Grid.h"

namespace Crowd
{

Grid::Grid(unsigned int numQuads)
{
	m_numQuads = numQuads;
}

Grid *Grid::Create(unsigned int numSideQuads)
{
	return new D3D10::Grid(numSideQuads);
}

} // namespace Crowd