#include "D3D10PostProcess.h"
#include "D3D10Renderer.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

ShaderFile::ptr					PostProcess::m_postProcessFile;
EffectPtr						PostProcess::m_postProcessEffect;

ResizeTexture::ptr				PostProcess::m_resizeTexture;

PostProcess::PostProcess()
{
	m_averageColor = ColorRGB(0.25f, 0.25f, 0.25f);
	/*m_postProcessTech = m_postProcessEffect->GetTechniqueByName("PostProcessTech");

	m_lightPassOutputResVar = m_postProcessEffect->GetVariableByName("LightPassOutput")->AsShaderResource();*/
}

PostProcess::~PostProcess()
{

}

PostProcess *PostProcess::Create()
{
	return new PostProcess;
}

Status PostProcess::Load(ShaderFile::ptr worldShader)
{
	Status status;
	m_postProcessFile = ShaderFile::Load("PostProcess.fx", false, worldShader, &status);
	if(!status) {
		return status;
	}
	m_postProcessEffect = m_postProcessFile->GetEffect();

	return Status::Success;
}

Status PostProcess::Initialize()
{
	Status status;
	m_postProcessCBuffer = m_postProcessFile->CreateConstantBuffer("PostProcessBuffer", sizeof(PostProcess::Parameters), &status);
	if(!status) {
		return Status(String("PostProcess::Initialize : ") + status.asString());
	}

	m_postProcessTech = m_postProcessEffect->GetTechniqueByName("PostProcessTech");

	m_lightPassOutputResVar = m_postProcessEffect->GetVariableByName("g_LightPassOutput")->AsShaderResource();
	m_depthBufResVar = m_postProcessEffect->GetVariableByName("g_DepthBuf")->AsShaderResource();

	m_resizeTexture = new D3D10::ResizeTexture;

	return Status::Success;
}

void PostProcess::PerformPostProcessRender(const RenderInfo &renderInfo)
{
	// Mise à jour des buffers pour le traitement du HDRI
	ResizeBuffers(renderInfo.lightPassTarget);

	// Rendu
	HRESULT hr;

	g_d3dDevice->IASetInputLayout(NULL);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Actualisation des variables de shader
	m_postProcessCBuffer->SetData(&m_parameters);

	hr = m_lightPassOutputResVar->SetResource(renderInfo.lightPassTargetResView);
			if(FAILED(hr)) { Logger::Error("PostProcess::PerformPostProcessRender"); return; }
	hr = m_depthBufResVar->SetResource(renderInfo.depthResView);
			if(FAILED(hr)) { Logger::Error("PostProcess::PerformPostProcessRender"); return; }

	// Rendu
	m_postProcessTech->GetPassByIndex(0)->Apply(0);
	g_d3dDevice->Draw(4, 0);

	// Récupération des informations de couleur
	D3D10_TEXTURE2D_DESC desc;
	renderInfo.lightPassTarget->GetDesc(&desc);
	D3D10_TEXTURE2D_DESC destDesc;
	m_resizedDisplayBuffer->GetDesc(&destDesc);
	g_d3dDevice->CopyResource(m_displayBufferCopy, renderInfo.lightPassTarget);
	m_resizeTexture->Process(m_resizedDisplayBufferView, m_displayBufferCopyView, destDesc.Width, destDesc.Height, desc.Width, desc.Height);
	g_d3dDevice->CopyResource(m_resizedDisplayBufferCopy, m_resizedDisplayBuffer);

	D3D10_MAPPED_TEXTURE2D mappedTex;
	hr = m_resizedDisplayBufferCopy->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_READ, 0, &mappedTex);
	if(FAILED(hr)) {
		Logger::Error("PostProcess::PerformPostProcessRender");
		return;
	}

	Color *colorArray = (Color*) mappedTex.pData;
	ColorRGB averageColor = ColorRGB(0, 0, 0);
	for(unsigned int y = 0; y < destDesc.Height; y++)
	{
		ColorRGB avRowColor(0, 0, 0);
		for(unsigned int x = 0; x < destDesc.Width; x++)
		{
			avRowColor.r += colorArray[y*destDesc.Width + x].r*colorArray[y*destDesc.Width + x].r;
			avRowColor.g += colorArray[y*destDesc.Width + x].g*colorArray[y*destDesc.Width + x].g;
			avRowColor.b += colorArray[y*destDesc.Width + x].b*colorArray[y*destDesc.Width + x].b;
		}
		averageColor += avRowColor/(float)(destDesc.Width);
	}
	averageColor /= (float)(destDesc.Height);
	m_averageColor = ColorRGB(sqrt(averageColor.r), sqrt(averageColor.g), sqrt(averageColor.b));
	m_resizedDisplayBufferCopy->Unmap(D3D10CalcSubresource(0, 0, 1));
}

void PostProcess::Unload()
{
	m_postProcessFile = NULL;
	m_postProcessEffect = NULL;

	m_resizeTexture = NULL;
}

Status PostProcess::ResizeBuffers(Texture2DPtr displayBuffer)
{
	HRESULT hr;
	D3D10_TEXTURE2D_DESC desc;
	displayBuffer->GetDesc(&desc);
	unsigned int displayBufferWidth = desc.Width;
	unsigned int displayBufferHeight = desc.Height;

	if(m_displayBufferCopy) {
		D3D10_TEXTURE2D_DESC displayCopyDesc;
		m_displayBufferCopy->GetDesc(&displayCopyDesc);
		if(displayCopyDesc.Width == displayBufferWidth && displayCopyDesc.Height == displayBufferHeight) {
			return Status::Success;
		}
	}
	// Les dimensions ne sont pas les bonnes, ou les buffers n'ont pas été créés.
	m_displayBufferCopy = NULL;
	m_displayBufferCopyView = NULL;
	m_resizedDisplayBuffer = NULL;
	m_resizedDisplayBufferView = NULL;
	m_resizedDisplayBufferCopy = NULL;

	//
	//	DisplayBufferCopy
	//
	/*desc.Width = displayBufferWidth;
	desc.Height = displayBufferHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;*/
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = 0;
	//desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_displayBufferCopy.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("D3D10::PostProcess::ResizeBuffers : ") + GetD3DXErrorString(hr));
	}

	hr = g_d3dDevice->CreateShaderResourceView(m_displayBufferCopy, NULL, m_displayBufferCopyView.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("D3D10::PostProcess::ResizeBuffers : ") + GetD3DXErrorString(hr));
	}


	//
	//	ResizedDisplayBuffer
	//
	desc.Width = displayBufferWidth/16; ///
	desc.Height = displayBufferHeight/16; ///
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_resizedDisplayBuffer.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("D3D10::PostProcess::ResizeBuffers : ") + GetD3DXErrorString(hr));
	}

	hr = g_d3dDevice->CreateRenderTargetView(m_resizedDisplayBuffer, NULL, m_resizedDisplayBufferView.GetPtrP());
	if(FAILED(hr)) {
		return Status(String("D3D10::PostProcess::ResizeBuffers : ") + GetD3DXErrorString(hr));
	}


	//
	//	ResizedDisplayBufferCopy
	//
	desc.Usage = D3D10_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;

	hr = g_d3dDevice->CreateTexture2D(&desc, NULL, m_resizedDisplayBufferCopy.GetPtrP());
	if(FAILED(hr)) {
		throw Exception(String("D3D10::PostProcess::ResizeBuffers : ") + GetD3DXErrorString(hr));
	}

	return Status::Success;
}

} // namespace D3D10

} // namespace Crowd