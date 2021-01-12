#ifndef CROWD_RESIZETEXTURE_H_INCLUDED
#define CROWD_RESIZETEXTURE_H_INCLUDED

#include "D3DEntry.h"
#include "Interface.h"

namespace Crowd
{

class ResizeTexture : public Interface
{
	public:
	typedef SmartPtr<ResizeTexture, IPol> ptr;

	virtual void Process(RenderTargetView *dest, ShaderResourceView *source, unsigned int targetWidth, unsigned int targetHeight, unsigned int sourceWidth, unsigned int sourceHeight) = 0;
};

} // namespace Crowd

#endif // CROWD_RESIZETEXTURE_H_INCLUDED