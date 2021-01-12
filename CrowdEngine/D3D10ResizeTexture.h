#ifndef CROWD_D3D10_RESIZETEXTURE_H_INCLUDED
#define CROWD_D3D10_RESIZETEXTURE_H_INCLUDED

#include "D3DEntry.h"

#include "D3D10ShaderFile.h"

#include "ResizeTexture.h"

namespace Crowd
{

namespace D3D10
{

class ResizeTexture : public Crowd::ResizeTexture
{
	public:
	typedef SmartPtr<ResizeTexture, IPol> ptr;

	struct Parameters
	{
		float SourceScaleX;
		float SourceScaleY;
		int DestWidth;
		int DestHeight;
		int NumSamplesX;
		int NumSamplesY;
	};

	ResizeTexture();
	virtual ~ResizeTexture();

	virtual void Process(RenderTargetView *dest, ShaderResourceView *source, unsigned int destWidth, unsigned int destHeight, unsigned int sourceWidth, unsigned int sourceHeight);

	protected:
	ShaderFile::ptr	m_resizeTextureFile;

	EffectTechnique						*m_resizeTextureTech;
	EffectShaderResourceVariable		*m_sourceTexture;

	ConstantBuffer::ptr					m_resTexCBuffer;
};

} // namespace D3D10

} // namespace Crowd

#endif // CROWD_D3D10_RESIZETEXTURE_H_INCLUDED