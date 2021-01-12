#include "Mesh.h"
#include "D3D10MeshLoader.h"

namespace Crowd
{

const AABB &Mesh::GetAABB() const
{
	return m_aabb;
}

Mesh *Mesh::Load(const String fileName, int loadValue, Status *status)
{
	return D3D10::MeshLoader::Load(fileName, loadValue, status);
}

} // namespace Crowd