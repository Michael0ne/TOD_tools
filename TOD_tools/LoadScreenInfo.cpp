#include "LoadScreenInfo.h"

LoadScreenInfo* g_LoadScreenInfo = nullptr;

LoadScreenInfo::LoadScreenInfo(const char* unk)
{
	MESSAGE_CLASS_CREATED(LoadScreenInfo);

	m_String_1 = String(unk);

	field_14 = 0;
}