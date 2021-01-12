#ifndef CRWD_GEOMETRY_SCENE_H_INCLUDED
#define CRWD_GEOMETRY_SCENE_H_INCLUDED

#include "CRWDMath.h"
#include "Entity.h"
#include "Mesh.h"
#include "ShadersLibrary.h"
#include "BaseScene.h"

namespace Crowd
{

typedef Tree<REntity, 16> ROctree;


/// classe Standard Geometry Entity
class SGEntity : public REntity
{
	public:
	typedef SmartPtr<SGEntity, IPol> ptr;

	CRWD_EXPORT static SGEntity::ptr Create(Mesh *mesh, MeshShader *shader, const AABB &aabb);
	CRWD_EXPORT static SGEntity::ptr Create(Mesh *mesh, MeshShader *shader);

	CRWD_EXPORT void Draw(const RenderInfo &info, const Camera &camera);
	CRWD_EXPORT virtual const AABB &GetAABB() const;
	CRWD_EXPORT virtual const AABB &GetMeshAABB() const;

	CRWD_EXPORT virtual void SetAABB(const AABB &aabb);
	CRWD_EXPORT virtual void SetPositionMatrix(const Matrix &position);

	CRWD_EXPORT Mesh::ptr GetMesh();
	CRWD_EXPORT MeshShader::ptr GetShader();

	protected:
	CRWD_EXPORT SGEntity(Mesh *mesh, MeshShader *shader, const AABB &aabb, const Matrix &position = Matrix::Identity());
	CRWD_EXPORT SGEntity(Mesh *mesh, MeshShader *shader);
	CRWD_EXPORT virtual ~SGEntity();

	protected:
	Mesh::ptr		m_mesh;
	MeshShader::ptr	m_shader;
	AABB			m_aabb; // bounding box originale, initialisée lors de l'instanciation de l'objet SGEntity
	Matrix			m_positionMatrix;
	AABB			m_currentAABB; // bouding box actuelle, calculée à partir de m_aabb et de m-position
};

class GeometryScene : public BaseScene<REntity>
{
	public:
	typedef SmartPtr<GeometryScene, IPol> ptr;

	CRWD_EXPORT GeometryScene(const AABB &box);
	CRWD_EXPORT virtual ~GeometryScene();

	CRWD_EXPORT virtual void Draw(const RenderInfo &info, const Camera &camera);
};

} // namespace Crowd

#endif // CRWD_GEOMETRY_SCENE_H_INCLUDED