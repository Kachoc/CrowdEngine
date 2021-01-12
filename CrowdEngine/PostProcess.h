#ifndef CRWD_POST_PROCESS_INCLUDED
#define CRWD_POST_PROCESS_INCLUDED
///
/// PostProcessManager
/// Est en charge du processus de post-rendering/post-processing.
///

#include "SmartPtr.h"

#include "RenderInfo.h"

#include "CRWDMath.h"

namespace Crowd
{

class PostProcess
{
	public:
	friend class RefCnt<PostProcess>;
	typedef SmartPtr<PostProcess, RefCnt> ptr;

	friend class EngineLoader;
	friend class Global;

	CRWD_EXPORT static PostProcess &GetInstance();

	CRWD_EXPORT PostProcess();
	CRWD_EXPORT virtual ~PostProcess();

	struct Parameters
	{
		Color	FogColor;
		float	FogIntensity;
		float	FogHeightDim;
		float	ColorIntensityAdjustment;
		float	unused1;
	};

	CRWD_EXPORT virtual void SetParameters(const Parameters &params);
	CRWD_EXPORT virtual void GetParameters(Parameters &params) const;

	CRWD_EXPORT virtual void SetFogDownScaleRatio(unsigned int ratio);

	CRWD_EXPORT virtual ColorRGB GetAverageColor() const;

	//virtual void Initialize() = 0;
	virtual void PerformPostProcessRender(const RenderInfo &renderInfo) = 0;

	protected:
	static PostProcess::ptr			m_instance;

	Parameters						m_parameters;
	unsigned int					m_fogDownScaleRatio;
	ColorRGB						m_averageColor;
};

} // namespace Crowd

#endif // CRWD_POST_PROCESS_INCLUDED