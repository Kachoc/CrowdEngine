#ifndef CRWDMATH_INL_INCLUDED
#define CRWDMATH_INL_INCLUDED

namespace Crowd
{

inline float MainMeasurement(float angle)
{
	int k = int((angle + ((angle < 0) ? -(float)CRWD_PI : (float)CRWD_PI))/2.0/(float)CRWD_PI);
	float ret = angle-2*k*(float)CRWD_PI;

	return ((angle == -(float)CRWD_PI) ? (float)CRWD_PI : angle);
}

inline float RandomFloat()
{
	return (float) rand() / (float) RAND_MAX;
}

} // namespace Crowd

#endif // CRWDMATH_INL_INCLUDED