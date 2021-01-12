#ifndef D3D10_FONT_H_INCLUDED
#define D3D10_FONT_H_INCLUDED

#include "Font.h"
#include "D3DEntry.h"

namespace Crowd
{

namespace D3D10
{

class Font : public Crowd::Font
{
	public:
	Font(unsigned int height, unsigned int width, bool bold, bool italic, const String &fontName);
	virtual ~Font();

	void Write(const String &text, const RECT &position, unsigned int Format, const Color &color);

	protected:
	DFontPtr	m_font;
};

} // namespace D3D10

} // namespace Crowd

#endif // D3D10_FONT_H_INCLUDED