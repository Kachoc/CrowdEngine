#include "D3D10MeshLoader.h"
#include "Renderer.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

//
//	LoadedMesh
//
LoadedMesh::LoadedMesh(const AABB &aabb, BufferPtr vertexBuffer, unsigned int numVertices, unsigned int vertexSize, BufferPtr indexBuffer, unsigned int numIndices)
{
	m_vertexBuffer = vertexBuffer;
	m_numVertices = numVertices;
	m_vertexSize = vertexSize;
	m_indexBuffer = indexBuffer;
	m_numIndices = numIndices;
	m_aabb = aabb;
}

LoadedMesh::~LoadedMesh()
{

}

Interface *LoadedMesh::Create(const AABB &aabb, BufferPtr vertexBuffer, unsigned int numVertices, unsigned int vertexSize, BufferPtr indexBuffer, unsigned int numIndices)
{
	return new LoadedMesh(aabb, vertexBuffer, numVertices, vertexSize, indexBuffer, numIndices);
}

void LoadedMesh::Draw(const RenderInfo &info, const Camera &camera)
{
	Crowd::Renderer &renderer = Renderer::GetInstance();

	UINT stride = m_vertexSize;
	UINT offset = 0;
	Buffer *vertexBuffer = m_vertexBuffer;
	g_d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_d3dDevice->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_d3dDevice->DrawIndexed(m_numIndices, 0, 0);

	renderer.AddGeometryToCount(m_numIndices/3, m_numVertices);
}

const AABB &LoadedMesh::GetAABB() const
{
	return m_aabb;
}


//
//	MeshLoader
//

Crowd::Mesh *MeshLoader::Load(const String fileName, int loadValue, Status *status)
{
	std::ifstream file(fileName, std::ios::in);
	if(!file) {
		if(status) { *status = Status(String("Impossible d'ouvrir le fichier: ") + fileName); }
		return NULL;
	}

	Mesh mesh;

	std::string s;
	unsigned int n;
	while(!file.eof())
	{
		file >> s;

		if(s == "Vertices") {
			file >> n;
			mesh.vertices.resize(n);
			mesh.normals.resize(n);
			mesh.UVs.resize(n);

			float x, y, z;
			for(unsigned int i = 0; i < n; i++)
			{
				file >> x >> y >> z;
				mesh.vertices[i] = Vector3(x, y, z);
			}
		}
		else if(s == "Triangles") {
			file >> n;
			mesh.indices.resize(3*n);
			for(unsigned int i = 0; i < n; i++)
			{
				file >> mesh.indices[3*i] >> mesh.indices[3*i+1] >> mesh.indices[3*i+2];
			}
		}
		else if(s == "Normals") {
			float x, y, z;
			for(unsigned int i = 0; i < mesh.vertices.size(); i++)
			{
				file >> x >> y >> z;
				mesh.normals[i] = Vector3(x, y, z);
			}
		}
		else if(s == "UVs") {
			float u, v;
			for(unsigned int i = 0; i < mesh.UVs.size(); i++)
			{
				file >> u >> v;
				mesh.UVs[i] = Vector2(u, v);
			}
		}
		else { // Erreur
			//if(status) { *status = Status(String("Erreur lors du chargement du fichier: ") + fileName); }
			//return NULL;
		}
	}

	return CreateMesh(fileName, loadValue, mesh, status);
}

Crowd::Mesh *MeshLoader::CreateMesh(String filename, int loadValue, MeshLoader::Mesh &mesh, Status *status)
{
	BufferPtr vertexBuffer;
	BufferPtr indexBuffer;

	AABB aabb;
	aabb.MinPoint = mesh.vertices[0];
	aabb.MaxPoint = mesh.vertices[0];

	for(unsigned int i = 1; i < mesh.vertices.size(); i++)
	{
		for(unsigned int j = 0; j < 3; j++) {
			if(mesh.vertices[i].f[j] < aabb.MinPoint.f[j]) {
				aabb.MinPoint.f[j] = mesh.vertices[i].f[j];
			}
			else if(mesh.vertices[i].f[j] > aabb.MaxPoint.f[j]) {
				aabb.MaxPoint.f[j] = mesh.vertices[i].f[j];
			}
		}
	}


	std::vector<PNUVVertex> PNUVVertices;
	unsigned int vertexSize = 0;
	void *initialVerticesDataMem = NULL;
	if(loadValue & CPM_LOAD_NORMAL)
	{
		vertexSize = sizeof(PNUVVertex);
		PNUVVertices.resize(mesh.vertices.size());

		// Calculer les normales si nécessaire...

		float width = aabb.MaxPoint.x - aabb.MinPoint.x, depth = aabb.MaxPoint.z - aabb.MinPoint.z;
		float xCenter = (aabb.MaxPoint.x + aabb.MinPoint.x)/2.0f;
		float zCenter = (aabb.MaxPoint.z + aabb.MinPoint.z)/2.0f;

		for(unsigned int i = 0; i < PNUVVertices.size(); i++)
		{
			PNUVVertices[i].Position = mesh.vertices[i];
			PNUVVertices[i].Normal = mesh.normals[i];
			if(mesh.UVs.size() != 0) {
				PNUVVertices[i].UV = mesh.UVs[i];
			}
			else {
				PNUVVertices[i].UV = Vector2F((mesh.vertices[i].x-xCenter)/width, (mesh.vertices[i].z-zCenter)/depth);
			}
		}
		initialVerticesDataMem = &PNUVVertices[0];
	}
	else
	{
		vertexSize = sizeof(Vector3);
		initialVerticesDataMem = &mesh.vertices[0];
	}

	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.ByteWidth = mesh.vertices.size()*vertexSize;
	vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initialVerticesData;
	initialVerticesData.pSysMem = initialVerticesDataMem;
	initialVerticesData.SysMemPitch = 0;
	initialVerticesData.SysMemSlicePitch = 0;
	HRESULT hr = g_d3dDevice->CreateBuffer(&vertexBufferDesc, &initialVerticesData, vertexBuffer.GetPtrP());
	if(FAILED(hr)) {
		if(status) { *status = Status(String("Erreur lors de l'ouverture du fichier: ") + filename + GetD3DXErrorString(hr)); }
		return NULL;
	}

	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = mesh.indices.size()*sizeof(unsigned int);
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initialIndexesData;
	initialIndexesData.pSysMem = &mesh.indices[0];
	initialIndexesData.SysMemPitch = 0;
	initialIndexesData.SysMemSlicePitch = 0;
	hr = g_d3dDevice->CreateBuffer(&indexBufferDesc, &initialIndexesData, indexBuffer.GetPtrP());
	if(FAILED(hr)) {
		if(status) { *status = Status(String("Erreur lors de l'ouverture du fichier: ") + filename + GetD3DXErrorString(hr)); }
		return NULL;
	}
	return new LoadedMesh(aabb, vertexBuffer, mesh.vertices.size(), vertexSize, indexBuffer, mesh.indices.size());
}

} // namespace D3D10

} // namespace Crowd