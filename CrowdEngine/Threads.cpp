#include <sstream>

#include "Threads.h"

#if defined(_WIN32) // _WIN32

//// CEDuplicateHandle: Macro pour la duplication de HANDLE
// targetHandle: contient le nouveau handle après duplication
// handle: handle à dupliquer
#define CEDuplicateHandle(targetHandle, handle)							\
																		\
if(targetHandle)														\
{																		\
	if(!CloseHandle(targetHandle))										\
	{																	\
		targetHandle = NULL;											\
		throw WCloseHandle(GetLastErrorString());						\
	}																	\
}																		\
																		\
targetHandle = NULL;													\
																		\
BOOL dupResult = DuplicateHandle(										\
		GetCurrentProcess(),											\
		handle,															\
		GetCurrentProcess(),											\
		&targetHandle,													\
		0,																\
		false,															\
		DUPLICATE_SAME_ACCESS);											\
																		\
if(!dupResult) throw WDuplicateHandle(GetLastErrorString());

namespace Crowd
{

DWORD WINAPI Thread::ThreadProc(LPVOID data)
{
	ThreadProcData *threadData = reinterpret_cast<ThreadProcData*>(data);
	if(!threadData) return 0;
	if(!threadData->thread || !threadData->task) return 0;

	ThreadTask *task = threadData->task;
	Thread *thread = threadData->thread;

	try
	{
		DWORD ret = (*task)();
		return ret;
	}
	catch(Exception &e)
	{
		std::ostringstream oss;
		oss << "Exception survenue dans un thread (id: " << thread->GetId() << "):\n";
		e.addFront(oss.str());

		thread->m_exception->Lock();
		thread->m_exception->exception = e.copy();
		thread->m_exception->Unlock();
	}
	catch(...)
	{
		std::ostringstream oss;
		oss << "Exception survenue dans un thread (id: " << thread->GetId() << "):\n";

		thread->m_exception->Lock();
		thread->m_exception->exception = new Exception();
		thread->m_exception->exception->addFront(oss.str());
		thread->m_exception->Unlock();
	}

	return 0;
}

Thread::Thread() : m_exception(new ThreadExceptionData), m_data(new ThreadProcData), m_thread(NULL)
{
	m_data->thread = this;
	m_data->task = NULL;

	m_thread = CreateThread(NULL, 0, ThreadProc, m_data, CREATE_SUSPENDED, &m_id);

	if(!m_thread)
	{
		throw InitFailure(GetLastErrorString(), "Création d'un thread");
	}
}

Thread::Thread(ThreadTask *task) : m_exception(new ThreadExceptionData), m_data(new ThreadProcData), m_thread(NULL)
{
	m_data->thread = this;
	m_data->task = task;

	m_thread = CreateThread(NULL, 0, ThreadProc, m_data.GetPtr(), CREATE_SUSPENDED, &m_id);
	if(!m_thread)
	{
		throw InitFailure(GetLastErrorString(), "Création d'un thread");
	}
}

Thread::Thread(Thread &t) : m_exception(t.m_exception), m_data(t.m_data), m_thread(NULL), m_id(t.m_id)
{
	CEDuplicateHandle(m_thread, t.m_thread);
}

Thread::~Thread()
{
	if(m_thread) CloseHandle(m_thread);
}

void Thread::Reset(ThreadTask *task)
{
	m_exception = new ThreadExceptionData;

	m_data = new ThreadProcData;
	m_data->thread = this;
	m_data->task = task;
	m_thread = CreateThread(NULL, 0, ThreadProc, m_data.GetPtr(), CREATE_SUSPENDED, &m_id);

	if(!m_thread)
	{
		throw InitFailure(GetLastErrorString(), "Création d'un thread");
	}
}

void Thread::GetException() throw()
{
	m_exception->Rethrow();
}

void Thread::Run()
{
	ResumeThread(m_thread);
}

/*void Thread::Suspend()
{
	SuspendThread(m_thread);
}*/

void Thread::Terminate(unsigned int exitCode)
{
	TerminateThread(m_thread, exitCode);
}

unsigned int Thread::GetId()
{
	return m_id;
}

Thread &Thread::operator=(Thread &t)
{
	m_exception = t.m_exception;
	m_data = t.m_data;

	CEDuplicateHandle(m_thread, t.m_thread);

	m_id = t.m_id;

	return *this;
}

bool Thread::WaitForThread(unsigned int ms)
{
	DWORD waitResult = WaitForSingleObject(m_thread, ms);
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

#endif // différentes plates-formes

} // namespace Crowd