#ifndef CRWDMATH_H_INCLUDED
#define CRWDMATH_H_INCLUDED

#include <cmath>
#include <limits>
#include <random>

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef min3
#define min3(a, b, c) min(a, min(b, c))
#endif

#ifndef min4
#define min4(a, b, c, d) min3(a, b, min(c, d))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef max3
#define max3(a, b, c) max(a, max(b, c))
#endif

#ifndef max4
#define max4(a, b, c, d) max3(a, b, max(c, d))
#endif

#ifndef CRWD_PI
#define CRWD_PI 3.1415926535897932
#endif

#include "CRWDVector.h"
#include "CRWDPoint.h"
#include "CRWDPlane.h"
#include "CRWDQuaternion.h"
#include "CRWDMatrix.h"
#include "CRWDVolumes.h"
#include "CRWDSurfaces.h"
#include "CRWDColor.h"
#include "CRWDFunctions.h"
#include "CRWDPlane.h"
#include "CRWDTree.h"
#include "CRWDRectangle.h"
#include "CRWDPolynome.h"

#include "CRWDIntersection.h"
#include "CRWDTgtBitangent.h"

#include "CRWDPerlinNoise.h"
#include "CRWDValueNoise.h"

namespace Crowd
{

float MainMeasurement(float angle);
float RandomFloat();

} // namespace Crowd

#include "CRWDMath.inl"

#if defined(CRWD_MIN_DEFINED)
	#undef min
	#undef CRWD_MIN_DEFINED
#endif

#if defined(CRWD_MAX_DEFINED)
	#undef max
	#undef CRWD_MAX_DEFINED
#endif

#endif // CRWDMATH_H_INCLUDED