#pragma once

#include "stdafx.h"

#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>			//	For direct input stuff.

class InputKeyboard
{
private:
	BYTE m_nButtonStates[256];
	BYTE m_nButtonStates1[256];
	BYTE m_bAcquired;
	BYTE _pad[3];
	IDirectInputDevice8 *m_pDInputDevice;
	IDirectInput8 *m_pDeviceObject;
	DIDEVICEOBJECTDATA *m_pBuffer;
	int m_nBufferSize;
};

extern InputKeyboard * g_pInputKeyboard;

static_assert(sizeof(InputKeyboard) == 532, MESSAGE_WRONG_CLASS_SIZE("InputKeyboard"));