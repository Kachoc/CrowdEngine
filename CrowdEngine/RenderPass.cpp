#include "RenderPass.h"
#include "D3D10Passes.h"

namespace Crowd
{

//
//	RenderPass
//

//
//	Constructeur, destructeur
//
RenderPass::RenderPass()
{

}

 RenderPass::~RenderPass()
{

}


//
//	Fonctions permettant de tester la compatibilit�, retournent 0 par d�faut
//

/// Permet de tester la compatibilit� avec le rendu standard (non diff�r�): un backbuffer et optionnellement un depth-stencil buffer
/*bool RenderPass::StandardRender() const
{
	return false;
}

/// Permet de tester la compatibilit� avec le rendu diff�r�: un backbuffer, un buffer de couleurs
/// et un buffer de normales, ainsi qu'un depth-stencil buffer
bool RenderPass::DifferedRender() const
{
	return false;
}*/

//
//	SunPass
//
SunPass *SunPass::Create()
{
	return new D3D10::SunPass();
}

} // namespace Crowd