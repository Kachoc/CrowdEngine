#ifndef CRWD_GRID_H_INCLUDED
#define CRWD_GRID_H_INCLUDED

#include "Mesh.h"

namespace Crowd
{

class Grid : public Mesh
{
	public:
	typedef SmartPtr<Grid, IPol> ptr;

	Grid(unsigned int numQuads);

	CRWD_EXPORT static Grid *Create(unsigned int numSideQuads);

	protected:
	unsigned int m_numQuads;
};

} // namespace Crowd

#endif // CRWD_GRID_H_INCLUDED