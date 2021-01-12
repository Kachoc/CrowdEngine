#include "Entity.h"
#include "SceneManager.h"

namespace Crowd
{

//
//	Entity
//

void Entity::Remove()
{
	if(m_manipulator)
	{
		m_manipulator->Remove();
		m_manipulator = NULL;
	}
}

void Entity::UpdateInScene()
{
	if(m_manipulator)
	{
		m_manipulator->Update();
	}
}


//
//	Object
//
Object::Object() {}
Object::Object(const String &name) : m_name(name) {}
Object::~Object() {} // { if(m_scene) { m_scene->Remove(this); } }

void Object::SetName(const String &name)
{
	m_name = name;
}

const char *Object::GetName() const
{
	return m_name;
}

void Object::SetScene(Scene *scene)
{
	m_scene = scene;
}

Scene *Object::GetScene() const
{
	return m_scene;
}

} // namespace Crowd