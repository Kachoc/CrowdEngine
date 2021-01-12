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
//	Fonctions permettant de tester la compatibilité, retournent 0 par défaut
//

/// Permet de tester la compatibilité avec le rendu standard (non différé): un backbuffer et optionnellement un depth-stencil buffer
/*bool RenderPass::StandardRender() const
{
	return false;
}

/// Permet de tester la compatibilité avec le rendu différé: un backbuffer, un buffer de couleurs
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