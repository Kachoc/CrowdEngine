#ifndef CRWD_RESOURCES_H_INCLUDED
#define CRWD_RESOURCES_H_INCLUDED

#include <map>

#include "Interface.h"

#define SET_CREATOR_FUNCTION static SetCreator(Creator *creator) { m_creator = creator; }

namespace Crowd
{

#define INTERFACE_CREATOR(INTERFACE) static Interface *Create(const Resource::CreationInfo *creationParam) { return new INTERFACE; }

//
//	Classe Resource
//
// Utilisée à la fois pour manipuler les ressources chargées (textures, géométries...), 
class Resource : public Interface
{
	INTERFACE_PTR(Resource);

	friend class ResourceManager;

	public:
	/*CRWD_EXPORT Resource(const String &name = "Unknown");
	virtual CRWD_EXPORT ~Resource();

	virtual void CRWD_EXPORT SetResourceName(const String &name);

	virtual String CRWD_EXPORT GetResourceName() const;

	virtual void Remove(); // Retire la ressource du ResourceManager*/

	Resource(const String &name = "Unknown");
	virtual ~Resource();

	virtual void SetResourceName(const String &name);

	virtual String GetResourceName() const;

	virtual void Remove(); // Retire la ressource du ResourceManager

	protected:
	String		m_name;
};

class Creator : public Resource
{
	INTERFACE_PTR(Creator);

	public:
	typedef Interface* (*Operator)(const Resource::CreationInfo *creationParam);

	static Creator::ptr New(Operator op) { Creator::ptr creator = new Creator(op); creator->Release(); return creator; }

	virtual Interface* operator()(const Resource::CreationInfo *creationParam) { if(m_operator) { return m_operator(creationParam); } return NULL; }

	protected:
	Creator() : m_operator(NULL) {}
	Creator(Operator op) : m_operator(op) {}
	virtual ~Creator() {}

	protected:
	Operator	m_operator;
};

class ResourceManager : public Interface
{
	public:
	typedef SmartPtr<ResourceManager, IPol>	ptr;

	ResourceManager();
	virtual ~ResourceManager();

	CRWD_EXPORT static ResourceManager &GetInstance();

	//
	//	Ressources
	//
	template<class T>
	void Add(typename T::ptr resource, const String &name)
	{
		resource->SetResourceName(name);
		Add<T>(resource);
	}

	template<class T>
	void Add(typename T::ptr resource)
	{
		m_resources[resource->GetResourceName()] = resource.GetPtr();
	}

	template<class T>
	typename T::ptr Get(const String &name)
	{
		std::map< String, Resource::ptr >::iterator it = m_resources.find(name);
		if(it != m_resources.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}

	void Remove(const String &name)
	{
		m_resources.erase(name.asChar());
	}


	//
	//	Creators
	//
	void AddCreator(Creator::ptr creator, const String &name)
	{
		creator->SetResourceName(name);
		m_creators[creator->GetResourceName()] = creator; //AddCreator<T>(creator);
	}

	template<class T>
	typename T::ptr Create(const String &name, typename const T::CreationInfo *info = NULL)
	{
		std::map< String, Creator::ptr >::iterator it = m_creators.find(name);
		if(it != m_creators.end())
		{
			T::ptr resource = (T*) (*it->second)((const Resource::CreationInfo*) info);
			if(resource) { resource->Release(); }
			return resource;
		}
		else
		{
			return NULL;
		}
	}

	void RemoveCreator(const String &name)
	{
		m_creators.erase(name);
	}

	protected:
	std::map< String, Resource::ptr >		m_resources;
	std::map< String, Creator::ptr >		m_creators;

	static ResourceManager::ptr	m_instance;
};

} // namespace Crowd

#endif // CRWD_RESOURCES_H_INCLUDED