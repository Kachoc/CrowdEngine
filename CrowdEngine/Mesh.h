#ifndef CRWD_MESH_H_INCLUDED
#define CRWD_MESH_H_INCLUDED

#include "Entity.h"
#include "CPM.h"
#include "Status.h"

namespace Crowd
{

//
//	Maillage g�om�trique
//
class Mesh : public REntity
{
	public:
	typedef SmartPtr<Mesh, IPol> ptr;

	virtual void Draw(const RenderInfo &info, const Camera &camera) = 0;
	CRWD_EXPORT virtual const AABB &GetAABB() const;

	// Load
	// loadValue: d�finit les �l�ments du mesh qui doivent �tre charg�s (normales, uvs... - voir CPM.h)
	CRWD_EXPORT static Mesh *Load(const String fileName, int loadValue, Status *status = NULL);

	protected:
	AABB m_aabb;
};

} // namespace Crowd

#endif // CRWD_MESH_H_INCLUDED