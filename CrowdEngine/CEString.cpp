#include "CEString.h"

namespace Crowd
{

String::String()
{
	m_string = new char[1];
	m_string[0] = '\0';
	m_length = 1;
}

String::String(const String &str)
{
	m_length = str.m_length;
	m_string = new char[m_length];
	for(unsigned int i = 0; i < m_length; i++)
	{
		m_string[i] = str.m_string[i];
	}
}

String::String(const std::string &str)
{
	m_length = str.size() + 1;
	m_string = new char[m_length];
	for(unsigned int i = 0; i < m_length - 1; i++)
	{
		m_string[i] = str[i];
	}
	m_string[m_length - 1] = '\0';
}

String::String(const char *str) : m_string(NULL), m_length(0)
{
	SetString(str);
}

String::~String()
{
	if(m_string) delete[] m_string; // ne devrait jamais être NULL
}

unsigned int String::length() const
{
	return m_length-1; /// m_length prend en compte le caractère de terminaison
}

/*String String::operator + (const String &str) const
{
	String retString;
	delete[] retString.m_string;
	
	retString.m_length = m_length + str.m_length -1;
	retString.m_string = new char[retString.m_length];

	unsigned int i = 0;
	for(; i < m_length - 1; i++) {
		retString.m_string[i] = m_string[i];
	}
	for(unsigned int j = 0; j < str.m_length; j++) {
		retString.m_string[i + j] = str.m_string[j];
	}

	return retString;
}*/

String String::operator + (const std::string &str) const
{
	String retString;
	delete[] retString.m_string;
	
	retString.m_length = m_length + str.size();
	retString.m_string = new char[retString.m_length];

	unsigned int i = 0;
	for(; i < m_length - 1; i++) {
		retString.m_string[i] = m_string[i];
	}
	for(unsigned int j = 0; j < str.size(); j++) {
		retString.m_string[i + j] = str[j];
	}
	retString.m_string[m_length - 1] = '\0';

	return retString;
}

String String::operator + (const char *str) const
{
	String retString;
	delete[] retString.m_string;

	unsigned int strLength = 0;
	while(str[strLength] != '\0') {
		strLength++;
	}
	strLength++;

	retString.m_length = m_length + strLength -1;
	retString.m_string = new char[retString.m_length];

	unsigned int i = 0;
	for(; i < m_length - 1; i++) {
		retString.m_string[i] = m_string[i];
	}
	for(unsigned int j = 0; j < strLength; j++) {
		retString.m_string[i + j] = str[j];
	}

	return retString;
}

String String::operator + (char c) const
{
	String retString;
	delete[] retString.m_string;

	retString.m_length = m_length+1;
	retString.m_string = new char[retString.m_length];
	for(unsigned int i = 0; i < m_length-1; i++)
	{
		retString.m_string[i] = m_string[i];
	} 
	retString.m_string[m_length-1] = c;
	retString.m_string[m_length] = '\0';

	return retString;
}

String &String::operator = (const String &str)
{
	if(m_string) delete m_string;
	m_length = str.m_length;

	m_string = new char[m_length];

	for(unsigned int i = 0; i < m_length; i++) {
		m_string[i] = str.m_string[i];
	}

	return *this;
}

String &String::operator = (const std::string &str)
{
	if(m_string) delete m_string;
	m_length = str.size();

	m_string = new char[m_length];

	for(unsigned int i = 0; i < m_length; i++) {
		m_string[i] = str[i];
	}

	return *this;
}

String &String::operator = (const char *str)
{
	SetString(str);
	return *this;
}

String &String::operator += (const String &str)
{
	unsigned int nLength = m_length + str.m_length - 1;
	char *nString = new char[nLength];

	unsigned int i = 0;
	for(; i < m_length - 1; i++) {
		nString[i] = m_string[i];
	}
	for(unsigned int j = 0; j < str.m_length; j++) {
		nString[i + j] = str.m_string[j];
	}

	delete[] m_string;
	m_string = nString;
	m_length = nLength;

	return *this;
}

String &String::operator += (const std::string &str)
{
	unsigned int nLength = m_length + str.size();
	char *nString = new char[nLength];

	unsigned int i = 0;
	for(; i < m_length - 1; i++) {
		nString[i] = m_string[i];
	}
	for(unsigned int j = 0; j < str.size(); j++) {
		nString[i + j] = str[j];
	}
	nString[nLength - 1] = '\0';

	delete[] m_string;
	m_string = nString;
	m_length = nLength;

	return *this;
}

String &String::operator += (const char *str)
{
	AddString(str);
	return *this;
}

bool String::operator == (const String &str) const
{
	if(m_length != str.m_length) return false;

	for(unsigned int i = 0; i < m_length; i++)
	{
		if(m_string[i] != str.m_string[i]) return false;
	}
	return true;
}

bool String::operator == (const std::string &str) const
{
	if(m_length != str.size() + 1) return false;

	for(unsigned int i = 0; i < m_length - 1; i++)
	{
		if(m_string[i] != str[i]) return false;
	}
	return true;
}

bool String::operator == (const char *str) const
{
	unsigned int i = 0;
	while(i < m_length)
	{
		if(m_string[i] != str[i]) return false;
		i++;
	}
	return true;
}

bool String::operator != (const String &str) const
{
	if(m_length != str.length()) return true;

	for(unsigned int i = 0; i < m_length; i++)
	{
		if(m_string[i] != str.m_string[i]) return true;
	}
	return false;
}

bool String::operator != (const std::string &str) const
{
	if(m_length != str.size() + 1) return true;

	for(unsigned int i = 0; i < m_length - 1; i++)
	{
		if(m_string[i] != str[i]) return true;
	}
	return false;
}

bool String::operator != (const char *str) const
{
	unsigned int i = 0;
	while(i < m_length)
	{
		if(m_string[i] != str[i]) return true;
		i++;
	}
	return false;
}

String::operator const char *() const
{
	return m_string;
}

const char *String::asChar() const
{
	return m_string;
}

void String::SetString(const char *str)
{
	if(m_string)
	{
		delete[] m_string;
	}

	m_length = 0;
	while(str[m_length] != '\0') {
		m_length++;
	}
	m_length++;

	m_string = new char[m_length];

	for(unsigned int i = 0; i < m_length; i++)
	{
		m_string[i] = str[i];
	}
}

void String::AddString(const char *str)
{
	unsigned int strLength = 0;
	while(str[strLength++] != '\0') {
		strLength++;
	}
	strLength++;


	unsigned int nLength = m_length + strLength - 1;
	char *nString = new char[nLength];


	unsigned int i = 0;
	for(; i < m_length - 1; i++) {
		nString[i] = m_string[i];
	}
	for(unsigned int j = 0; j < strLength; j++) {
		nString[i + j] = str[j];
	}

	delete[] m_string;
	m_string = nString;
	m_length = nLength;
}

bool String::operator < (const String &str) const
{
	unsigned int m = min(m_length, str.m_length);

	for(unsigned int i = 0; i < m; i++)
	{
		if(m_string[i] != str.m_string[i])
		{
			return m_string[i] < str.m_string[i];
		}
	}
	return m < m_length;
}

String operator + (const String &str1, const String &str2)
{
	String retString;
	delete[] retString.m_string;
	
	retString.m_length = str1.m_length + str2.m_length -1;
	retString.m_string = new char[retString.m_length];

	unsigned int i = 0;
	for(; i < str1.m_length - 1; i++) {
		retString.m_string[i] = str1.m_string[i];
	}
	for(unsigned int j = 0; j < str2.m_length; j++) {
		retString.m_string[i + j] = str2.m_string[j];
	}

	return retString;
}

} // namespace Crowd