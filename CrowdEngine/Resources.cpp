#include "Resources.h"

namespace Crowd
{

//
//	Resources
//

Resource::Resource(const String &name) : m_name(name)
{
	
}

Resource::~Resource()
{

}

void Resource::SetResourceName(const String &name)
{
	m_name = name;
}
String Resource::GetResourceName() const
{
	return m_name;
}

void Resource::Remove()
{
	ResourceManager::GetInstance().Remove(m_name);
}

//
//	ResourceManager
//

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

ResourceManager &ResourceManager::GetInstance()
{
	return *m_instance;
}

ResourceManager::ptr ResourceManager::m_instance(new ResourceManager);

} // namespace Crowd