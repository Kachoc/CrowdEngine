#ifndef CRWDCOLOR_INL_INCLUDED
#define CRWDCOLOR_INL_INCLUDED

#define EQUAL(x, color) abs(x - color.x) <= std::numeric_limits<float>::epsilon()
#define NEQUAL(x, color) abs(x - color.x) > std::numeric_limits<float>::epsilon()

namespace Crowd
{


//
//	ColorRGB
//

// Opérateurs d'assignement
inline ColorRGB &ColorRGB::operator = (const ColorRGB &color) { r = color.r; g = color.g; b = color.b; return *this; }
inline ColorRGB &ColorRGB::operator = (const Vector3 &vector) { r = vector.x; g = vector.y; b = vector.z; return *this; }

inline ColorRGB &ColorRGB::operator += (const ColorRGB &color) { r += color.r, g += color.g, b += color.b; return *this; }
inline ColorRGB &ColorRGB::operator -= (const ColorRGB &color) { r -= color.r, g -= color.g, b -= color.b; return *this; }

inline ColorRGB &ColorRGB::operator *= (const ColorRGB &color) { r *= color.r, g *= color.g, b *= color.b; return *this; }
inline ColorRGB &ColorRGB::operator /= (const ColorRGB &color) { r /= color.r, g /= color.g, b /= color.b; return *this; }

inline ColorRGB &ColorRGB::operator *= (float f) { r *= f; g *= f; b *= f; return *this; }
inline ColorRGB &ColorRGB::operator /= (float f) { r /= f; g /= f; b /= f; return *this; }

// Opérateurs unaires
inline ColorRGB ColorRGB::operator + () const { return ColorRGB(r, g, b); }
inline ColorRGB ColorRGB::operator - () const { return ColorRGB(-r, -g, -b); }

// Opérateurs binaires
inline ColorRGB ColorRGB::operator + (const ColorRGB &color) const { return ColorRGB(r + color.r, g + color.g, b + color.b); }
inline ColorRGB ColorRGB::operator - (const ColorRGB &color) const { return ColorRGB(r - color.r, g - color.g, b - color.b); }

inline ColorRGB ColorRGB::operator * (const ColorRGB &color) const { return ColorRGB(r * color.r, g * color.g, b * color.b); }
inline ColorRGB ColorRGB::operator / (const ColorRGB &color) const { return ColorRGB(r / color.r, g / color.g, b / color.b); }

inline ColorRGB ColorRGB::operator * (float f) const { return ColorRGB(r * f, g * f, b * f); }
inline ColorRGB ColorRGB::operator / (float f) const { return ColorRGB(r / f, g / f, b / f); }

// Opérateurs de comparaison
inline bool ColorRGB::operator == (const ColorRGB &color) const { return (EQUAL(r, color) && EQUAL(g, color) && EQUAL(b, color)); }
inline bool ColorRGB::operator != (const ColorRGB &color) const { return (NEQUAL(r, color) || NEQUAL(g, color) || NEQUAL(b, color)); }

// Autres fonctions
inline float ColorRGB::GetValue() const { return (r + g + b) / 3.0f; }


//
//	ColorRGBA
//

// Opérateurs d'assignement
inline ColorRGBA &ColorRGBA::operator = (const ColorRGBA &color) { r = color.r, g = color.g, b = color.b, a = color.a; return *this; }
inline ColorRGBA &ColorRGBA::operator = (const Vector4 &vector) { r = vector.x; g = vector.y; b = vector.z; a = vector.w; return *this; }

inline ColorRGBA &ColorRGBA::operator += (const ColorRGBA &color) { r += color.r, g += color.g, b += color.b, a += color.a; return *this; }
inline ColorRGBA &ColorRGBA::operator -= (const ColorRGBA &color) { r -= color.r, g -= color.g, b -= color.b, a -= color.a; return *this; }

inline ColorRGBA &ColorRGBA::operator *= (const ColorRGBA &color) { r *= color.r, g *= color.g, b *= color.b, a *= color.a; return *this; }
inline ColorRGBA &ColorRGBA::operator /= (const ColorRGBA &color) { r /= color.r, g /= color.g, b /= color.b, a /= color.a; return *this; }

inline ColorRGBA &ColorRGBA::operator *= (float f) { r *= f; g *= f; b *= f; a *= f; return *this; }
inline ColorRGBA &ColorRGBA::operator /= (float f) { r /= f; g /= f; b /= f; a /= f; return *this; }

// Opérateurs unaires
inline ColorRGBA ColorRGBA::operator + () const { return ColorRGBA(r, g, b, a); }
inline ColorRGBA ColorRGBA::operator - () const { return ColorRGBA(-r, -g, -b, -a); }

// Opérateurs binaires
inline ColorRGBA ColorRGBA::operator + (const ColorRGBA &color) const { return ColorRGBA(r + color.r, g + color.g, b + color.b, a + color.a); }
inline ColorRGBA ColorRGBA::operator - (const ColorRGBA &color) const { return ColorRGBA(r - color.r, g - color.g, b - color.b, a - color.a); }

inline ColorRGBA ColorRGBA::operator * (const ColorRGBA &color) const { return ColorRGBA(r * color.r, g * color.g, b * color.b, a * color.a); }
inline ColorRGBA ColorRGBA::operator / (const ColorRGBA &color) const { return ColorRGBA(r / color.r, g / color.g, b / color.b, a / color.a); }

inline ColorRGBA ColorRGBA::operator * (float f) const { return ColorRGBA(r * f, g * f, b * f, a * f); }
inline ColorRGBA ColorRGBA::operator / (float f) const { return ColorRGBA(r / f, g / f, b / f, a / f); }

// Opérateurs de comparaison
inline bool ColorRGBA::operator == (const ColorRGBA &color) const { return (EQUAL(r, color) && EQUAL(g, color) && EQUAL(b, color) && EQUAL(a, color)); }
inline bool ColorRGBA::operator != (const ColorRGBA &color) const { return (NEQUAL(r, color) || NEQUAL(g, color) || NEQUAL(b, color) || NEQUAL(a, color)); }

// Opérateurs de cast
inline ColorRGBA::operator float *() { return (float*)this; }
inline ColorRGBA::operator const float *() const { return (const float*)this; }

// Autres fonctions
inline float ColorRGBA::GetValue() const { return (r + g + b) / 3.0f; }

} // namespace Crowd

// Macros
#undef EQUAL
#undef NEQUAL

#endif // CRWDCOLOR_INL_INCLUDED