#ifdef CE_SMARTPOINTERS_H_INCLUDED

#ifndef CE_SMARTPOINTERS_INL_INCLUDED
#define CE_SMARTPOINTERS_INL_INCLUDED

namespace Crowd
{

// Constructeurs, destructeur
template <class T, template<class> class Police>
SmartPtr<T, Police>::SmartPtr() : m_data(NULL) {}

template <class T, template<class> class Police>
SmartPtr<T, Police>::SmartPtr(T* data) : m_data(data) { FirstAssignment(data); }

template <class T, template<class> class Police>
SmartPtr<T, Police>::SmartPtr(const SmartPtr &sp) : m_data(NULL) { *this = sp; }

template <class T, template<class> class Police>
template <class E>
SmartPtr<T, Police>::SmartPtr(const SmartPtr<E> &sp) : m_data(NULL) { *this = sp; }

template <class T, template<class> class Police>
SmartPtr<T, Police>::~SmartPtr()
{
	Release(m_data);
}


// Opérateurs d'affectation
template <class T, template<class> class Police>
SmartPtr<T, Police> &SmartPtr<T, Police>::operator=(const SmartPtr<T, Police> &sp)
{
	if(m_data != sp.m_data)
	{
		Release(m_data);
		m_data = sp.m_data;
		Assignment((Police<T> &) sp, m_data);
	}

	return *this;
}

template <class T, template<class> class Police>
SmartPtr<T, Police> &SmartPtr<T, Police>::operator=(T *data)
{
	if(m_data != data)
	{
		SmartPtr<T, Police> sp(data);
		*this = sp;
	}

	return *this;
}

template <class T, template<class> class Police>
template <class E>
SmartPtr<T, Police> &SmartPtr<T, Police>::operator=(const SmartPtr<E> &sp)
{
	if(m_data != (E*) sp)
	{
		Release(m_data);
		m_data = (E*) sp;
		Assignment((Police<T> &) sp, m_data);
	}

	return *this;
}

// Opérateurs de déréférencement
template <class T, template<class> class Police>
T &SmartPtr<T, Police>::operator*() const
{
	return *m_data;
}

template <class T, template<class> class Police>
T *SmartPtr<T, Police>::operator->() const
{
	return m_data;
}

// Opérateurs de cast
template <class T, template<class> class Police>
T &SmartPtr<T, Police>::operator [] (unsigned int i)
{
	return m_data[i];
}

template <class T, template<class> class Police>
SmartPtr<T, Police>::operator T*() const
{
	return m_data;
}

template <class T, template<class> class Police>
SmartPtr<T, Police>::operator T*&()
{
	return m_data;
}

template <class T, template<class> class Police>
SmartPtr<T, Police>::operator bool() const
{
	return m_data != NULL;
}

// Fonction Get
template <class T, template<class> class Police>
T *SmartPtr<T, Police>::GetPtr()
{
	return m_data;
}

template <class T, template<class> class Police>
T **SmartPtr<T, Police>::GetPtrP()
{
	if(m_data) { PtrToPtr(m_data); m_data = NULL; }
	return &m_data;
}

template <class T, template<class> class Police>
bool SmartPtr<T, Police>::operator ! () const
{
	return m_data == NULL;
}

/*template <class T, template<class> class Police>
bool SmartPtr<T, Police>::operator == (const SmartPtr<T> &ptr)
{
	return m_data == ptr.m_data;
}

template <class T, template<class> class Police>
bool SmartPtr<T, Police>::operator != (const SmartPtr<T> &ptr)
{
	return m_data != ptr.m_data;
}*/

template <class T, template<class> class Police>
bool SmartPtr<T, Police>::operator == (T *ptr) const
{
	return m_data == ptr;
}

template <class T, template<class> class Police>
bool SmartPtr<T, Police>::operator != (T *ptr) const
{
	return m_data != ptr;
}

} // namespace Crowd

#endif // CE_SMARTPOINTERS_INL_INCLUDED

#endif // CE_SMARTPOINTERS_H_INCLUDED