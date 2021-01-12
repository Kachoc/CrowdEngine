#ifndef CRWD_LOADER_MANAGER_H_INCLUDED
#define CRWD_LOADER_MANAGER_H_INCLUDED

#include <map>
#include "Resources.h"
#include "Status.h"

namespace Crowd
{

struct LoadInfo {};

struct SaveInfo {};

class Loader
{
	public:
	typedef SmartPtr<Loader, RefCnt> ptr;

	// Static Load
	virtual Resource *Load(const String fileName, const LoadInfo *info = NULL, Status *status = NULL) = 0;
};

class Saver
{
	public:
	typedef SmartPtr<Saver, RefCnt> ptr;

	// Static Load
	virtual Status Save(const String fileName, Resource *resource, const SaveInfo *info = NULL) = 0;
};

class LoaderManager
{
	public:
	friend class EngineLoader;
	typedef SmartPtr<LoaderManager> ptr;

	static LoaderManager &GetInstance();

	template<class T> typename T::ptr Load(const String &fileName, const String &loaderName, const LoadInfo *loadInfo = NULL, Status *status = NULL)
	{
		std::map<String, Loader::ptr>::iterator it = m_loaders.find(loaderName);
		if(it == m_loaders.end()) {
			if(status) {
				*status = Status("Le loader " + loaderName + " n'a pas été chargé.");
			}

			return NULL;
		}
		else
		{
			T::ptr res = (T*) it->second->Load(fileName, loadInfo, status);
			if(res) {
				res->Release();
				// Global::GetResourceManager().Add<T>(res, fileName);
			}
			return res;
		}
	}

	template<class T> Status Save(const String &fileName, typename T::ptr resource, const String &saverName, const SaveInfo *saveInfo = NULL)
	{
		std::map<String, Saver::ptr>::iterator it = m_savers.find(saverName);
		if(it == m_savers.end()) {
			return Status("Le saver " + saverName + " n'a pas été chargé.");
		}
		else
		{
			return it->second->Save(fileName, (T*) resource, saveInfo);
		}
	}

	Status AddLoader(Loader::ptr loader, const String &name);
	Status RemoveLoader(const String &name);

	Status AddSaver(Saver::ptr saver, const String &name);
	Status RemoveSaver(const String &name);

	protected:


	protected:
	static LoaderManager::ptr			m_instance;

	std::map<String, Loader::ptr>		m_loaders;
	std::map<String, Saver::ptr>		m_savers;

	//std::map<String, Loader::ptr> m_loaders;
};

} // namespace Crowd

#endif // CRWD_LOADER_MANAGER_H_INCLUDED