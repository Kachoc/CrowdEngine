#ifndef CROWD_POINTLIGHT_H_INCLUDED
#define CROWD_POINTLIGHT_H_INCLUDED

#include "Status.h"

#include "Lights.h"

namespace Crowd
{

class PointLight : public Light
{
	public:
	typedef SmartPtr<PointLight, IPol> ptr;

	struct ShadowMapDesc
	{
		unsigned int	Resolution;
		float			ProjMinDist, ProjMaxDist;
		unsigned int	ShadowMapSceneIndice;
		bool			Use;
	};

	// diffuseSpec : d�finit la couleur �mise par la lampe. L'intensit� re�ue par un objet est exactement �gale � color � la distance de 1 unit� de distance.
	CRWD_EXPORT static PointLight *Create(const Vector4 &position, const Color &diffuseSpec);

	CRWD_EXPORT PointLight(const Vector4 &position, const Color &diffuseSpec);

	CRWD_EXPORT virtual const AABB &GetAABB() const;

	CRWD_EXPORT virtual void SetDesc(const Vector4 &position, const Color &diffuseSpec);
	CRWD_EXPORT virtual void GetDesc(Vector4 &position, Color &diffuseSpec) const;

	/// Param�tre l'utilisation des shadow maps
	/// resolution : r�solution en pixel d'un des 6 buffers de la shadow map
	/// shadowMapSceneIndice : indice de la sc�ne g�om�trique de lightScene � utiliser pour le rendu des shadow maps
	/// use : vrai s'il faut utiliser des shadows maps (si faux: les deux pr�c�dents param�tres ne sont pas pris en compte)
	virtual Status UseShadowMaps(unsigned int resolution, float projMinDist, float projMaxDist, unsigned int shadowMapSceneIndice, bool use) = 0;

	protected:
	virtual void ComputeViewMatrices();
	virtual void ComputeProjectionMatrices();

	protected:
	Vector4					m_position;
	Color					m_diffuseSpec;
	PerspectiveCamera		m_shadowMapsCameras[6];
	ShadowMapDesc			m_shadowMapsDesc;
};

} // namespace Crowd

#endif // CROWD_POINTLIGHT_H_INCLUDED