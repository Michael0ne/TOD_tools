#include "Progress.h"

Progress* g_Progress = nullptr;

void Progress::Init()
{
	patch(0xA3D7D0, this, 4);

	(*(void(__thiscall*)(Progress*))0x87B720)(this);

	//	TODO: Create empty texture 8x8 and set it as m_pTexture.
}