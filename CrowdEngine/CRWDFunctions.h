#ifndef CRWD_FUNCTIONS_H_INCLUDED
#define CRWD_FUNCTIONS_H_INCLUDED

namespace Crowd
{

class SquareFunction
{
	public:
	SquareFunction() : a(0.0f), b(0.0f), c(0.0f) {}
	SquareFunction(float a, float b, float c, float d) : a(a), b(b), c(c) {}

	void SetCoef(float a1, float b1, float c1) { a = a1; b = b1; c = c1; }
	void CalculateCoef(float y0, float y1, float y2) // y0: image de la fonction en 0; y1: image en 1; y2: image en 2;
	{
		c = y0;
		a = (y2 - 2*y1 + c)/2.0f;
		b = y1 - a - c;
	}

	float operator()(float x)
	{
		return a*x*x + b*x + c;
	}

	float a, b, c; // coefficients
};

class CubicFunction
{
	public:
	CubicFunction() : a(0.0f), b(0.0f), c(0.0f), d(0.0f) {}
	CubicFunction(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}

	void SetCoef(float a1, float b1, float c1, float d1) { a = a1; b = b1; c = c1; d = d1; }
	void CalculateCoef(float y, float y0, float y1, float y2) // y: image de la fonction en -1; y0: image en 0; y1: image en 1; y2: image en 2
	{
		d = y0;
		b = (y1 + y - 2*d)/2.0f;
		a = (y2 - 2*y1 + d - 2*b)/6.0f;
		c = y1 - a - b - d;
	}

	float operator()(float x)
	{
		float x2 = x*x;
		return a*x*x2 + b*x2 + c*x + d;
	}

	float a, b, c, d; // coefficients
};

} // namespace Crowd

#include "CRWDFunctions.inl"

#endif // CRWD_FUNCTIONS_H_INCLUDED