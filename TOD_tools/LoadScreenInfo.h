#pragma once

#include "StringsPool.h"

class LoadScreenInfo
{
protected:
	int field_0;
	String m_String_1;
	char field_14;
	int field_18;

public:
	LoadScreenInfo(const char* unk);	//	@87C230
};

extern LoadScreenInfo* g_LoadScreenInfo;