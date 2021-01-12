#include "LoaderManager.h"

namespace Crowd
{

LoaderManager::ptr LoaderManager::m_instance(new LoaderManager);

LoaderManager &LoaderManager::GetInstance()
{
	return *m_instance;
}

Status LoaderManager::AddLoader(Loader::ptr loader, const String &name)
{
	if(loader)
	{
		m_loaders[name] = loader;
	}

	return Status::Success;
}

Status LoaderManager::RemoveLoader(const String &name)
{
	std::map<String, Loader::ptr>::iterator it;

	it = m_loaders.find(name);
	if(it != m_loaders.end())
	{
		m_loaders.erase(it);
	}

	return Status::Success;
}


Status LoaderManager::AddSaver(Saver::ptr saver, const String &name)
{
	if(saver)
	{
		m_savers[name] = saver;
	}

	return Status::Success;
}

Status LoaderManager::RemoveSaver(const String &name)
{
	std::map<String, Saver::ptr>::iterator it;

	it = m_savers.find(name);
	if(it != m_savers.end())
	{
		m_savers.erase(it);
	}

	return Status::Success;
}



} // namespace Crowd