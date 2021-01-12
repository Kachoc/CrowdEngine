#include "StandardShaders.h"
#include "D3D10Shaders.h"

namespace Crowd
{

//
//	CustomShader
//
void CustomShader::SetMatrix(const String &name, const Matrix &matrix)
{
	m_matrices[name] = matrix;
}

void CustomShader::SetColor(const String &name, const Color &color)
{
	m_colors[name] = color;
}

void CustomShader::SetResource(const String &name, Resource::ptr resource)
{
	m_resources[name] = resource;
}

Matrix CustomShader::GetMatrix(const String &name)
{
	std::map<String, Matrix>::iterator it = m_matrices.find(name);
	if(it != m_matrices.end()) {
		return it->second;
	}
	else {
		return Matrix();
	}
}

Color CustomShader::GetColor(const String &name)
{
	std::map<String, Color>::iterator it = m_colors.find(name);
	if(it != m_colors.end()) {
		return it->second;
	}
	else {
		return Color();
	}
}

Resource::ptr CustomShader::GetResource(const String &name)
{
	std::map<String, Resource::ptr>::iterator it = m_resources.find(name);
	if(it != m_resources.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}



//
//	LambertShader
//
LambertShader::LambertShader()
{

}

LambertShader *LambertShader::Create(const Color &diffuse, Texture::ptr diffuseSpecularTexture)
{
	return new D3D10::LambertShader(diffuse, diffuseSpecularTexture);
}

void LambertShader::SetMaterialDiffuse(const Color &diffuse)
{
	m_materialDiffuse = diffuse;
}

void LambertShader::SetMaterialDiffSpecTexture(Texture::ptr texture)
{
	m_diffuseSpecularTexture = texture;
}


//
//	DepthShader
//
 DepthShader *DepthShader::Create()
{
	return new D3D10::DepthShader;
}

} // namespace Crowd