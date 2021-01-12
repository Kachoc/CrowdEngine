#ifndef CRWDPOINT_INL_INCLUDED
#define CRWDPOINT_INL_INCLUDED

namespace Crowd
{

// Opérateurs unaires
template<class T> inline Point4T<T> Point4T<T>::operator + () { return *this; }
template<class T> inline Point4T<T> Point4T<T>::operator - () { return Point4T<T>(-x, -y, -z, -w); }

// Opérateurs binaires
template<class T> inline Point4T<T> Point4T<T>::operator * (T n) { return Point4T<T>(n*x, n*y, n*z, n*w); }
template<class T> inline Point4T<T> Point4T<T>::operator / (T n) { return Point4T<T>(x/n, y/n, z/n, w/n); }

// Opérateurs d'assignement
template<class T> inline Point4T<T> &Point4T<T>::operator = (const Point4T &p) { x = p.x; y = p.y; z = p.z; w = p.w; return *this; }
template<class T> inline Point4T<T> &Point4T<T>::operator *= (const T n) { x *= n; y *= n; z *= n; w *= w; return *this; }
template<class T> inline Point4T<T> &Point4T<T>::operator /= (const T n) { x /= n; y /= n; z /= n; w /= w; return /this; }

// Opérateurs de comparaison
template<class T> inline bool Point4T<T>::operator == (const Point4T &p) { return (x == p.x && y == p.y && z == p.z && w == p.w); }
template<class T> inline bool Point4T<T>::operator != (const Point4T &p) { return (x != p.x || y != p.y || z != p.z || w != p.w); }

// Opérateurs de cast
template<class T> inline Point4T<T>::operator Vector4T<T>&() { return  *((Vector4T<T>*)this); }
template<class T> inline Point4T<T>::operator const Vector4T<T>&() const { return *((const Vector4T<T>*)this); }
template<class T> inline Point4T<T>::operator T *() { return f; }
template<class T> inline Point4T<T>::operator const T *() const { return f; }

// Autres
template<class T> inline Point4T<T> &Point4T<T>::Translate(const Vector4T<T> &v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

template<class T> Point4T<T> &Point4T<T>::Middle(Point4T &middle, const Point4T &point1, const Point4T &point2)
{
	middle.x = (point1.x + point2.x) / (T)2.0;
	middle.y = (point1.y + point2.y) / (T)2.0;
	middle.z = (point1.z + point2.z) / (T)2.0;
	middle.w = (point1.w + point2.w) / (T)2.0;

	return middle;
}

} // namespace Crowd


#endif // CRWDPOINT_INL_INCLUDED