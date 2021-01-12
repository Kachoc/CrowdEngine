#include "PointLight.h"
#include "D3D10PointLight.h"

#define PL_FADE_DISTANCE 22.5831795813f

namespace Crowd
{

PointLight *PointLight::Create(const Vector4 &position, const Color &diffuseSpec)
{
	return new D3D10::PointLight(position, diffuseSpec);
}

PointLight::PointLight(const Vector4 &position, const Color &diffuseSpec)
{
	m_shadowMapsDesc.Use = false;
	SetDesc(position, diffuseSpec);
}

const AABB &PointLight::GetAABB() const
{
	return m_aabb;
}

void PointLight::SetDesc(const Vector4 &position, const Color &diffuseSpec)
{
	m_position = position;
	m_diffuseSpec = diffuseSpec;

	float m = max(max(m_diffuseSpec.r, m_diffuseSpec.g), m_diffuseSpec.b);
	float midDiametre = (float)sqrt((double)m)*PL_FADE_DISTANCE;
	m_aabb.MinPoint.x = m_position.x - midDiametre;
	m_aabb.MinPoint.y = m_position.y - midDiametre;
	m_aabb.MinPoint.z = m_position.z - midDiametre;
	m_aabb.MaxPoint.x = m_position.x + midDiametre;
	m_aabb.MaxPoint.y = m_position.y + midDiametre;
	m_aabb.MaxPoint.z = m_position.z + midDiametre;

	ComputeViewMatrices();
}

void PointLight::GetDesc(Vector4 &position, Color &diffuseSpec) const
{
	position = m_position;
	diffuseSpec = m_diffuseSpec;
}

// Permet de calculer les caméras pointant vers les différentes faces du cube de textures.
// Les faces du cube sont supposées indicées comme suit:
// i = 0 : x = -1
// i = 1 : x = 1
// i = 2 : y = -1
// i = 3 : y = 1
// i = 4 : z = 1
// i = 5 : z = -1
void PointLight::ComputeViewMatrices()
{
	Vector3 position(m_position);
	Vector3 lookAt, up;

	lookAt = Vector3(-1, 0, 0);
	up = Vector3(0, 1, 0);
	m_shadowMapsCameras[0].SetOrientation(m_position, position + lookAt, up);

	lookAt = Vector3(1, 0, 0);
	up = Vector3(0, 1, 0);
	m_shadowMapsCameras[1].SetOrientation(m_position, position + lookAt, up);

	lookAt = Vector3(0, -1, 0);
	up = Vector3(0, 0, -1);
	m_shadowMapsCameras[2].SetOrientation(m_position, position + lookAt, up);

	lookAt = Vector3(0, 1, 0);
	up = Vector3(0, 0, -1);
	m_shadowMapsCameras[3].SetOrientation(m_position, position + lookAt, up);

	lookAt = Vector3(0, 0, -1);
	up = Vector3(0, 1, 0);
	m_shadowMapsCameras[4].SetOrientation(m_position, position + lookAt, up);

	lookAt = Vector3(0, 0, 1);
	up = Vector3(0, 1, 0);
	m_shadowMapsCameras[5].SetOrientation(m_position, position + lookAt, up);

	/*m_shadowMapsCameras[0].SetOrientation(m_position, 0, 90, 0);
	m_shadowMapsCameras[1].SetOrientation(m_position, 0, -90, 0);

	m_shadowMapsCameras[2].SetOrientation(m_position, 90, 180, 0);
	m_shadowMapsCameras[3].SetOrientation(m_position, 90, 0, 0);

	m_shadowMapsCameras[4].SetOrientation(m_position, 0, 180, 0);
	m_shadowMapsCameras[5].SetOrientation(m_position, 0, 0, 0);*/
}

void PointLight::ComputeProjectionMatrices()
{
	for(unsigned int i = 0; i < 6; i++)
	{
		m_shadowMapsCameras[i].SetParameters((float) CRWD_PI * 0.5f, 1.0f, m_shadowMapsDesc.ProjMinDist, m_shadowMapsDesc.ProjMaxDist);
	}
}

} // namespace Crowd