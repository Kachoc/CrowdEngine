#ifndef CRWD_SHADERS_LIBRARY_H_INCLUDED
#define CRWD_SHADERS_LIBRARY_H_INCLUDED

#include "Resources.h"
#include "D3DEntry.h"
#include "Status.h"
#include "LoaderManager.h"

#include "CRWDMath.h"

#include <map>

namespace Crowd
{

class Shader : public Interface
{
	public:
	typedef SmartPtr<Shader, IPol> ptr;

	Shader(unsigned int numPasses = 1) : m_numPasses(numPasses) {}
	virtual ~Shader() {}

	/// Permet de définir qu'il faut utiliser ce shader pour le prochain rendu
	virtual Status Apply(unsigned int passe) = 0;

	/// Retourne le nombre de passes utilisées par ce shader. Il faut les appliquer une à une au moyen de la fonction Apply(passe);
	CRWD_EXPORT virtual unsigned int GetNumPasses() const;

	protected:
	unsigned int m_numPasses; // nombre de passes du shader
};


//
//	MeshShader
//
// Contient les attributs et fonctions de base pour implémenter un shader de mesh (ie: géométrique).
// Cette classe n'est destinée qu'à être héritée.
class MeshShader : public Shader
{
	public:
	typedef SmartPtr<MeshShader, IPol> ptr;

	MeshShader() : m_positionMatrix(Matrix::Identity()) {}

	CRWD_EXPORT virtual void SetPositionMatrix(const Matrix &position);

	protected:
	Matrix			m_positionMatrix;
};

class ShaderFile : public Resource
{
	public:
	typedef SmartPtr<ShaderFile, IPol> ptr;

	ShaderFile() {}
	virtual ~ShaderFile() {}

	virtual String GetName() { return m_name; }
	//virtual EffectPtr GetEffect() = 0;

	struct LoadInfo : public Crowd::LoadInfo
	{
		LoadInfo() : isParent(false), parent(NULL) {}

		bool				isParent; // est un fichier parent, dont vont dépendre d'autres fichiers
		ShaderFile::ptr		parent; // si a un fichier parent
	};

	CRWD_EXPORT static ShaderFile::ptr Load(const String fileName, bool isParent, ShaderFile *parent, Status *status = NULL);

	protected:
	String		m_name;
};

struct WorldShaderInfo
{
	// Description de la caméra
	Matrix			viewMatrix;
	Matrix			projMatrix;
	Matrix			viewProjMatrix;
	Matrix			invViewMatrix;
	Matrix			invProjMatrix;
	Matrix			invViewProjMatrix;

	Vector4T<float>			cameraPosition;
	Vector4T<float>			cameraLookDir;
	float			cameraFovY;
	float			cameraRatio;
	float			cameraNearZ;
	float			cameraFarZ;
};

class ShaderLibrary
{
	public:
	friend class RefCnt<ShaderLibrary>;
	typedef SmartPtr<ShaderLibrary, RefCnt> ptr;

	friend class EngineLoader;

	CRWD_EXPORT static ShaderLibrary &GetInstance();

	CRWD_EXPORT static void SetShadersPath(const String &path);
	CRWD_EXPORT static String GetShadersPath();
	virtual Status LoadStandardShaders() = 0;

	virtual void SetWorldShaderInfo(const WorldShaderInfo &info) = 0;

	protected:
	CRWD_EXPORT ShaderLibrary();
	CRWD_EXPORT virtual ~ShaderLibrary();

	protected:
	static ShaderLibrary::ptr					m_instance;
	static String								m_shadersPath;

	std::map<String, ShaderFile::ptr>			m_shaderFiles;
};

} // namespace Crowd

#endif // CRWD_SHADERS_LIBRARY_H_INCLUDED