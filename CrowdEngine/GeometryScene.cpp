#include "GeometryScene.h"

namespace Crowd
{

//
//	StandardGeometryEntity
//
SGEntity::ptr SGEntity::Create(Mesh *mesh, MeshShader *shader)
{
	if(!mesh || !shader) {
		return NULL;
	}

	SGEntity::ptr entity = new SGEntity(mesh, shader);
	return entity;
}

SGEntity::ptr SGEntity::Create(Mesh *mesh, MeshShader *shader, const AABB &aabb)
{
	if(!mesh || !shader) {
		return NULL;
	}

	SGEntity::ptr entity = new SGEntity(mesh, shader, aabb);
	return entity;
}

void SGEntity::Draw(const RenderInfo &info, const Camera &camera)
{
	m_shader->Apply(0);
	m_mesh->Draw(info, camera);
}

const AABB &SGEntity::GetAABB() const
{
	return m_currentAABB;
}

const AABB &SGEntity::GetMeshAABB() const
{
	return m_aabb;
}

void SGEntity::SetAABB(const AABB &aabb)
{
	m_aabb = aabb;

	SetPositionMatrix(m_positionMatrix);
}

void SGEntity::SetPositionMatrix(const Matrix &position)
{
	Vector3 axis = m_aabb.MaxPoint-m_aabb.MinPoint;
	Vector4 origin = Vector4(m_aabb.MinPoint, 1.0f);
	Vector4 xAxis = Vector4(axis.x, 0.0f, 0.0f, 0.0f);
	Vector4 yAxis = Vector4(0.0f, axis.y, 0.0f, 0.0f);
	Vector4 zAxis = Vector4(0.0f, 0.0f, axis.z, 0.0f);

	origin = origin*position;
	xAxis = xAxis*position;
	yAxis = yAxis*position;
	zAxis = zAxis*position;

	m_currentAABB.MinPoint.x = origin.x + min4(0, xAxis.x, yAxis.x, zAxis.x);
	m_currentAABB.MinPoint.y = origin.y + min4(0, xAxis.y, yAxis.y, zAxis.y);
	m_currentAABB.MinPoint.z = origin.z + min4(0, xAxis.z, yAxis.z, zAxis.z);

	m_currentAABB.MaxPoint.x = origin.x + max4(0, xAxis.x, yAxis.x, zAxis.x);
	m_currentAABB.MaxPoint.y = origin.y + max4(0, xAxis.y, yAxis.y, zAxis.y);
	m_currentAABB.MaxPoint.z = origin.z + max4(0, xAxis.z, yAxis.z, zAxis.z);

	m_positionMatrix = position;
	m_shader->SetPositionMatrix(position);
	UpdateInScene();
}

SGEntity::SGEntity(Mesh *mesh, MeshShader *shader, const AABB &aabb, const Matrix &position) : m_mesh(mesh), m_shader(shader), m_aabb(aabb), m_currentAABB(aabb), m_positionMatrix(Matrix::Identity())
{
	m_mesh->AddRef();
	m_shader->AddRef();
}

SGEntity::SGEntity(Mesh *mesh, MeshShader *shader) : m_mesh(mesh), m_shader(shader), m_positionMatrix(Matrix::Identity())
{
	m_mesh->AddRef();
	m_shader->AddRef();
}

SGEntity::~SGEntity()
{

}

Mesh::ptr SGEntity::GetMesh()
{
	return m_mesh;
}

MeshShader::ptr SGEntity::GetShader()
{
	return m_shader;
}


//
//	GeometryScene
//
GeometryScene::GeometryScene(const AABB &box) : BaseScene<REntity>(box)// : m_octree(box)
{

}

GeometryScene::~GeometryScene()
{

}

void GeometryScene::Draw(const RenderInfo &info, const Camera &camera)
{
	ROctree::BoolIterator<CameraFunct> funct(camera, m_octree);

	while(funct)
	{
		if(camera.IsVisible((*funct).GetAABB()))  {
			(*funct).Draw(info, camera);
		}

		++funct;
	}
}

} // namespace Crowd