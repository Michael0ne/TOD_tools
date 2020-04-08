#include "Scratchpad.h"

Scratchpad* g_Scratchpad = NULL;

void Scratchpad::Init()
{
	patch(0xA3DE4C, this, 4);
}