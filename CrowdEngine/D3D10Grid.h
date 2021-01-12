#ifndef CRWD_D3D10_GRID_H_INCLUDED
#define CRWD_D3D10_GRID_H_INCLUDED

#include "D3DEntry.h"

#include "Grid.h"

namespace Crowd
{

namespace D3D10
{

/*class Grid : public Crowd::Grid
{
	public:
	typedef SmartPtr<Grid, IPol> ptr;

	static Interface *Create(const Resource::CreationInfo *creationParam);

	virtual void Draw(const RenderInfo &info, const Camera &camera);

	protected:
	Grid(const CreationInfo &info);
	virtual ~Grid();

	protected:
	BufferPtr				m_vertexBuffer;
	unsigned int			m_numVertices;
	unsigned int			m_vertexSize;
	BufferPtr				m_indexBuffer;
	unsigned int			m_numIndices;
	RasterizerStatePtr		m_RSState;
};*/

class Grid : public Crowd::Grid
{
	public:
	typedef SmartPtr<Grid, IPol> ptr;

	Grid(unsigned int numSideQuads);
	virtual ~Grid();

	virtual void Draw(const RenderInfo &info, const Camera &camera);

	protected:
	BufferPtr				m_vertexBuffer;
	unsigned int			m_numVertices;
	unsigned int			m_vertexSize;
	BufferPtr				m_indexBuffer;
	unsigned int			m_numIndices;
	RasterizerStatePtr		m_RSState;
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_GRID_H_INCLUDED