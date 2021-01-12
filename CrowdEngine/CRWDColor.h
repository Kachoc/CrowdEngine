#ifndef CRWDCOLOR_H_INCLUDED
#define CRWDCOLOR_H_INCLUDED

#include "CRWDVector.h"

namespace Crowd
{

struct ColorRGB;
struct ColorRGBA;

typedef ColorRGBA Color;

struct ColorRGB
{
	//
	//	Fonctions
	//

	// Constructeurs
	ColorRGB() : r(0.0f), g(0.0f), b(0.0f) {}
	ColorRGB(const ColorRGB &c) : r(c.r), g(c.g), b(c.b) {}
	ColorRGB(float r, float g, float b) : r(r), g(g), b(b) {}
	ColorRGB(const float *f) : r(f[0]), g(f[1]), b(f[2]) {}

	// Opérateurs d'assignement
	ColorRGB &operator = (const ColorRGB &);
	ColorRGB &operator = (const Vector3 &);

	ColorRGB &operator += (const ColorRGB &);
	ColorRGB &operator -= (const ColorRGB &);

	ColorRGB &operator *= (const ColorRGB &);
	ColorRGB &operator /= (const ColorRGB &);

	ColorRGB &operator *= (float );
	ColorRGB &operator /= (float );

	// Opérateurs unaires
	ColorRGB operator + () const;
	ColorRGB operator - () const;

	// Opérateurs binaires
	ColorRGB operator + (const ColorRGB &) const;
	ColorRGB operator - (const ColorRGB &) const;

	ColorRGB operator * (const ColorRGB &) const;
	ColorRGB operator / (const ColorRGB &) const;

	ColorRGB operator * (float ) const;
	ColorRGB operator / (float ) const;

	// Opérateurs de comparaison
	bool operator == (const ColorRGB &) const;
	bool operator != (const ColorRGB &) const;

	// Autres fonctions
	float GetValue() const; // luminosité

	//
	//	Attributs
	//
	float r, g, b;
};

struct ColorRGBA
{
	//
	//	Fonctions
	//

	// Constructeurs
	ColorRGBA() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
	ColorRGBA(const ColorRGB &c, float a) : r(c.r), g(c.g), b(c.b), a(a) {}
	ColorRGBA(const ColorRGBA &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
	ColorRGBA(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	ColorRGBA(float *f) : r(f[0]), g(f[1]), b(f[2]), a(f[3]) {}

	// Opérateurs d'assignement
	ColorRGBA &operator = (const ColorRGBA &);
	ColorRGBA &operator = (const Vector4 &);

	ColorRGBA &operator += (const ColorRGBA &);
	ColorRGBA &operator -= (const ColorRGBA &);

	ColorRGBA &operator *= (const ColorRGBA &);
	ColorRGBA &operator /= (const ColorRGBA &);

	ColorRGBA &operator *= (float );
	ColorRGBA &operator /= (float );

	// Opérateurs unaires
	ColorRGBA operator + () const;
	ColorRGBA operator - () const;

	// Opérateurs binaires
	ColorRGBA operator + (const ColorRGBA &) const;
	ColorRGBA operator - (const ColorRGBA &) const;

	ColorRGBA operator * (const ColorRGBA &) const;
	ColorRGBA operator / (const ColorRGBA &) const;

	ColorRGBA operator * (float ) const;
	ColorRGBA operator / (float ) const;

	// Opérateurs de comparaison
	bool operator == (const ColorRGBA &) const;
	bool operator != (const ColorRGBA &) const;

	//Opérateurs de cast
	operator float *();
	operator const float *() const;

	// Autres fonctions
	float GetValue() const; // luminosité

	//
	//	Attributs
	//
	float r, g, b, a;
};

} // namespace Crowd

#include "CRWDColor.inl"

#endif // CRWDCOLOR_H_INCLUDED