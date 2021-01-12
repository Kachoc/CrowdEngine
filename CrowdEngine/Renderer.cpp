#include <sstream>

#include "Renderer.h"
#include "D3D10Renderer.h"

namespace Crowd
{

//
//	Instance unique du Renderer
//
SmartPtr<Renderer>		Renderer::m_instance(new D3D10::Renderer);


//
//	Fonction statique
//
Renderer &Renderer::GetInstance()
{
	return *m_instance;
}


//
//	Constructeur, destructeur, opérateur d'assignement
//
Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

Renderer &Renderer::operator=(const Renderer &)
{
	return *this;
}


//
//	Fonctions par défaut, définies comme non gérées
//
void Renderer::SetTargetWindowForDifRender(HWND targetWindow, FORMAT backBufferFormat, FORMAT colorBufferFormat, FORMAT normalBufferFormat)
{
	throw Exception("Le Renderer actuel ne prend pas en charge la fonction SetTargetWindowForDifRender");
}

void Renderer::SetMultisampling(const Crowd::Sample &multisampling)
{
	throw Exception("Le Renderer actuel ne prend pas en charge la fonction SetMultisampling");
}

void Renderer::SetCamera(const Camera &camera)
{
	m_camera = camera;
}



//
//	Fonctions de paramétrage du rendu par défaut
//
void Renderer::SetBeginPass(RenderPass::ptr pass)
{
	m_beginPass = pass;
}

void Renderer::SetIntermediatePasses(const std::vector<RenderPass::ptr> &passes)
{
	m_intermediatePasses = passes;
}

void Renderer::AddPass(RenderPass::ptr pass)
{
	m_intermediatePasses.push_back(pass);
}

void Renderer::SetEndPass(RenderPass::ptr pass)
{
	m_endPass = pass;
}

//
// Performance
//
void Renderer::InitDrawnGeometryCount()
{
	m_drawnTrisCount = 0;
	m_drawnVerticesCount = 0;
}

void Renderer::AddGeometryToCount(unsigned int numTris, unsigned int numVertices)
{
	m_drawnTrisCount += numTris;
	m_drawnVerticesCount += numVertices;
}

unsigned int Renderer::GetDrawnTrisCount() const
{
	return m_drawnTrisCount;
}

unsigned int Renderer::GetDrawnVerticesCount() const
{
	return m_drawnVerticesCount;
}

} // namespace Crowd