#ifndef CE_SINGLETON_H_INCLUDED
#define CE_SINGLETON_H_INCLUDED

#include "Interface.h"
#include "SmartPtr.h"
#include "Synchronization.h"
#include "Export.h"

namespace Crowd
{

template <class T>
class Singleton
{
	public:
	static T &GetInstance()
	{
		return *m_instance;
	}

	protected:
	Singleton() {}
	~Singleton() {}

	static typename SmartPtr<T> m_instance;

	private:
	Singleton(Singleton &);
	Singleton &operator=(Singleton&);
};

template <class T>
typename SmartPtr<T> Singleton<T>::m_instance(NULL);

} // namespace Crowd

#endif // CE_SINGLETON_H_INCLUDED