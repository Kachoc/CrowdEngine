#include <sstream>

#include "Synchronization.h"

namespace Crowd
{

//
//	Exceptions
//
WLock::WLock(const String &msg)
{
	setString(msg);
}

void WLock::addFront(const String &msg)
{
	std::ostringstream oss;
	oss << "W lock: " << msg;

	Exception::addFront(oss.str());
}

WUnlock::WUnlock(const String &msg)
{
	setString(msg);
}

void WUnlock::addFront(const String &msg)
{
	std::ostringstream oss;
	oss << "W lock: " << msg;

	Exception::addFront(oss.str());
}

HandleError::HandleError(const String &error, const String &type) : Exception("")
{
	std::ostringstream oss;
	oss << "Erreur avec un HANDLE: " << type << ": " << error;

	addFront(oss.str());
}

WCloseHandle::WCloseHandle(const String &msg) : HandleError(msg, "Mauvaise fermeture") {}

WDuplicateHandle::WDuplicateHandle(const String &msg) : HandleError(msg, "Mauvaise duplication") {}


//
//	Synchronisation objects
//

#if defined(_WIN32) // _WIN32

//// SynchroDuplicateHandle: Macro pour la duplication de HANDLE
// targetHandle: contient le nouveau handle après duplication
// handle: handle à dupliquer
#define SynchroDuplicateHandle(targetHandle, handle)						\
																\
if(targetHandle)													\
{																\
	if(!CloseHandle(targetHandle))									\
	{															\
		targetHandle = NULL;										\
		throw WCloseHandle(GetLastErrorString());						\
	}															\
}																\
																\
targetHandle = NULL;												\
																\
BOOL dupResult = DuplicateHandle(										\
		GetCurrentProcess(),										\
		handle,													\
		GetCurrentProcess(),										\
		&targetHandle,												\
		0,														\
		false,													\
		DUPLICATE_SAME_ACCESS);										\
																\
if(!dupResult) throw WDuplicateHandle(GetLastErrorString());


//
//	Mutex
//
Mutex::Mutex(bool initialOwnership)
{
	m_mutex = CreateMutex(NULL, initialOwnership, NULL);
	if(!m_mutex)
	{
		throw InitFailure(GetLastErrorString(), "Création d'un mutex");
	}
}

Mutex::Mutex(Mutex &m) : m_mutex(NULL)
{
	SynchroDuplicateHandle(m_mutex, m.m_mutex);
}

Mutex::~Mutex()
{
	if(!CloseHandle(m_mutex))
	{
		throw WCloseHandle(GetLastErrorString());
	}
}

void Mutex::Lock()
{
	DWORD waitResult = WaitForSingleObject(m_mutex, INFINITE);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return;

		default:
		throw WLock(GetLastErrorString());
	}
}

bool Mutex::TryLock()
{
	DWORD waitResult = WaitForSingleObject(m_mutex, 0);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return true;

		case WAIT_TIMEOUT:
		return false;

		default:
		throw WLock(GetLastErrorString());
	}
}

bool Mutex::TimedLock(unsigned int ms)
{
	DWORD waitResult = WaitForSingleObject(m_mutex, ms);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return true;

		case WAIT_TIMEOUT:
		return false;

		default:
		throw WLock(GetLastErrorString());
	}
}

void Mutex::Unlock()
{
	if(!ReleaseMutex(m_mutex))
	{
		throw WUnlock(GetLastErrorString());
	}
}

Mutex &Mutex::operator=(Mutex &m)
{
	SynchroDuplicateHandle(m_mutex, m.m_mutex);

	return *this;
}


//
// ReadWriteLock
//
RWLock::RWLock() : m_lock(new LockData) {}

RWLock::RWLock(RWLock &l) : m_lock(l.m_lock) {}

RWLock::~RWLock() {}

void RWLock::LockShared()
{
	AcquireSRWLockShared(&m_lock->lock);
}

void RWLock::LockExclusive()
{
	AcquireSRWLockExclusive(&m_lock->lock);
	m_lock->exclusive = true;
}

void RWLock::Unlock()
{
	if(!m_lock->exclusive)
	{
		ReleaseSRWLockShared(&m_lock->lock);
	}
	else
	{
		m_lock->exclusive = false;
		ReleaseSRWLockExclusive(&m_lock->lock);
	}
}

bool RWLock::TryLockShared()
{
	if(TryAcquireSRWLockShared(&m_lock->lock))
	{
		return true;
	}
	return false;
}

bool RWLock::TryLockExclusive()
{
	if(TryAcquireSRWLockExclusive(&m_lock->lock))
	{
		m_lock->exclusive = true;
		return true;
	}
	return false;
}

RWLock &RWLock::operator=(RWLock &) { return *this; }

RWLock::LockData::LockData() : exclusive(false) { InitializeSRWLock(&lock); }
RWLock::LockData::~LockData() {}


//
//	CriticalSection
//
/*CriticalSection::CriticalSection() : m_criticalSection(new LockData) {}

CriticalSection::CriticalSection(CriticalSection &cs)
{
	m_criticalSection = cs.m_criticalSection;
	m_criticalSection->AddRef();
}

CriticalSection::~CriticalSection() { m_criticalSection->Release(); }


void CriticalSection::Lock()
{
	EnterCriticalSection(&m_criticalSection->m_criticalSection);
}


bool CriticalSection::TryLock()
{
	return TryEnterCriticalSection(&m_criticalSection->m_criticalSection) != 0;
}


void CriticalSection::Unlock()
{
	LeaveCriticalSection(&m_criticalSection->m_criticalSection);
}

CriticalSection &CriticalSection::operator=(CriticalSection &cs)
{
	m_criticalSection->Release();
	m_criticalSection = cs.m_criticalSection;
	m_criticalSection->AddRef();

	return *this;
}

CriticalSection::LockData::LockData()
{
	InitializeCriticalSection(&m_criticalSection);
	m_count = 1;
	InitializeSRWLock(&m_countLock);
}


CriticalSection::LockData::~LockData()
{
	DeleteCriticalSection(&m_criticalSection);
}

void CriticalSection::LockData::AddRef()
{
	m_count++;
}

void CriticalSection::LockData::Release()
{
	AcquireSRWLockExclusive(&m_countLock);
	m_count--;
	if(m_count == 0)
	{
		delete this;
		return;
	}
	ReleaseSRWLockExclusive(&m_countLock);
}


//
//	Semaphore
//
Semaphore::Semaphore(int maxCnt, int initialCnt) : m_maxCnt(maxCnt)
{
	m_semaphore = CreateSemaphore(NULL, initialCnt, maxCnt, NULL);
	if(!m_semaphore)
	{
		throw InitFailure(GetLastErrorString(), "Création d'un sémaphore");
	}
	m_maxCnt = maxCnt;
}

Semaphore::Semaphore(Semaphore &s) : m_semaphore(NULL), m_maxCnt(s.m_maxCnt)
{
	SynchroDuplicateHandle(m_semaphore, s.m_semaphore);
}

Semaphore::~Semaphore()
{
	if(!CloseHandle(m_semaphore))
	{
		throw WCloseHandle(GetLastErrorString());
	}
}

int Semaphore::GetMaxCnt()
{
	return m_maxCnt;
}

void Semaphore::Lock()
{
	DWORD waitResult = WaitForSingleObject(m_semaphore, INFINITE);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return;

		default:
		throw WLock(GetLastErrorString());
	}
}

bool Semaphore::TryLock()
{
	DWORD waitResult = WaitForSingleObject(m_semaphore, 0);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return true;

		case WAIT_TIMEOUT:
		return false;

		default:
		throw WLock(GetLastErrorString());
	}
}

bool Semaphore::TimedLock(unsigned int ms)
{
	DWORD waitResult = WaitForSingleObject(m_semaphore, ms);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return true;

		case WAIT_TIMEOUT:
		return false;

		default:
		throw WLock(GetLastErrorString());
	}
}

int Semaphore::Unlock()
{
	long ret;
	if(!ReleaseSemaphore(m_semaphore, 1, &ret))
	{
		throw WUnlock(GetLastErrorString());
	}

	return ret;
}

Semaphore &Semaphore::operator=(Semaphore &s)
{
	SynchroDuplicateHandle(m_semaphore, s.m_semaphore);

	m_maxCnt = s.m_maxCnt;

	return *this;
}


//
//	Event
//
Event::Event(bool manualReset, bool state) : m_manualReset(manualReset)
{
	m_event = CreateEvent(NULL, manualReset, state, NULL);
	if(!m_event)
	{
		throw InitFailure(GetLastErrorString(), "Création d'un événement (objet Event)");
	}
}

Event::Event(Event &e) : m_event(NULL), m_manualReset(e.m_manualReset)
{
	SynchroDuplicateHandle(m_event, e.m_event);
}

Event::~Event()
{
	if(!CloseHandle(m_event))
	{
		throw WCloseHandle(GetLastErrorString());
	}
}

void Event::Signal()
{
	if(!SetEvent(m_event))
	{
		throw Error(GetLastErrorString(), "SetEvent");
	}
}

void Event::Unsignal()
{
	if(!ResetEvent(m_event))
	{
		throw Error(GetLastErrorString(), "ResetEvent");
	}
}

void Event::WaitForSignal()
{
	DWORD waitResult = WaitForSingleObject(m_event, INFINITE);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return;
		
		default:
		throw WLock(GetLastErrorString());
	}
}

bool Event::TimedWaitForSignal(unsigned int ms)
{
	DWORD waitResult = WaitForSingleObject(m_event, ms);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return true;

		case WAIT_TIMEOUT:
		return false;
		
		default:
		throw WLock(GetLastErrorString());
	}
}

bool Event::TestSignal()
{
	DWORD waitResult = WaitForSingleObject(m_event, 0);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		return true;

		case WAIT_TIMEOUT:
		return false;
		
		default:
		throw WLock(GetLastErrorString());
	}
}

bool Event::ManualReset()
{
	return m_manualReset;
}

Event &Event::operator=(Event &e)
{
	SynchroDuplicateHandle(m_event, e.m_event);

	m_manualReset = e.m_manualReset;

	return *this;
}*/

#else // _WIN32

#endif // différentes plates-formes

} // namespace Crowd