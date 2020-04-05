#include "Blocks.h"

Blocks* g_Blocks = NULL;

void Blocks::Init(bool unk)
{
	patch(0xA3D7C4, this, 4);

	void(__thiscall * _Init)(Blocks * _this, bool _unk) = (void(__thiscall*)(Blocks*, bool))0x876E20;

	_Init(this, unk);
}