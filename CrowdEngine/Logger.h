#ifndef CE_LOGGER_H_INCLUDED
#define CE_LOGGER_H_INCLUDED

#include <fstream>

#include "Misc.h"
#include "SmartPtr.h"
#include "CEString.h"
#include "Export.h"

#define CE_LOG_MAX_SIZE 1024

namespace Crowd
{

class Logger
{
	public:
	CRWD_EXPORT static void Log(const char *s, ...);
	CRWD_EXPORT static void Log(const String &s, ...);
	CRWD_EXPORT static void Error(const char *s, ...);
	CRWD_EXPORT static void Error(const String &s, ...);

	CRWD_EXPORT static void SetLogger(Logger *l);
	CRWD_EXPORT static void SetErrorLogger(Logger *l);

	CRWD_EXPORT virtual ~Logger() {}

	CRWD_EXPORT virtual void Write(const char *msg) = 0;

	protected:
	static SmartPtr<Logger, RefCnt> m_logger;
	static SmartPtr<Logger, RefCnt> m_errorLogger;
	
	static char m_logTemp[CE_LOG_MAX_SIZE];
};

class DefaultLogger : public Logger
{
	public:
	CRWD_EXPORT DefaultLogger();
	CRWD_EXPORT virtual ~DefaultLogger();

	CRWD_EXPORT virtual void Write(const char *msg);
};

class DefaultErrorLogger : public Logger
{
	public:
	CRWD_EXPORT DefaultErrorLogger();
	CRWD_EXPORT virtual ~DefaultErrorLogger();

	CRWD_EXPORT virtual void Write(const char *msg);
};

} // namespace Crowd

#endif // CE_LOGGER_H_INCLUDED