#ifndef CRWD_POLYNOME_INL_INCLUDED
#define CRWD_POLYNOME_INL_INCLUDED


namespace Crowd
{

template<class T>
Polynome<T>::Polynome() {}

template<class T>
Polynome<T>::Polynome(const std::vector<T> &coefficients) : m_coefficients(coefficients) {}

template<class T>
Polynome<T>::~Polynome() {}

template<class T>
T Polynome<T>::operator()(T point)
{
	T value = 0;
	for(int i = m_coefficients.size() - 1; i >= 0; i--)
	{
		value = m_coefficients[i] + point*value;
	}

	return value;
}

template<class T>
void Polynome<T>::DerivedPolynome(Polynome &derived) const
{
	if(m_coefficients.size() <= 1)
	{
		derived.m_coefficients.resize(0);
	}
	else
	{
		derived.m_coefficients.resize(m_coefficients.size()-1);

		for(unsigned int i = m_coefficients.size() - 1; i > 0; i--)
		{
			derived.m_coefficients[i-1] = i*m_coefficients[i];
		}
	}
}

template<class T>
int Polynome<T>::Degree() const
{
	return ((int)m_coefficients.size())-1;
}

template<class T>
void Polynome<T>::ComputeInterpolationPolynome(Polynome &p, T a, T b, T va, T vb, T ta, T tb)
{
	p.m_coefficients.resize(4);

	Matrix m((float) 1.0, (float) a, (float) a*a, (float) a*a*a, (float) 1.0, (float) b, (float) b*b, (float) b*b*b, (float) 0, (float) 1.0, (float) 2.0*a, (float) 3.0*a*a, (float) 0, (float) 1.0, (float) 2.0*b, (float) 3.0*b*b);
	T det = m.Determinant();

	m = Matrix((float) va, (float) a, (float) a*a, (float) a*a*a, (float) vb, (float) b, (float) b*b, (float) b*b*b, (float) ta, (float) 1.0, (float) 2.0*a, (float) 3*a*a, (float) tb, (float) 1.0, (float) 2*b, (float) 3*b*b);
	p.m_coefficients[0] = m.Determinant()/det;

	m = Matrix((float) 1.0, (float) va, (float) a*a, (float) a*a*a, (float) 1.0, (float) vb, (float) b*b, (float) b*b*b, (float) 0, (float) ta, (float) 2.0*a, (float) 3*a*a, (float) 0, (float) tb, (float) 2.0*b, (float) 3.0*b*b);
	p.m_coefficients[1] = m.Determinant()/det;

	m = Matrix((float) 1.0, (float) a, (float) va, (float) a*a*a, (float) 1.0, (float) b, (float) vb, (float) b*b*b, (float) 0, (float) 1.0, (float) ta, (float) 3.0*a*a, (float) 0, (float) 1.0, (float) tb, (float) 3.0*b*b);
	p.m_coefficients[2] = m.Determinant()/det;

	m = Matrix((float) 1.0, (float) a, (float) a*a, (float) va, (float) 1.0, (float) b, (float) b*b, (float) vb, (float) 0, (float) 1.0, (float) 2.0*a, (float) ta, (float) 0, (float) 1.0, (float) 2.0*b, (float) tb);
	p.m_coefficients[3] = m.Determinant()/det;

	int length = -1;
	for(unsigned int i = 0; i < 4; i++)
	{
		if(p.m_coefficients[i] != 0) {
			length = i;
		}
	}

	if(length != 3) {
		p.m_coefficients.resize(length+1);
	}
}

} // namespace Crowd

#endif // CRWD_POLYNOME_INL_INCLUDED