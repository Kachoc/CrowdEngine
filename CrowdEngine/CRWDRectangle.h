#ifndef CRWDRECTANGLE_H_INCLUDED
#define CRWDRECTANGLE_H_INCLUDED

namespace Crowd
{

struct Rectangle
{
	public:
	Rectangle() {}
	Rectangle(float top, float bottom, float left, float right) : top(top), bottom(bottom), left(left), right(right) {}
	~Rectangle() {}

	public:
	float top;
	float bottom;
	float left;
	float right;
};

} // namespace Crowd

#include "CRWDRectangle.inl"

#endif // CRWDRECTANGLE_H_INCLUDED