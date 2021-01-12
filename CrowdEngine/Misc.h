#ifndef CE_MISC_H_INCLUDED
#define CE_MISC_H_INCLUDED

#define UNUSED_PARAMETER(var)		(void) var
#define UNUSED_VAR(var)			(void) var

namespace Crowd
{

struct AUTOCALL
{
	AUTOCALL(void (*function)(void)) : call(function) {}
	~AUTOCALL() { if(call) { call(); } }

	void (*call)(void);
};

} // namespace Crowd

#endif // CE_MISC_H_INCLUDED