#ifndef CRWD_TEXTURES_H_INCLUDED
#define CRWD_TEXTURES_H_INCLUDED

#include "D3DEntry.h"
#include "Resources.h"

namespace Crowd
{

struct Sample
{
	Sample() : Count(1), Quality(0) {}
	Sample(unsigned int count, unsigned int quality) : Count(count), Quality(quality) {}

	unsigned int Count;
	unsigned int Quality;
};

class Texture : public Resource
{
	public:
	typedef SmartPtr<Texture, IPol> ptr;

	enum ACCESS_TYPE {
		DEFAULT = 0, // le GPU peut lire et écrire
		CPU_READ = 1, // le CPU peut lire et le GPU peut lire et écrire
		CPU_WRITE = 2, // le CPU peut écrire et le GPU peut lire et écrire
		IMMUTABLE = 4 // accès par le GPU en lecture uniquement
	};

	struct Info
	{
		int AccessType;
		unsigned int Width;
		unsigned int Height;
		unsigned int NumMipLevels;
		bool GreyScale;
	};

	virtual void *Map(unsigned int &rowPitch) = 0;
	virtual void Unmap() = 0;

	CRWD_EXPORT virtual void GetTextureInfo(Info &info);

	CRWD_EXPORT static Texture *Load(const String &fileName, int accessType = 0, unsigned int Width = 0, unsigned int Height = 0, unsigned int NumMipLevels = 0, bool GreyScale = false, Status *status = NULL);
	CRWD_EXPORT static Status Save(const String &fileName, Texture *texture, const String &fileFormat);

	protected:
	Info		m_textureInfo;
};

} // namespace Crowd

#endif // CRWD_TEXTURES_H_INCLUDED