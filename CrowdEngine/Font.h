#ifndef CRWD_FONT_H_INCLUDED
#define CRWD_FONT_H_INCLUDED

#include "Resources.h"
#include "CRWDMath.h"
#include "Export.h"

namespace Crowd
{

class Font : public Resource
{
	public:
	typedef SmartPtr<Font, IPol> ptr;

	CRWD_EXPORT Font();
	CRWD_EXPORT virtual ~Font();

	virtual void Write(const String &text, const RECT &position, unsigned int Format, const Color &color) = 0;

	CRWD_EXPORT static Font *Create(unsigned int height, unsigned int width, bool bold, bool italic, const String &fontName);
};

} // namespace Crowd

#endif // CRWD_FONT_H_INCLUDED