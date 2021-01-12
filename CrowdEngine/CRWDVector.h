#ifndef CRWDVECTOR_H_INCLUDED
#define CRWDVECTOR_H_INCLUDED

#include <iostream>

namespace Crowd
{

#ifndef EPSILON
#define EPSILON 0.0001f
#endif

template<class T> struct Vector2T;
template<class T> struct Vector3T;
template<class T> struct Vector4T;

typedef Vector2T<int> Vector2I;
typedef Vector3T<int> Vector3I;
typedef Vector4T<int> Vector4I;

typedef Vector2T<float> Vector2;
typedef Vector3T<float> Vector3;
typedef Vector4T<float> Vector4;

typedef Vector2T<float> Vector2F;
typedef Vector3T<float> Vector3F;
typedef Vector4T<float> Vector4F;

typedef Vector2T<double> Vector2D;
typedef Vector3T<double> Vector3D;
typedef Vector4T<double> Vector4D;

template<class T>
struct Vector2T
{
	//
	//	Fonctions et opérateurs
	//

	// Constructeurs
	Vector2T() {} // : x(0.0f), y(0.0f) {}
	Vector2T(const Vector2T &v) : x(v.x), y(v.y) {}
	Vector2T(T x, T y) : x(x), y(y) {}
	Vector2T(const T *pf) : x(pf[0]), y(pf[1]) {}

	// Opérateurs =
	Vector2T &operator = (const Vector2T &);

	Vector2T &operator += (const Vector2T &);
	Vector2T &operator -= (const Vector2T &);

	Vector2T &operator *= (T );
	Vector2T &operator /= (T );

	// Opérateurs unaires
	Vector2T operator + () const;
	Vector2T operator - () const;

	// Opérations avec des vecteurs
	Vector2T operator + (const Vector2T &) const;
	Vector2T operator - (const Vector2T &) const;

	T operator * (const Vector2T &) const;

	// Opérations avec des floats
	Vector2T operator * (T ) const;
	Vector2T operator / (T ) const;

	// Opérations de comparaison
	bool operator == (const Vector2T &) const;
	bool operator != (const Vector2T &) const;

	//Opérateurs de cast
	operator T *();
	operator const T *() const;

	// Autres
	T Length() const;
	Vector2T &Normalize();

	// Fonctions statiques
	static T Dot(const Vector2T &, const Vector2T &);

	static Vector2T &Normalize(Vector2T &out, const Vector2T &in);
	static Vector2T Normalize(const Vector2T &);

	//
	//	Attributs
	//
	union
	{
		struct
		{
		T x, y;
		};
		T f[2];
	};
};

template<class T>
struct Vector3T
{
	//
	//	Fonctions et opérateurs
	//

	// Constructeurs
	Vector3T() {} // : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3T(const Vector2T<T> &v, T z) : x(v.x), y(v.y), z(z) {}
	Vector3T(T x, const Vector2T<T> &v) : x(x), y(v.x), z(v.y) {}
	Vector3T(const Vector3T &v) : x(v.x), y(v.y), z(v.z) {}
	Vector3T(T x, T y, T z) : x(x), y(y), z(z) {}
	Vector3T(const T *pf) : x(pf[0]), y(pf[1]), z(pf[2]) {}

	// Opérateurs =
	Vector3T &Vector3T::operator = (const Vector3T &);

	Vector3T &operator += (const Vector3T &);
	Vector3T &operator -= (const Vector3T &);

	Vector3T operator *= (const Vector3T &);

	// Opérateurs unaires
	Vector3T operator + () const;
	Vector3T operator - () const;

	// Opérations avec des vecteurs
	Vector3T operator + (const Vector3T &) const;
	Vector3T operator - (const Vector3T &) const;

	T operator * (const Vector3T &) const;

	// Opérations avec des floats
	Vector3T &operator *= (T );
	Vector3T &operator /= (T );

	Vector3T operator * (T ) const;
	Vector3T operator / (T ) const;

	// Opérations de comparaison
	bool operator==(const Vector3T &) const;
	bool operator!=(const Vector3T &) const;

	//Opérateurs de cast
	operator T *();
	operator const T *() const;

	// Autres
	T Length() const;
	Vector3T &Normalize();

	// Fonctions statiques
	static T Dot(const Vector3T &, const Vector3T &);

	static Vector3T &Cross(Vector3T &out, const Vector3T &, const Vector3T &); // cross-product
	static Vector3T Cross(const Vector3T &, const Vector3T &); // cross-product

	static Vector3T &Normalize(Vector3T &out, const Vector3T &in);
	static Vector3T Normalize(const Vector3T &);

	//
	//	Attributs
	//
	union
	{
		struct
		{
		T x, y, z;
		};
		T f[3];
	};
};

template<class T>
struct Vector4T
{
	//
	//	Fonctions et opérateurs
	//

	// Constructeurs
	Vector4T() {} // : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4T(const Vector4T &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	Vector4T(const Vector3T<T> &v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}
	Vector4T(T x, const Vector3T<T> &v) : x(x), y(v.x), z(v.y), w(v.z) {}
	Vector4T(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	Vector4T(const T *pf) : x(pf[0]), y(pf[1]), z(pf[2]), w(pf[3]) {}

	// Opérateurs =
	Vector4T &operator = (const Vector4T &);

	Vector4T &operator += (const Vector4T &);
	Vector4T &operator -= (const Vector4T &);

	// Opérateurs unaires
	Vector4T operator + () const;
	Vector4T operator - () const;

	// Opérations avec des vecteurs
	Vector4T operator + (const Vector4T &) const;
	Vector4T operator - (const Vector4T &) const;

	T operator * (const Vector4T &v) const;

	// Opérations avec des floats
	Vector4T &operator *= (T );
	Vector4T &operator /= (T );

	Vector4T operator * (T ) const;
	Vector4T operator / (T ) const;

	// Opérations de comparaison
	bool operator==(const Vector4T &) const;
	bool operator!=(const Vector4T &) const;

	// Opérateurs de cast
	operator Vector3T<T>&();
	operator const Vector3T<T>&() const;

	operator T *();
	operator const T *() const;

	// Autres
	T Length() const;
	Vector4T &Normalize();

	// Fonctions statiques
	static T Dot(const Vector4T &, const Vector4T &);

	static Vector4T &Normalize(Vector4T &out, const Vector4T &in);
	static Vector4T Normalize(const Vector4T &);

	//
	//	Attributs
	//
	union
	{
		struct
		{
		T x, y, z, w;
		};
		T f[4];
	};
};

// Input/output
template<class T> std::istream &operator >> (std::istream &stream, Vector2T<T> &v);
template<class T> std::istream &operator >> (std::istream &stream, Vector3T<T> &v);
template<class T> std::istream &operator >> (std::istream &stream, Vector4T<T> &v);

template<class T> std::ostream &operator << (std::ostream &stream, Vector2T<T> &v);
template<class T> std::ostream &operator << (std::ostream &stream, Vector3T<T> &v);
template<class T> std::ostream &operator << (std::ostream &stream, Vector4T<T> &v);

} // namespace Crowd

#include "CRWDVector.inl"

#endif // CRWDVECTOR_H_INCLUDED