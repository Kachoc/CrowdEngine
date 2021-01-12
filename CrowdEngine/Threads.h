#ifndef CE_THREADS_H_INCLUDED
#define CE_THREADS_H_INCLUDED

#include <map>

#include "Synchronization.h"

namespace Crowd
{

class ThreadTask;
class Thread;

class ThreadTask
{
	public:
	virtual unsigned int operator()(void) = 0;
};

#if defined(_WIN32) // _WIN32

class Thread
{
	public:
	CRWD_EXPORT Thread();
	CRWD_EXPORT Thread(ThreadTask *task);
	CRWD_EXPORT Thread(Thread &);
	CRWD_EXPORT ~Thread();

	CRWD_EXPORT virtual void Reset(ThreadTask *task);
	CRWD_EXPORT virtual void GetException() throw();

	CRWD_EXPORT virtual void Run();						// à appeler après la création du thread
	//virtual void Suspend();				// met le thread en pause, ne doit être appelé que par le thread lui-même
	CRWD_EXPORT virtual void Terminate(unsigned int exitCode);

	CRWD_EXPORT unsigned int GetId();

	CRWD_EXPORT Thread &operator=(Thread &);

	CRWD_EXPORT virtual bool WaitForThread(unsigned int ms = INFINITE);
	
	template <class Conteneur>	// conteneur de la SL qui contient des pointeurs sur thread
	static bool WaitForMultipleThreads(Conteneur &threads, bool WaitAll = true, unsigned int ms = INFINITE);

	protected:
	struct ThreadProcData
	{
		Thread *thread;
		ThreadTask *task;
	};
	static DWORD WINAPI ThreadProc(LPVOID data);

	struct ThreadExceptionData
	{
		ThreadExceptionData() : exception(NULL) { InitializeSRWLock(&lock); }
		~ThreadExceptionData() { if(exception) delete exception; }

		void Lock() { AcquireSRWLockExclusive(&lock); }
		void Unlock() { ReleaseSRWLockExclusive(&lock); }

		void Rethrow() { Lock(); if(exception) { throw (*exception); } Unlock(); }	// renvoie l'exception

		Exception *exception;
		SRWLOCK lock;
	};

	protected:
	SmartPtr<ThreadExceptionData, RefCnt> m_exception;

	SmartPtr<ThreadProcData, RefCnt> m_data;

	HANDLE m_thread;
	DWORD m_id;
};

template <class Conteneur>
bool Thread::WaitForMultipleThreads(Conteneur &threads, bool WaitAll, unsigned int ms)
{
	unsigned int numThreads = threads.size();
	HANDLE *hThreads = new HANDLE[numThreads];

	Conteneur::iterator it = threads.begin();
	for(unsigned int i = 0; i < numThreads; i++)
	{
		hThreads[i] = (*it)->m_thread;
		it++;
	}

	DWORD waitResult = WaitForMultipleObjects(numThreads, hThreads, WaitAll, ms);
	delete[] hThreads;

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

#else // _WIN32

#error Plate-forme non supportée

#endif // différentes plates-formes

} // namespace Crowd

#endif // CE_THREADS_H_INCLUDED