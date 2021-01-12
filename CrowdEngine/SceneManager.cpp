#include "SceneManager.h"

namespace Crowd
{

//
//	RenderScene
//

void RenderScene::Manipulator::Update()
{
	if(m_ptr)
	{
		ROctree::Iterator it = m_ptr->m_parent->Move(*this);
		*this = it;
	}
}

RenderScene::Manipulator &RenderScene::Manipulator::operator = (const ROctree::Iterator &it)
{
	(ROctree::Iterator&)(*this) = it;
	return *this;
}

RenderScene::RenderScene(const AABB &box) : m_octree(box)
{
	
}

RenderScene::~RenderScene()
{

}

RenderScene::ptr RenderScene::Create(const AABB &box)
{
	RenderScene::ptr scene = new RenderScene(box);

	return scene;
}

void RenderScene::Update(float ms)
{
	// rien pour le moment
}

void RenderScene::Draw(const RenderInfo &info)
{
	ROctree::BoolIterator<CameraFunct> funct(m_camera);

	while(funct)
	{
		m_camera.IsVisible((*funct).GetAABB());
		(*funct).Draw(info, m_camera);

		++funct;
	}
}

RenderScene::Manipulator RenderScene::Add(REntity::ptr entity)
{
	return (Manipulator&) m_octree.Add(entity);
}

void RenderScene::SetParameters(const Camera &camera)
{
	m_camera = camera;
}

Camera RenderScene::GetCamera() const
{
	return m_camera;
}



//
//	Scene
//

Scene::Scene(const AABB &box)
{
	m_renderScene = new RenderScene(box);
}

Scene::~Scene()
{

}

Scene::ptr Scene::Create(const AABB &box)
{
	Scene::ptr scene = new Scene(box);

	return scene;
}

void Scene::SetParameters(const Camera &camera)
{
	m_renderScene->SetParameters(camera);
}

Scene::Manipulator Scene::Add(Object::ptr object)
{
	return m_objects.Add(object);
}

void Scene::Update(float ms)
{
	m_renderScene->Update(ms);
}

RenderScene::ptr Scene::GetRenderScene()
{
	return m_renderScene;
}



//
//	SceneManager
//
SceneManager &SceneManager::GetInstance()
{
	return *m_instance;
}

SceneManager::Manipulator SceneManager::Add(Scene::ptr scene)
{
	return m_scenes.Add(scene);
}

void SceneManager::Update(float ms)
{
	for(List<Scene>::Iterator it = m_scenes.Begin(); (bool)it; ++it)
	{
		it->Update(ms);
	}
}

SceneManager::ptr SceneManager::m_instance(new SceneManager);

} // namespace Crowd