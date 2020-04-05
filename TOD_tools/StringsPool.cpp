#include "StringsPool.h"

//	TODO: implementation!
//	NOTE: not to be used out of class.
void String::_AllocateSpaceForString()
{
	void(__thiscall * AllocString)(String * _this) = (void(__thiscall*)(String*))0x4056E0;

	AllocString(this);
}

inline void PATCH_STRING_BUFFER()
{
	return;
}