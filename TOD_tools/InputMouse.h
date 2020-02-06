#pragma once

#include "stdafx.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>			//	For direct input stuff.

enum eMouseButtons {
	MOUSE_BUTTON_0,
	MOUSE_BUTTON_1,
	MOUSE_BUTTON_2,
	MOUSE_BUTTON_3,
	MOUSE_BUTTON_4,
	MOUSE_BUTTON_5,
	MOUSE_BUTTON_6,
	MOUSE_BUTTON_7,
	MOUSE_BUTTON_WHEEL_UP,
	MOUSE_BUTTON_WHEEL_DOWN
};

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *---------- Wrapper around DirectInput Mouse ----------------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class InputMouse
{
private:
	unsigned int			m_nMouseButtons[10];
	int						field_44;
	tagPOINT				m_NewLocalPosition;
	tagPOINT				m_OldLocalPosition;
	DIMOUSESTATE			*m_MouseState;
	int						field_a;
	int						field_b;
	BYTE					m_bShouldBeProcessed;
	BYTE					field_c[3];
	IDirectInput8			*m_pDeviceObject;
	IDirectInputDevice8		*m_pDInputDevice;
	BYTE					m_bAcquired;
	BYTE					field_d[3];
	DIDEVICEOBJECTDATA		*m_pBuffer;
	int						m_nBufferSize;
public:

};

extern InputMouse * g_pInputMouse;

static_assert(sizeof(InputMouse) == 96, MESSAGE_WRONG_CLASS_SIZE("InputMouse"));