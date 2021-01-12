//
//	D3D10Renderer.h
//
// Version D3D10 du Renderer.


#ifndef D3D10_RENDERER_H_INCLUDED
#define D3D10_RENDERER_H_INCLUDED

#include "Renderer.h"

namespace Crowd
{

namespace D3D10
{

class Renderer : public Crowd::Renderer
{
	friend class Crowd::Renderer;

	//
	//	Constructeur, destructeur
	//
	protected:
	Renderer();
	virtual ~Renderer();

	public:
	//
	// Fonctions d'initialisation
	//
	public:
	virtual void					SetTargetWindowForDifRender(HWND targetWindow, FORMAT backBufferFormat, FORMAT diffuseBufferFormat, FORMAT normalBufferFormat);

	protected:
	virtual void					CreateSwapChain(HWND targetWindow, FORMAT backBufferFormat, FORMAT diffuseBufferFormat, FORMAT normalBufferFormat, unsigned int width, unsigned int height);
	virtual void					CreateBackBufferView();
	virtual void					CreateBuffers(FORMAT diffuseBufferFormat, FORMAT normalBufferFormat, unsigned int width, unsigned int height);
	virtual void					DestroyBuffers();
	virtual void					DestroySwapChain();


	//
	// Fonctions d'utilisation
	//
	public:
	virtual void					GetRenderInfo(RenderInfo &info) const;

	virtual void					ResizeTargetWindow(unsigned int width, unsigned int height);
	virtual HWND					GetTargetWindow() const;
	virtual FORMAT					GetBackBufferFormat() const;

	virtual void					SetFullscreenState(bool fullscreen = true);
	virtual bool					IsFullscreen() const;

	virtual void					RenderScene();
	virtual void					BeginGeometryPass();
	virtual void					BeginLightPass();
	virtual void					BeginPostPass();
	virtual void					Present(bool VerticalSync = false);



	//
	//	Attributs
	//
	protected:
	AdapterPtr						m_adapter;
	D3D_FEATURE_LEVEL				m_featureLevel;
	SwapChainPtr					m_swapChain;

	HWND							m_targetWindow;
	unsigned int					m_width;
	unsigned int					m_height;
	bool							m_isFullscreen;

	Texture2DPtr					m_backBuffer;
	RenderTargetViewPtr				m_backBufferView;

	FORMAT							m_backBufferFormat;
	DXGI_FORMAT						m_backBufferDXGIFormat;
	FORMAT							m_diffuseBufferFormat;
	DXGI_FORMAT						m_diffuseBufferDXGIFormat;
	FORMAT							m_normalBufferFormat;
	DXGI_FORMAT						m_normalBufferDXGIFormat;

	Texture2DPtr					m_diffuseBuffer; // rgb: usage normal, a: specularité
	RenderTargetViewPtr				m_diffuseBufferView;
	Texture2DPtr					m_normalBuffer;
	RenderTargetViewPtr				m_normalBufferView;
	Texture2DPtr					m_depthBuffer;
	DepthStencilViewPtr				m_depthBufferView;

	ShaderResourceViewPtr			m_diffuseResView;
	ShaderResourceViewPtr			m_normalResView;
	Texture2DPtr					m_depthRes;
	ShaderResourceViewPtr			m_depthResView;

	//
	//	Pour le post-processing
	//
	Texture2DPtr					m_lightPassTarget;
	RenderTargetViewPtr				m_lightPassTargetView;
	ShaderResourceViewPtr			m_lightPassTargetResView;
};

} // namespace D3D10

} // namespace Crowd

#endif // D3D10_RENDERER_H_INCLUDED