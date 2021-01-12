#ifndef CRWD_OBJECT_CREATOR_H_INCLUDED
#define CRWD_OBJECT_CREATOR_H_INCLUDED

#include "SmartPtr.h"

namespace Crowd
{

template<class T, class PTR = T::ptr>
class ObjectCreator
{
	public:
	friend T;

	typedef SmartPtr<ObjectCreator> ptr;

	static PTR Create() { if(m_instance) { return (*m_instance)(); } else { return NULL; } }
	virtual PTR operator ()(void) = 0;

	static ptr GetInstance() { return m_instance; }

	protected:
	static typename ObjectCreator::ptr m_instance;
};

template<class T, class PTR>
typename ObjectCreator<T, PTR>::ptr ObjectCreator<T, PTR>::m_instance(NULL);

} // namespace Crowd

#endif // CRWD_OBJECT_CREATOR_H_INCLUDED