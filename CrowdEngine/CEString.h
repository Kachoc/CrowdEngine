#ifndef CRWD_CESTRING_H_INCLUDED
#define CRWD_CESTRING_H_INCLUDED

#include <string>

#include "Export.h"

namespace Crowd
{

class String
{
	public:
	friend String operator + (const String &str1, const String &str2);

	CRWD_EXPORT String();
	CRWD_EXPORT String(const String &str);
	CRWD_EXPORT String(const std::string &str);
	CRWD_EXPORT String(const char *str);
	CRWD_EXPORT ~String();

	CRWD_EXPORT unsigned int length() const;

	//String operator + (const String &str) const;
	CRWD_EXPORT String operator + (const std::string &str) const;
	CRWD_EXPORT String operator + (const char *str) const;
	CRWD_EXPORT String operator + (char c) const;

	CRWD_EXPORT String &operator = (const String &str);
	CRWD_EXPORT String &operator = (const std::string &str);
	CRWD_EXPORT String &operator = (const char *str);

	CRWD_EXPORT String &operator += (const String &str);
	CRWD_EXPORT String &operator += (const std::string &str);
	CRWD_EXPORT String &operator += (const char *str);

	CRWD_EXPORT bool operator == (const String &str) const;
	CRWD_EXPORT bool operator == (const std::string &str) const;
	CRWD_EXPORT bool operator == (const char *str) const;

	CRWD_EXPORT bool operator != (const String &str) const;
	CRWD_EXPORT bool operator != (const std::string &str) const;
	CRWD_EXPORT bool operator != (const char *str) const;

	CRWD_EXPORT operator const char *() const;
	CRWD_EXPORT const char *asChar() const;

	CRWD_EXPORT bool operator < (const String &str) const;

	protected:
	void SetString(const char *str);
	void AddString(const char *str);

	protected:
	char			*m_string; // jamais NULL : la chaîne contient au minimum le caractère '\0'
	unsigned int	m_length;
};

String operator + (const String &str1, const String &str2);

} // namespace Crowd

#endif // CRWD_CESTRING_H_INCLUDED