#include "CRWDRandom.h"

namespace Crowd
{

Random::Random(unsigned long seed) : m_generator(seed)
{

}

void Random::Seed(unsigned long s)
{
	m_generator.seed(s);
}

long Random::Max() const
{
	return m_generator.max();
}

long Random::Min() const
{
	return m_generator.min();
}

unsigned long Random::GenerateULONG()
{
	return m_generator();
}

float Random::GenerateFloat()
{
	unsigned long amplitude = m_generator.max() - m_generator.min();
	return ((float) m_generator() / (float) amplitude)*0.5f-0.5f;
}

double Random::GenerateDouble()
{
	unsigned long amplitude = m_generator.max() - m_generator.min();
	return ((double) m_generator() / (double) amplitude)*0.5-0.5;
}

void Random::GenerateVector2F(Vector2F &v)
{
	unsigned long amplitude = m_generator.max() - m_generator.min();
	double angle = ((double) m_generator() / (double) amplitude)*CRWD_PI;

	v.x = (float) cos(angle);
	v.y = (float) sin(angle);
}

void Random::GenerateVector2D(Vector2D &v)
{
	unsigned long amplitude = m_generator.max() - m_generator.min();
	double angle = ((double) m_generator() / (double) amplitude)*CRWD_PI;

	v.x = cos(angle);
	v.y = sin(angle);
}

void Random::GenerateVector3F(Vector3F &v)
{
	double theta = GenerateDouble()*2.0*CRWD_PI;
	double sinphi = GenerateDouble();
	double phi = asin(sinphi);

}

void Random::GenerateVector3D(Vector3D &v)
{

}

} // namespace Crowd