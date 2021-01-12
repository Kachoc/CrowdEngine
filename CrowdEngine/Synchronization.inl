#if defined(CE_SYNCHRONIZATION_H_INCLUDED)

#ifndef CE_SYNCHRONIZATION_INL_INCLUDED
#define CE_SYNCHRONIZATION_INL_INCLUDED

#include <string>

namespace Crowd
{

//
//	DataLock
//
/*template <class T>
DataLock<T>::DataLock() : m_data(NULL) {}

template <class T>
DataLock<T>::~DataLock() {}

template <class T>
void DataLock<T>::SetData(const T *data, SmartPtr<Mutex<T>> &m)
{
	m_data = data;
	if(m_data)
	{
		m_mutex = m;
	}
	else
	{
		m_mutex = NULL;
	}
}

template <class T>
T &DataLock<T>::operator*()
{
	return *m_data;
}

template <class T>
T *DataLock<T>::operator->()
{
	return m_data;
}

template <class T>
DataLock<T>::operator bool()
{
	return m_data;
}


//
//	Mutex
//
template <class T>
Mutex<T>::Mutex(T &data) : m_data(&data), m_lock(NULL)
{
	m_mutex = CreateMutex(NULL, false, NULL);
	if(!mutex)
	{
		String str;
		GetLastErrorString(str);
		throw InitFailure(str, "Mutex creation");
	}
}

template <class T>
Mutex<T>::~Mutex()
{
	CloseHandle(m_mutex);
	if(m_data) delete data;
}

template <class T>
void Mutex<T>::Lock(SmartPtr<DataLock<T>> &data)
{
	DWORD waitResult = WaitForSingleObject(m_mutex, INFINITE);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		m_lock = &data;
		data.SetData(m_data, *this);
		return;

		default:
		String msg;
		GetLastErrorString(msg);
		throw WLock(msg);
	}
}

template <class T>
bool Mutex<T>::TryLock(SmartPtr<DataLock<T>> &data)
{
	DWORD waitResult = WaitForSingleObject(m_mutex, 0);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		m_lock = &data;
		data.SetData(m_data, *this);
		return true;

		case WAIT_TIMEOUT:
		return false;

		default:
		String msg;
		GetLastErrorString(msg);
		throw WLock(msg);
	}
}

template <class T>
bool Mutex<T>::TimedLock(SmartPtr<DataLock<T>> &data, unsigned int ms)
{
	DWORD waitResult = WaitForSingleObject(m_mutex, ms);
	switch(waitResult)
	{
		case WAIT_OBJECT_0:
		m_lock = &data;
		data.SetData(m_data, *this);
		return true;

		case WAIT_TIMEOUT:
		return false;

		default:
		String msg;
		GetLastErrorString(msg);
		throw WLock(msg);
	}
}

template <class T>
void Mutex<T>::Unlock()
{
	if(m_lock)
	{
		m_lock->SetData(NULL, NULL);
		m_lock = NULL;
		Release(m_mutex);
	}
}*/

} // namespace Crowd

#endif // CE_SYNCHRONIZATION_INL_INCLUDED

#endif // CE_SYNCHRONIZATION_H_INCLUDED