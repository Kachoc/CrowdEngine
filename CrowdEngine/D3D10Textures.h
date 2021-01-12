#ifndef CROWD_D3D10_TEXTURES_H_INCLUDED
#define CROWD_D3D10_TEXTURES_H_INCLUDED

#include "D3DEntry.h"

#include "Textures.h"

namespace Crowd
{

namespace D3D10
{

class Texture : public Crowd::Texture
{
	public:
	typedef SmartPtr<Texture, IPol> ptr;

	Texture(Texture2DPtr texture, ShaderResourceViewPtr textureResView, const Info &info);
	virtual ~Texture();

	virtual Texture2D *GetTexture() const;
	virtual ShaderResourceView *GetShaderResourceView() const;
	virtual void *Map(unsigned int &rowPitch);
	virtual void Unmap();

	static Texture *Load(const String &fileName, int accessType, unsigned int Width, unsigned int Height, unsigned int NumMipLevels, bool GreyScale, Status *status = NULL);
	static Status Save(const String &fileName, Crowd::Texture *texture, const String &fileFormat);

	protected:
	Texture2DPtr				m_texture;
	ShaderResourceViewPtr		m_textureResView;
};

} // namespace D3D10

} // namespace Crowd

#endif // CROWD_D3D10_TEXTURES_H_INCLUDED