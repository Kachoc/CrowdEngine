#include <cstdio>
#include <cstdarg>

#include <Windows.h>

#include "Logger.h"

namespace Crowd
{

////
////
//// Logger
////
////

std::ofstream out("Logout.log", std::ios::trunc);

SmartPtr<Logger, RefCnt> Logger::m_logger(new DefaultLogger);
SmartPtr<Logger, RefCnt> Logger::m_errorLogger(new DefaultErrorLogger);
char Logger::m_logTemp[CE_LOG_MAX_SIZE];

void Logger::SetLogger(Logger *l)
{
	m_logger = l;
}

void Logger::SetErrorLogger(Logger *l)
{
	m_errorLogger = l;
}

void Logger::Log(const char *s, ...)
{
	if(s && m_logger)
	{
		va_list Format;
		va_start(Format, s);
		vsprintf_s(m_logTemp, CE_LOG_MAX_SIZE, s, Format);
		va_end(Format);

		if(m_logger) m_logger->Write(m_logTemp);
	}
}

void Logger::Log(const String &s, ...)
{
	if(s.length() && m_logger)
	{
		va_list Format;
		va_start(Format, s);
		vsprintf_s(m_logTemp, CE_LOG_MAX_SIZE, s.asChar(), Format);
		va_end(Format);

		if(m_logger) m_logger->Write(m_logTemp);
	}
}

void Logger::Error(const char *s, ...)
{
	if(s && m_logger)
	{
		va_list Format;
		va_start(Format, s);
		vsprintf_s(m_logTemp, CE_LOG_MAX_SIZE, s, Format);
		va_end(Format);

		if(m_errorLogger) m_errorLogger->Write(m_logTemp);
	}
}

void Logger::Error(const String &s, ...)
{
	if(s.length() && m_logger)
	{
		va_list(Format);
		va_start(Format, s);
		vsprintf_s(m_logTemp, CE_LOG_MAX_SIZE, s.asChar(), Format);
		va_end(Format);

		if(m_errorLogger) m_errorLogger->Write(m_logTemp);
	}
}


//
// DefaultLogger
//

DefaultLogger::DefaultLogger() {}
DefaultLogger::~DefaultLogger() {}

void DefaultLogger::Write(const char *s)
{
	out << s << std::endl;
}


//
// DefaultErrorLogger
//

DefaultErrorLogger::DefaultErrorLogger() {}
DefaultErrorLogger::~DefaultErrorLogger() {}

void DefaultErrorLogger::Write(const char *s)
{
	out << "error: " << s << std::endl;

	#ifdef UNICODE
	SmartPtr<WCHAR> ws = NULL;
	UINT size = MultiByteToWideChar(0, 0, s, -1, NULL, 0);

	ws = new WCHAR[size];

	MultiByteToWideChar(0, 0, s, size, ws, size);
	MessageBox(NULL, ws, TEXT("Erreur"), MB_ICONERROR);

	#else
	MessageBox(NULL, s, TEXT("Erreur"), MB_ICONERROR);
	#endif // UNICODE
}

} // namespace Crowd