#ifndef CRWDVECTOR_INL_INCLUDED
#define CRWDVECTOR_INL_INCLUDED

namespace Crowd
{


//
//	Vector2T<T>
//

// Opérateurs =
template<class T> inline Vector2T<T> &Vector2T<T>::operator = (const Vector2T<T> &v) { x = v.x; y = v.y; return *this; }

template<class T> inline Vector2T<T> &Vector2T<T>::operator += (const Vector2T<T> &v) { x += v.x; y += v.y; return *this; }
template<class T> inline Vector2T<T> &Vector2T<T>::operator -= (const Vector2T<T> &v) { x -= v.x; y -= v.y; return *this; }

template<class T> inline Vector2T<T> &Vector2T<T>::operator *= (T f) { x *= f; y *= f; return *this; }
template<class T> inline Vector2T<T> &Vector2T<T>::operator /= (T f) { x /= f; y /= f; return *this; }

// Opérateurs unaires
template<class T> inline Vector2T<T> Vector2T<T>::operator + () const { return *this; }
template<class T> inline Vector2T<T> Vector2T<T>::operator - () const { return Vector2T<T>(-x, -y); }

// Opérations avec des vecteurs
template<class T> inline Vector2T<T> Vector2T<T>::operator + (const Vector2T<T> &v) const { return Vector2T<T>(x + v.x, y + v.y); }
template<class T> inline Vector2T<T> Vector2T<T>::operator - (const Vector2T<T> &v) const { return Vector2T<T>(x - v.x, y - v.y); }

template<class T> inline T Vector2T<T>::operator * (const Vector2T<T> &v) const { return x*v.x + y*v.y; }

// Opérations avec des floats
template<class T> inline Vector2T<T> Vector2T<T>::operator * (T f) const { return Vector2T<T>(x*f, y*f); }
template<class T> inline Vector2T<T> Vector2T<T>::operator / (T f) const { return Vector2T<T>(x/f, y/f); }

// Opérations de comparaison
template<> inline bool Vector2T<int>::operator==(const Vector2T<int> &v) const { return ( x == v.x && y == v.y ); }
template<> inline bool Vector2T<int>::operator!=(const Vector2T<int> &v) const { return ( x != v.x || y != v.y ); }

template<class T> inline bool Vector2T<T>::operator == (const Vector2T<T> &v) const { return ( abs(x - v.x) <= std::numeric_limits<T>::epsilon() && abs(y - v.y) <= std::numeric_limits<T>::epsilon() ); }
template<class T> inline bool Vector2T<T>::operator != (const Vector2T<T> &v) const { return ( abs(x - v.x) > std::numeric_limits<T>::epsilon() || abs(y - v.y) > std::numeric_limits<T>::epsilon() ); }

// Opérateurs de cast
template<class T> inline Vector2T<T>::operator T *() { return (T*)this; }
template<class T> inline Vector2T<T>::operator const T *() const { return (const T*)this; }

// Autres
template<class T> inline T Vector2T<T>::Length() const { return (T) sqrt((double) x*x + y*y); }
template<class T> inline Vector2T<T> &Vector2T<T>::Normalize() { T l = Length(); x /= l; y /= l; return *this; }

// Fonctions statiques
template<class T> inline T Vector2T<T>::Dot(const Vector2T<T> &v1, const Vector2T<T> &v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

template<class T> inline Vector2T<T> &Vector2T<T>::Normalize(Vector2T<T> &out, const Vector2T<T> &in) { T l = in.Length(); out.x = in.x/l; out.y = in.y/l;  }
template<class T> inline Vector2T<T> Vector2T<T>::Normalize(const Vector2T<T> &v) { T l = v.Length(); return Vector2T<T>(v.x/l, v.y/l); }


//
//	Vector3T<T>
//

// Opérateurs =
template<class T> inline Vector3T<T> &Vector3T<T>::operator = (const Vector3T<T> &v) { x = v.x, y = v.y, z = v.z; return *this; }

template<class T> inline Vector3T<T> &Vector3T<T>::operator += (const Vector3T<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
template<class T> inline Vector3T<T> &Vector3T<T>::operator -= (const Vector3T<T> &v) { x -= v.x, y -= v.y, z -= v.z; return *this; }

template<class T> inline Vector3T<T> Vector3T<T>::operator *= (const Vector3T<T> &v) { Cross(*this, *this, v); }

// Opérateurs unaires
template<class T> inline Vector3T<T> Vector3T<T>::operator + () const { return *this; }
template<class T> inline Vector3T<T> Vector3T<T>::operator - () const { return Vector3T<T>(-x, -y, -z); }

// Opérations avec des vecteurs
template<class T> inline Vector3T<T> Vector3T<T>::operator + (const Vector3T<T> &v) const { return Vector3T<T>(x + v.x, y + v.y, z + v.z); }
template<class T> inline Vector3T<T> Vector3T<T>::operator - (const Vector3T<T> &v) const { return Vector3T<T>(x - v.x, y - v.y, z - v.z); }

template<class T> inline T Vector3T<T>::operator * (const Vector3T<T> &v) const { return x*v.x + y*v.y + z*v.z; }

// Opérations avec des floats
template<class T> inline Vector3T<T> &Vector3T<T>::operator *= (T f) { x *= f; y *= f, z *= f; return *this; }
template<class T> inline Vector3T<T> &Vector3T<T>::operator /= (T f) { x /= f; y /= f; z /= f; return *this; }

template<class T> inline Vector3T<T> Vector3T<T>::operator * (T f) const { return Vector3T<T>(x*f, y*f, z*f); }
template<class T> inline Vector3T<T> Vector3T<T>::operator / (T f) const { return Vector3T<T>(x/f, y/f, z/f); }

// Opérations de comparaison
template<> inline bool Vector3T<int>::operator==(const Vector3T<int> &v) const { return ( x == v.x && y == v.y && z == v.z ); }
template<> inline bool Vector3T<int>::operator!=(const Vector3T<int> &v) const { return ( x != v.x || y != v.y || z != v.z ); }

template<class T> inline bool Vector3T<T>::operator==(const Vector3T<T> &v) const { return ( abs(x - v.x) <= std::numeric_limits<T>::epsilon && abs(y - v.y) <= std::numeric_limits<T>::epsilon && abs(z - v.z) <= std::numeric_limits<T>::epsilon ); }
template<class T> inline bool Vector3T<T>::operator!=(const Vector3T<T> &v) const { return ( abs(x - v.x) > std::numeric_limits<T>::epsilon || abs(y - v.y) > std::numeric_limits<T>::epsilon || abs(z - v.z) > std::numeric_limits<T>::epsilon ); }

// Opérateurs de cast
template<class T> inline Vector3T<T>::operator T *() { return (T*)this; }
template<class T> inline Vector3T<T>::operator const T *() const { return (const T*)this; }

// Autres
template<class T> inline T Vector3T<T>::Length() const { return (T) sqrt((double) x*x + y*y + z*z); }
template<class T> inline Vector3T<T> &Vector3T<T>::Normalize() { T l = Length(); x /= l; y /= l; z /= l; return *this; }

// Fonctions statiques
template<class T> inline T Vector3T<T>::Dot(const Vector3T<T> &v1, const Vector3T<T> &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template<class T> inline Vector3T<T> &Vector3T<T>::Cross(Vector3T<T> &vout, const Vector3T<T> &v1, const Vector3T<T> &v2)
{
	vout.x = v1.y*v2.z - v1.z*v2.y;
	vout.y = v1.z*v2.x - v1.x*v2.z;
	vout.z = v1.x*v2.y - v1.y*v2.x;

	return vout;
}

template<class T> inline Vector3T<T> Vector3T<T>::Cross(const Vector3T<T> &v1, const Vector3T<T> &v2)
{
	return Vector3T<T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template<class T> inline Vector3T<T> &Vector3T<T>::Normalize(Vector3T<T> &vout, const Vector3T<T> &v)
{
	T l = v.Length();
	vout = v / l;
	
	return vout;
}

template<class T> inline Vector3T<T> Vector3T<T>::Normalize(const Vector3T<T> &v)
{
	T l = v.Length();
	return Vector3T<T>(v.x / l, v.y / l, v.z / l);
}



//
//	Vector4T<T>
//

// Opérateurs =
template<class T> inline Vector4T<T> &Vector4T<T>::operator = (const Vector4T<T> &v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

template<class T> inline Vector4T<T> &Vector4T<T>::operator += (const Vector4T<T> &v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
template<class T> inline Vector4T<T> &Vector4T<T>::operator -= (const Vector4T<T> &v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }

// Opérateurs unaires
template<class T> inline Vector4T<T> Vector4T<T>::operator + () const { return *this; }
template<class T> inline Vector4T<T> Vector4T<T>::operator - () const { return Vector4T<T>(-x, -y, -z, -w); }

// Opérations avec des vecteurs
template<class T> inline Vector4T<T> Vector4T<T>::operator + (const Vector4T<T> &v) const { return Vector4T<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
template<class T> inline Vector4T<T> Vector4T<T>::operator - (const Vector4T<T> &v) const { return Vector4T<T>(x - v.x, y - v.y, z - v.z, w - v.w); }

template<class T> inline T Vector4T<T>::operator * (const Vector4T<T> &v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }

// Opérations avec des floats
template<class T> inline Vector4T<T> &Vector4T<T>::operator *= (T f) { x *= f; y *= f; z *= f; w *= f; return *this; }
template<class T> inline Vector4T<T> &Vector4T<T>::operator /= (T f) { x /= f; y /= f; z /= f; w /= f; return *this; }

template<class T> inline Vector4T<T> Vector4T<T>::operator * (T f) const { return Vector4T<T>(x * f, y * f, z * f, w * f); }
template<class T> inline Vector4T<T> Vector4T<T>::operator / (T f) const { return Vector4T<T>(x / f, y / f, z / f, w / f); }

// Opérations de comparaison
template<> inline bool Vector4T<int>::operator==(const Vector4T<int> &v) const { return ( x == v.x && y == v.y && z == v.z && w == v.w ); }
template<> inline bool Vector4T<int>::operator!=(const Vector4T<int> &v) const { return ( x != v.x || y != v.y || z != v.z || w != v.w ); }

template<class T> inline bool Vector4T<T>::operator==(const Vector4T<T> &v) const { return ( abs(x - v.x) <= std::numeric_limits<T>::epsilon && abs(y - v.y) <= std::numeric_limits<T>::epsilon && abs(z - v.z) <= std::numeric_limits<T>::epsilon && abs(w - v.w) <= std::numeric_limits<T>::epsilon ); }
template<class T> inline bool Vector4T<T>::operator!=(const Vector4T<T> &v) const { return ( abs(x - v.x) > std::numeric_limits<T>::epsilon || abs(y - v.y) > std::numeric_limits<T>::epsilon || abs(z - v.z) > std::numeric_limits<T>::epsilon || abs(w - v.w) > std::numeric_limits<T>::epsilon ); }

// Opérateurs de cast
template<class T> inline Vector4T<T>::operator Vector3T<T>&() { return (Vector3T<T>&) *((Vector3T<T>*) this); }
template<class T> inline Vector4T<T>::operator const Vector3T<T>&() const { return (const Vector3T<T>&) *((const Vector3T<T>*) this); }

template<class T> inline Vector4T<T>::operator T *() { return (T*)this; }
template<class T> inline Vector4T<T>::operator const T *() const { return (const T*)this; }

// Autres
template<class T> inline T Vector4T<T>::Length() const { return (T) sqrt((double) x*x + y*y + z*z + w*w); }
template<class T> inline Vector4T<T> & Vector4T<T>::Normalize() { T l = Length(); x /= l; y /= l; z /= l; w /= l; return *this; }


// Fonctions statiques
template<class T> inline T Vector4T<T>::Dot(const Vector4T<T> &v1, const Vector4T<T> &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
}

template<class T> inline Vector4T<T> &Vector4T<T>::Normalize(Vector4T<T> &out, const Vector4T<T> &in)
{
	out = in / in.Length();
	return out;
}

template<class T> inline Vector4T<T> Vector4T<T>::Normalize(const Vector4T<T> &v)
{
	return Vector4T<T>(v / v.Length());
}

// Input/output
template<class T> inline std::istream &operator >> (std::istream &stream, Vector2T<T> &v) { stream >> v.x >> v.y; return stream; }
template<class T> inline std::istream &operator >> (std::istream &stream, Vector3T<T> &v) { stream >> v.x >> v.y >> v.z; return stream; }
template<class T> inline std::istream &operator >> (std::istream &stream, Vector4T<T> &v) { stream >> v.x >> v.y >> v.z >> v.w; return stream; }

template<class T> inline std::ostream &operator << (std::ostream &stream, Vector2T<T> &v) { stream << v.x << " " << v.y; return stream; }
template<class T> inline std::ostream &operator << (std::ostream &stream, Vector3T<T> &v) { stream << v.x << " " << v.y << " " << v.z; return stream; }
template<class T> inline std::ostream &operator << (std::ostream &stream, Vector4T<T> &v) { stream << v.x << " " << v.y << " " << v.z << " " << v.w; return stream; }

} // namespace Crowd

#endif // CRWDVECTOR_INL_INCLUDED