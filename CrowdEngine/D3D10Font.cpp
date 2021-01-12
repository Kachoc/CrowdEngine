#include "D3D10Font.h"
#include "Renderer.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

Font::Font(unsigned int height, unsigned int width, bool bold, bool italic, const String &fontName)
{
	int weight;
	if(bold) {
		weight = FW_BOLD;
	}
	else {
		weight = FW_DONTCARE;
	}

	DFontPtr font;
	HRESULT hr = D3DX10CreateFont(g_d3dDevice, height, width, weight, 0, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName.asChar(), font.GetPtrP());
	if(FAILED(hr)) {
		throw InterfaceCreationFail("Font", GetD3DXErrorString(hr));
	}
	else
	{
		m_font = font;
	}
}

Font::~Font()
{

}

void Font::Write(const String &text, const RECT &position, unsigned int Format, const Color &color)
{
	D3D10_VIEWPORT viewport;
	viewport.Width = position.right;
	viewport.Height = position.bottom;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = position.left;
	viewport.TopLeftY = position.top;
	g_d3dDevice->RSSetViewports(1, &viewport);

	ID3D10BlendState *originalBlendState = NULL;
	ID3D10DepthStencilState *originalDepthStencilState = NULL;
	float originalBlendFactor[4];
	UINT originalSampleMask;
	UINT stencilRef;
	g_d3dDevice->OMGetBlendState(&originalBlendState, originalBlendFactor, &originalSampleMask);
	g_d3dDevice->OMGetDepthStencilState(&originalDepthStencilState, &stencilRef);
	m_font->DrawText(NULL, text.asChar(), -1, (RECT*) &position, DT_LEFT, (D3DXCOLOR) color);
	g_d3dDevice->OMSetBlendState(originalBlendState, originalBlendFactor, originalSampleMask);
	g_d3dDevice->OMSetDepthStencilState(originalDepthStencilState, stencilRef);

	SAFE_RELEASE(originalBlendState);
	SAFE_RELEASE(originalDepthStencilState);
}

} // namespace D3D10

} // namespace Crowd