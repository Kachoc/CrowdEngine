#ifndef CE_SMARTPTRPOLICES_H_INCLUDED
#define CE_SMARTPTRPOLICES_H_INCLUDED

// Polices utilis�es par les SmartPtr.
// Doivent �tre construites selon le mod�le suivant:
//
// template <class T>
// class Police {
// Police();
// Police(Police &);
// void FirstAssignment(T *):
//		est appel� dans le constructeur
// void Assignment(Police &, T *);
//		est appel�e avec l'op�rateur d'affectation de SmartPtr
// void Release(T *);
//		est appel�e lorsque le SmartPtr doit se d�barrasser du pointeur
//		(op�rateur d'affectation et destructeur)
// void PtrToPtr(T *);
//		est appel�e lorsque l'on veut utiliser le SmartPtr pour allouer
//		ou obtenir une nouvelle instance de la classe T avec une fonction
//		du type Create(T **)
//		on peut �tre oblig� d'utiliser ce type de fonction lorsque l'on
//		utilise une API ou une biblioth�que ne g�rant pas les SmartPtr
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

	void FirstAssignment(T *data) // constructeur de d�part
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

	void FirstAssignment(T *data) // constructeur de d�part
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
	
	void FirstAssignment(T *data) {} // Pas d'incr�mentation du compteur lors de la premi�re assignation: ptr = new Interface -> OK
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