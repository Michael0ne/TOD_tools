#include "Fragment.h"

const char* Fragment::_GetResourcePath()
{
	//	TODO: this is bullshit. What is field_4 type exactly?
	return (*(const char* (__thiscall*)(void*))0x851720)(field_4);
}
