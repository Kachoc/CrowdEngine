#ifndef CE_ERRORS_H_INCLUDED
#define CE_ERRORS_H_INCLUDED

#if defined(_WIN32)	// _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include "CEString.h"

#else // _WIN32
	#error Plate-forme non supportée
#endif // différentes plates-formes

namespace Crowd
{

void CRWD_EXPORT GetErrorString(DWORD dw, String &str);
String CRWD_EXPORT GetErrorString(DWORD dw);

void CRWD_EXPORT GetLastErrorString(String &str);
String CRWD_EXPORT GetLastErrorString(void);

} // namespace Crowd

#endif // CE_ERRORS_H_INCLUDED