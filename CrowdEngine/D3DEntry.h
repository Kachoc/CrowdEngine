//
//	D3DEntry.h
//
// Définit les fonctions, macros et autres nécessaires pour le déboguage de la partie Direct3D de l'application.

#ifndef CRWD_D3D_ENTRY_H_INCLUDED
#define CRWD_D3D_ENTRY_H_INCLUDED

#ifdef CRWD_DIRECT3D

#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include <D3D10.h>
#include <D3DX10math.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <D3DCompiler.h>

#include <DxErr.h>

#endif // CRWD_DIRECT3D

#include "CEString.h"
#include "Export.h"
#include "Exceptions.h"
#include "Status.h"
#include "Logger.h"

namespace Crowd
{

#ifdef CRWD_DIRECT3D

void GetD3DXErrorString(HRESULT hr, String &str);
String GetD3DXErrorString(HRESULT hr);

#endif // CRWD_DIRECT3D

class InterfaceCreationFail : public Exception
{
	public:
	CRWD_EXPORT InterfaceCreationFail(const String &intName = "", const String &error = "") throw();
	// donner le nom de l'initialisation (ex: "Direct 3D 10") et l'erreur (ex: "Il faut créer une fenêtre avant.")
	CRWD_EXPORT InterfaceCreationFail(const InitFailure &) throw();

	CRWD_EXPORT virtual ~InterfaceCreationFail() throw();

	EXCEPTION_COPY(InterfaceCreationFail);

	protected:
	String m_interfaceName;
	String m_error;
};

} // namespace Crowd

#include "D3DClasses.h"

#endif // CRWD_D3D_ENTRY_H_INCLUDED