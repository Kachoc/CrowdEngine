#ifndef CRWD_TIME_H_INCLUDED
#define CRWD_TIME_H_INCLUDED

namespace Crowd
{

template<class T>
class Timer
{
	public:
	Timer();
	virtual ~Timer();

	virtual T GetElapsedTime();

	protected:
	__int64 m_countsPerSec;
	T		m_secPerCnt;
	__int64	m_t1;
	__int64	m_t2;
};

template<class T>
Timer<T>::Timer()
{
	QueryPerformanceFrequency((LARGE_INTEGER*) &m_countsPerSec);
	m_secPerCnt = 1/((T)m_countsPerSec);

	QueryPerformanceCounter((LARGE_INTEGER*) &m_t1);
	m_t2 = m_t1;
}

template<class T>
Timer<T>::~Timer()
{

}

template<class T>
T Timer<T>::GetElapsedTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*) &m_t2);

	T t = ((T)(m_t2 - m_t1))*m_secPerCnt;
	m_t1 = m_t2;

	return t;
}

} // namespace Crowd

#endif // CRWD_TIME_H_INCLUDED