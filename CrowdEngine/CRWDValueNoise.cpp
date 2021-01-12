#include "CRWDValueNoise.h"

namespace Crowd
{

std::mt19937	ValueNoise::m_generator;

double ValueNoise::GeneratePoint()
{
	unsigned long amplitude = m_generator.max() - m_generator.min();
	return ((double) m_generator() / (double) amplitude)*0.5-0.5;
}

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

void ValueNoise::GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, const std::vector<Vector2D> &frequencies, const std::vector<float> &amplitudes, std::vector<float> &grid)
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

		RandomGrid randomGrid(gradGridX, gradGridY);

		for(unsigned int x  = 0; x < resolutionX; x++)
		{
			for(unsigned int y = 0; y < resolutionY; y++)
			{
				double coordX = (double)x/(double)(resolutionX-1);
				double coordY = (double)y/(double)(resolutionY-1);

				grid[x*resolutionY+y] += (float)randomGrid.GenerateHeight(coordX, coordY)*amplitudes[i]*0.5f;
			}
		}
	}
}

void ValueNoise::GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, unsigned int period, unsigned int numOctaves, ValueNoise::AmplitudeFunction amplitude, std::vector<float> &grid)
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

ValueNoise::RandomGrid::RandomGrid(unsigned int width, unsigned int height)
{
	m_grid.resize((width+1)*(height+1));
	m_resolutionX = width+1;
	m_resolutionY = height+1;

	for(unsigned int i = 0; i < m_resolutionX-1; i++)
	{
		for(unsigned int j = 0; j < m_resolutionY-1; j++)
		{
			m_grid[m_resolutionY*i + j] = GeneratePoint();
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

double ValueNoise::RandomGrid::GenerateHeight(double x, double y)
{
	x = x*(double) (m_resolutionX-1);
	y = y*(double) (m_resolutionY-1);

	unsigned int Xi = (unsigned int) x;
	unsigned int Yi = (unsigned int) y;

	double dx = x-(double) Xi;
	double dy = y-(double) Yi;
	double v0 = GetHeight(Xi, Yi);
	double v1 = GetHeight(Xi+1, Yi);
	double v2 = GetHeight(Xi, Yi+1);
	double v3 = GetHeight(Xi+1, Yi+1);

	double v4 = InterpolateQuadratic(v0, v1, dx);
	double v5 = InterpolateQuadratic(v2, v3, dx);
	return InterpolateQuadratic(v4, v5, dy);
}

double ValueNoise::RandomGrid::GetHeight(int Xi, int Yi)
{
	Xi = max(min(Xi, (int)m_resolutionX-1), 0);
	Yi = max(min(Yi, (int)m_resolutionY-1), 0);

	return m_grid[Xi*m_resolutionY + Yi];
}

double ValueNoise::RandomGrid::InterpolateLinear(double u, double v, double coef)
{
	return u*(1-coef)+v*coef;
}

double ValueNoise::RandomGrid::InterpolateQuadratic(double u, double v, double coef)
{
	double weight = coef*coef*(3.0-2.0*coef);
	return u*(1-weight)+v*weight;
}

} // namespace Crowd