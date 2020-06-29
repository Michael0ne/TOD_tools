#include "Fragment.h"

Fragment::Fragment(const Entity* owner)
{
	MESSAGE_CLASS_CREATED(Fragment);

	m_pUnkStructPtr = nullptr;
	field_8 = 1;
	m_pOwner = (Entity*)owner;
	//	NOTE: 40FEA0 inlined.
	m_nUniqueId0 = (*(time_t(*)(time_t*))0x9513DD)(NULL);	//	NOTE: if NULL is passed in, it returns current timestamp, otherwise value is returned in passed param.
	m_nUniqueId1 = __rdtsc();	//	NOTE: this uses high dword returned by func.
	m_szName = nullptr;
}

const char* Fragment::_GetResourcePath()
{
	//	TODO: this is bullshit. What is field_4 type exactly?
	return (*(const char* (__thiscall*)(void*))0x851720)(m_pUnkStructPtr);
}
