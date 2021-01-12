//
//	D3D10Renderer.h
//
// Version D3D10 du Renderer.


#ifndef D3D10_POST_PROCESS_H_INCLUDED
#define D3D10_POST_PROCESS_H_INCLUDED

#include "D3DEntry.h"
#include "PostProcess.h"

#include "D3D10ShaderFile.h"
#include "D3D10ResizeTexture.h"

namespace Crowd
{

namespace D3D10
{

class PostProcess : public Crowd::PostProcess
{
	friend class Crowd::PostProcess;

	public:
	typedef SmartPtr<D3D10::PostProcess, IPol> ptr;

	static PostProcess *Create();

	static Status Load(ShaderFile::ptr worldShader); /// Provisoire
	virtual Status Initialize();
	virtual void PerformPostProcessRender(const RenderInfo &renderInfo);

	static void Unload(); /// Provisoire

	protected:
	PostProcess();
	virtual ~PostProcess();

	virtual Status ResizeBuffers(Texture2DPtr displayBuffer);

	protected:
	static ShaderFile::ptr					m_postProcessFile;
	static EffectPtr						m_postProcessEffect;

	static ResizeTexture::ptr				m_resizeTexture;

	EffectTechniquePtr						m_postProcessTech;

	ConstantBuffer::ptr						m_postProcessCBuffer;
	EffectShaderResourceVariablePtr			m_depthBufResVar;
	EffectShaderResourceVariablePtr			m_lightPassOutputResVar;

	Texture2DPtr							m_displayBufferCopy;
	ShaderResourceViewPtr					m_displayBufferCopyView;
	Texture2DPtr							m_resizedDisplayBuffer;
	RenderTargetViewPtr						m_resizedDisplayBufferView;
	Texture2DPtr							m_resizedDisplayBufferCopy;
};

} // namespace Crowd

} // namespace Crowd

#endif // D3D10_POST_PROCESS_H_INCLUDED