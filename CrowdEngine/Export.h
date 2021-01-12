#ifndef CRWD_EXPORT_H_INCLUDED
#define CRWD_EXPORT_H_INCLUDED

#if defined(_WIN32) // _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#else // _WIN32
	#error Plate-forme non supportée
#endif // différentes plates-formes

#if defined(CRWD_LIBRARY)
	#define CRWD_EXPORT	__declspec(dllexport)
#else
	#define CRWD_EXPORT __declspec(dllimport)
	//#define CRWD_EXPORT
#endif

#define CRWD_EXPORT_C_FUNCTION extern "C" __declspec(dllexport)

namespace Crowd
{

} // namespace Crowd

#endif // CRWD_EXPORT_H_INCLUDED