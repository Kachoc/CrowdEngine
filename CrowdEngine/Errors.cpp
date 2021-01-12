#include <cstdlib>
#include "Logger.h"
#include "Exceptions.h"
#include "Errors.h"

namespace Crowd
{

#if defined(_WIN32)	// _WIN32

void GetErrorString(DWORD dw, String &str)
{
	LPTSTR msg = NULL;

	DWORD cnt = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &msg,
		0,
		NULL);
	cnt++; // '\0' est ignoré par FormatMessage

	if(!msg) throw Exception("Un message d'erreur n'a pu être formaté pour une raison inconnue.");

	#ifndef UNICODE
	str = msg;
	LocalFree(msg);
	#else // UNICODE
	char *mbChar = new char[cnt];
	wcstombs(mbChar, msg, cnt);

	str = mbChar;

	LocalFree(msg);
	delete[] mbChar;
	#endif // UNICODE not defined
}

String GetErrorString(DWORD dw)
{
	String str;
	GetErrorString(dw, str);

	return str;
}

void GetLastErrorString(String &str)
{
	GetErrorString(GetLastError(), str);
}

String GetLastErrorString(void)
{
	return GetErrorString(GetLastError());
}

#else // _WIN32

#endif // différentes plates-formes

} // namespace Crowd