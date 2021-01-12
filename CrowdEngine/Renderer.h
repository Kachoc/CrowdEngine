//
//	Renderer.h
//
//	D�clare la classe abstraite du Renderer, d�finie dans une dll.
//	Le Renderer sert � initialiser l'API 3D utilis�e et les objets n�cessaires au rendu,
//	et g�re et configure le rendu avec des fonctions comme BeginScene() et EndScene().
//	Il se charge en particulier de g�rer la cible de rendu.

#ifndef CRWD_RENDERER_H_INCLUDED
#define CRWD_RENDERER_H_INCLUDED

#include <Windows.h>

#include "Core.h"

#include "Resources.h"

#include "RenderPass.h"

#include "Formats.h"
#include "Textures.h"

#include "RenderInfo.h"
#include "Camera.h"

namespace Crowd
{

class Renderer
{
	public:
	friend class RefCnt<Renderer>;

	//
	//	Fonction statique
	//
	public:
	CRWD_EXPORT static Renderer		&GetInstance(); ///< Permet de r�cup�rer l'unique instance du Renderer. A n'utiliser que dans l'pplication, les dll doivent utiliser Global::GetRenderer().


	//
	//	Fonctions d'utilisation du Renderer
	//
	//	Fonctions d'initialisation
	CRWD_EXPORT virtual void					SetTargetWindowForDifRender(HWND targetWindow, FORMAT backBufferFormat, FORMAT colorBufferFormat = FORMAT_R32G32B32A32_FLOAT, FORMAT normalBufferFormat = FORMAT_R32G32B32A32_FLOAT); ///< D�finit la fen�tre de rendu et param�tre le Renderer pour un rendu en diff�r�.
	CRWD_EXPORT virtual void					SetMultisampling(const Sample &multisampling); ///< Permet de changer la valeur du multisampling sans appeler la fonction SetTargetWindow.

	//
	//	Fonctions d'utilisation
	//
	virtual void					GetRenderInfo(RenderInfo &info) const = 0;

	//	Param�trage de la fen�tre
	virtual void					ResizeTargetWindow(unsigned int w, unsigned int h) = 0; ///< Redimensionne la fen�tre de rendu. Cette fonction est d�finie pour le mode plein-�cran.
	virtual HWND					GetTargetWindow() const = 0; ///< Retourne la fen�tre de rendu.
	virtual FORMAT					GetBackBufferFormat() const = 0; ///< Retourne le format du backbuffer.

	virtual void					SetFullscreenState(bool fullscreen = true) = 0; ///< Permet de passer du mode fen�tr� au mode plein-�cran et inversement.
	virtual bool					IsFullscreen() const = 0; ///< Permet de savoir si la fen�tre de rendu est actuellement en mode fen�tr� ou plein-�cran.

	virtual void					SetCamera(const Camera &camera); ///< Permet de d�finir la cam�ra principale utilis�e actuellement. Cette cam�ra est communiqu�e aux proc�dures r�alisant les passes de rendu.

	// Param�trage du rendu
	CRWD_EXPORT virtual void					SetBeginPass(RenderPass::ptr pass); ///< Permet de d�terminer la passe commen�ant le rendu. Cette passe doit s'occuper du d�but du rendu (g�n�ralement, l'initialisation des buffers de rendu) et d�finir les buffers de rendu (le Renderer s'en occupe s'il n'y a pas de passe de commencement du rendu).
	CRWD_EXPORT virtual void					SetIntermediatePasses(const std::vector<RenderPass::ptr> &passes); ///< Permet de d�terminer les passes interm�diaires de rendu, qui seront appel�es dans l'ordre pour faire le rendu.
	CRWD_EXPORT virtual void					AddPass(RenderPass::ptr pass); ///< Permet d'ajouter une passe de rendu � la fin de la pile des passes interm�diaires.
	CRWD_EXPORT virtual void					SetEndPass(RenderPass::ptr pass); ///< Permet de d�terminer la passe terminant le rendu.

	//	Sc�ne
	///	Fait le rendu de la sc�ne: appelle les fonctions Process des passes d�finies par SetIntermediatePasses
	virtual void					RenderScene() = 0;

	/// D�finit le color buffer, le normal buffer et le depth/stencil buffer comme cibles de rendu apr�s les avoir r�initialis�s. A appeler pour effectuer un pr�-rendu de la g�om�trie.
	virtual void					BeginGeometryPass() = 0;

	/// R�alise la copie des depth/stencil buffers. D�finit une cible de rendu int�rm�diaire comme cible de rendu. A appeler avant le rendu des lumi�res.
	virtual void					BeginLightPass() = 0;

	/// D�finit la fen�tre de l'application comme cible de rendu. A appeler en fin de rendu por r�aliser le post-process (color grading, anti-aliasing, fog, volumetric lightning...).
	virtual void					BeginPostPass() = 0;

	/// Rafra�chit la fen�tre principale. Termine le rendu de la sc�ne.
	virtual void					Present(bool VerticalSync = false) = 0;

	// Performance
	CRWD_EXPORT virtual void					InitDrawnGeometryCount(); /// A appeler lorsque l'on commence le rendu d'une nouvelle frame.

	CRWD_EXPORT virtual void					AddGeometryToCount(unsigned int numTris, unsigned int numVertices);
	CRWD_EXPORT virtual unsigned int			GetDrawnTrisCount() const;
	CRWD_EXPORT virtual unsigned int			GetDrawnVerticesCount() const;

	protected:
	CRWD_EXPORT Renderer();
	CRWD_EXPORT virtual ~Renderer();

	CRWD_EXPORT Renderer &operator=(const Renderer &);

	protected:
	static SmartPtr<Renderer, RefCnt>	m_instance;

	//
	//	Attributs par d�faut
	//

	// Passes de rendu
	RenderPass::ptr					m_beginPass;
	std::vector<RenderPass::ptr>	m_intermediatePasses;
	RenderPass::ptr					m_endPass;

	Camera							m_camera;

	// Performance
	unsigned int					m_drawnTrisCount;
	unsigned int					m_drawnVerticesCount;
};

} // namespace Crowd

#endif // CRWD_RENDERER_H_INCLUDED