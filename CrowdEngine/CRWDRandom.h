#ifndef CRWD_RANDOM_H_INCLUDED
#define CRWD_RANDOM_H_INCLUDED

#include <random>
#include "CRWDVector.h"

#ifndef CRWD_PI
#define CRWD_PI 3.1415926535897932
#endif

namespace Crowd
{

class Random
{
	public:
	Random(unsigned long seed);

	void Seed(unsigned long seed);

	long Max() const;
	long Min() const;

	unsigned long GenerateULONG();
	float GenerateFloat();
	double GenerateDouble();
	void GenerateVector2F(Vector2F &v);
	void GenerateVector2D(Vector2D &v);
	void GenerateVector3F(Vector3F &v);
	void GenerateVector3D(Vector3D &v);

	protected:
	std::mt19937 m_generator;
};

} // namespace Crowd

#endif // CRWD_RANDOM_H_INCLUDED