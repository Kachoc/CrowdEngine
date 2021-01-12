#ifndef CRWD_LIGHTSCENE_H_INCLUDED
#define CRWD_LIGHTSCENE_H_INCLUDED

#include "Lights.h"
#include "Camera.h"
#include <list>

#include "GeometryScene.h"

#include "PointLight.h"

namespace Crowd
{


/// Light Scene
class LightScene : public BaseScene<Light>
{
	INTERFACE_PTR(LightScene);

	public:
	//
	//	Création
	//
	CRWD_EXPORT static LightScene *Create(const AABB &boundingBox);

	//
	//	Structures
	//
	struct SunShadowMapLevel
	{
		unsigned int Width;
		unsigned int Height;
	};

	struct ShadowMapInfo
	{
		Matrix		LightViewProjMatrix;
	};

	virtual void SetWorldShaderInfo(const WorldShaderInfo &info) = 0;

	//
	//	Gestion générale des lampes
	//
	CRWD_EXPORT virtual void ComputeShadowMaps(const RenderInfo &info, const Camera &camera);
	CRWD_EXPORT virtual void ApplyLightPass(const RenderInfo &info, const Camera &camera);

	/// Permet d'initialiser la shadow map utilisée par le soleil. Permet de redimensionner et replacer la shadow map si elle
	/// a déjà été initialisée (LightScene vérifie la taille des buffers avant éventuellement de les redimensionner,
	/// d'en supprimer ou d'en rajouter).
	virtual Status CreateSunShadowMaps(unsigned int resolution, unsigned int numShadowMaps, const std::vector<float> &epsilons, const unsigned int *shadowScenesIndices = NULL) = 0;//(const std::vector<SunShadowMapLevel> &sunShadowMapDesc) = 0;

	CRWD_EXPORT virtual void SetSunDirection(const Vector3 &direction);
	CRWD_EXPORT virtual void SetSunAmbient(const Color &ambient);
	CRWD_EXPORT virtual void SetSunDiffuse(const Color &diffuse);
	CRWD_EXPORT virtual void SetSunSpecular(const Color &specular);

	CRWD_EXPORT virtual void SetSkyDesc(const Color &SkyColor, const Color &HorizonColor, float	SkyBlendExponent, float SunRadiusExponent, float SunRadiusMultiplier);

	CRWD_EXPORT virtual void SetShadowMappingEpsilons(const std::vector<float> &epsilons);
	virtual void RenderSunShadowMaps(const RenderInfo &info, const std::vector<OrthographicCamera> &cameras) = 0;
	virtual void RenderSun(const RenderInfo &info, const Camera &camera) = 0;


	//
	//	Gestion des shadow maps
	//

	/// Initialise le nombre de scènes d'entités géométriques à utiliser pour les shadow maps (ie: le nombre de niveau de détails pour les shadow maps).
	CRWD_EXPORT virtual void InitializeShadowScenes(const AABB &aabb, unsigned int num);

	/// Ajoute à la shadow scene un objet géométrique. Le paramètre level indique la shdow map dans laquelle ajouter cet objet.
	CRWD_EXPORT virtual void AddShadowGeometry(REntity *entity, unsigned int level);

	protected:
	CRWD_EXPORT LightScene(const AABB &boundingBox);
	CRWD_EXPORT virtual ~LightScene();

	protected:
	Vector4							m_sunPosition;
	Vector4							m_sunDirection;
	Color							m_sunAmbient;
	Color							m_sunDiffuse;
	Color							m_sunSpecular;

	Color							m_SkyColor;
	Color							m_HorizonColor;
	float							m_SkyBlendExponent;
	float							m_SunRadiusExponent;
	float							m_SunRadiusMultiplier;

	AABB								m_shadowScenesAABB;
	std::vector<float>					m_sunShadowMapEpsilons;
	unsigned int						m_shadowMapsResolution;
	unsigned int						m_numShadowMaps;
	std::vector<unsigned int>			m_shadowMapsLinking; // indique quelles shadowScene doivent être utilisées pour le rendu de chaque shadow map
	std::vector<GeometryScene::ptr>		m_shadowScenes;
};

} // namespace Crowd

#endif // CRWD_LIGHTSCENE_H_INCLUDED