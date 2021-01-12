#ifndef CE_INTERFACE_H_INCLUDED
#define CE_INTERFACE_H_INCLUDED

#include "Synchronization.h"

namespace Crowd
{

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if(x != NULL) { x->Release(); x = NULL; }
#endif

#ifndef SAFE_ADDREF
#define SAFE_ADDREF(x) if(x != NULL) { x->AddRef(); }
#endif


//
//	Compatibilité
//

#define INTERFACE_PTR(CLASS) public: typedef SmartPtr<CLASS, IPol> ptr;
#define CLASS_PTR(CLASS) public: typedef SmartPtr<CLASS> ptr;

//
//	Interface
//
class CRWD_EXPORT NInterface
{
	INTERFACE_PTR(NInterface);

	public:
	virtual ~NInterface() {}

	virtual unsigned int AddRef()
	{
		m_count++;

		return m_count;
	}

	virtual unsigned int Release()
	{
		m_count--;
		if(m_count == 0)
		{
			delete this;
			return 0;
		}
		else
		{
			return m_count;
		}
	}

	protected:
	NInterface() : m_count(1) {}

	protected:
	unsigned int		m_count;
};

class Interface
{
	INTERFACE_PTR(Interface);

	public:
	// struct CreationInfo
	// Contient les informations permettant la création d'une interface (des
	// structures dérivées doivent être définies dans les classes héritant de
	// Interface. En particulier, un pointeur sur une structure Interface::Info
	// est demandée lors de la création d'une Resource par le ResourceManager.
	struct CreationInfo {};

	virtual ~Interface() {}

	virtual unsigned int AddRef()
	{
		m_countLock.LockExclusive();
		m_count++;
		unsigned int count = m_count;
		m_countLock.Unlock();

		return count;
	}

	virtual unsigned int Release()
	{
		m_countLock.LockExclusive();
		m_count--;
		if(m_count == 0)
		{
			delete this;
			return 0;
		}
		else
		{
			unsigned int actualCount = m_count;
			m_countLock.Unlock();
			return actualCount;
		}
	}

	Interface() : m_count(1) {}

	protected:
	unsigned int		m_count;
	RWLock				m_countLock;
};

} // namespace Crowd

#endif // CE_INTERFACE_H_INCLUDED