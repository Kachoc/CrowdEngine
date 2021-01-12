#include "D3D10ResizeTexture.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr g_d3dDevice;

ResizeTexture::ResizeTexture()
{
	Status status;

	//
	//	Global
	//	
	m_resizeTextureFile = ShaderFile::Load("ResizeTexture.fx", false, NULL, &status);
	if(!status) {
		throw Exception(String("ResizeTexture::ResizeTexture() : ") + status.asString());
	}

	m_resTexCBuffer = m_resizeTextureFile->CreateConstantBuffer("ResizeTextureBuffer", 32, &status); // sizeof(ResizeTexture::Parameters)
	if(!status) {
		throw Exception(String("ResizeTexture::ResizeTexture() : ") + status.asString());
	}

	m_resizeTextureTech = m_resizeTextureFile->GetTechnique("ResizeTextureTech");
	m_sourceTexture = m_resizeTextureFile->GetShaderResourceVariable("g_SourceTexture");
}

ResizeTexture::~ResizeTexture()
{

}

void ResizeTexture::Process(RenderTargetView *dest, ShaderResourceView *source, unsigned int destWidth, unsigned int destHeight, unsigned int sourceWidth, unsigned int sourceHeight)
{
	g_d3dDevice->IASetInputLayout(NULL);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	// Actualisation des variables de shader
	m_sourceTexture->SetResource(source);

	Parameters params;
	params.SourceScaleX = ((float)((sourceWidth/destWidth)*destWidth))/(float)(sourceWidth*sourceWidth);
	params.SourceScaleY = ((float)((sourceHeight/destHeight)*destHeight))/(float)(sourceHeight*sourceHeight);
	params.DestWidth = destWidth;
	params.DestHeight = destHeight;
	params.NumSamplesX = sourceWidth/destWidth;
	params.NumSamplesY = sourceHeight/destHeight;

	m_resTexCBuffer->SetData(&params);

	// Rendu
	D3D10_VIEWPORT viewport;
	viewport.Width = destWidth;
	viewport.Height = destHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_d3dDevice->RSSetViewports(1, &viewport);

	RenderTargetView *renderTargetViews[] = { dest };
	g_d3dDevice->OMSetRenderTargets(1, renderTargetViews, NULL);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);

	m_resizeTextureTech->GetPassByIndex(0)->Apply(0);
	g_d3dDevice->Draw(4, 0);
}

} // namespace D3D10

} // namespace Crowd