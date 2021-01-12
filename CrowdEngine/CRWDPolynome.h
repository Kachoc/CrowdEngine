#ifndef CRWD_POLYNOME_H_INCLUDED
#define CRWD_POLYNOME_H_INCLUDED


namespace Crowd
{

template<class T>
class Polynome {
	public:
	Polynome();
	Polynome(const std::vector<T> &coefficients);
	~Polynome();

	T operator()(T point);

	void DerivedPolynome(Polynome &derived) const;

	int Degree() const;

	static void ComputeInterpolationPolynome(Polynome &p, T a, T b, T va, T vb, T ta, T tb); // va, vb = P(a), P(b) -- ta, tb = P'(a), P'(b)

	protected:

	protected:
	std::vector<T> m_coefficients;
};

} // namespace Crowd

#include "CRWDPolynome.inl"

#endif // CRWD_POLYNOME_H_INCLUDED