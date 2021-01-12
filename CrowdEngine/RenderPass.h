#ifndef CRWD_RENDER_PASS_H_INCLUDED
#define CRWD_RENDER_PASS_H_INCLUDED

#include <vector>
#include <Windows.h>

#include "Resources.h"
#include "D3DEntry.h"

namespace Crowd
{

struct RenderInfo;

class RenderPass : public Resource
{
	public:
	typedef SmartPtr<RenderPass, IPol> ptr;

	//
	//	Fonctions permettant de tester la compatibilité, retournent 0 par défaut
	//
	//virtual bool StandardRender() const;
	//virtual bool DifferedRender() const;

	///
	///	Fonction de traitement des buffers du Renderer
	///
	/// \param renderViews Les vues des buffers de rendu: le backbuffer si le rendu est standard, le backbuffer,
	/// le buffer de couleur et le buffer de normales.
	/// \param depthStencilView La vue du depth-stencil buffer, s'il y en a un
	virtual void Process(const RenderInfo &info) = 0;

	protected:
	CRWD_EXPORT RenderPass();
	CRWD_EXPORT virtual ~RenderPass();
};


///
///	Classe dérivée de RenderPass permettant de réinitialiser les buffers (passe par défaut)
///
class ClearPass : public RenderPass
{
	public:
	typedef SmartPtr<ClearPass, IPol> ptr;

	//
	//	Constructeur, destructeur
	//
	ClearPass() { ZeroMemory(m_clearColor, sizeof(m_clearColor)); }
	virtual ~ClearPass() {}

	//
	//	Paramétrage
	//
	virtual void SetDefaultColor(const float rgba[4]) { memcpy(m_clearColor, rgba, sizeof(m_clearColor)); }

	//
	//	Fonctions permettant de tester la compatibilité
	//
	virtual bool StandardRender() const { return true; }
	virtual bool DifferedRender() const { return true; }

	//
	//	Fonction de traitement des buffers du Renderer
	//
	virtual void Process(const RenderInfo &info) = 0;

	protected:
	float m_clearColor[4];
};


///
///	Classe dérivée de RenderPass permettant de terminer le rendu de la scène (ne fait pas de post-production: passe par défaut)
///
class EndPass : public RenderPass
{
	public:
	typedef SmartPtr<EndPass, IPol> ptr;

	//
	//	Constructeur, destructeur
	//
	EndPass() {}
	virtual ~EndPass() {}

	//
	//	Fonctions permettant de tester la compatibilité
	//
	virtual bool StandardRender() const { return true; }
	virtual bool DifferedRender() const { return true; }

	//
	//	Fonction de traitement des buffers du Renderer
	//
	virtual void Process(const RenderInfo &info) = 0;

	protected:
	float m_clearColor[4];
};

/// Directional Lights Pass
class SunPass : public RenderPass
{
	public:
	typedef SmartPtr<SunPass, IPol> ptr;

	CRWD_EXPORT static SunPass *Create();
	
	///	Echange les buffers du Renderer, commence la phase post-géométrique, réalise le rendu des lumières directionnelles
	virtual void Process(const RenderInfo &info) = 0;
};

} // namespace Crowd

#endif // CRWD_RENDER_PASS_H_INCLUDED