#ifndef CRWD_D3D10_PASSES_H_INCLUDED
#define CRWD_D3D10_PASSES_H_INCLUDED

#include "D3DEntry.h"
#include "RenderPass.h"

namespace Crowd
{

namespace D3D10
{

Status LoadPasses();

/// Draw Standard Geometry Pass
class DSGPass : public Crowd::RenderPass
{
	public:
	DSGPass();
	virtual ~DSGPass();

	virtual void Process(const RenderInfo &info);
};

class LightsPass : public Crowd::RenderPass
{
	public:
	LightsPass();
	virtual ~LightsPass();

	virtual void Process(const RenderInfo &info);
};

class SunPass : public Crowd::SunPass
{
	public:
	typedef SmartPtr<SunPass, IPol> ptr;

	SunPass();
	virtual ~SunPass();

	static Status Load(EffectPtr effect);
	
	virtual void Process(const RenderInfo &info);

	protected:
	static EffectPtr						m_effect;

	EffectTechniquePtr						m_technique;

	EffectShaderResourceVariablePtr			m_normalBufferView;
	EffectShaderResourceVariablePtr			m_diffuseBufferView;
	EffectShaderResourceVariablePtr			m_depthBufferView;
};

} // namespace D3D10

} // namespace Crowd

#endif // CRWD_D3D10_PASSES_H_INCLUDED