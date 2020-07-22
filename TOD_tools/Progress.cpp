#include "Progress.h"

Progress* g_Progress = nullptr;

Progress::Progress()
{
	MESSAGE_CLASS_CREATED(Progress);

	(*(Progress * (__thiscall*)(Progress*))0x87B720)(this);
}