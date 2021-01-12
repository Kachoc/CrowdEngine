#include "D3D10Renderer.h"
#include "D3D10Formats.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

//
// Constructeur, destructeur
//
Renderer::Renderer()
{
	m_targetWindow = NULL;
	m_width = NULL;
	m_height = NULL;
	m_isFullscreen = false;

	m_backBufferFormat = FORMAT_UNKNOWN;
	m_backBufferDXGIFormat = DXGI_FORMAT_UNKNOWN;
	m_diffuseBufferFormat = FORMAT_UNKNOWN;
	m_diffuseBufferDXGIFormat = DXGI_FORMAT_UNKNOWN;
	m_normalBufferFormat = FORMAT_UNKNOWN;
	m_normalBufferDXGIFormat = DXGI_FORMAT_UNKNOWN;
}

Renderer::~Renderer()
{
	DestroySwapChain();
}


//
// Fonctions d'initialisation
//
void Renderer::SetTargetWindowForDifRender(HWND targetWindow, FORMAT backBufferFormat, FORMAT diffuseBufferFormat, FORMAT normalBufferFormat)
{
	if(!g_d3dDevice) InitializeCrowdEngine();
	DestroySwapChain();

	if(!targetWindow) throw WParameters("D3D10::Renderer::SetTargetWindowForDifRender");

	RECT rect;
	GetClientRect(targetWindow, &rect);

	try
	{
		CreateSwapChain(targetWindow, backBufferFormat, diffuseBufferFormat, normalBufferFormat, rect.right, rect.bottom);
	}
	catch(Exception &e)
	{
		DestroySwapChain();
		e.addFront("Renderer::SetTragetWindowForDifRender:\n");
		throw e;
	}
}

#define SAFE_RELEASE_TEST(x) if(x != NULL) { count = x->Release(); x = NULL; }

void Renderer::CreateSwapChain(HWND targetWindow, FORMAT backBufferFormat, FORMAT diffuseBufferFormat, FORMAT normalBufferFormat, unsigned int width, unsigned int height)
{
	if(!g_d3dDevice) throw Exception("D3D10::Renderer::CreateSwapChain : Renderer pas initialisé");
	if(!targetWindow) throw WParameters("D3D10::Renderer::CreateSwapChain");

	// Libération des ressources
	DestroySwapChain(); // au cas où

	m_targetWindow = NULL; // valeur par défaut en cas d'erreur

	unsigned int count;

	// Récupération de l'interface IDXGIFactory
	IDXGIDevice *pIDXGIDevice;
	HRESULT hr = g_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **) &pIDXGIDevice);
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateSwapChain : ") + GetD3DXErrorString(hr));

	IDXGIAdapter *pIDXGIAdapter;
	hr = pIDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **) &pIDXGIAdapter);
	SAFE_RELEASE_TEST(pIDXGIDevice);
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateSwapChain : ") + GetD3DXErrorString(hr));

	IDXGIFactory *pDXGIFactory;
	pIDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **) &pDXGIFactory);
	SAFE_RELEASE_TEST(pIDXGIAdapter);
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateSwapChain : ") + GetD3DXErrorString(hr));

	// Création de la chaîne de swap
	DXGI_SWAP_CHAIN_DESC scDesc;
	ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	scDesc.BufferDesc.Width								= width;
	scDesc.BufferDesc.Height							= height;
	scDesc.BufferDesc.RefreshRate.Numerator				= 60;
	scDesc.BufferDesc.RefreshRate.Denominator			= 1;
	scDesc.BufferDesc.Format							= ConvertFormat(backBufferFormat);
	scDesc.BufferDesc.Scaling							= DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.ScanlineOrdering					= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.SampleDesc.Count								= 1;
	scDesc.SampleDesc.Quality							= 0;
	scDesc.BufferUsage									= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount									= 1;
	scDesc.OutputWindow									= targetWindow;
	scDesc.Windowed										= true;
	scDesc.SwapEffect									= DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags										= 0;

	hr = pDXGIFactory->CreateSwapChain(g_d3dDevice, &scDesc, m_swapChain.GetPtrP());
	SAFE_RELEASE_TEST(pDXGIFactory);
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateSwapChain : ") + GetD3DXErrorString(hr));
	}


	try
	{
		CreateBackBufferView();
		CreateBuffers(diffuseBufferFormat, normalBufferFormat, width, height);
	}
	catch(Exception &e)
	{
		DestroySwapChain();
		e.addFront("D3D10::Renderer::CreateSwapChain:\n");
		throw e;
	}

	m_backBufferFormat = backBufferFormat;
	m_backBufferDXGIFormat = ConvertFormat(backBufferFormat);
	m_targetWindow = targetWindow;
	m_width = width;
	m_height = height;
}

void Renderer::CreateBackBufferView()
{
	if(!m_swapChain) throw Exception("D3D10::Renderer::CreateBackBufferView : cible de rendu non définie");

	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (void**) m_backBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("D3D10::Renderer::CreateBackBufferView : ") + GetD3DXErrorString(hr));
	}

	hr = g_d3dDevice->CreateRenderTargetView(m_backBuffer.GetPtr(), NULL, m_backBufferView.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("D3D10::Renderer::CreateBackBufferView : ") + GetD3DXErrorString(hr));
	}
}

void Renderer::CreateBuffers(FORMAT diffuseBufferFormat, FORMAT normalBufferFormat, unsigned int width, unsigned int height)
{
	if(!m_swapChain) throw Exception("D3D10::Renderer::CreateRenderBuffers : cible de rendu non définie");

	//
	//	Depth stencil buffer
	//
	D3D10_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_depthBuffer.GetPtrP());
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));


	hr = g_d3dDevice->CreateDepthStencilView(m_depthBuffer, NULL, m_depthBufferView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}


	//
	//	Diffuse buffer
	//
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = ConvertFormat(diffuseBufferFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_diffuseBuffer.GetPtrP());
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateRenderTargetView(m_diffuseBuffer, NULL, m_diffuseBufferView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}


	//
	//	Normal buffer
	//
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = ConvertFormat(normalBufferFormat);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_normalBuffer.GetPtrP());
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateRenderTargetView(m_normalBuffer, NULL, m_normalBufferView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}


	//
	// Diffuse copy buffer
	//
	hr = g_d3dDevice->CreateShaderResourceView(m_diffuseBuffer, NULL, m_diffuseResView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}

	//
	// Normal copy buffer
	//
	hr = g_d3dDevice->CreateShaderResourceView(m_normalBuffer, NULL, m_normalResView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}

	//
	// Depth/stencil copy buffer
	//
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_depthRes.GetPtrP());
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateShaderResourceView(m_depthRes, NULL, m_depthResView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}

	//
	//	LightsPassTarget
	//
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //DXGI_FORMAT_R11G11B10_FLOAT; ///////////////////
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_lightPassTarget.GetPtrP());
	if(FAILED(hr)) throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));

	hr = g_d3dDevice->CreateRenderTargetView(m_lightPassTarget, NULL, m_lightPassTargetView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}

	hr = g_d3dDevice->CreateShaderResourceView(m_lightPassTarget, NULL, m_lightPassTargetResView.GetPtrP());
	if(FAILED(hr)) {
		DestroySwapChain();
		throw Exception(String("D3D10::Renderer::CreateRenderBuffers : ") + GetD3DXErrorString(hr));
	}

	//
	//	Formats de buffers
	//
	m_diffuseBufferFormat = diffuseBufferFormat;
	m_diffuseBufferDXGIFormat = ConvertFormat(diffuseBufferFormat);
	m_normalBufferFormat = normalBufferFormat;
	m_normalBufferDXGIFormat = ConvertFormat(normalBufferFormat);
}

void Renderer::DestroyBuffers()
{
	m_backBuffer = NULL;
	m_backBufferView = NULL;

	m_diffuseBuffer = NULL;
	m_diffuseBufferView = NULL;
	m_normalBuffer = NULL;
	m_normalBufferView = NULL;
	m_depthBuffer = NULL;
	m_depthBufferView = NULL;

	m_diffuseResView = NULL;
	m_normalResView = NULL;
	m_depthRes = NULL;
	m_depthResView = NULL;

	m_lightPassTarget = NULL;
	m_lightPassTargetView = NULL;
	m_lightPassTargetResView = NULL;
}

void Renderer::DestroySwapChain()
{
	DestroyBuffers();

	m_swapChain = NULL;
}

void Renderer::GetRenderInfo(RenderInfo &info) const
{
	RECT rect;
	GetClientRect(m_targetWindow, &rect);
	info.targetWidth = rect.right;
	info.targetHeight = rect.bottom;
	info.backBuffer = m_backBuffer;
	info.backBufferView = m_backBufferView;
	info.diffuseBufferView = m_diffuseBufferView;
	info.normalBufferView = m_normalBufferView;
	info.depthView = m_depthBufferView;
	info.diffuseResView = m_diffuseResView;
	info.normalResView = m_normalResView;
	info.depthResView = m_depthResView;
	info.lightPassTarget = m_lightPassTarget;
	info.lightPassTargetResView = m_lightPassTargetResView;
}

void Renderer::ResizeTargetWindow(unsigned int width, unsigned int height)
{
	if(!m_swapChain) {
		throw Exception("D3D10::Renderer::ResizeRenderTarget : pas de cible de rendu");
	}
	
	if(m_width != width || m_height != height)
	{
		DestroyBuffers();
		g_d3dDevice->OMSetRenderTargets(0, NULL, NULL);

		DXGI_MODE_DESC mDesc;
		mDesc.Width = width;
		mDesc.Height = height;
		mDesc.RefreshRate.Numerator = 60;
		mDesc.RefreshRate.Denominator = 1;
		mDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		mDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		mDesc.Format = m_backBufferDXGIFormat;

		HRESULT hr = m_swapChain->ResizeTarget(&mDesc);
		if(FAILED(hr)) {
			throw Exception(String("D3D10::Renderer::ResizeTargetWindow : ") +GetD3DXErrorString(hr));
		}

		hr = m_swapChain->ResizeBuffers(2, width, height, m_backBufferDXGIFormat, 0);
		if(FAILED(hr)) {
			throw Exception(String("D3D10::Renderer::ResizeTargetWindow : ") + GetD3DXErrorString(hr));
		}


		try
		{
			CreateBackBufferView();
			CreateBuffers(m_diffuseBufferFormat, m_normalBufferFormat, width, height);
		}
		catch(Exception &e)
		{
			e.addFront("D3D10::Renderer:ResizeTargetWindow:\n");
		}

		m_width = width;
		m_height = height;
	}
}

HWND Renderer::GetTargetWindow() const
{
	return m_targetWindow;
}

FORMAT Renderer::GetBackBufferFormat() const
{
	return m_backBufferFormat;
}

void Renderer::SetFullscreenState(bool fullscreen)
{
	if(!m_swapChain) throw Exception("D3D10::Renderer::SetFullscreenState : assignement de la cible de rendu non effectué");

	if(fullscreen == m_isFullscreen) return;

	HRESULT hr = m_swapChain->SetFullscreenState(fullscreen, NULL);
	if(FAILED(hr)) {
		throw Exception(String("D3D10::Renderer::SetFullscreenState : ") + GetD3DXErrorString(hr));
	}
	m_isFullscreen = fullscreen;
}

bool Renderer::IsFullscreen() const
{
	return m_isFullscreen;
}

void Renderer::RenderScene()
{
	std::vector<RenderTargetView*> targetsViews(3);
	targetsViews[0] = m_backBufferView;
	targetsViews[1] = m_diffuseBufferView;
	targetsViews[2] = m_normalBufferView;

	RenderInfo renderInfo;

	for(unsigned int i = 0; i < m_intermediatePasses.size(); i++)
	{
		m_intermediatePasses[i]->Process(renderInfo);
	}
}

void Renderer::BeginGeometryPass()
{
	if(!m_swapChain) throw Exception("D3D10::Renderer::BeginScene() : pas de cible de rendu");

	RECT rect;
	GetClientRect(m_targetWindow, &rect);
	if(rect.right <= 0 || rect.bottom <= 0) return;
	if(rect.right != m_width || rect.bottom != m_height) // on redimensionne les buffers
	{
		ResizeTargetWindow(rect.right, rect.bottom);
	}

	RenderInfo renderInfo;
	if(m_beginPass)
	{
		m_beginPass->Process(renderInfo);
	}
	else
	{
		RECT rect;
		GetClientRect(m_targetWindow, &rect);

		D3D10_VIEWPORT viewport;
		viewport.Width = rect.right;
		viewport.Height = rect.bottom;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		g_d3dDevice->RSSetViewports(1, &viewport);

		float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		g_d3dDevice->ClearRenderTargetView(m_diffuseBufferView, rgba);
		float normals[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		g_d3dDevice->ClearRenderTargetView(m_normalBufferView, normals);
		g_d3dDevice->ClearDepthStencilView(m_depthBufferView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

		RenderTargetView *renderTargetViews[] = { m_diffuseBufferView, m_normalBufferView };
		g_d3dDevice->OMSetRenderTargets(2, renderTargetViews, m_depthBufferView);
		g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);
	}
}

void Renderer::BeginLightPass()
{
	float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//g_d3dDevice->ClearRenderTargetView(m_lightPassTargetView, rgba); ////////////////////////////////////////

	g_d3dDevice->CopyResource(m_depthRes, m_depthBuffer);

	RECT rect;
	GetClientRect(m_targetWindow, &rect);

	D3D10_VIEWPORT viewport;
	viewport.Width = rect.right;
	viewport.Height = rect.bottom;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_d3dDevice->RSSetViewports(1, &viewport);

	RenderTargetView *renderTargetViews[] = { m_lightPassTargetView }; // { m_backBufferView };
	g_d3dDevice->OMSetRenderTargets(1, renderTargetViews, NULL);
	//g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void Renderer::BeginPostPass()
{
	float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//g_d3dDevice->ClearRenderTargetView(m_backBufferView, rgba);

	RECT rect;
	GetClientRect(m_targetWindow, &rect);

	D3D10_VIEWPORT viewport;
	viewport.Width = rect.right;
	viewport.Height = rect.bottom;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	g_d3dDevice->RSSetViewports(1, &viewport);

	RenderTargetView *renderTargetViews[] = { m_backBufferView };
	g_d3dDevice->OMSetRenderTargets(1, renderTargetViews, NULL);
	g_d3dDevice->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void Renderer::Present(bool VerticalSync)
{
	if(!m_swapChain) {
		throw Exception("D3D10::Renderer::BeginScene() : pas de cible de rendu");
	}

	HRESULT hr = m_swapChain->Present(VerticalSync, 0);
}

} // namespace D3D10

} // namespace Crowd