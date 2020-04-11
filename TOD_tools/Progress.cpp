#include "Progress.h"

Progress* g_Progress = NULL;

void Progress::Init()
{
	patch(0xA3D7D0, this, 4);

	(*(void(__thiscall*)(Progress*))0x87B720)(this);
}