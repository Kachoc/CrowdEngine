#ifndef CRWD_CELIST_INCLUDED
#define CRWD_CELIST_INCLUDED

#ifndef NULL
	#define NULL 0
#endif

namespace Crowd
{

///
///	Classe utilisée par le moteur.
///	Cette classe ne peut être utilisée qu'avec des pointeurs intelligents.
///

template<class T, class PTR = T::ptr>
class List
{
	public:
	class Iterator;
	class Element;

	friend class Element;

	List();
	~List();

	Iterator Add(PTR object);

	unsigned int GetSize() const;

	Iterator Begin() const;
	Iterator End() const;

	class Element
	{
		friend class List;
		friend class Iterator;

		public:
		Element();
		~Element();

		protected:
		Element		*m_previous;
		Element		*m_next;
		List		*m_list;
		PTR			m_object;
	};

	class Iterator
	{
		friend class List;

		public:
		Iterator();
		Iterator(Element *element);

		Iterator &operator ++ ();
		Iterator &operator -- ();

		void Remove();

		operator bool () const;
		bool operator ! () const;

		T& operator * () const;
		T* operator -> () const;

		protected:
		Element *m_ptr;
	};

	protected:
	static void Remove(Element *);

	protected:
	Element *m_firstElement;
	Element *m_lastElement;
	unsigned int m_size;
};

} // namespace Crowd

#include "CEList.inl"

#endif // CRWD_CELIST_INCLUDED