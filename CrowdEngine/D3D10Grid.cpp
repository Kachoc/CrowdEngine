#include "D3D10Grid.h"
#include "D3D10Renderer.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

/*Interface *Grid::Create(const Resource::CreationInfo *creationParam)
{
	if(!creationParam) {
		return NULL;
	}
	return new Grid((CreationInfo&) *creationParam);
}

Grid::Grid(const CreationInfo &info)
{
	std::vector<PNUVVertex>			vertices; ////
	std::vector<unsigned int>		indices;

	m_vertexSize = sizeof(PNUVVertex); /////
	m_numVertices = (info.NumSideQuads+1)*(info.NumSideQuads+1);
	m_numIndices = info.NumSideQuads*info.NumSideQuads*6;

	vertices.resize(m_numVertices);
	indices.resize(m_numIndices);

	//
	//	Calcul des vertices
	//
	float x = -0.5f*(float)info.NumSideQuads*info.QuadSize;
	float u = -0.5f*(float)info.NumSideQuads*info.UVQuadSize;
	for(unsigned int i = 0; i < info.NumSideQuads+1; i++) // on parcours selon x
	{
		float y = 0.5f*(float)info.NumSideQuads*info.QuadSize;
		float v = -0.5f*(float)info.NumSideQuads*info.UVQuadSize;

		for(unsigned int j = 0; j < info.NumSideQuads+1; j++) // puis selon y (z en fait)
		{
			vertices[i*(info.NumSideQuads+1) + j].Normal = Vector3(0, 1.0f, 0);
			vertices[i*(info.NumSideQuads+1) + j].Position = Vector3(x, 0, y);
			vertices[i*(info.NumSideQuads+1) + j].UV = Vector2(u, v);

			y -= info.QuadSize;
			v += info.UVQuadSize;
		}
		x += info.QuadSize;
		u += info.UVQuadSize;
	}

	//
	//	Calcul des indices
	//
	for(unsigned int i = 0; i < info.NumSideQuads; i++) // itération quad par quad
	{
		for(unsigned int j = 0; j < info.NumSideQuads; j++)
		{
			unsigned int firstIndice = 6*(info.NumSideQuads*i+j);
			unsigned int firstVertIndice = i*(info.NumSideQuads+1) + j;

			indices[firstIndice] = firstVertIndice;
			indices[firstIndice+1] = firstVertIndice + info.NumSideQuads + 2;
			indices[firstIndice+2] = firstVertIndice + 1;

			indices[firstIndice+3] = firstVertIndice;
			indices[firstIndice+4] = firstVertIndice + info.NumSideQuads + 1;
			indices[firstIndice+5] = firstVertIndice + info.NumSideQuads + 2;

			//		0					0		1
			//
			//		2		1					2
		}
	}


	//
	//	Création des ressources
	//
	HRESULT hr;

	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.ByteWidth = m_numVertices*m_vertexSize;
	vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC; // D3D10_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initialVerticesData;
	initialVerticesData.pSysMem = &vertices[0];
	initialVerticesData.SysMemPitch = 0;
	initialVerticesData.SysMemSlicePitch = 0;
	hr = g_d3dDevice->CreateBuffer(&vertexBufferDesc, &initialVerticesData, m_vertexBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception("Grid::Grid");
	}

	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = m_numIndices*sizeof(unsigned int);
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initialIndexesData;
	initialIndexesData.pSysMem = &indices[0];
	initialIndexesData.SysMemPitch = 0;
	initialIndexesData.SysMemSlicePitch = 0;
	hr = g_d3dDevice->CreateBuffer(&indexBufferDesc, &initialIndexesData, m_indexBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception("Grid::Grid");
	}

	//
	// Rasterizer State
	//
    D3D10_RASTERIZER_DESC rasterizerState;
    rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
    rasterizerState.CullMode = D3D10_CULL_FRONT;
    rasterizerState.FrontCounterClockwise = true;
    rasterizerState.DepthBias = false;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = true;
    rasterizerState.ScissorEnable = true;
    rasterizerState.MultisampleEnable = false;
    rasterizerState.AntialiasedLineEnable = false;
    g_d3dDevice->CreateRasterizerState( &rasterizerState, m_RSState.GetPtrP() );

	m_aabb.MinPoint = Vector3(-0.5f*(float)info.NumSideQuads*info.QuadSize, 0, -0.5f*(float)info.NumSideQuads*info.QuadSize);
	m_aabb.MaxPoint = Vector3(0.5f*(float)info.NumSideQuads*info.QuadSize, 0, 0.5f*(float)info.NumSideQuads*info.QuadSize);
}

Grid::~Grid()
{

}

void Grid::Draw(const RenderInfo &info, const Camera &camera)
{
	Crowd::Renderer &renderer = Global::GetRenderer();

	UINT stride = m_vertexSize;
	UINT offset = 0;
	Buffer *vertexBuffer = m_vertexBuffer;
	g_d3dDevice->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	g_d3dDevice->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_d3dDevice->DrawIndexed(m_numIndices, 0, 0);

	renderer.AddGeometryToCount(m_numIndices/3, m_numVertices);
}*/

/*Interface *Grid::Create(const Resource::CreationInfo *creationParam)
{
	if(!creationParam) {
		return NULL;
	}
	return new Grid((CreationInfo&) *creationParam);
}*/

Grid::Grid(unsigned int numSideQuads) : Crowd::Grid(numSideQuads)
{
	std::vector<Vector2F>			vertices; ////
	std::vector<unsigned int>		indices;

	m_vertexSize = sizeof(Vector2F); /////
	m_numVertices = (numSideQuads+1)*(numSideQuads+1);
	m_numIndices = numSideQuads*numSideQuads*6;

	vertices.resize(m_numVertices);
	indices.resize(m_numIndices);

	//
	//	Calcul des vertices
	//
	//float x = -0.5f*(float)info.NumSideQuads;
	float x = 0.0f;
	for(unsigned int i = 0; i < numSideQuads+1; i++) // on parcours selon x
	{
		float y = 1.0f*(float)numSideQuads;//0.5f*(float)info.NumSideQuads;

		for(unsigned int j = 0; j < numSideQuads+1; j++) // puis selon y (z en fait)
		{
			vertices[i*(numSideQuads+1) + j] = Vector2(x, y);

			y -= 1.0f;
		}
		x += 1.0f;
	}

	//
	//	Calcul des indices
	//
	for(unsigned int i = 0; i < numSideQuads; i++) // itération quad par quad
	{
		for(unsigned int j = 0; j < numSideQuads; j++)
		{
			unsigned int firstIndice = 6*(numSideQuads*i+j);
			unsigned int firstVertIndice = i*(numSideQuads+1) + j;

			indices[firstIndice] = firstVertIndice;
			indices[firstIndice+1] = firstVertIndice + numSideQuads + 2;
			indices[firstIndice+2] = firstVertIndice + 1;

			indices[firstIndice+3] = firstVertIndice;
			indices[firstIndice+4] = firstVertIndice + numSideQuads + 1;
			indices[firstIndice+5] = firstVertIndice + numSideQuads + 2;

			//		0					0		1
			//
			//		2		1					2
		}
	}


	//
	//	Création des ressources
	//
	HRESULT hr;

	D3D10_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.ByteWidth = m_numVertices*m_vertexSize;
	vertexBufferDesc.Usage = D3D10_USAGE_DYNAMIC; // D3D10_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initialVerticesData;
	initialVerticesData.pSysMem = &vertices[0];
	initialVerticesData.SysMemPitch = 0;
	initialVerticesData.SysMemSlicePitch = 0;
	hr = g_d3dDevice->CreateBuffer(&vertexBufferDesc, &initialVerticesData, m_vertexBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception("Grid::Grid");
	}

	D3D10_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.ByteWidth = m_numIndices*sizeof(unsigned int);
	indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D10_SUBRESOURCE_DATA initialIndexesData;
	initialIndexesData.pSysMem = &indices[0];
	initialIndexesData.SysMemPitch = 0;
	initialIndexesData.SysMemSlicePitch = 0;
	hr = g_d3dDevice->CreateBuffer(&indexBufferDesc, &initialIndexesData, m_indexBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception("Grid::Grid");
	}

	//
	// Rasterizer State
	//
    D3D10_RASTERIZER_DESC rasterizerState;
    rasterizerState.FillMode = D3D10_FILL_WIREFRAME;
    rasterizerState.CullMode = D3D10_CULL_FRONT;
    rasterizerState.FrontCounterClockwise = true;
    rasterizerState.DepthBias = false;
    rasterizerState.DepthBiasClamp = 0;
    rasterizerState.SlopeScaledDepthBias = 0;
    rasterizerState.DepthClipEnable = true;
    rasterizerState.ScissorEnable = true;
    rasterizerState.MultisampleEnable = false;
    rasterizerState.AntialiasedLineEnable = false;
    g_d3dDevice->CreateRasterizerState( &rasterizerState, m_RSState.GetPtrP() );

	m_aabb.MinPoint = Vector3(-0.5f*(float)numSideQuads, 0, -0.5f*(float)numSideQuads);
	m_aabb.MaxPoint = Vector3(0.5f*(float)numSideQuads, 0, 0.5f*(float)numSideQuads);
}

Grid::~Grid()
{

}

void Grid::Draw(const RenderInfo &info, const Camera &camera)
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

} // namespace Crowd

} // namespace D3D10