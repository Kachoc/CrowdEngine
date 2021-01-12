#include "CRWDPerlinNoise.h"

#include "CRWDMath.h"

#include <random>

namespace Crowd
{

std::mt19937	PerlinNoise::m_generator;

void PerlinNoise::SetSeed(unsigned long seed)
{
	m_generator = std::mt19937(seed);
}

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

void PerlinNoise::GeneratePoint(Vector2D &point)
{
	unsigned long amplitude = m_generator.max() - m_generator.min();
	double angle = ((double) m_generator() / (double) amplitude)*2.0*CRWD_PI;

	point.x = cos(angle);
	point.y = sin(angle);
}

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

void PerlinNoise::GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, const std::vector<Vector2D> &frequencies, const std::vector<float> &amplitudes, std::vector<float> &grid)
{
	grid.resize(resolutionX*resolutionY);

	for(unsigned int i = 0; i < resolutionX*resolutionY; i++)
	{
		grid[i] = 0;
	}

	for(unsigned int i = 0; i < frequencies.size(); i++)
	{
		unsigned int gradGridX = max((unsigned int) ((double)resolutionX*frequencies[i].x), 1);
		unsigned int gradGridY = max((unsigned int) ((double)resolutionY*frequencies[i].y), 1);

		GradientGrid gradientGrid(gradGridX, gradGridY);

		for(unsigned int x  = 0; x < resolutionX; x++)
		{
			for(unsigned int y = 0; y < resolutionY; y++)
			{
				double coordX = (double)x/(double)(resolutionX-1);
				double coordY = (double)y/(double)(resolutionY-1);

				grid[x*resolutionY+y] += (float)gradientGrid.GenerateHeight(coordX, coordY)*amplitudes[i]*0.5f;
			}
		}
	}
}

void PerlinNoise::GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, unsigned int period, unsigned int numOctaves, PerlinNoise::AmplitudeFunction amplitude, std::vector<float> &grid)
{
	std::vector<Vector2D> frequencies;
	std::vector<float> amplitudes;

	frequencies.resize(numOctaves);
	amplitudes.resize(numOctaves);

	double fundamentalFreqX = (double)resolutionX / (double)period;
	double fundamentalFreqY = (double)resolutionY / (double)period;

	for(unsigned int i = 0; i < numOctaves; i++)
	{
		frequencies[i].x = (double)(i+1)*fundamentalFreqX;
		frequencies[i].y = (double)(i+1)*fundamentalFreqY;

		amplitudes[i] = amplitude(i+1);
	}

	GenerateGrid(resolutionX, resolutionY, frequencies, amplitudes, grid);
}


//
//	GradientGrid
//

PerlinNoise::GradientGrid::GradientGrid(unsigned int width, unsigned int height)
{
	m_grid.resize((width+1)*(height+1));
	m_resolutionX = width+1;
	m_resolutionY = height+1;

	for(unsigned int i = 0; i < m_resolutionX-1; i++)
	{
		for(unsigned int j = 0; j < m_resolutionY-1; j++)
		{
			GeneratePoint(m_grid[m_resolutionY*i + j]);
		}
	}

	unsigned int i = m_resolutionX-1;
	unsigned int j;
	for(j = 0; j < m_resolutionY-1; j++)
	{
		m_grid[m_resolutionY*i + j] = m_grid[j];
	}

	j = m_resolutionY-1;
	for(i = 0; i < m_resolutionX; i++)
	{
		m_grid[m_resolutionY*i + j] = m_grid[m_resolutionY*i];
	}
}

double PerlinNoise::GradientGrid::GenerateHeight(double x, double y)
{
	x = x*(double) (m_resolutionX-1);
	y = y*(double) (m_resolutionY-1);

	unsigned int Xi = (unsigned int) x;
	unsigned int Yi = (unsigned int) y;

	double dx = x-(double) Xi;
	double dy = y-(double) Yi;
	double v0 = DotProduct(Xi, Yi, dx, dy);
	double v1 = DotProduct(Xi+1, Yi, dx-1.0, dy);
	double v2 = DotProduct(Xi, Yi+1, dx, dy-1.0);
	double v3 = DotProduct(Xi+1, Yi+1, dx-1.0, dy-1.0);

	double v4 = InterpolateQuadratic(v0, v1, dx);
	double v5 = InterpolateQuadratic(v2, v3, dx);
	return InterpolateQuadratic(v4, v5, dy);
}

double PerlinNoise::GradientGrid::DotProduct(int Xi, int Yi, double dx, double dy)
{
	Xi = max( min(Xi, (int)m_resolutionX-1), 0);
	Yi = max( min(Yi, (int)m_resolutionY-1), 0);

	unsigned int i = Xi*m_resolutionX+Yi;
	return dx*m_grid[i].x + dy*m_grid[i].y;
}

double PerlinNoise::GradientGrid::InterpolateLinear(double u, double v, double coef)
{
	return u*(1-coef)+v*coef;
}

double PerlinNoise::GradientGrid::InterpolateQuadratic(double u, double v, double coef)
{
	double weight = coef*coef*(3.0-2.0*coef);
	return u*(1-weight)+v*weight;
}

} // namespace Crowd