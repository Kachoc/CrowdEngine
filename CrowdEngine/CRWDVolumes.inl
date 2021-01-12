#ifndef CRWDVOLUMES_INL_INCLUDED
#define CRWDVOLUMES_INL_INCLUDED

namespace Crowd
{

//
//	AABB
//

// Opérateurs d'assignement
inline AABB &AABB::operator *= (float f)
{
	Vector3 center = (MinPoint + MaxPoint) / 2.0f;
	MinPoint = center + (MinPoint - center)*f;
	MaxPoint = center + (MaxPoint - center)*f;

	return *this;
}

inline AABB &AABB::operator /= (float f)
{
	Vector3 center = (MinPoint + MaxPoint) / 2.0f;
	MinPoint = center + (MinPoint - center)/f;
	MaxPoint = center + (MaxPoint - center)/f;

	return *this;
}

// Opérateurs binaires
inline AABB AABB::operator * (float f) const
{
	Vector3 min, max;

	Vector3 center = (MinPoint + MaxPoint) / 2.0f;
	min = center + (MinPoint - center)*f;
	max = center + (MaxPoint - center)*f;

	return AABB(min, max);
}

inline AABB AABB::operator / (float f) const
{
	Vector3 min, max;

	Vector3 center = (MinPoint + MaxPoint) / 2.0f;
	min = center + (MinPoint - center)/f;
	max = center + (MaxPoint - center)/f;

	return AABB(min, max);
}

// Fonctions
inline void AABB::Move(const Vector3 &translation)
{
	MinPoint += translation;
	MaxPoint += translation;
}

inline float AABB::GetWidth() const
{
	return MaxPoint.x - MinPoint.x;
}

inline float AABB::GetHeight() const
{
	return MaxPoint.y - MinPoint.y;
}

inline float AABB::GetDepth() const
{
	return MaxPoint.z - MinPoint.z;
}

inline void AABB::GetCenter(Vector3 &center) const
{
	center = (MaxPoint - MinPoint)/2.0f;
}

inline Vector3 AABB::GetCenter() const
{
	return (MaxPoint - MinPoint)/2.0f;
}

inline void AABB::Translate(const Vector3 &translation)
{
	MinPoint += translation;
	MaxPoint += translation;
}

inline void AABB::TransformLH(AABB &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up) const
{
	float dWidth = GetWidth()*0.5f, dHeight = GetHeight()*0.5f, dDepth = GetDepth()*0.5f;
	Vector3 center = GetCenter();

	// Rotation
	Vector3 z = Vector3::Normalize(zAxis);
	Vector3 x = Vector3::Normalize(Vector3::Cross(Up, z));
	Vector3 y = Vector3::Cross(z, x);

	// Redimensionnement
	x *= Scale.x;
	y *= Scale.y;
	z *= Scale.z;

	Out.MinPoint = Translate + center;
	Out.MaxPoint = Translate + center;

	 // Maximum relatif (au centre de la boîte)
	Vector3 rMax;
	rMax.x = abs(x.x)*dWidth + abs(y.x)*dHeight + abs(z.x)*dDepth;
	rMax.y = abs(x.y)*dWidth + abs(y.y)*dHeight + abs(z.y)*dDepth;
	rMax.z = abs(x.y)*dWidth + abs(y.z)*dHeight + abs(z.z)*dDepth;

	// Calcul de MinPoint et de MaxPoint
	Out.MinPoint = Translate + center - rMax;
	Out.MaxPoint = Translate + center + rMax;
}

inline void AABB::TransformRH(AABB &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up) const
{
	float dWidth = GetWidth()*0.5f, dHeight = GetHeight()*0.5f, dDepth = GetDepth()*0.5f;
	Vector3 center = GetCenter();

	// Rotation
	Vector3 z = Vector3::Normalize(zAxis);
	Vector3 x = Vector3::Normalize(Vector3::Cross(Up, z));
	Vector3 y = Vector3::Cross(z, x);

	// Redimensionnement
	x *= Scale.x;
	y *= Scale.y;
	z *= Scale.z;

	Out.MinPoint = Translate + center;
	Out.MaxPoint = Translate + center;

	 // Maximum relatif (au centre de la boîte)
	Vector3 rMax;
	rMax.x = abs(x.x)*dWidth + abs(y.x)*dHeight + abs(z.x)*dDepth;
	rMax.y = abs(x.y)*dWidth + abs(y.y)*dHeight + abs(z.y)*dDepth;
	rMax.z = abs(x.y)*dWidth + abs(y.z)*dHeight + abs(z.z)*dDepth;

	// Calcul de MinPoint et de MaxPoint
	Out.MinPoint = Translate + center - rMax;
	Out.MaxPoint = Translate + center + rMax;
}

inline bool AABB::Contains(const AABB &box) const
{
	if(box.MinPoint.x >= MinPoint.x && box.MinPoint.y >= MinPoint.y && box.MinPoint.z >= MinPoint.z
	&& box.MaxPoint.x <= MaxPoint.x && box.MaxPoint.y <= MaxPoint.y && box.MaxPoint.z <= MaxPoint.z) return true;
	return false;
}

inline bool AABB::Contains(const Sphere &sphere) const
{
	if(MinPoint.x <= sphere.Center.x - sphere.Radius
	&& MinPoint.y <= sphere.Center.y - sphere.Radius
	&& MinPoint.z <= sphere.Center.z - sphere.Radius
	&& MaxPoint.x >= sphere.Center.x + sphere.Radius
	&& MaxPoint.y >= sphere.Center.y + sphere.Radius
	&& MaxPoint.z >= sphere.Center.z + sphere.Radius) return true;
	return false;
}

inline bool AABB::Intersects(const AABB &box) const
{
	if(box.MinPoint.x >= MaxPoint.x || box.MinPoint.y >= MaxPoint.y || box.MinPoint.z >= MaxPoint.z) return false;
	if(box.MaxPoint.x <= MinPoint.x || box.MaxPoint.y <= MinPoint.y || box.MaxPoint.z <= MinPoint.z) return false;
	return true;
}

inline bool AABB::Intersects(const Sphere &sphere) const
{
	if(sphere.Center.x + sphere.Radius <= MinPoint.x ||
	   sphere.Center.y + sphere.Radius <= MinPoint.y ||
	   sphere.Center.z + sphere.Radius <= MinPoint.z) return false;
	if(sphere.Center.x - sphere.Radius >= MaxPoint.x ||
	   sphere.Center.y - sphere.Radius >= MaxPoint.y ||
	   sphere.Center.z - sphere.Radius >= MaxPoint.z) return false;
	return true;
}

// Fonctions statiques
inline void AABB::BoxFromVertices(AABB &out, const void *vertices, unsigned int elementSize, unsigned int numElements)
{
	const char *vb = (char*) vertices;

	out.MinPoint = (const Vector3&) *vb;
	out.MaxPoint = (const Vector3&) *vb;
	for(unsigned int i = 1; i < numElements; i++)
	{
		Vector3 actual;
		actual = (const Vector3&) vb[elementSize*i];

		out.MinPoint.x = min(out.MinPoint.x, actual.x);
		out.MinPoint.y = min(out.MinPoint.y, actual.y);
		out.MinPoint.z = min(out.MinPoint.z, actual.z);

		out.MaxPoint.x = max(out.MaxPoint.x, actual.x);
		out.MaxPoint.y = max(out.MaxPoint.y, actual.y);
		out.MaxPoint.z = max(out.MaxPoint.z, actual.z);
	}
}

inline void AABB::BoxFromVertices(AABB &out, const void *vertices, unsigned int offsetToPos, unsigned int elementSize, unsigned int numElements)
{
	const char *vb = (char*) vertices + offsetToPos;

	out.MinPoint = (const Vector3&) *vb;
	out.MaxPoint = (const Vector3&) *vb;
	for(unsigned int i = 1; i < numElements; i++)
	{
		Vector3 actual;
		actual = (const Vector3&) vb[elementSize*i];

		out.MinPoint.x = min(out.MinPoint.x, actual.x);
		out.MinPoint.y = min(out.MinPoint.y, actual.y);
		out.MinPoint.z = min(out.MinPoint.z, actual.z);

		out.MaxPoint.x = max(out.MaxPoint.x, actual.x);
		out.MaxPoint.y = max(out.MaxPoint.y, actual.y);
		out.MaxPoint.z = max(out.MaxPoint.z, actual.z);
	}
}

// Input/Output
inline std::istream &operator >> (std::istream &stream, AABB &box)
{
	stream >> box.MinPoint >> box.MaxPoint;

	return stream;
}

inline std::ostream &operator << (std::ostream &stream, const AABB &box)
{
	stream << box.MinPoint.x << " " << box.MinPoint.y << " " << box.MinPoint.z  << "     " << box.MaxPoint.x << " " << box.MaxPoint.y << " " << box.MaxPoint.z;

	return stream;
}

//
//	Sphere
//

// Opérateurs d'assignement
inline Sphere &Sphere::operator *= (float f)
{
	Radius *= f;

	return *this;
}

inline Sphere &Sphere::operator /= (float f)
{
	Radius /= f;

	return *this;
}

// Opérateurs binaires
inline Sphere Sphere::operator * (float f) const
{
	return Sphere(Center, Radius*f);
}

inline Sphere Sphere::operator / (float f) const
{
	return Sphere(Center, Radius/f);
}

// Fonctions
inline bool Sphere::Contains(const AABB &box) const
{
	if(box.MinPoint.x >= Center.x - Radius
	&& box.MinPoint.y >= Center.y - Radius
	&& box.MinPoint.z >= Center.z - Radius
	&& box.MaxPoint.x <= Center.x + Radius
	&& box.MaxPoint.y <= Center.y + Radius
	&& box.MaxPoint.z <= Center.z + Radius) return true;
	return false;
}

inline bool Sphere::Contains(const Sphere &sphere) const
{
	Vector3 CTC(sphere.Center - Center);
	if((CTC + Vector3(CTC).Normalize()*sphere.Radius).Length() <= Radius) return true;
	return false;
}

inline bool Sphere::Intersects(const AABB &box) const
{
	if(Center.x + Radius <= box.MinPoint.x ||
	   Center.y + Radius <= box.MinPoint.y ||
	   Center.z + Radius <= box.MinPoint.z) return false;
	if(Center.x - Radius >= box.MaxPoint.x ||
	   Center.y - Radius >= box.MaxPoint.y ||
	   Center.z - Radius >= box.MaxPoint.z) return false;
	return true;
}

inline bool Sphere::Intersects(const Sphere &sphere) const
{
	if((Center - sphere.Center).Length() >= Radius + sphere.Radius) return false;
	return true;
}

// Fonctions statiques
inline void Sphere::SphereFromVertices(Sphere &out, const void *vertices, unsigned int elementSize, unsigned int numElements)
{
	const char *vb = (char*) vertices;

	Vector3 MinPoint = (const Vector3&) *vb;
	Vector3 MaxPoint = (const Vector3&) *vb;
	for(unsigned int i = 1; i < numElements; i++)
	{
		Vector3 actual;
		actual = (const Vector3&) vb[elementSize*i];

		MinPoint.x = min(MinPoint.x, actual.x);
		MinPoint.y = min(MinPoint.y, actual.y);
		MinPoint.z = min(MinPoint.z, actual.z);

		MaxPoint.x = max(MaxPoint.x, actual.x);
		MaxPoint.y = max(MaxPoint.y, actual.y);
		MaxPoint.z = max(MaxPoint.z, actual.z);
	}

	out.Center = (MinPoint + MaxPoint) / 2.0f;
	out.Radius = (MaxPoint - out.Center).Length();
}

inline void Sphere::SphereFromVertices(Sphere &out, const void *vertices, unsigned int offsetToPos, unsigned int elementSize, unsigned int numElements)
{
	const char *vb = (char*) vertices + offsetToPos;

	Vector3 MinPoint = (const Vector3&) *vb;
	Vector3 MaxPoint = (const Vector3&) *vb;
	for(unsigned int i = 1; i < numElements; i++)
	{
		Vector3 actual;
		actual = (const Vector3&) vb[elementSize*i];

		MinPoint.x = min(MinPoint.x, actual.x);
		MinPoint.y = min(MinPoint.y, actual.y);
		MinPoint.z = min(MinPoint.z, actual.z);

		MaxPoint.x = max(MaxPoint.x, actual.x);
		MaxPoint.y = max(MaxPoint.y, actual.y);
		MaxPoint.z = max(MaxPoint.z, actual.z);
	}

	out.Center = (MinPoint + MaxPoint) / 2.0f;
	out.Radius = (MaxPoint - out.Center).Length();
}

} // namespace Crowd

#endif // CRWDVOLUMES_INL_INCLUDED