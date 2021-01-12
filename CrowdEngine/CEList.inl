#ifndef CRWD_CELIST_INL_INCLUDED
#define CRWD_CELIST_INL_INCLUDED

namespace Crowd
{

//
//	List
//
template<class T, class PTR>
List<T, PTR>::List()
{
	m_firstElement = NULL;
	m_lastElement = NULL;
	m_size = 0;
}

template<class T, class PTR>
List<T, PTR>::~List()
{
	Element *e;
	while(m_firstElement)
	{
		e = m_firstElement;
		Remove(m_firstElement);
		delete e;
	}
}

template<class T, class PTR>
typename List<T, PTR>::Iterator List<T, PTR>::Add(PTR object)
{
	Element *elem = new Element;

	elem->m_list = this;
	elem->m_object = object;

	elem->m_next = NULL;

	if(m_size == 0)
	{
		elem->m_previous = NULL;

		m_firstElement = elem;
		m_lastElement = elem;
	}
	else
	{
		elem->m_previous = m_lastElement;
		m_lastElement->m_next = elem;

		m_lastElement = elem;
	}

	m_size++;

	return Iterator(elem);
}

template<class T, class PTR>
unsigned int List<T, PTR>::GetSize() const
{
	return m_size;
}

template<class T, class PTR>
typename List<T, PTR>::Iterator List<T, PTR>::Begin() const
{
	return Iterator(m_firstElement);
}

template<class T, class PTR>
typename List<T, PTR>::Iterator List<T, PTR>::End() const
{
	return Iterator(m_lastElement);
}

template<class T, class PTR>
void List<T, PTR>::Remove(typename List<T, PTR>::Element *object)
{
	if(object)
	{
		if(object->m_list)
		{
			if(object->m_previous)
			{
				object->m_previous->m_next = object->m_next;
			}
			else
			{
				object->m_list->m_firstElement = object->m_next;
			}

			if(object->m_next)
			{
				object->m_next->m_previous = object->m_previous;
			}
			else
			{
				object->m_list->m_lastElement = object->m_previous;
			}

			object->m_list->m_size--;

			object->m_previous = NULL;
			object->m_next = NULL;
			object->m_list = NULL;
		}
	}
}


//
//	Element
//
template<class T, class PTR>
List<T, PTR>::Element::Element()
{

}

template<class T, class PTR>
List<T, PTR>::Element::~Element()
{

}


//
//	Iterator
//
template<class T, class PTR>
List<T, PTR>::Iterator::Iterator()
{
	m_ptr = NULL;
}

template<class T, class PTR>
List<T, PTR>::Iterator::Iterator(typename List<T, PTR>::Element *element)
{
	m_ptr = element;
}

template<class T, class PTR>
typename List<T, PTR>::Iterator &List<T, PTR>::Iterator::operator ++ ()
{
	if(m_ptr) m_ptr = m_ptr->m_next;
	return *this;
}

template<class T, class PTR>
typename List<T, PTR>::Iterator &List<T, PTR>::Iterator::operator -- ()
{
	if(m_ptr) m_ptr = m_ptr->m_previous;
	return *this;
}

template<class T, class PTR>
void List<T, PTR>::Iterator::Remove()
{
	List::Remove(m_ptr);
	delete m_ptr;
	m_ptr = NULL;
}

template<class T, class PTR>
List<T, PTR>::Iterator::operator bool () const
{
	return m_ptr != NULL;
}

template<class T, class PTR>
bool List<T, PTR>::Iterator::operator ! () const
{
	return m_ptr == NULL;
}

template<class T, class PTR>
T& List<T, PTR>::Iterator::operator * () const
{
	return *(m_ptr->m_object);
}

template<class T, class PTR>
T* List<T, PTR>::Iterator::operator -> () const
{
	return m_ptr->m_object;
}


} // namespace Crowd

#endif // CRWD_CELIST_INL_INCLUDED