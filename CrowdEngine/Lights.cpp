#include "Lights.h"

namespace Crowd
{

Sun::Sun()
{

}

Sun::~Sun()
{

}

void Sun::SetPosition(const Vector3 &position)
{
	m_sunData.Position = position;
}

void Sun::SetDirection(const Vector3 &direction)
{
	m_sunData.Direction = direction;
}

void Sun::SetAmbient(const Color &ambient)
{
	m_sunData.Ambient = ambient;
}

void Sun::SetDiffuse(const Color &diffuse)
{
	m_sunData.Diffuse = diffuse;
}

void Sun::SetSpecular(float specular)
{
	m_sunData.Specular = specular;
}

void Sun::GetData(Data &data)
{
	m_sunData = data;
}

} // namespace Crowd