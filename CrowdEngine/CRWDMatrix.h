#ifndef CRWDMATRIX_H_INCLUDED
#define CRWDMATRIX_H_INCLUDED

#include <iostream>

#include "CRWDVector.h"

#ifndef CRWD_PI
	#define CRWD_PI 3.1415926535897932
#endif

namespace Crowd
{

struct Matrix
{
	//
	//	Fonctions et opérateurs
	//

	// Constructeurs
	Matrix() /*: _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
			 _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
			 _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
			 _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)*/ {}

	Matrix(const Matrix &matrix) { memcpy(m, matrix.m, sizeof(m)); }
	Matrix(float* pf) { memcpy(m, pf, sizeof(m)); }

	Matrix(float _11, float _12, float _13, float _14,
			 float _21, float _22, float _23, float _24,
			 float _31, float _32, float _33, float _34,
		      float _41, float _42, float _43, float _44)
		      : _11(_11), _12(_12), _13(_13), _14(_14),
			   _21(_21), _22(_22), _23(_23), _24(_24),
			   _31(_31), _32(_32), _33(_33), _34(_34),
			   _41(_41), _42(_42), _43(_43), _44(_44) {}

	Matrix(const Vector4 *pv) { memcpy(m, pv, sizeof(m)); }

	Matrix(const Vector4 &_1j,
		  const Vector4 &_2j,
		  const Vector4 &_3j,
		  const Vector4 &_4j) { memcpy(m, &_1j, sizeof(Vector4));
						    memcpy(&m[4], &_2j, sizeof(Vector4));
						    memcpy(&m[8], &_3j, sizeof(Vector4));
						    memcpy(&m[12], &_4j, sizeof(Vector4)); }

	// Opérateurs =
	Matrix &operator = (const Matrix &);
	
	Matrix &operator *= (const Matrix &);
	Matrix &operator += (const Matrix &);
	Matrix &operator -= (const Matrix &);

	// Opérateurs ()
	float &operator () (unsigned int r, unsigned int c); // r et c doivent être compris entre 0 et 3
	Vector4 &operator () (unsigned int r); // r doit être compris entre 0 et 3
	float *operator[] (unsigned int r);

	// Opérateurs + et -
	Matrix operator + () const;
	Matrix operator - () const;

	// Opérations avec une matrice
	Matrix operator * (const Matrix &) const;
	Matrix operator + (const Matrix &) const;
	Matrix operator - (const Matrix &) const;

	// Opérations avec un vecteur
	Vector4 operator * (const Vector4 &) const;

	// Opérations avec un float
	Matrix operator * (float ) const;
	Matrix operator / (float ) const;

	// Opérations de comparaison
	bool operator == (const Matrix &) const;
	bool operator != (const Matrix &) const;

	// Autres fonctions
	Matrix &Transpose();
	Matrix &Invert();

	float Determinant() const;

	// Fonctions statiques
	static void Identity(Matrix &);
	static Matrix Identity();

	static void Transpose(Matrix &);
	static Matrix &Transpose(Matrix &out, const Matrix &in);
	static Matrix &Invert(Matrix &out, const Matrix &in);

	// Fonctions propres à la 3D
	static void InvertRotationMatrix(Matrix &);
	static Matrix &InvertRotationMatrix(Matrix &out, const Matrix &in);

	static void BuildTransformLH(Matrix &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up);
	static void BuildTransformRH(Matrix &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up);

	static void BuildViewLH(Matrix &Matrix, const Vector3 &Eye, float yAngle, float xAngle);
	static void BuildViewLH(Matrix &Matrix, const Vector3 &Eye, const Vector3 &LookAt, const Vector3 &Up);
	static void BuildViewRH(Matrix &Matrix, const Vector3 &Eye, const Vector3 &LookAt, const Vector3 &Up);

	static void BuildProjectionLH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MinDepth, float MaxDepth);
	static void BuildProjectionRH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MinDepth, float MaxDepth);

	static void BuildCrowdProjectionLH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MaxDepth);
	static void BuildCrowdProjectionRH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MaxDepth);

	static void BuildTranslationMatrix(Matrix &Matrix, Vector3 Translation);
	static void BuildTranslationMatrix(Matrix &Matrix, float x, float y, float z);

	static void BuildScaleMatrix(Matrix &Matrix, float xScale, float yScale, float zScale);

	static Matrix &Multiply(Matrix &out, const Matrix &m1, const Matrix &m2);
	static Matrix &SMultiply(Matrix &out, const Matrix &m1, const Matrix &m2);
	static Vector4 &Multiply(Vector4 &out, const Matrix &m, const Vector4 &v);

	//
	//	Attributs
	//

	union // Attributs: représentations différentes
	{
		struct // élément par élément
		{

		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;

		/*float	_11, _21, _31, _41;
		float	_12, _22, _32, _42;
		float	_13, _23, _33, _43;
		float	_14, _24, _34, _44;*/

		};

		float m[4][4]; // tableau de floats
		//float pf[16]; // tableau de floats

	};
};

Vector4 operator * (const Vector4 &, const Matrix &);

// Input/Output
std::istream &operator >> (std::istream &stream, Matrix &matrix);
std::ostream &operator << (std::ostream &stream, const Matrix &matrix);


//
//	Fonction profitant de l'accélération matérielle
//
/*#ifndef CRWDMATH_NO_OPTIMIZATION
	Matrix CRWD_EXPORT &MatrixMultiply(Matrix &out, const Matrix &m1, const Matrix &m2);
#endif // CRWDMATH_NO_OPTIMIZATION*/

} // namespace Crowd

#include "CRWDMatrix.inl"

#endif // CRWDMATRIX_H_INCLUDED