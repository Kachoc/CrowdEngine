#ifndef CRWD_SCENE_MANAGER_H_INCLUDED
#define CRWD_SCENE_MANAGER_H_INCLUDED

#include "CEList.h"

#include "Renderer.h"
#include "Entity.h"
#include "RenderPass.h"
#include "ObjectCreator.h"
#include "Camera.h"
#include "GeometryScene.h"

namespace Crowd
{

class SceneManager;
class Scene;
class RenderScene;

class RenderScene : public Interface
{
	public:
	typedef SmartPtr<RenderScene, IPol> ptr;

	class Manipulator : public ROctree::Iterator
	{
		public:
		virtual void Update();

		Manipulator &operator = (const ROctree::Iterator &it);
	};

	public:
	CRWD_EXPORT RenderScene(const AABB &box);
	CRWD_EXPORT virtual ~RenderScene();

	CRWD_EXPORT static ptr Create(const AABB &box);


	CRWD_EXPORT virtual void SetParameters(const Camera &camera);
	CRWD_EXPORT virtual Camera GetCamera() const;

	CRWD_EXPORT virtual void Update(float ms);
	CRWD_EXPORT virtual void Draw(const RenderInfo &info);

	CRWD_EXPORT virtual Manipulator Add(REntity::ptr entity);


	protected:
	Camera					m_camera;
	ROctree					m_octree;
};

class Scene : public Interface
{
	public:
	typedef SmartPtr<Scene, IPol>			ptr;
	typedef List<Object>::Iterator		Manipulator;

	public:
	CRWD_EXPORT Scene(const AABB &box);
	CRWD_EXPORT virtual ~Scene();

	CRWD_EXPORT static Scene::ptr Create(const AABB &box);

	CRWD_EXPORT virtual void SetParameters(const Camera &camera);

	CRWD_EXPORT virtual Manipulator Add(Object::ptr object);

	CRWD_EXPORT virtual void Update(float ms);

	// Fonctions utilisées par les objets
	CRWD_EXPORT virtual RenderScene::ptr GetRenderScene();
	// GetPhysicsScene...
	// GetAIScene...

	protected:


	protected:
	RenderScene::ptr			m_renderScene;

	List<Object>			m_objects;
};

class SceneManager : public Interface
{
	public:
	typedef SmartPtr<SceneManager, IPol>	ptr;
	typedef List<Scene>::Iterator			Manipulator;

	public:
	CRWD_EXPORT static SceneManager &GetInstance();

	CRWD_EXPORT virtual Manipulator Add(Scene::ptr scene);

	CRWD_EXPORT virtual void Update(float ms);

	protected:
	List<Scene> m_scenes;

	static SceneManager::ptr	m_instance;
};

} // namespace Crowd

#endif CRWD_SCENE_MANAGER_H_INCLUDED