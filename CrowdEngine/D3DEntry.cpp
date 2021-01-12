#include "D3DEntry.h"

#include <sstream>

namespace Crowd
{

void GetD3DXErrorString(HRESULT hr, String &str)
{
	LPCTSTR msg = DXGetErrorString(hr);
	if(!msg) {
		str = "GetD3DXErrorString : un message sur l'erreur de D3D10 n'a pu être généré pour une raison inconnue";
		return;
	}
	
	#ifndef UNICODE

	str = msg;

	#else // UNICODE



	#endif // UNICODE
}

String GetD3DXErrorString(HRESULT hr)
{
	String str;
	GetD3DXErrorString(hr, str);

	return str;
}

// InitFailure
InterfaceCreationFail::InterfaceCreationFail(const String &intName, const String &error) throw() : Exception("")
{
	m_interfaceName = intName;
	m_error = error;

	std::ostringstream oss;
	oss << "Erreur lors de la création de: " << intName << ": " << error;

	setString(oss.str());
}

InterfaceCreationFail::InterfaceCreationFail(const InitFailure &e) throw() : Exception(e) {}

InterfaceCreationFail::~InterfaceCreationFail() throw() {}

} // namespace Crowd