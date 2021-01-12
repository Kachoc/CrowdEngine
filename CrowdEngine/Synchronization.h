#ifndef CE_SYNCHRONIZATION_H_INCLUDED
#define CE_SYNCHRONIZATION_H_INCLUDED

#if defined(_WIN32) // _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#else // _WIN32
#error Plate-forme non supportée
#endif // différentes plates-formes

#include "Exceptions.h"
#include "Errors.h"

#include "Misc.h"
#include "SmartPtr.h"

namespace Crowd
{

class WLock : public Exception
{
	public:
	CRWD_EXPORT WLock(const String &msg = "Mauvais lock");
	CRWD_EXPORT virtual void addFront(const String &msg);

	EXCEPTION_COPY(WLock);
};

class WUnlock : public Exception
{
	public:
	CRWD_EXPORT WUnlock(const String &msg = "Mauvais unlock");
	CRWD_EXPORT virtual void addFront(const String &msg);

	EXCEPTION_COPY(WUnlock);
};

class HandleError : public Exception
{
	public:
	CRWD_EXPORT HandleError(const String &error = "", const String &type = "");
	EXCEPTION_COPY(HandleError);
};

class WCloseHandle : public HandleError
{
	public:
	CRWD_EXPORT WCloseHandle(const String &msg = "Mauvaise fermeture de HANDLE");
	EXCEPTION_COPY(WCloseHandle);
};

class WDuplicateHandle : public HandleError
{
	public:
	CRWD_EXPORT WDuplicateHandle(const String &msg = "Mauvaise duplication de HANDLE");
	EXCEPTION_COPY(WDuplicateHandle);
};


//
//	Classes de synchronization
//

class Mutex;
class RWLock;
class CriticalSection;

class Semaphore;

class Event;

#if defined(_WIN32)	// _WIN32

class Mutex
{
	public:
	CRWD_EXPORT Mutex(bool initialOwnership = false);
	CRWD_EXPORT Mutex(Mutex &);
	CRWD_EXPORT virtual ~Mutex();

	CRWD_EXPORT virtual void Lock();
	CRWD_EXPORT virtual bool TryLock();
	CRWD_EXPORT virtual bool TimedLock(unsigned int ms);	// ms: temps d'attente maximal en millisecondes
	CRWD_EXPORT virtual void Unlock();

	CRWD_EXPORT Mutex &operator=(Mutex &);
	
	protected:
	HANDLE m_mutex;
};

class RWLock // Read/write lock
{
	public:
	CRWD_EXPORT RWLock();
	CRWD_EXPORT RWLock(RWLock &);
	CRWD_EXPORT virtual ~RWLock();

	CRWD_EXPORT virtual void LockShared();		// pour lecture seule
	CRWD_EXPORT virtual void LockExclusive();	// pour lecture/écriture

	CRWD_EXPORT bool TryLockShared();
	CRWD_EXPORT bool TryLockExclusive();

	CRWD_EXPORT virtual void Unlock();

	CRWD_EXPORT RWLock &operator=(RWLock &);

	protected:
	struct LockData
	{
		CRWD_EXPORT LockData();
		CRWD_EXPORT ~LockData();

		SRWLOCK lock;
		volatile bool exclusive;
	};
	SmartPtr<LockData, RefCnt> m_lock;
};

/*class CriticalSection
{
	public:
	CriticalSection();
	CriticalSection(CriticalSection &cs);
	~CriticalSection();

	void Lock();
	bool TryLock();
	void Unlock();

	CriticalSection &operator=(CriticalSection &);

	protected:
	struct LockData
	{
		LockData();
		~LockData();

		void AddRef();
		void Release();

		CRITICAL_SECTION m_criticalSection;

		unsigned int m_count; // pour la libération des ressources
		SRWLOCK m_countLock;
	};
	LockData *m_criticalSection;
};

class Semaphore
{
	public:
	Semaphore(int maxCnt, int initialCnt = 0); // non signalé par défaut
	Semaphore(Semaphore &);
	virtual ~Semaphore();

	int GetMaxCnt();

	virtual void Lock();
	virtual bool TryLock();
	virtual bool TimedLock(unsigned int ms);
	virtual int Unlock();	// 

	Semaphore &operator=(Semaphore &);

	protected:
	HANDLE m_semaphore;
	int m_maxCnt;
};

class Event
{
	public:
	Event(bool manualReset = true, bool state = false);
	Event(Event &);
	virtual ~Event();

	virtual void Signal();
	virtual void Unsignal();

	virtual void WaitForSignal();
	virtual bool TimedWaitForSignal(unsigned int ms);	// ms: temps d'attente maximal en millisecondes
	virtual bool TestSignal();	// équivaut à TimedWait(0);

	bool ManualReset();

	Event &operator=(Event &);

	protected:
	HANDLE m_event;
	bool m_manualReset;
};*/

#else	// _WIN32

#endif	// différentes plates-formes

} // namespace Crowd

#include "Synchronization.inl"

#endif // CE_SYNCHRONIZATION_H_INCLUDED