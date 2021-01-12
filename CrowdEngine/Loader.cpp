#include "Loader.h"

namespace Crowd
{

//
//	MFileLoader
//

Status MFileLoader::LoadFile(MFileInfo::ptr &fileInfo, const String &fileName)
{
	Status status;
	MFileInfo::ptr info = new MFileInfo;
	info->Release();

	std::ifstream file(fileName, std::ios::binary);
	if(!file) {
		return Status("Erreur lors de l'ouverture du fichier: " + fileName);
	}

	// Variables utilisées dans la suite
	MeshInfo::ptr nMesh;
	CPM_INFO type;
	
	
	// Boucle de lecture
	file.read((char*) &type, 4);

	while(!file.eof() && type != CPM_END_FILE)
	{
		switch(type)
		{
			case CPM_END_FILE:
				break;

			case CPM_MESH:
				nMesh = new MeshInfo;
				nMesh->Release();
				status = LoadMesh(*nMesh, file, type);
				info->meshes.push_back(nMesh);
				break;
			default:
				return Status("Fichier : " + fileName + " corrompu.");
		}

		if(!status) {
			status.addFront("MFileLoader::LoadFile : Erreur lors de la lecture du fichier: " + fileName + " : ");
			return status;
		}
	}

	fileInfo = info;

	return Status::Success;
}

void MFileLoader::UnloadFile(const String &fileName)
{

}

Status MFileLoader::LoadMesh(MeshInfo &mesh, std::ifstream &file, CPM_INFO &lastType)
{
	Status status;
	CPM_INFO type = __32;

	String s;
	int n;
	Material material;
	Matrix transform;
	
	while(!file.eof())
	{
		file.read((char*) &type, 4);
		switch(type)
		{
			case CPM_MESH_NAME:
				status = ExtractString(mesh.name, file);
				break;
			case CPM_TRANSFORM_MATRIX:
				status = ExtractMatrix(mesh.transformMatrix, file);
				break;

			case CPM_NUM_VERTICES:
				file.read((char*) &mesh.numVertices, 4);
				break;
			case CPM_VERTICES:
				mesh.vertices.resize(mesh.numVertices);
				file.read((char*) &mesh.vertices[0], 12*mesh.numVertices);
				/*transform = mesh.transformMatrix;
				transform.Transpose();
				for(unsigned int i = 0; i < mesh.numVertices; i++)
				{
					mesh.vertices[i] = Vector4(mesh.vertices[i], 1.0f)*transform;
				}*/
				break;
			case CPM_NORMALS:
				mesh.normals.resize(mesh.numVertices);
				file.read((char*) &mesh.normals[0], 12*mesh.numVertices);
				break;
			case CPM_UVS:
				mesh.uvs.resize(mesh.numVertices);
				file.read((char*) &mesh.uvs[0], 8*mesh.numVertices);
				break;

			case CPM_TRIANGLES:
				file.read((char*) &mesh.triangles[0], 4*mesh.triangles.size());
				break;
			case CPM_NUM_TRIANGLES:
				file.read((char*) &n, 4);
				mesh.triangles.resize(n*3);
				break;

			case CPM_MATERIAL:
				material = Material();
				LoadMaterial(material, file, lastType);
				mesh.materials.resize(mesh.materials.size() + 1);
				mesh.materials[ mesh.materials.size() - 1 ] = material;
				n = (int) file.tellg();
				if(!file.eof()) {
					file.seekg(n - 4);
				}
				break;

			default:
				lastType = type;
				return Status::Success;
		}
	}

	return Status::Success;
}

Status MFileLoader::LoadMaterial(Material &material, std::ifstream &file, CPM_INFO &lastType)
{
	Status status;
	CPM_INFO type;

	while(!file.eof())
	{
		file.read((char*) &type, 4);
		switch(type)
		{
			case CPM_COLOR:
				file.read((char*) &material.diffuse, 16);
				break;
			case CPM_COLOR_TEX:
				status = ExtractString(material.colorTex, file);
				break;
			case CPM_SPECULAR_POWER:
				file.read((char*) &material.specularPower, 4);
				break;
			case CPM_SPECULAR_POWER_TEX:
				status = ExtractString(material.specularMap, file);
				break;
			case CPM_AMBIENT:
				file.read((char*) &material.ambient, 16);
				break;
			case CPM_AMBIENT_TEX:
				status = ExtractString(material.ambientTex, file);
				break;
			case CPM_NORMAL_MAP:
				status = ExtractString(material.normalMap, file);
				break;
			case CPM_BUMP_MAP:
				status = ExtractString(material.bumpMap, file);
				break;
			case CPM_ALPHA:
				file.read((char*) &material.alpha, 16);
				break;
			case CPM_ALPHA_TEX:
				status = ExtractString(material.alphaMap, file);
				break;

			default:
				lastType = type;
				return Status::Success;
		}
	}
	return Status::Success;
}

Status MFileLoader::ExtractString(String &s, std::ifstream &file)
{
	s = "";

	char c;
	file.get(c); // le premier caractère est "
	file.get(c);

	while(c != '"')
	{
		s = s + c;
		file.get(c);
	}

	return Status::Success;
}

Status MFileLoader::ExtractMatrix(Matrix &m, std::ifstream &file)
{
	file.read((char*) &m, 4*16);

	return Status::Success;
}



//
//	Loader
//
//Loader::ptr Loader::m_instance(NULL);

} // namespace Crowd