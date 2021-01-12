#ifndef CRWD_VALUE_NOISE_H_INCLUDED
#define CRWD_VALUE_NOISE_H_INCLUDED

#include "CRWDVector.h"
#include <vector>
#include <random>

namespace Crowd
{

class ValueNoise {
	public:
	typedef float(*AmplitudeFunction)(unsigned int);

	static void SetSeed(unsigned long seed);

	static void GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, const std::vector<Vector2D> &frequencies, const std::vector<float> &amplitudes, std::vector<float> &grid); // resolution: nombre de points sur la grille
	static void GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, unsigned int period, unsigned int numOctaves, AmplitudeFunction amplitude,std::vector<float> &grid);

	protected:
	static double GeneratePoint();

	class RandomGrid
	{
		public:
		RandomGrid(unsigned int width, unsigned int height); // width, height: nombre de cases sur la grille : pas la même définition que pour resolutionX et resolutionY de GenerateGrid!!
		double GenerateHeight(double x, double y); // x et y doivent être compris entre 0 et 1

		double InterpolateLinear(double u, double v, double coef);
		double InterpolateQuadratic(double u, double v, double coef);

		protected:
		double GetHeight(int Xi, int Yi);

		protected:
		std::vector<double>			m_grid;
		unsigned int				m_resolutionX;
		unsigned int				m_resolutionY;
	};

	static std::mt19937				m_generator;
};

} // namespace Crowd

#endif // CRWD_VALUE_NOISE_H_INCLUDED