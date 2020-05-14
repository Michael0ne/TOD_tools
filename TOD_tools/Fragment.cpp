#include "Fragment.h"

Fragment::Fragment(const Entity* owner)
{
	m_pUnkStructPtr = nullptr;
	field_8 = 1;
	m_pOwner = (Entity*)owner;
	//	TODO: setTime
	m_szName = nullptr;
}

const char* Fragment::_GetResourcePath()
{
	//	TODO: this is bullshit. What is field_4 type exactly?
	return (*(const char* (__thiscall*)(void*))0x851720)(m_pUnkStructPtr);
}
