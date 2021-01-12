#ifndef CRWDPLANE_H_INCLUDED
#define CRWDPLANE_H_INCLUDED

#include "CRWDVector.h"

namespace Crowd
{

template<class T> struct PlaneT;
typedef PlaneT<float> Plane;
typedef PlaneT<double> PlaneD;

template<class T>
struct PlaneT
{
	//
	//	Fonctions et opérateurs
	//

	//	Constructeurs
	PlaneT() : Dist((T) 1.0) {}
	PlaneT(T x, T y, T z, T d) : Normal(x, y, z), Dist(d) {}
	PlaneT(const Vector3T<T> &normal, T d) : Normal(normal), Dist(d) {}
	PlaneT(const Vector3T<T> &point, const Vector3T<T> &normal) { PlaneFromPointNormal(*this, point, normal); }
	PlaneT(const Vector3T<T> &point1, const Vector3T<T> &point2, const Vector3T<T> &point3) { PlaneFromPoints(*this, point1, point2, point3); }

	// Opérateurs d'assignement
	PlaneT & operator *= (T f);
	PlaneT & operator /= (T f);

	// Opérateurs binaire
	PlaneT operator+() const;
	PlaneT operator-() const;

	// Opérateurs unaires
	PlaneT operator*(T f) const;
	PlaneT operator/(T f) const;

	// Opérateurs de cast
	operator T *();
	operator const T *() const;

	// Autres fonctions
	void Normalize();
	T Distance(const Vector3T<T> &point) const;

	// Fonctions statiques
	static T Dot(const PlaneT &, const Vector3T<T> &);

	static PlaneT &PlaneFromPointNormal(PlaneT &out, const Vector3T<T> &point, const Vector3T<T> &normal);
	static PlaneT &PlaneFromPoints(PlaneT &out, const Vector3T<T> &point1, const Vector3T<T> &point2, const Vector3T<T> &point3);
							// Si on regarde les points et qu'on les donne en les numérotant dans le sens des aiguilles d'une montre,
							// la normale pointe vers soit.

	
	//
	//	Attributs
	//
	union
	{
		struct
		{
		Vector3T<T> Normal;
		T Dist;
		};
		struct
		{
			float a, b, c, d;
		}; 
		T f[4];
	};
};

} // namespace Crowd

#include "CRWDPlane.inl"

#endif // CRWDPLANE_H_INCLUDED