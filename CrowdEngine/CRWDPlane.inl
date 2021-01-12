#ifndef CRWDPLANE_INL_INCLUDED
#define CRWDPLANE_INL_INCLUDED

namespace Crowd
{

// Opérateurs d'assignement
template<class T> inline PlaneT<T> & PlaneT<T>::operator *= (T f) { Normal *= f; Dist *= f; return *this; }
template<class T> inline PlaneT<T> & PlaneT<T>::operator /= (T f) { Normal /= f; Dist /= f; return *this; }

// Opérateurs binaire
template<class T> inline PlaneT<T> PlaneT<T>::operator+() const { return *this; }
template<class T> inline PlaneT<T> PlaneT<T>::operator-() const { return PlaneT<T>(-Normal, -Dist); }

// Opérateurs unaires
template<class T> inline PlaneT<T> PlaneT<T>::operator*(T f) const { return PlaneT<T>(Normal*f, Dist*f); }
template<class T> inline PlaneT<T> PlaneT<T>::operator/(T f) const { return PlaneT<T>(Normal/f, Dist/f); }

// Opérateurs de cast
template<class T> inline PlaneT<T>::operator T *() { return (T*)this; }
template<class T> inline PlaneT<T>::operator const T *() const { return  (const T*)this; }

// Autres fonctions
template<class T> inline void PlaneT<T>::Normalize()
{
	*this /= Normal.Length();
}

template<class T> inline T PlaneT<T>::Distance(const Vector3T<T> &point) const
{
	return Normal*point + Dist;
}

// Fonctions statiques
template<class T> inline T PlaneT<T>::Dot(const PlaneT<T> &p, const Vector3T<T> &v)
{
	return p.a*v.x + p.b*v.y + p.c*v.z + p.d;
}

template<class T> inline PlaneT<T> &PlaneT<T>::PlaneFromPointNormal(PlaneT<T> &out, const Vector3T<T> &point, const Vector3T<T> &normal)
{
	out.Normal = normal;
	out.Normal.Normalize();
	out.Dist = -normal*point;
	return out;
}

template<class T> inline PlaneT<T> &PlaneT<T>::PlaneFromPoints(PlaneT<T> &out, const Vector3T<T> &point1, const Vector3T<T> &point2, const Vector3T<T> &point3)
{
	Vector3T<T> v1(point2 - point1);
	Vector3T<T> v2(point3 - point1);
	Vector3T<T>::Cross(out.Normal, v1, v2);
	out.Normal.Normalize();
	out.Dist = -out.Normal*point1;

	return out;
}

} // namespace Crowd

#endif // CRWDPLANE_INL_INCLUDED