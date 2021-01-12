#ifndef CRWD_VERTEX_H_INCLUDED
#define CRWD_VERTEX_H_INCLUDED

#include "CRWDMath.h"

namespace Crowd
{

struct PNVertex
{
	Vector3 Position;
	Vector3 Normal;
};

struct PNUVVertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 UV;
};

struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector3 Binormal;
	Vector3 Tangent;
	Vector2 UV;
};

} // namespace Crowd

#endif // CRWD_VERTEX_H_INCLUDED