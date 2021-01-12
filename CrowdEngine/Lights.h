#ifndef CRWD_LIGHTS_H_INCLUDED
#define CRWD_LIGHTS_H_INCLUDED

#include "CRWDMath.h"

#include "GeometryScene.h"

namespace Crowd
{

/// Lampe
class Light : public Entity
{
	public:
	typedef SmartPtr<Light, IPol> ptr;

	virtual void Draw(const RenderInfo &info, const Camera &camera) = 0;
	virtual const AABB &GetAABB() const = 0;

	/// Génère les shadow maps de la lampe à partir des objets de la scène si nécessaire.
	virtual void ComputeShadowMaps(const RenderInfo &info, const Camera &camera, std::vector<GeometryScene::ptr> &scene) = 0;
	
	protected:
	Light() {}
	virtual ~Light() {}

	AABB m_aabb;
};

struct SunShaderInfo
{
	Vector4T<float>		SunDirection;
	Color				SunDiffuse;
	Color				SunAmbient;
	Color				SunSpecular;

	Color		SkyColor;
	Color		HorizonColor;
	float		SkyBlendExponent;
	float		SunRadiusExponent;
	float		SunRadiusMultiplier;
	
	float unused1;
};

// Inutile.
class Sun : public Entity
{
	INTERFACE_PTR(Sun);

	public:
	struct SunShadowMapLevel
	{
		unsigned int Width;
		unsigned int Height;
	};

	struct ShadowMapInfo
	{
		Matrix		SunViewProjMatrix;
		float		ShadowMapDepth;
		float		ShadowMapWidth;
		float		ShadowMapHeight;
		float		unused1;
	};

	public:
	CRWD_EXPORT virtual void SetPosition(const Vector3 &position);
	CRWD_EXPORT virtual void SetDirection(const Vector3 &direction);
	CRWD_EXPORT virtual void SetAmbient(const Color &ambient);
	CRWD_EXPORT virtual void SetDiffuse(const Color &diffuse);
	CRWD_EXPORT virtual void SetSpecular(float specular);

	struct Data
	{
		Vector3				Position;
		Vector3				Direction;
		Color				Ambient;
		Color				Diffuse;
		float				Specular;
	};

	CRWD_EXPORT virtual void GetData(Data &data);

	protected:
	Sun();
	virtual ~Sun();

	protected:
	Data m_sunData;
	std::vector<SunShadowMapLevel>		m_shadowMapsDesc;
	std::vector<OrthographicCamera>		m_shadowMapCameras;
};

} // namespace Crowd

#endif // CRWD_LIGHTS_H_INCLUDED