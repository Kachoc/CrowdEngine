#include "Status.h"

namespace Crowd
{

Status::Status() : m_status(Success), m_statusString()
{

}

Status::Status(Value status) : m_status(status), m_statusString()
{

}

Status::Status(const String &statusString, Value status) : m_status(status), m_statusString(statusString)
{

}

void Status::setStatusValue(Value status)
{
	m_status = status;
}

void Status::setStatusString(const String &status)
{
	m_statusString = status;
}

void Status::addFront(const String &status)
{
	m_statusString = status + m_statusString;
}

void Status::addBack(const String &status)
{
	m_statusString = m_statusString + status;
}

bool Status::operator == (Value status) const
{
	return (m_status == status) ? true : false;
}

bool Status::operator != (Value status) const
{
	return (m_status != status) ? true : false;
}

bool Status::operator ! () const
{
	return (m_status == Success) ? false : true;
}

const String &Status::asString() const
{
	return m_statusString;
}

const char *Status::asChar() const
{
	return m_statusString.asChar();
}

Status::Value Status::getValue() const
{
	return m_status;
}

Status::operator const String&() const
{
	return m_statusString;
}

Status::operator const char*() const
{
	return m_statusString.asChar();
}


} // namespace Crowd