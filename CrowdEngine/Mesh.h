#ifndef CRWD_MESH_H_INCLUDED
#define CRWD_MESH_H_INCLUDED

#include "Entity.h"
#include "CPM.h"
#include "Status.h"

namespace Crowd
{

//
//	Maillage géométrique
//
class Mesh : public REntity
{
	public:
	typedef SmartPtr<Mesh, IPol> ptr;

	virtual void Draw(const RenderInfo &info, const Camera &camera) = 0;
	CRWD_EXPORT virtual const AABB &GetAABB() const;

	// Load
	// loadValue: définit les éléments du mesh qui doivent être chargés (normales, uvs... - voir CPM.h)
	CRWD_EXPORT static Mesh *Load(const String fileName, int loadValue, Status *status = NULL);

	protected:
	AABB m_aabb;
};

} // namespace Crowd

#endif // CRWD_MESH_H_INCLUDED