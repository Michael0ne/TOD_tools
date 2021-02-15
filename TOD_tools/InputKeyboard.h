#pragma once

#include "MemoryAllocators.h"

namespace Input {

#define INPUT_KEYBOARD_CLASS_SIZE 532
#define INPUT_KEYBOARD_BUFFERS_COUNT 30

	class Keyboard
	{
		friend class ScriptType_Builtin;
	protected:
		unsigned char			m_nButtonStates[256];
		unsigned char			m_nButtonStates1[256];
		bool					m_bAcquired;
		unsigned char			_pad[3];
		IDirectInputDevice8*	m_pDInputDevice;
		IDirectInput8*			m_pDeviceObject;
		DIDEVICEOBJECTDATA**	m_pBuffer;	//	NOTE: judging by the code in exe, this is initialized with exact size, but because actual structure is much less, this more looks like an array of structures.
		unsigned int			m_nBufferSize;

	public:
		Keyboard();		//	@43AD80
		~Keyboard();	//	@43AF20

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		void					Process();	//	@43AF60
		void					Reset();	//	@43A740
		void					ResetKeyboardButtons();	//	@43A750
		void					UnacquireAndResetKeyboard();	//	@43A790
		const char*				KeyboardButtonToStr(int keyCode);	//	@43A7D0
	};
}

extern Input::Keyboard*			g_InputKeyboard;
extern const char*				g_KeyboardButtons[];

static_assert(sizeof(Input::Keyboard) == INPUT_KEYBOARD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Input::Keyboard));