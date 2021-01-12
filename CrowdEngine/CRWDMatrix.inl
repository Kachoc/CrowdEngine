#ifndef CRWDMATRIX_INL_INCLUDED
#define CRWDMATRIX_INL_INCLUDED

#define CMUL(i, j, matrix) _##i##1 * matrix._##1##j + _##i##2 * matrix._##2##j + _##i##3 * matrix._##3##j + _##i##4 * matrix._##4##j
#define CMUL_2(i, j) m1._##i##1 * m2._##1##j + m1._##i##2 * m2._##2##j + m1._##i##3 * m2._##3##j + m1._##i##4 * m2._##4##j
#define PLUS(i, j, matrix) _##i##j += matrix._##i##j
#define MINUS(i, j, matrix) _##i##j -= matrix._##i##j
#define EQUAL(i, j, matrix) abs(_##i##j - matrix._##i##j) <= std::numeric_limits<float>::epsilon()
#define NEQUAL(i, j, matrix) abs(_##i##j - matrix._##i##j) > std::numeric_limits<float>::epsilon()
#define TRANS(i, j, out, in) out._##i##j = in._##j##i

namespace Crowd
{

// Opérateurs =
inline Matrix &Matrix::operator = (const Matrix &matrix) { memcpy(m, matrix.m, sizeof(m)); return *this; }

#ifndef CRWD_DIRECT3D
inline Matrix &Matrix::operator *= (const Matrix &matrix)
{
	*this = *this * matrix;

	return *this;
}
#else
inline Matrix &Matrix::operator *= (const Matrix &matrix)
{
	return SMultiply(*this, *this, matrix); // MatrixMultiply(*this, *this, matrix);
}
#endif

inline Matrix &Matrix::operator += (const Matrix &matrix) { PLUS(1, 1, matrix); PLUS(1, 2, matrix); PLUS(1, 3, matrix); PLUS(1, 4, matrix);
														  PLUS(2, 1, matrix); PLUS(2, 2, matrix); PLUS(2, 3, matrix); PLUS(2, 4, matrix);
														  PLUS(3, 1, matrix); PLUS(3, 2, matrix); PLUS(3, 3, matrix); PLUS(3, 4, matrix);
														  PLUS(4, 1, matrix); PLUS(4, 2, matrix); PLUS(4, 3, matrix); PLUS(4, 4, matrix);
														  return *this; }
inline Matrix &Matrix::operator -= (const Matrix &matrix) { MINUS(1, 1, matrix); MINUS(1, 2, matrix); MINUS(1, 3, matrix); MINUS(1, 4, matrix);
														  MINUS(2, 1, matrix); MINUS(2, 2, matrix); MINUS(2, 3, matrix); MINUS(2, 4, matrix);
														  MINUS(3, 1, matrix); MINUS(3, 2, matrix); MINUS(3, 3, matrix); MINUS(3, 4, matrix);
														  MINUS(4, 1, matrix); MINUS(4, 2, matrix); MINUS(4, 3, matrix); MINUS(4, 4, matrix);
														  return *this; }

// Opérateurs ()
inline float &Matrix::operator () (unsigned int r, unsigned int c) { return m[r][c]; }
inline Vector4 &Matrix::operator () (unsigned int r) { return reinterpret_cast<Vector4 &>(m[r * 4]); }
inline float *Matrix::operator[] (unsigned int r) { return m[r]; }

// Opérateurs + et -
inline Matrix Matrix::operator + () const { return *this; }
inline Matrix Matrix::operator - () const { return Matrix(-_11, -_12, -_13, -_14,
												          -_21, -_22, -_23, -_24,
												          -_31, -_32, -_33, -_34,
												          -_41, -_42, -_43, -_44); }

// Opérations avec une matrice
#ifndef CRWD_DIRECT3D
inline Matrix Matrix::operator * (const Matrix &matrix) const {
							#if defined(CRWDMATH_OPTIMIZE) && defined(_WIN32)
							Matrix out;
							return (Matrix&) *D3DXMatrixMultiply((D3DXMATRIX*) &out, (D3DXMATRIX*) this, (D3DXMATRIX*) &matrix);
							#else
							return Matrix(
							CMUL(1, 1, matrix), CMUL(1, 2, matrix), CMUL(1, 3, matrix), CMUL(1, 4, matrix),
							CMUL(2, 1, matrix), CMUL(2, 2, matrix), CMUL(2, 3, matrix), CMUL(2, 4, matrix),
							CMUL(3, 1, matrix), CMUL(3, 2, matrix), CMUL(3, 3, matrix), CMUL(3, 4, matrix),
							CMUL(4, 1, matrix), CMUL(4, 2, matrix), CMUL(4, 3, matrix), CMUL(4, 4, matrix));
							#endif
							}
#else

inline Matrix Matrix::operator * (const Matrix &matrix) const {
							Matrix out;
							return SMultiply(out, *this, matrix); // return MatrixMultiply(out, *this, matrix);
							}
#endif

inline Matrix Matrix::operator + (const Matrix &matrix) const { return Matrix(
											_11 + matrix._11, _12 + matrix._12, _13 + matrix._13, _14 + matrix._14,
											_21 + matrix._21, _22 + matrix._22, _23 + matrix._23, _24 + matrix._24,
											_31 + matrix._31, _32 + matrix._32, _33 + matrix._33, _34 + matrix._34,
											_41 + matrix._41, _42 + matrix._42, _43 + matrix._43, _44 + matrix._44);
											}

inline Matrix Matrix::operator - (const Matrix &matrix) const { return Matrix(
											_11 - matrix._11, _12 - matrix._12, _13 - matrix._13, _14 - matrix._14,
											_21 - matrix._21, _22 - matrix._22, _23 - matrix._23, _24 - matrix._24,
											_31 - matrix._31, _32 - matrix._32, _33 - matrix._33, _34 - matrix._34,
											_41 - matrix._41, _42 - matrix._42, _43 - matrix._43, _44 - matrix._44);
											}

// Opérations avec un vecteur
inline Vector4 Matrix::operator * (const Vector4 &v) const { return Vector4(
															_12*v.x + _12*v.y + _13*v.z + _14*v.w,
															_22*v.x + _22*v.y + _23*v.z + _24*v.w,
															_32*v.x + _32*v.y + _33*v.z + _34*v.w,
															_42*v.x + _42*v.y + _43*v.z + _44*v.w); }

// Opérations avec un float
inline Matrix Matrix::operator * (float f) const { return Matrix(
						_11 * f, _12 * f, _13 * f, _14 * f,
						_21 * f, _22 * f, _23 * f, _24 * f,
						_31 * f, _32 * f, _33 * f, _34 * f,
						_41 * f, _42 * f, _43 * f, _44 * f);
						}

inline Matrix Matrix::operator / (float f) const { return Matrix(
						_11 / f, _12 / f, _13 / f, _14 / f,
						_21 / f, _22 / f, _23 / f, _24 / f,
						_31 / f, _32 / f, _33 / f, _34 / f,
						_41 / f, _42 / f, _43 / f, _44 / f);
						}

// Opérations de comparaison
inline bool Matrix::operator == (const Matrix &matrix) const { return (
						EQUAL(1, 1, matrix) && EQUAL(1, 2, matrix) && EQUAL(1, 3, matrix) && EQUAL(1, 4, matrix) &&
						EQUAL(2, 1, matrix) && EQUAL(2, 2, matrix) && EQUAL(2, 3, matrix) && EQUAL(2, 4, matrix) &&
						EQUAL(3, 1, matrix) && EQUAL(3, 2, matrix) && EQUAL(3, 3, matrix) && EQUAL(3, 4, matrix) &&
						EQUAL(4, 1, matrix) && EQUAL(4, 2, matrix) && EQUAL(4, 3, matrix) && EQUAL(4, 4, matrix) );
						}

inline bool Matrix::operator != (const Matrix &matrix) const { return (
						NEQUAL(1, 1, matrix) || NEQUAL(1, 2, matrix) || NEQUAL(1, 3, matrix) || NEQUAL(1, 4, matrix) &&
						NEQUAL(2, 1, matrix) || NEQUAL(2, 2, matrix) || NEQUAL(2, 3, matrix) || NEQUAL(2, 4, matrix) &&
						NEQUAL(3, 1, matrix) || NEQUAL(3, 2, matrix) || NEQUAL(3, 3, matrix) || NEQUAL(3, 4, matrix) &&
						NEQUAL(4, 1, matrix) || NEQUAL(4, 2, matrix) || NEQUAL(4, 3, matrix) || NEQUAL(4, 4, matrix) );
						}

// Autres fonctions
inline Matrix &Matrix::Transpose()
{
	Transpose(*this);
	return *this;
}

// Fonctions statiques
inline void Matrix::Identity(Matrix &matrix)
{
	matrix._11 = 1.0f; matrix._12 = 0.0f; matrix._13 = 0.0f; matrix._14 = 0.0f;
	matrix._21 = 0.0f; matrix._22 = 1.0f; matrix._23 = 0.0f; matrix._24 = 0.0f;
	matrix._31 = 0.0f; matrix._32 = 0.0f; matrix._33 = 1.0f; matrix._34 = 0.0f;
	matrix._41 = 0.0f; matrix._42 = 0.0f; matrix._43 = 0.0f; matrix._44 = 1.0f;
}

inline Matrix Matrix::Identity()
{
	return Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
			);
}

inline void Matrix::Transpose(Matrix &matrix)
{
	Matrix t(matrix._11, matrix._21, matrix._31, matrix._41,
		    matrix._12, matrix._22, matrix._32, matrix._42,
		    matrix._13, matrix._23, matrix._33, matrix._43,
		    matrix._14, matrix._24, matrix._34, matrix._44
			);

	matrix = t;
}

inline Matrix &Matrix::Transpose(Matrix &out, const Matrix &in)
{
	TRANS(1, 1, out, in); TRANS(1, 2, out, in); TRANS(1, 3, out, in); TRANS(1, 4, out, in);
	TRANS(2, 1, out, in); TRANS(2, 2, out, in); TRANS(2, 3, out, in); TRANS(2, 4, out, in);
	TRANS(3, 1, out, in); TRANS(3, 2, out, in); TRANS(3, 3, out, in); TRANS(3, 4, out, in);
	TRANS(4, 1, out, in); TRANS(4, 2, out, in); TRANS(4, 3, out, in); TRANS(4, 4, out, in);

	return out;
}

inline Matrix &Matrix::Invert(Matrix &out, const Matrix &in)
{
	float d =	  (in._11*in._22*in._33*in._44) - (in._11*in._22*in._43*in._34) - (in._11*in._32*in._23*in._44) + (in._11*in._32*in._43*in._24) + (in._11*in._42*in._23*in._34) - (in._11*in._42*in._33*in._24)
				- (in._21*in._12*in._33*in._44) + (in._21*in._12*in._43*in._34) + (in._21*in._32*in._13*in._44) - (in._21*in._32*in._43*in._14) - (in._21*in._42*in._13*in._34) + (in._21*in._42*in._33*in._14)
				+ (in._31*in._12*in._23*in._44) - (in._31*in._12*in._43*in._24) - (in._31*in._22*in._13*in._44) + (in._31*in._22*in._43*in._14) + (in._31*in._42*in._13*in._24) - (in._31*in._42*in._23*in._14)
				- (in._41*in._12*in._23*in._34) + (in._41*in._12*in._33*in._24) + (in._41*in._22*in._13*in._34) - (in._41*in._22*in._33*in._14) - (in._41*in._32*in._13*in._24) + (in._41*in._32*in._23*in._14);

	if(d != 0) // d>0
	{
		out._11 = (in._22*in._33*in._44 - in._22*in._43*in._34 - in._32*in._23*in._44 + in._32*in._43*in._24 + in._42*in._23*in._34 - in._42*in._33*in._24)/d;
		out._21 = (-in._21*in._33*in._44 + in._21*in._43*in._34 + in._31*in._23*in._44 - in._31*in._43*in._24 - in._41*in._23*in._34 + in._41*in._33*in._24)/d;
		out._31 = (in._21*in._32*in._44 - in._21*in._42*in._34 - in._31*in._22*in._44 + in._31*in._42*in._24 + in._41*in._22*in._34 - in._41*in._32*in._24)/d;
		out._41 = (-in._21*in._32*in._43 + in._21*in._42*in._33 + in._31*in._22*in._43 - in._31*in._42*in._23 - in._41*in._22*in._33 + in._41*in._32*in._23)/d;

		out._12 = (-in._12*in._33*in._44 + in._12*in._43*in._34 + in._32*in._13*in._44 - in._32*in._43*in._14 - in._42*in._13*in._34 + in._42*in._33*in._14)/d;
		out._22 = (in._11*in._33*in._44 - in._11*in._43*in._34 - in._31*in._13*in._44 + in._31*in._43*in._14 + in._41*in._13*in._34 - in._41*in._33*in._14)/d;
		out._32 = (-in._11*in._32*in._44 + in._11*in._42*in._34 + in._31*in._12*in._44 - in._31*in._42*in._14 - in._41*in._12*in._34 + in._41*in._32*in._14)/d;
		out._42 = (in._11*in._32*in._43 - in._11*in._42*in._33 - in._31*in._12*in._43 + in._31*in._42*in._13 + in._41*in._12*in._33 - in._41*in._32*in._13)/d;

		out._13 = (in._12*in._23*in._44 - in._12*in._43*in._24 - in._22*in._13*in._44 + in._22*in._43*in._14 + in._42*in._13*in._24 - in._42*in._23*in._14)/d;
		out._23 = (-in._11*in._23*in._44 + in._11*in._43*in._24 + in._21*in._13*in._44 - in._21*in._43*in._14 - in._41*in._13*in._24 + in._41*in._23*in._14)/d;
		out._33 = (in._11*in._22*in._44 - in._11*in._42*in._24 - in._21*in._12*in._44 + in._21*in._42*in._14 + in._41*in._12*in._24 - in._41*in._22*in._14)/d;
		out._43 = (-in._11*in._22*in._43 + in._11*in._42*in._23 + in._21*in._12*in._43 - in._21*in._42*in._13 - in._41*in._12*in._23 + in._41*in._22*in._13)/d;

		out._14 = (-in._12*in._23*in._34 + in._12*in._33*in._24 + in._22*in._13*in._34 - in._22*in._33*in._14 - in._32*in._13*in._24 + in._32*in._23*in._14)/d;
		out._24 = (in._11*in._23*in._34 - in._11*in._33*in._24 - in._21*in._13*in._34 + in._21*in._33*in._14 + in._31*in._13*in._24 - in._31*in._23*in._14)/d;
		out._34 = (-in._11*in._22*in._34 + in._11*in._32*in._24 + in._21*in._12*in._34 - in._21*in._32*in._14 - in._31*in._12*in._24 + in._31*in._22*in._14)/d;
		out._44 = (in._11*in._22*in._33 - in._11*in._32*in._23 - in._21*in._12*in._33 + in._21*in._32*in._13 + in._31*in._12*in._23 - in._31*in._22*in._13)/d;

		/*out._11 = (f*k*p - f*o*l - j*g*p + j*o*h + n*g*l - n*k*h);
		out._21 = (-e*k*p + e*o*l + i*g*p - i*o*h - m*g*l + m*k*h);
		out._31 = (e*j*p - e*n*l - i*f*p + i*n*h + m*f*l - m*j*h);
		out._41 = (-e*j*o + e*n*k + i*f*o - i*n*g - m*f*k + m*j*g);

		out._12 = (-b*k*p + b*o*l + j*c*p - j*o*d - n*c*l + n*k*d);
		out._22 = (a*k*p - a*o*l - i*c*p + i*o*d + m*c*l - m*k*d);
		out._32 = (-a*j*p + a*n*l + i*b*p - i*n*d - m*b*l + m*j*d);
		out._42 = (a*j*o - a*n*k - i*b*o + i*n*c + m*b*k - m*j*c);

		out._13 = (b*g*p - b*o*h - f*c*p + f*o*d + n*c*h - n*g*d);
		out._23 = (-a*g*p + a*o*h + e*c*p - e*o*d - m*c*h + m*g*d);
		out._33 = (a*f*p - a*n*h - e*b*p + e*n*d + m*b*h - m*f*d);
		out._43 = (-a*f*o + a*n*g + e*b*o - e*n*c - m*b*g + m*f*c);

		out._14 = (-b*g*l + b*k*h + f*c*l - f*k*d - j*c*h + j*g*d);
		out._24 = (a*g*l - a*k*h - e*c*l + e*k*d + i*c*h - i*g*d);
		out._34 = (-a*f*l + a*j*h + e*b*l - e*j*d - i*b*h + i*f*d);
		out._44 = (a*f*k - a*j*g - e*b*k + e*j*c + i*b*g - i*f*c);*/
	}

	return out;
}

inline float Matrix::Determinant() const
{
	return      (_11*_22*_33*_44) - (_11*_22*_43*_34) - (_11*_32*_23*_44) + (_11*_32*_43*_24) + (_11*_42*_23*_34) - (_11*_42*_33*_24)
				- (_21*_12*_33*_44) + (_21*_12*_43*_34) + (_21*_32*_13*_44) - (_21*_32*_43*_14) - (_21*_42*_13*_34) + (_21*_42*_33*_14)
				+ (_31*_12*_23*_44) - (_31*_12*_43*_24) - (_31*_22*_13*_44) + (_31*_22*_43*_14) + (_31*_42*_13*_24) - (_31*_42*_23*_14)
				- (_41*_12*_23*_34) + (_41*_12*_33*_24) + (_41*_22*_13*_34) - (_41*_22*_33*_14) - (_41*_32*_13*_24) + (_41*_32*_23*_14);
}

// Fonctions propres à la 3D
inline void Matrix::InvertRotationMatrix(Matrix &matrix)
{
	Transpose(matrix);
}

inline Matrix &Matrix::InvertRotationMatrix(Matrix &out, const Matrix &in)
{
	Transpose(out, in);

	return out;
}

inline void Matrix::BuildTransformLH(Matrix &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up)
{
	// Rotation
	Vector3 z = Vector3::Normalize(zAxis);
	Vector3 x = Vector3::Normalize(Vector3::Cross(Up, z));
	Vector3 y = Vector3::Cross(z, x);

	// Matrice (redimensionnement, rotation, positionnement)
	Out._11 = Scale.x*x.x;		Out._12 = Scale.x*x.y;		Out._13 = Scale.x*x.z;		Out._14 = 0.0f;
	Out._21 = Scale.y*y.x;		Out._22 = Scale.y*y.y;		Out._23 = Scale.y*y.z;		Out._24 = 0.0f;
	Out._31 = Scale.z*z.x;		Out._32 = Scale.z*z.y;		Out._33 = Scale.z*z.z;		Out._34 = 0.0f;
	Out._41 = Translate.x;		Out._42 = Translate.y;		Out._43 = Translate.z;		Out._44 = 1.0f;
}

inline void Matrix::BuildTransformRH(Matrix &Out, const Vector3 &Translate, const Vector3 &Scale, const Vector3 &zAxis, const Vector3 &Up)
{
	// Rotation
	Vector3 z = Vector3::Normalize(zAxis);
	Vector3 x = Vector3::Normalize(Vector3::Cross(Up, z));
	Vector3 y = Vector3::Cross(z, x);

	// Matrice (redimensionnement, rotation, positionnement)
	Out._11 = Scale.x*x.x;		Out._12 = Scale.x*x.y;		Out._13 = Scale.x*x.z;		Out._14 = 0.0f;
	Out._21 = Scale.y*y.x;		Out._22 = Scale.y*y.y;		Out._23 = Scale.y*y.z;		Out._24 = 0.0f;
	Out._31 = Scale.z*z.x;		Out._32 = Scale.z*z.y;		Out._33 = Scale.z*z.z;		Out._34 = 0.0f;
	Out._41 = Translate.x;		Out._42 = Translate.y;		Out._43 = Translate.z;		Out._44 = 1.0f;
}

inline void Matrix::BuildViewLH(Matrix &Matrix, const Vector3 &Eye, float yAngle, float xAngle)
{
	Vector3 z = Vector3((float) cos(yAngle + CRWD_PI/2.0)*cos(xAngle), (float) sin(xAngle), (float) sin(yAngle + CRWD_PI/2.0)*cos(xAngle));
	Vector3 x = Vector3((float) cos(yAngle), 0.0f, (float) sin(yAngle));
	Vector3 y = Vector3::Cross(z, x);

	Matrix._11 = x.x;			Matrix._12 = y.x;			Matrix._13 = z.x;			Matrix._14 = 0.0f;
	Matrix._21 = x.y;			Matrix._22 = y.y;			Matrix._23 = z.y;			Matrix._24 = 0.0f;
	Matrix._31 = x.z;			Matrix._32 = y.z;			Matrix._33 = z.z;			Matrix._34 = 0.0f;
	Matrix._41 = -Eye*x;		Matrix._42 = -Eye*y;		Matrix._43 = -Eye*z;		Matrix._44 = 1.0f;
}

inline void Matrix::BuildViewLH(Matrix &Matrix, const Vector3 &Eye, const Vector3 &LookAt, const Vector3 &Up)
{
	Vector3 z = Vector3::Normalize(LookAt - Eye);
	Vector3 x = Vector3::Normalize(Vector3::Cross(Up, z));
	Vector3 y = Vector3::Cross(z, x);

	Matrix._11 = x.x;			Matrix._12 = y.x;			Matrix._13 = z.x;			Matrix._14 = 0.0f;
	Matrix._21 = x.y;			Matrix._22 = y.y;			Matrix._23 = z.y;			Matrix._24 = 0.0f;
	Matrix._31 = x.z;			Matrix._32 = y.z;			Matrix._33 = z.z;			Matrix._34 = 0.0f;
	Matrix._41 = -Eye*x;		Matrix._42 = -Eye*y;		Matrix._43 = -Eye*z;		Matrix._44 = 1.0f;
}

inline void Matrix::BuildViewRH(Matrix &Matrix, const Vector3 &Eye, const Vector3 &LookAt, const Vector3 &Up)
{
	Vector3 z = Vector3::Normalize(Eye - LookAt);
	Vector3 x = Vector3::Normalize(Vector3::Cross(Up, z));
	Vector3 y = Vector3::Cross(z, x);

	Matrix._11 = x.x;			Matrix._12 = y.x;			Matrix._13 = z.x;			Matrix._14 = 0.0f;
	Matrix._21 = x.y;			Matrix._22 = y.y;			Matrix._23 = z.y;			Matrix._24 = 0.0f;
	Matrix._31 = x.z;			Matrix._32 = y.z;			Matrix._33 = z.z;			Matrix._34 = 0.0f;
	Matrix._41 = -Eye*x;		Matrix._42 = -Eye*y;		Matrix._43 = -Eye*z;		Matrix._44 = 1.0f;
}

inline void Matrix::BuildProjectionLH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MinDepth, float MaxDepth)
{
	float tanViewAngle = tan(ViewAngle / 2.0f);

	Matrix._11 = 1.0f / (AspectRatio*tanViewAngle);	Matrix._12 = 0.0f;					Matrix._13 = 0.0f;									Matrix._14 = 0.0f;
	Matrix._21 = 0.0f;							Matrix._22 = 1.0f / tanViewAngle;		Matrix._23 = 0.0f;									Matrix._24 = 0.0f;
	Matrix._31 = 0.0f;							Matrix._32 = 0.0f;					Matrix._33 = MaxDepth / (MaxDepth - MinDepth);			Matrix._34 = 1.0f;
	Matrix._41 = 0.0f;							Matrix._42 = 0.0f;					Matrix._43 = -MaxDepth*MinDepth / (MaxDepth - MinDepth);	Matrix._44 = 0.0f;
}

inline void Matrix::BuildProjectionRH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MinDepth, float MaxDepth)
{
	float tanViewAngle = tan(ViewAngle / 2.0f);

	Matrix._11 = 1.0f / (AspectRatio*tanViewAngle);	Matrix._12 = 0.0f;					Matrix._13 = 0.0f;									Matrix._14 = 0.0f;
	Matrix._21 = 0.0f;							Matrix._22 = 1.0f / tanViewAngle;		Matrix._23 = 0.0f;									Matrix._24 = 0.0f;
	Matrix._31 = 0.0f;							Matrix._32 = 0.0f;					Matrix._33 = MaxDepth / (MaxDepth - MinDepth);			Matrix._34 = -1.0f;
	Matrix._41 = 0.0f;							Matrix._42 = 0.0f;					Matrix._43 = MaxDepth*MinDepth / (MaxDepth - MinDepth);	Matrix._44 = 0.0f;
}

inline void Matrix::BuildCrowdProjectionLH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MaxDepth)
{
	float tanViewAngle = tan(ViewAngle / 2.0f);

	Matrix._11 = 1.0f / (AspectRatio*tanViewAngle);	Matrix._12 = 0.0f;					Matrix._13 = 0.0f;			Matrix._14 = 0.0f;
	Matrix._21 = 0.0f;							Matrix._22 = 1.0f / tanViewAngle;		Matrix._23 = 0.0f;			Matrix._24 = 0.0f;
	Matrix._31 = 0.0f;							Matrix._32 = 0.0f;					Matrix._33 = 1.0f;			Matrix._34 = 0.0f;
	Matrix._41 = 0.0f;							Matrix._42 = 0.0f;					Matrix._43 = 0.0f;			Matrix._44 = MaxDepth;
}

inline void Matrix::BuildCrowdProjectionRH(Matrix &Matrix, float ViewAngle, float AspectRatio, float MaxDepth)
{
	float tanViewAngle = tan(ViewAngle / 2.0f);

	Matrix._11 = 1.0f / (AspectRatio*tanViewAngle);	Matrix._12 = 0.0f;					Matrix._13 = 0.0f;			Matrix._14 = 0.0f;
	Matrix._21 = 0.0f;							Matrix._22 = 1.0f / tanViewAngle;		Matrix._23 = 0.0f;			Matrix._24 = 0.0f;
	Matrix._31 = 0.0f;							Matrix._32 = 0.0f;					Matrix._33 = -1.0f;			Matrix._34 = 0.0f;
	Matrix._41 = 0.0f;							Matrix._42 = 0.0f;					Matrix._43 = 0.0f;			Matrix._44 = MaxDepth;
}

inline void Matrix::BuildTranslationMatrix(Matrix &Matrix, Vector3 Translation)
{
	Matrix._11 = 1.0f;				Matrix._12 = 0.0f;				Matrix._13 = 0.0f;				Matrix._14 = 0.0f;
	Matrix._21 = 0.0f;				Matrix._22 = 1.0f;				Matrix._23 = 0.0f;				Matrix._24 = 0.0f;
	Matrix._31 = 0.0f;				Matrix._32 = 0.0f;				Matrix._33 = 1.0f;				Matrix._34 = 0.0f;
	Matrix._41 = Translation.x;		Matrix._42 = Translation.y;		Matrix._43 = Translation.z;		Matrix._44 = 1.0f;
}

inline void Matrix::BuildTranslationMatrix(Matrix &Matrix, float x, float y, float z)
{
	Matrix._11 = 1.0f;		Matrix._12 = 0.0f;		Matrix._13 = 0.0f;		Matrix._14 = 0.0f;
	Matrix._21 = 0.0f;		Matrix._22 = 1.0f;		Matrix._23 = 0.0f;		Matrix._24 = 0.0f;
	Matrix._31 = 0.0f;		Matrix._32 = 0.0f;		Matrix._33 = 1.0f;		Matrix._34 = 0.0f;
	Matrix._41 = x;			Matrix._42 = y;			Matrix._43 = z;			Matrix._44 = 1.0f;
}

inline void Matrix::BuildScaleMatrix(Matrix &Matrix, float xScale, float yScale, float zScale)
{
	Matrix._11 = xScale;		Matrix._12 = 0;				Matrix._13 = 0;				Matrix._14 = 0;
	Matrix._21 = 0;				Matrix._22 = yScale;		Matrix._23 = 0;				Matrix._24 = 0;
	Matrix._31 = 0;				Matrix._32 = 0;				Matrix._33 = zScale;		Matrix._34 = 0;
	Matrix._41 = 0;				Matrix._42 = 0;				Matrix._43 = 0;				Matrix._44 = 1.0f;
}

inline Matrix &Matrix::Multiply(Matrix &out, const Matrix &m1, const Matrix &m2)
{
	return SMultiply(out, m1, m2);
}

/*inline Matrix &Matrix::SMultiply(Matrix &out, const Matrix &m1, const Matrix &m2)
{
	out = Matrix(
							CMUL_2(1, 1), CMUL_2(1, 2), CMUL_2(1, 3), CMUL_2(1, 4),
							CMUL_2(2, 1), CMUL_2(2, 2), CMUL_2(2, 3), CMUL_2(2, 4),
							CMUL_2(3, 1), CMUL_2(3, 2), CMUL_2(3, 3), CMUL_2(3, 4),
							CMUL_2(4, 1), CMUL_2(4, 2), CMUL_2(4, 3), CMUL_2(4, 4));

	return out;
}*/

inline Vector4 &Matrix::Multiply(Vector4 &out, const Matrix &m, const Vector4 &v)
{
	out = Vector4(	m._11*v.x + m._12*v.y + m._13*v.z + m._14*v.w,
					m._21*v.x + m._22*v.y + m._13*v.z + m._24*v.w,
					m._31*v.x + m._32*v.y + m._13*v.z + m._34*v.w,
					m._41*v.x + m._42*v.y + m._13*v.z + m._44*v.w);
	return out;
}


// Produit d'un vecteur et d'une matrice
inline Vector4 operator * (const Vector4 &v, const Matrix &m)
{
	return Vector4(
					v.x*m._11 + v.y*m._21 + v.z*m._31 + v.w*m._41,
					v.x*m._12 + v.y*m._22 + v.z*m._32 + v.w*m._42,
					v.x*m._13 + v.y*m._23 + v.z*m._33 + v.w*m._43,
					v.x*m._14 + v.y*m._24 + v.z*m._34 + v.w*m._44
					);
}

// Input/Output
inline std::istream &operator >> (std::istream &stream, Matrix &matrix) {
										stream >> matrix._11 >> matrix._12 >> matrix._13 >> matrix._14
										>> matrix._21 >> matrix._22 >> matrix._23 >> matrix._24
										>> matrix._31 >> matrix._32 >> matrix._33 >> matrix._34
										>> matrix._41 >> matrix._42 >> matrix._43 >> matrix._44;
										return stream;
										}

inline std::ostream &operator << (std::ostream &stream, const Matrix &matrix) {
										stream << matrix._11 << " " << matrix._12 << " " << matrix._13 << " " << matrix._14 << std::endl
										<< matrix._21 << " " << matrix._22 << " " << matrix._23 << " " << matrix._24 << std::endl
										<< matrix._31 << " " << matrix._32 << " " << matrix._33 << " " << matrix._34 << std::endl
										<< matrix._41 << " " << matrix._42 << " " << matrix._43 << " " << matrix._44 << std::endl;
										return stream;
										}

} // namespace Crowd

#undef CMUL
#undef PLUS
#undef MINUS
#undef EQUAL
#undef NEQUAL
#undef TRANS

#endif // CRWDMATRIX_INL_INCLUDED