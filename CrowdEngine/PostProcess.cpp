#include "PostProcess.h"
#include "D3D10PostProcess.h"

namespace Crowd
{

PostProcess::ptr PostProcess::m_instance(new D3D10::PostProcess);

PostProcess &PostProcess::GetInstance()
{
	return *m_instance;
}

PostProcess::PostProcess()
{
	m_fogDownScaleRatio = 4;
}

PostProcess::~PostProcess()
{

}

void PostProcess::SetParameters(const Parameters &params)
{
	m_parameters = params;
}

void PostProcess::GetParameters(Parameters &params) const
{
	params = m_parameters;
}

void PostProcess::SetFogDownScaleRatio(unsigned int ratio)
{
	m_fogDownScaleRatio = ratio;
}

ColorRGB PostProcess::GetAverageColor() const
{
	return m_averageColor;
}

} // namespace Crowd