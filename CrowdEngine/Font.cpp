#include "Font.h"
#include "D3D10Font.h"

namespace Crowd
{

Font::Font() {}
Font::~Font() {}

Font *Font::Create(unsigned int height, unsigned int width, bool bold, bool italic, const String &fontName)
{
	return new D3D10::Font(height, width, bold, italic, fontName);
}

} // namespace Crowd