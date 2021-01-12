#ifndef CRWD_PERLIN_NOISE_H_INCLUDED
#define CRWD_PERLIN_NOISE_H_INCLUDED

#include "CRWDVector.h"
#include <vector>
#include <random>

namespace Crowd
{

class PerlinNoise {
	public:
	typedef float(*AmplitudeFunction)(unsigned int);

	static void SetSeed(unsigned long seed);

	static void GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, const std::vector<Vector2D> &frequencies, const std::vector<float> &amplitudes, std::vector<float> &grid); // resolution: nombre de points sur la grille
	static void GenerateGrid(unsigned int resolutionX, unsigned int resolutionY, unsigned int period, unsigned int numOctaves, AmplitudeFunction amplitude,std::vector<float> &grid);

	protected:
	static void GeneratePoint(Vector2D &point); // génère un point sur le cercle unité

	class GradientGrid
	{
		public:
		GradientGrid(unsigned int width, unsigned int height); // width, height: nombre de cases sur la grille : pas la même définition que pour resolutionX et resolutionY de GenerateGrid!!
		double GenerateHeight(double x, double y); // x et y doivent être compris entre 0 et 1

		protected:
		double DotProduct(int Xi, int Yi, double dx, double dy); // dx et dy doivent être compris entre -1 et 1
		double InterpolateLinear(double u, double v, double coef);
		double InterpolateQuadratic(double u, double v, double coef);

		protected:
		std::vector<Vector2D>		m_grid;
		unsigned int				m_resolutionX;
		unsigned int				m_resolutionY;
	};

	static std::mt19937				m_generator;
};

} // namespace Crowd

#endif // CRWD_PERLIN_NOISE_H_INCLUDED