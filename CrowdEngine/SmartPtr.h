#ifndef CE_SMARTPOINTERS_H_INCLUDED
#define CE_SMARTPOINTERS_H_INCLUDED

#ifndef NULL
#define NULL 0
#endif

#include "SmartPtrPolices.h"

namespace Crowd
{

//template <class T, template<class> class Police = RefCnt>
template <class T, template<class> class Police = RefCnt>
class SmartPtr : protected Police<T>
{
	public:
	// Constructeurs, destructeur
	SmartPtr();
	SmartPtr(T*);
	SmartPtr(const SmartPtr &);

	template <class E>
	SmartPtr(const SmartPtr<E> &);

	~SmartPtr();

	// Opérateurs d'affectation
	SmartPtr &operator=(const SmartPtr &);
	SmartPtr &operator=(T *);
	
	template <class E>
	SmartPtr &operator=(const SmartPtr<E> &);

	// Opérateurs de déréférencement
	T &operator*() const;
	T *operator->() const;

	// Opérateurs de cast
	T &operator [] (unsigned int i);

	operator T*() const;
	operator T*&();
	operator bool() const;

	// Fonction Get
	T *GetPtr();
	T **GetPtrP();

	bool operator ! () const;

	/*bool operator == (const SmartPtr<T> &);
	bool operator != (const SmartPtr<T> &);*/

	bool operator == (T *) const;
	bool operator != (T *) const;

	protected:
	T *m_data;
};

} // namespace Crowd

#include "SmartPtr.inl"

#endif // CE_SMARTPOINTERS_H_INCLUDED