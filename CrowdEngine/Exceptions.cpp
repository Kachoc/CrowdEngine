#include <sstream>

#include "Exceptions.h"

namespace Crowd
{

////
////
//// Exception
////
////

Exception::Exception(const String &msg) throw() : m_msg(msg) {}

Exception::Exception(const Exception &e) throw() : m_msg(e.m_msg) {}

Exception::~Exception() throw() {}

const char *Exception::what() const throw()
{
	return m_msg.asChar();
}

void Exception::addFront(const String &msg)
{
	m_msg = msg + m_msg;
}

void Exception::addBack(const String &msg)
{
	m_msg = m_msg + msg;
}

void Exception::setString(const String &msg)
{
	m_msg = msg;
}

Exception *Exception::copy()
{
	return new Exception(*this);
}

void Exception::setMsg(const String &msg)
{
	m_msg = msg;
}

////
////
//// Classes dérivées
////
////

////
////
//// Classes dérivées
////
////

// LoadFailure
LoadFailure::LoadFailure(const String &msg) throw() : Exception()
{
	std::ostringstream oss;
	oss << "Erreur de chargement: " << msg;

	setString(oss.str());
}

LoadFailure::LoadFailure(const LoadFailure &e) throw() : Exception(e) {}

LoadFailure::~LoadFailure() throw() {}

// InitFailure
InitFailure::InitFailure(const String &error, const String &init) throw() : Exception("")
{
	std::ostringstream oss;
	oss << "Erreur d'initialisation: " << init << ": " << error;

	setString(oss.str());
}

InitFailure::InitFailure(const InitFailure &e) throw() : Exception(e) {}

InitFailure::~InitFailure() throw() {}

// WCompatibility
WCompatibility::WCompatibility(const String &error) throw()
{
	setString("Erreur de compatibilité: " + error);
}

WCompatibility::~WCompatibility() throw() {}

// OutOfMemory
OutOfMemory::OutOfMemory() throw() : Exception("Out of memory") {}
OutOfMemory::~OutOfMemory() throw() {}

// Error
Error::Error(const String &error, const String &type) : Exception("")
{
	std::ostringstream oss;
	oss << "Erreur: " << type << ": " << error;

	setString(oss.str());
}

Error::~Error() {}

// WMemoryAccess
WMemoryAccess::WMemoryAccess(const String &msg)
{
	std::ostringstream oss;
	oss << "Tentative d'accès à une zone mémoire non accessible:\n" << msg;

	setString(oss.str());
}

// RequestConditions
RequestConditions::RequestConditions(const String &what, const String &conditions)
{
	std::ostringstream oss;
	oss << what << ":\n Nécessite: " << conditions;

	setString(oss.str());
}

// WParameters
WParameters::WParameters(const String &function, const String &parameters)
{
	std::ostringstream oss;
	oss << function << ":\n Nécessite: " << parameters;

	setString(oss.str());
}

} // namespace Crowd