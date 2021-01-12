#include "D3D10Passes.h"
#include "Renderer.h"

namespace Crowd
{

namespace D3D10
{

extern DevicePtr	g_d3dDevice;

DSGPass::DSGPass()
{

}

DSGPass::~DSGPass()
{

}

void DSGPass::Process(const RenderInfo &info)
{

}

LightsPass::LightsPass()
{

}

LightsPass::~LightsPass()
{

}

void LightsPass::Process(const RenderInfo &info)
{

}


//
//	SunPass
//
EffectPtr SunPass::m_effect(NULL);

/*Interface *SunPass::Create(const Resource::CreationInfo *creationParam)
{
	return new SunPass(m_effect);
}*/

Status SunPass::Load(EffectPtr effect)
{
	m_effect = effect;
	return Status::Success;
}
	
void SunPass::Process(const RenderInfo &info)
{
	HRESULT hr;
	Renderer &renderer = Renderer::GetInstance();

	renderer.BeginLightPass();

	g_d3dDevice->IASetInputLayout(NULL);
	g_d3dDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	hr = m_diffuseBufferView->SetResource(info.diffuseResView);
			if(FAILED(hr)) { Logger::Error("SunPass::Process"); return; }
	hr = m_normalBufferView->SetResource(info.normalResView);
			if(FAILED(hr)) { Logger::Error("SunPass::Process"); return; }
	hr = m_depthBufferView->SetResource(info.depthResView);
			if(FAILED(hr)) { Logger::Error("SunPass::Process"); return; }

	m_technique->GetPassByIndex(0)->Apply(0);
	g_d3dDevice->Draw(4, 0);
}

SunPass::SunPass()
{
	m_technique = m_effect->GetTechniqueByName("RenderSun");

	m_normalBufferView = m_effect->GetVariableByName("NormalBuf")->AsShaderResource();
	m_diffuseBufferView = m_effect->GetVariableByName("DiffuseBuf")->AsShaderResource();
	m_depthBufferView = m_effect->GetVariableByName("DepthBuf")->AsShaderResource();
}

SunPass::~SunPass()
{

}

}

} // namespace Crowd