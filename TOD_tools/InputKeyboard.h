#pragma once

#include "stdafx.h"

#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>			//	For direct input stuff.

class InputKeyboard
{
private:
	BYTE	_pad[532];
};

static_assert(sizeof(InputKeyboard) == 532, MESSAGE_WRONG_CLASS_SIZE("InputKeyboard"));