#ifndef CE_SMARTPTRPOLICES_H_INCLUDED
#define CE_SMARTPTRPOLICES_H_INCLUDED

// Polices utilisées par les SmartPtr.
// Doivent être construites selon le modèle suivant:
//
// template <class T>
// class Police {
// Police();
// Police(Police &);
// void FirstAssignment(T *):
//		est appelé dans le constructeur
// void Assignment(Police &, T *);
//		est appelée avec l'opérateur d'affectation de SmartPtr
// void Release(T *);
//		est appelée lorsque le SmartPtr doit se débarrasser du pointeur
//		(opérateur d'affectation et destructeur)
// void PtrToPtr(T *);
//		est appelée lorsque l'on veut utiliser le SmartPtr pour allouer
//		ou obtenir une nouvelle instance de la classe T avec une fonction
//		du type Create(T **)
//		on peut être obligé d'utiliser ce type de fonction lorsque l'on
//		utilise une API ou une bibliothèque ne gérant pas les SmartPtr
//
// };


#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(var)			(void) var
#define UNUSED_PARAMETER_NOT_DEFINED
#endif

namespace Crowd
{

template <class T>
class RefCnt
{
	public:
	RefCnt()
	{
		m_count = new int(1);
	}

	RefCnt(RefCnt &rc)
	{
		m_count = rc.m_count;
		(*m_count)++;
	}

	void FirstAssignment(T *data) // constructeur de départ
	{
		UNUSED_PARAMETER(data);
	}

	void Assignment(RefCnt &rc, T *data)
	{
		UNUSED_PARAMETER(data);
		m_count = rc.m_count;
		(*m_count)++;
	}

	void Release(T *data)
	{
		(*m_count) = (*m_count) - 1;
		if((*m_count) == 0)
		{
			if(data) delete data;
			delete m_count;
		}
	}

	void PtrToPtr(T *data)
	{
		Release(data);
	}

	protected:
	int *m_count;
};

template <class T>
class ArrayRefCnt
{
	public:
	ArrayRefCnt()
	{
		m_count = new int(1);
	}

	ArrayRefCnt(ArrayRefCnt &rc)
	{
		m_count = rc.m_count;
		(*m_count)++;
	}

	void FirstAssignment(T *data) // constructeur de départ
	{
		UNUSED_PARAMETER(data);
	}

	void Assignment(ArrayRefCnt &rc, T *data)
	{
		UNUSED_PARAMETER(data);
		m_count = rc.m_count;
		(*m_count)++;
	}

	void Release(T *data)
	{
		(*m_count) = (*m_count) - 1;
		if((*m_count) == 0)
		{
			if(data) delete[] data;
			delete m_count;
		}
	}

	void PtrToPtr(T *data)
	{
		Release(data);
	}

	protected:
	int *m_count;
};

template <class T>
class IPol
{
	public:
	IPol() {}
	IPol(IPol &rc) { UNUSED_PARAMETER(rc); }
	
	void FirstAssignment(T *data) {} // Pas d'incrémentation du compteur lors de la première assignation: ptr = new Interface -> OK
	void Assignment(IPol &rc, T *data) { UNUSED_PARAMETER(rc); if(data) data->AddRef(); }
	void Release(T *data) { if(data) data->Release(); }

	void PtrToPtr(T *data) { data->Release(); }
};

} // namespace Crowd

#ifdef UNUSED_PARAMETER_NOT_DEFINED
#undef UNUSED_PARAMETER
#undef UNUSED_PARAMETER_NOT_DEFINED
#endif

#endif // CE_SMARTPTRPOLICES_H_INCLUDED