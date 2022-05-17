#include "Scratchpad.h"

Scratchpad* g_Scratchpad = nullptr;

Scratchpad::Scratchpad()
{
    MESSAGE_CLASS_CREATED(Scratchpad);
}

Scratchpad::~Scratchpad()
{
    MESSAGE_CLASS_DESTROYED(Scratchpad);
}

void Scratchpad::Create()
{
    g_Scratchpad = new Scratchpad;
}