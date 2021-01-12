//
//	D3D10CpmLoader.h 
//
// Définit les classes dérivant de Loader et nécessaires au chargement des fichiers de géométrie de type .cpm
// Retourne 

#ifndef CRWD_D3D10_MESHLOADER_H_INCLUDED
#define CRWD_D3D10_MESHLOADER_H_INCLUDED

#include <map>
#include <fstream>

#include "CRWDMath.h"
#include "Status.h"
#include "Mesh.h"
#include "CPM.h"
#include "Vertex.h"

namespace Crowd
{

namespace D3D10
{

class LoadedMesh : public Mesh
{
	public:
	LoadedMesh(const AABB &aabb, BufferPtr vertexBuffer, unsigned int numVertices, unsigned int vertexSize, BufferPtr indexBuffer, unsigned int numIndices);
	virtual ~LoadedMesh();
	static Interface *Create(const AABB &aabb, BufferPtr vertexBuffer, unsigned int numVertices, unsigned int vertexSize, BufferPtr indexBuffer, unsigned int numIndices);

	void Draw(const RenderInfo &info, const Camera &camera);

	const AABB &GetAABB() const;

	protected:
	BufferPtr		m_vertexBuffer;
	unsigned int	m_numVertices;
	unsigned int	m_vertexSize;
	BufferPtr		m_indexBuffer;
	unsigned int	m_numIndices;
};

class MeshLoader
{
	public:
	static Crowd::Mesh *Load(const String fileName, int loadValue, Status *status = NULL);

	struct Mesh
	{
		std::vector<Vector3>			vertices;
		std::vector<Vector3>			normals;
		std::vector<unsigned int>		indices;
		std::vector<Vector2>			UVs;
	};

	protected:
	/*Status LoadVertices(std::ifstream &file, CpmLoader::Mesh &mesh);
	Status LoadNormals(std::ifstream &file, CpmLoader::Mesh &mesh);
	Status LoadIndices(std::ifstream &file, CpmLoader::Mesh &mesh);
	Status LoadUVs(std::ifstream &file, CpmLoader::Mesh &mesh);*/

	static Crowd::Mesh *CreateMesh(String filename, int loadValue, MeshLoader::Mesh &mesh, Status *status);

	protected:
	
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_MESHLOADER_H_INCLUDED