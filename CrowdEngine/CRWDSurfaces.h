#ifndef CRWDSURFACE_H_INCLUDED
#define CRWDSURFACE_H_INCLUDED

#include "CRWDVector.h"

namespace Crowd
{

struct Rect;
struct Disc;

struct Rect
{
	float Left, Right, Top, Bottom;
};

struct Disc
{
	Vector2 Center;
	float Radius;
};

} // namespace Crowd

#include "CRWDSurface.inl"

#endif // CRWDSURFACE_H_INCLUDED