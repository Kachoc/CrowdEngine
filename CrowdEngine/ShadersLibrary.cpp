#include "ShadersLibrary.h"
#include "Exceptions.h"

#include "D3D10ShaderFile.h"
#include "D3D10ShadersLibrary.h"

namespace Crowd
{

//
//	Shader
//
unsigned int Shader::GetNumPasses() const
{
	return m_numPasses;
}


//
//	MeshShader
//
void MeshShader::SetPositionMatrix(const Matrix &position)
{
	m_positionMatrix = position;
}


//
//	ShaderFile
//
ShaderFile::ptr ShaderFile::Load(const String fileName, bool isParent, ShaderFile *parent, Status *status)
{
	return D3D10::ShaderFile::Load(fileName, isParent, parent, status);
}

//
//	ShaderLibrary
//
ShaderLibrary::ptr ShaderLibrary::m_instance(new D3D10::ShaderLibrary);
String ShaderLibrary::m_shadersPath;

ShaderLibrary::ShaderLibrary()
{

}

ShaderLibrary::~ShaderLibrary()
{

}

ShaderLibrary &ShaderLibrary::GetInstance()
{
	return *m_instance;
}

void ShaderLibrary::SetShadersPath(const String &path)
{
	m_shadersPath = path;
}
String ShaderLibrary::GetShadersPath()
{
	return m_shadersPath;
}

} // namespace Crowd