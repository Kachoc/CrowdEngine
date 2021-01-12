#ifndef CRWDPOINT_H_INCLUDED
#define CRWDPOINT_H_INCLUDED

namespace Crowd
{

template<class T> class Point2T;
template<class T> class Point3T;
template<class T> class Point4T;

typedef Point2T<int> Point2I;
typedef Point3T<int> Point3I;
typedef Point4T<int> Point4I;

typedef Point2T<float> Point2;
typedef Point3T<float> Point3;
typedef Point4T<float> Point4;

typedef Point2T<float> Point2F;
typedef Point3T<float> Point3F;
typedef Point4T<float> Point4F;

typedef Point2T<double> Point2D;
typedef Point3T<double> Point3D;
typedef Point4T<double> Point4D;

template<class T>
class Point2T
{
	public:
	Point2T(T x, T y) : x(x), y(y) {}
	Point2T(const Vector2T<T> v) : x(v.x), y(v.y) {}

	// Opérateurs unaires
	Point2T operator + ();
	Point2T operator - ();

	// Opérateurs binaires
	Point2T operator * (T );
	Point2T operator / (T );

	// Opérateurs d'assignement
	Point2T &operator = (const Point2T &);
	Point2T &operator *= (const T );
	Point2T &operator /= (const T );

	// Opérateurs de comparaison
	bool operator == (const Point2T &);
	bool operator != (const Point2T &);

	// Opérateurs de cast
	operator Vector2T<T>&();
	operator const Vector2T<T>&() const;
	operator T *();
	operator const T *() const;

	// Autres
	Point2T &Translate(const Vector2T<T> &);

	static Point2T &Middle(Point2T &middle, const Point2T &point1, const Point2T &point2);

	public:
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
class Point3T
{
	public:
	Point3T(T x, T y, T z) : x(x), y(y), z(z) {}
	Point3T(const Vector3T<T> v) : x(v.x), y(v.y), z(v.z) {}

	// Opérateurs unaires
	Point3T operator + ();
	Point3T operator - ();

	// Opérateurs binaires
	Point3T operator * (T );
	Point3T operator / (T );

	// Opérateurs d'assignement
	Point3T &operator = (const Point3T &);
	Point3T &operator *= (const T );
	Point3T &operator /= (const T );

	// Opérateurs de comparaison
	bool operator == (const Point3T &);
	bool operator != (const Point3T &);

	// Opérateurs de cast
	operator Vector3T<T>&();
	operator const Vector3T<T>&() const;
	operator T *();
	operator const T *() const;

	// Autres
	Point3T &Translate(const Vector3T<T> &);

	static Point3T &Middle(Point3T &middle, const Point3T &point1, const Point3T &point2);

	public:
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
class Point4T
{
	public:
	Point4T(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
	Point4T(const Vector4T<T> v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	// Opérateurs unaires
	Point4T operator + ();
	Point4T operator - ();

	// Opérateurs binaires
	Point4T operator * (T );
	Point4T operator / (T );

	// Opérateurs d'assignement
	Point4T &operator = (const Point4T &);
	Point4T &operator *= (const T );
	Point4T &operator /= (const T );

	// Opérateurs de comparaison
	bool operator == (const Point4T &);
	bool operator != (const Point4T &);

	// Opérateurs de cast
	operator Vector4T<T>&();
	operator const Vector4T<T>&() const;
	operator T *();
	operator const T *() const;

	// Autres
	Point4T &Translate(const Vector4T<T> &);

	static Point4T &Middle(Point4T &middle, const Point4T &point1, const Point4T &point2);

	public:
	union
	{
		struct
		{
			T x, y, z, w;
		};

		T f[4];
	};
};

} // namespace Crowd

#include "CRWDPoint.inl"

#endif // CRWDPOINT_H_INCLUDED