#ifndef CRWD_STATUS_H_INCLUDED
#define CRWD_STATUS_H_INCLUDED

#include "Export.h"
#include "CEString.h"

namespace Crowd
{

#if defined(_DEBUG) | defined(DEBUG)
	#define TEST_STATUS(STATUS) if(!STATUS) { Logger::Error(STATUS.asString()); }
#else
	#define TEST_STATUS(STATUS) (void) STATUS
#endif

class Status
{
	public:
	enum Value {
		Success = 0,
		Failure = 1,
		WParameters = 2,
	};

	// Constructeurs, destructeur
	// Status() : initialise l'objet de façon à ce qu'il représente une réussite ( équivaut à Status("", Success) )
	CRWD_EXPORT Status();
	CRWD_EXPORT Status(Value status);
	CRWD_EXPORT Status(const String &statusString, Value status = Failure);

	CRWD_EXPORT void setStatusValue(Value status);
	CRWD_EXPORT void setStatusString(const String &status);

	// Ajoute la chaîne de caractères au début de la chaîne existante
	CRWD_EXPORT void addFront(const String &status);
	// Ajoute la chaîne de caractères à la fin de la chaîne existante
	CRWD_EXPORT void addBack(const String &status);

	CRWD_EXPORT bool operator == (Value status) const;
	CRWD_EXPORT bool operator != (Value status) const;

	CRWD_EXPORT bool operator ! () const;

	CRWD_EXPORT const String &asString() const;
	CRWD_EXPORT const char *asChar() const;

	CRWD_EXPORT Value getValue() const;

	CRWD_EXPORT operator const String&() const;
	CRWD_EXPORT operator const char*() const;

	protected:
	Value		m_status;
	String		m_statusString;
};

} // namespace Crowd

#endif // CRWD_STATUS_H_INCLUDED