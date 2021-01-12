#ifndef CRWD_INTERSECTION_H_INCLUDED
#define CRWD_INTERSECTION_H_INCLUDED

namespace Crowd
{

inline bool Intersection(Vector3 &Intersection, const Vector3 &RayPos, const Vector3 &Ray, const Vector3 &P1, const Vector3 &P2, const Vector3 &P3)
{
	// Vecteurs:
	Vector3 u(P3 - P1);
	Vector3 v(P2 - P1);
	
	// Déterminant:
	float d = -u.x*v.y*Ray.z + u.x*v.z*Ray.y + u.y*v.x*Ray.z - u.y*v.z*Ray.x - u.z*v.x*Ray.y + u.z*v.y*Ray.x;
	if(d == 0) return false;

	// On calcule les coordonnées locales du point d'intersection (dans le repère formé par le triangle)
	float x, y, k;
	x = ((RayPos.x - P1.x)*(-v.y*Ray.z + v.z*Ray.y) + (RayPos.y - P1.y)*(v.x*Ray.z - v.z*Ray.x) + (RayPos.z - P1.z)*(-v.x*Ray.y + v.y*Ray.x))/d;
	if(x < 0) return false;
	y = ((RayPos.x - P1.x)*(u.y*Ray.z - u.z*Ray.y) + (RayPos.y - P1.y)*(-u.x*Ray.z + u.z*Ray.x) + (RayPos.z - P1.z)*(u.x*Ray.y - u.y*Ray.x))/d;
	if(y < 0 || y + x - 1 > 0) return false;
	k = ((RayPos.x - P1.x)*(u.y*v.z - u.z*v.y) + (RayPos.y - P1.y)*(-u.x*v.z + u.z*v.x) + (RayPos.z - P1.z)*(u.x*v.y - u.y*v.x))/d;
	if(k < 0) return false;

	// On calcule les coordonnées globales du point
	Intersection = P1 + Ray*k;
	return true;
}

/*inline bool Intersection2(Vector3 &Intersection, const Vector3 &RayPos, const Vector3 &Ray, const Vector3 &P1, const Vector3 &P2, const Vector3 &P3)
{
	//
	//	Face Aplatie
	//
	// Si la face est aplatie (les trois points sont alignés), on ne recherche pas d'intersection.
	Vector3 U, V;
	U = P3 - P1;
	V = P2 - P1;
	float ULength = U.Length();
	float VLength = V.Length();
	// abs(U*V) = U*V*cos(U, V)
	// Donc: abs(U*V) = U*V si et seulement si (U, V) = 0° ou 180° (la face est aplatie)
	// Cela fonctionne aussi si deux des points sont confondus, car alors l'un des deux vecteurs est
	// le vecteur nul et on a: abs(U*V) = U*V = 0
	if(abs(U*V) == ULength*VLength) return false;


	//
	//	Intersection avec le plan
	//
	Plane plane(P1, P2, P3);
	if(plane.Distance(RayPos) == 0) // le point RayPos appartient au plan
	{
		Intersection = RayPos;
	}
	else
	{
		if(plane.Normal*Ray == 0) return false; // les deux vecteurs sont orthogonaux et le point RayPos n'appartient pas au plan: il n'y a pas d'intersection

		float k = -(plane.a*RayPos.x + plane.b*RayPos.y + plane.c*RayPos.z + plane.d)/(plane.a*Ray.x + plane.b*Ray.y + plane.c*Ray.z); // distance entre RayPos et le points d'intersection
		Intersection = RayPos + Ray*k;
		if((Intersection - RayPos)*Ray < 0) return false; // Si < 0 : il y a intersection, mais le vecteur Ray ne pointe pas dans le bon sens, l'intersection est ignorée
	}


	//
	//	Appartenance au triangle
	//
	// On définit le repère (P1, U, V) avec U = P3 - P1 et V = P2 - P1
	// On détermine les coordonnées (uCoord, vCoord) du point d'intersection dans ce repère, ce point appartient au triangle P1P2P3 si et seulement si:
	// uCoord >= 0
	// vCoord >= 0
	// vCoord + uCoord - 1 <= 0 (le point se situe en-dessous de la droite passant par les points de coordonnées (0 , 1) et (1 , 0) dans ce repère)

	Vector3 X, Y, ToPoint; // Vecteurs U et V déjà définis au tout début de la fonction
	ToPoint = Intersection - P1;
	Vector3::Normalize(X, U);
	Vector3::Cross(Y, X, plane.Normal);
	Y.Normalize();
	if(Y*V < 0) Y = -Y;

	float xDist = ToPoint*X;
	float yDist = ToPoint*Y;
	float xAxeProjDist = V*X;
	float yAxeProjDist = V*Y;

	float vCoord = yDist/yAxeProjDist;
	float uCoord = (xDist - vCoord*xAxeProjDist)/ULength;

	if(uCoord >= 0.0f && vCoord >= 0.0f && vCoord + uCoord - 1 <= 0) return true;
	return false;
}*/

} // namespace Crowd

#include "CRWDIntersection.inl"

#endif // CRWD_INTERSECTION_H_INCLUDED