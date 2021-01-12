#ifndef CRWDVOLUMES_H_INCLUDED
#define CRWDVOLUMES_H_INCLUDED

#include "CRWDVector.h"
#include "CRWDPlane.h"

namespace Crowd
{

struct AABB;
struct Sphere;

struct AABB
{
	//
	//	Fonctions et opérateurs
	//
	AABB() {}
	AABB(const Vector3 &MinPoint, const Vector3 &MaxPoint) : MinPoint(MinPoint), MaxPoint(MaxPoint) {}
	AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) : MinPoint(minX, minY, minZ),
																   MaxPoint(maxX, maxY, maxZ) {}
	
	// Opérateurs d'assignement
	AABB &operator *= (float f);
	AABB &operator /= (float f);

	// Opérateurs binaires
	AABB operator * (float f) const;
	AABB operator / (float f) const;

	// Fonctions
	void Move(const Vector3 &translation);

	float GetWidth() const;
	float GetHeight() const;
	float GetDepth() const;

	void GetCenter(Vector3 &center) const;
	Vector3 GetCenter() const;

	void Translate(const Vector3 &translation);
	void TransformLH(AABB &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up) const;
					// Out: la AABB contenant la boîte issue de la transformation de la AABB actuelle
	void TransformRH(AABB &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up) const;
					// Out: la AABB contenant la boîte issue de la transformation de la AABB actuelle

	bool Contains(const AABB &box) const;
	bool Contains(const Sphere &sphere) const;

	bool Intersects(const AABB &box) const;
	bool Intersects(const Sphere &sphere) const;

	// Fonctions statiques
	static void BoxFromVertices(AABB &out, const void *vertices, unsigned int elementSize, unsigned int numElements);
		// vertices pointe sur le vecteur position du premier vertice à traiter
	static void BoxFromVertices(AABB &out, const void *vertices, unsigned int offsetToPos,
						   unsigned int elementSize, unsigned int numElements);
		// vertices pointe sur le début du tableau de vertices, offsetToPos donne la position du vecteur position du premier vertice à traiter

	//
	//	Attributs
	//
	Vector3 MinPoint;
	Vector3 MaxPoint;
};

// Input/Output
std::istream &operator >> (std::istream &stream, AABB &box);
std::ostream &operator << (std::ostream &stream, const AABB &box);

struct Sphere
{
	//
	//	Fonctions et opérateurs
	//
	Sphere() : Center(), Radius(0.0f) {}
	Sphere(const Vector3 &Center, float Radius) : Center(Center), Radius(Radius) {}
	Sphere(float posX, float posY, float posZ, float radius) : Center(posX, posY, posZ), Radius(radius) {}

	// Opérateurs d'assignement
	Sphere &operator *= (float f);
	Sphere &operator /= (float f);

	// Opérateurs binaires
	Sphere operator * (float f) const;
	Sphere operator / (float f) const;

	// Fonctions
	bool Contains(const AABB &box) const;
	bool Contains(const Sphere &sphere) const;

	bool Intersects(const AABB &box) const;
	bool Intersects(const Sphere &sphere) const;

	// Fonctions statiques
	static void SphereFromVertices(Sphere &out, const void *vertices, unsigned int elementSize, unsigned int numElements);
		// vertices pointe sur le vecteur position du premier vertice à traiter
	static void SphereFromVertices(Sphere &out, const void *vertices, unsigned int offsetToPos,
						      unsigned int elementSize, unsigned int numElements);
		// vertices pointe sur le début du tableau de vertices, offsetToPos donne la position du vecteur position du premier vertice à traiter

	//
	//	Attributs
	//
	Vector3 Center;
	float Radius;
};

} // namespace Crowd

#include "CRWDVolumes.inl"

#endif // CRWDVOLUMES_H_INCLUDED