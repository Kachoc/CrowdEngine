#include "Textures.h"
#include "D3D10Textures.h"

namespace Crowd
{

void Texture::GetTextureInfo(Info &info)
{
	info = m_textureInfo;
}

Texture *Texture::Load(const String &fileName, int accessType, unsigned int Width, unsigned int Height, unsigned int NumMipLevels, bool GreyScale, Status *status)
{
	return D3D10::Texture::Load(fileName, accessType, Width, Height, NumMipLevels, GreyScale, status);
}

Status Texture::Save(const String &fileName, Texture *texture, const String &fileFormat)
{
	return D3D10::Texture::Save(fileName, texture, fileFormat);
}

} // namespace Crowd