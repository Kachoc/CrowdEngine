#include "LightScene.h"
#include "D3D10LightScene.h"

namespace Crowd
{

LightScene::LightScene(const AABB &boundingBox) : BaseScene(boundingBox)
{

}

LightScene::~LightScene()
{

}

LightScene *LightScene::Create(const AABB &boundingBox)
{
	return new D3D10::LightScene(boundingBox);
}

void LightScene::ComputeShadowMaps(const RenderInfo &info, const Camera &camera)
{
	BOctree::BoolIterator<CameraFunct> funct(camera, m_octree);

	while(funct)
	{
		if(camera.IsVisible((*funct).GetAABB()))  {
			(*funct).ComputeShadowMaps(info, camera, m_shadowScenes);
		}

		++funct;
	}
}

void LightScene::ApplyLightPass(const RenderInfo &info, const Camera &camera)
{
	BOctree::BoolIterator<CameraFunct> funct(camera, m_octree);

	while(funct)
	{
		if(camera.IsVisible((*funct).GetAABB()))  {
			(*funct).Draw(info, camera);
		}

		++funct;
	}
}

void LightScene::SetSunDirection(const Vector3 &direction)
{
	m_sunDirection = Vector4::Normalize(Vector4(direction, 0.0f));
}

void LightScene::SetSunAmbient(const Color &ambient)
{	
	m_sunAmbient = ambient;
}

void LightScene::SetSunDiffuse(const Color &diffuse)
{
	m_sunDiffuse = diffuse;
}

void LightScene::SetSunSpecular(const Color &specular)
{
	m_sunSpecular = specular;
}

void LightScene::SetSkyDesc(const Color &SkyColor, const Color &HorizonColor, float	SkyBlendExponent, float SunRadiusExponent, float SunRadiusMultiplier)
{
	m_SkyColor = SkyColor;
	m_HorizonColor = HorizonColor;
	m_SkyBlendExponent = SkyBlendExponent;
	m_SunRadiusExponent = SunRadiusExponent;
	m_SunRadiusMultiplier = SunRadiusMultiplier;
}

void LightScene::InitializeShadowScenes(const AABB &aabb, unsigned int num)
{
	m_shadowScenesAABB = aabb;

	m_shadowScenes.empty();
	m_shadowScenes.resize(num);
	for(unsigned int i = 0; i < num; i++)
	{
		m_shadowScenes[i] = new GeometryScene(aabb);
	}
}

void LightScene::AddShadowGeometry(REntity *entity, unsigned int level)
{
	if(level < m_shadowScenes.size())
	{
		return m_shadowScenes[level]->Add(entity);
	}
}

void LightScene::SetShadowMappingEpsilons(const std::vector<float> &epsilons)
{
	m_sunShadowMapEpsilons = epsilons;
}

} // namespace Crowd