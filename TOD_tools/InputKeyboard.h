#pragma once

#include "stdafx.h"

namespace Input {

#define INPUT_KEYBOARD_CLASS_SIZE 532

	class Keyboard
	{
	private:
		BYTE m_nButtonStates[256];
		BYTE m_nButtonStates1[256];
		BYTE m_bAcquired;
		BYTE _pad[3];
		IDirectInputDevice8* m_pDInputDevice;
		IDirectInput8* m_pDeviceObject;
		DIDEVICEOBJECTDATA* m_pBuffer;
		int m_nBufferSize;

	public:
		Keyboard()
		{
			debug("Input::Keyboard created at %X\n", this);
		}

		~Keyboard()
		{
			debug("Input::Keyboard destroyed!\n");
		}

		void Init();	//	@43AD80
		void Process();	//	@43AF60
		void Reset();	//	@43A740
	};
}

extern Input::Keyboard * g_InputKeyboard;

static_assert(sizeof(Input::Keyboard) == INPUT_KEYBOARD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("InputKeyboard"));