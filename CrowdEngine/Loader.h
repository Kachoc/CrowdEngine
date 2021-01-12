#ifndef CRWD_LOADER_H_INCLUDED
#define CRWD_LOADER_H_INCLUDED

#include "Status.h"

#include "Resources.h"
#include "CEString.h"
#include "Debug.h"

#include "CRWDMath.h"

#include "CPM.h"

#include <map>
#include <fstream>

namespace Crowd
{

struct Material;
struct SkeletonInfo;
struct AnimationInfo;
struct MeshInfo;

class MFileInfo;
class MFileLoader;

struct Material
{
	Material() : diffuse(1, 1, 1, 1), ambient(0, 0, 0, 0), specularPower(255), alpha(1, 1, 1, 1) {}

	Color						diffuse;
	Color						ambient;
	float						specularPower;
	Color						alpha;

	String						colorTex;
	String						ambientTex;
	String						normalMap;
	String						bumpMap;
	String						specularMap;
	String						alphaMap;

	//std::vector<unsigned int>	tris; // faces du mesh concernées
};

struct SkeletonInfo : public Resource
{
	INTERFACE_PTR(SkeletonInfo);
};

struct AnimationInfo : public Resource
{
	INTERFACE_PTR(AnimationInfo);
};

struct MeshInfo : public Resource
{
	INTERFACE_PTR(MeshInfo);

	public:
	MeshInfo() : numVertices(0), transformMatrix(Matrix::Identity()) {}

	String								name;
	Matrix								transformMatrix;

	unsigned int						numVertices;
	std::vector<unsigned int>			triangles;
	std::vector<Vector3>				vertices;
	std::vector<Vector3>				normals;
	std::vector<Vector3>				tangents;
	std::vector<Vector3>				bitangents;
	std::vector<Vector2>				uvs;

	std::vector<Material>				materials;

	SkeletonInfo::ptr					m_skeleton;
	std::vector<AnimationInfo::ptr>		m_animations;

};

class MFileInfo : public Resource
{
	INTERFACE_PTR(MFileInfo);

	public:
	std::vector<SkeletonInfo::ptr>		skeletons;
	std::vector<AnimationInfo::ptr>		animations;
	std::vector<MeshInfo::ptr>			meshes;
};

/// Mesh File Loader: charge les meshes et informations associées (textures, armatures, matériaux...)
class MFileLoader : public Interface
{
	INTERFACE_PTR(MFileLoader);

	public:
	/// Charge les informations d'un mesh à partir d'un fichier.
	static Status LoadFile(MFileInfo::ptr &fileInfo, const String &fileName);

	/// Décharge les informations d'un mesh préalablement chargé.
	static void UnloadFile(const String &fileName);


	protected:
	static Status LoadMesh(MeshInfo &mesh, std::ifstream &file, CPM_INFO &lastType);
	static Status LoadMaterial(Material &material, std::ifstream &file, CPM_INFO &lastType);

	static Status ExtractString(String &s, std::ifstream &file);
	static Status ExtractMatrix(Matrix &m, std::ifstream &file);

	protected:
	std::map< std::string, MFileInfo >	m_infos;
};

/*class Loader
{
	public:
	typedef SmartPtr<Loader, RefCnt> ptr;

	// Static Load
	virtual Resource::ptr Load(MFileInfo &info, Status *status = NULL) = 0;

	protected:
	static Loader::ptr m_instance;
};*/

} // namespace Crowd

#endif // CRWD_LOADER_H_INCLUDED