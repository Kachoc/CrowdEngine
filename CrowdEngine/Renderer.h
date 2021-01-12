//
//	Renderer.h
//
//	Déclare la classe abstraite du Renderer, définie dans une dll.
//	Le Renderer sert à initialiser l'API 3D utilisée et les objets nécessaires au rendu,
//	et gère et configure le rendu avec des fonctions comme BeginScene() et EndScene().
//	Il se charge en particulier de gérer la cible de rendu.

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
	CRWD_EXPORT static Renderer		&GetInstance(); ///< Permet de récupérer l'unique instance du Renderer. A n'utiliser que dans l'pplication, les dll doivent utiliser Global::GetRenderer().


	//
	//	Fonctions d'utilisation du Renderer
	//
	//	Fonctions d'initialisation
	CRWD_EXPORT virtual void					SetTargetWindowForDifRender(HWND targetWindow, FORMAT backBufferFormat, FORMAT colorBufferFormat = FORMAT_R32G32B32A32_FLOAT, FORMAT normalBufferFormat = FORMAT_R32G32B32A32_FLOAT); ///< Définit la fenêtre de rendu et paramètre le Renderer pour un rendu en différé.
	CRWD_EXPORT virtual void					SetMultisampling(const Sample &multisampling); ///< Permet de changer la valeur du multisampling sans appeler la fonction SetTargetWindow.

	//
	//	Fonctions d'utilisation
	//
	virtual void					GetRenderInfo(RenderInfo &info) const = 0;

	//	Paramétrage de la fenêtre
	virtual void					ResizeTargetWindow(unsigned int w, unsigned int h) = 0; ///< Redimensionne la fenêtre de rendu. Cette fonction est définie pour le mode plein-écran.
	virtual HWND					GetTargetWindow() const = 0; ///< Retourne la fenêtre de rendu.
	virtual FORMAT					GetBackBufferFormat() const = 0; ///< Retourne le format du backbuffer.

	virtual void					SetFullscreenState(bool fullscreen = true) = 0; ///< Permet de passer du mode fenêtré au mode plein-écran et inversement.
	virtual bool					IsFullscreen() const = 0; ///< Permet de savoir si la fenêtre de rendu est actuellement en mode fenêtré ou plein-écran.

	virtual void					SetCamera(const Camera &camera); ///< Permet de définir la caméra principale utilisée actuellement. Cette caméra est communiquée aux procédures réalisant les passes de rendu.

	// Paramétrage du rendu
	CRWD_EXPORT virtual void					SetBeginPass(RenderPass::ptr pass); ///< Permet de déterminer la passe commençant le rendu. Cette passe doit s'occuper du début du rendu (généralement, l'initialisation des buffers de rendu) et définir les buffers de rendu (le Renderer s'en occupe s'il n'y a pas de passe de commencement du rendu).
	CRWD_EXPORT virtual void					SetIntermediatePasses(const std::vector<RenderPass::ptr> &passes); ///< Permet de déterminer les passes intermédiaires de rendu, qui seront appelées dans l'ordre pour faire le rendu.
	CRWD_EXPORT virtual void					AddPass(RenderPass::ptr pass); ///< Permet d'ajouter une passe de rendu à la fin de la pile des passes intermédiaires.
	CRWD_EXPORT virtual void					SetEndPass(RenderPass::ptr pass); ///< Permet de déterminer la passe terminant le rendu.

	//	Scène
	///	Fait le rendu de la scène: appelle les fonctions Process des passes définies par SetIntermediatePasses
	virtual void					RenderScene() = 0;

	/// Définit le color buffer, le normal buffer et le depth/stencil buffer comme cibles de rendu après les avoir réinitialisés. A appeler pour effectuer un pré-rendu de la géométrie.
	virtual void					BeginGeometryPass() = 0;

	/// Réalise la copie des depth/stencil buffers. Définit une cible de rendu intérmédiaire comme cible de rendu. A appeler avant le rendu des lumières.
	virtual void					BeginLightPass() = 0;

	/// Définit la fenêtre de l'application comme cible de rendu. A appeler en fin de rendu por réaliser le post-process (color grading, anti-aliasing, fog, volumetric lightning...).
	virtual void					BeginPostPass() = 0;

	/// Rafraîchit la fenêtre principale. Termine le rendu de la scène.
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
	//	Attributs par défaut
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