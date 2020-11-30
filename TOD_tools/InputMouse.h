#pragma once

#include "Globals.h"

namespace Input {

#define INPUT_MOUSE_CLASS_SIZE 96
#define INPUT_MOUSE_BUFFERS_COUNT 30
#define INPUT_MOUSE_MAX_BUTTONS 10

	enum eMouseButtons {
		MOUSE_BUTTON_0 = 0,
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
	class Mouse
	{
	private:
		unsigned int			m_nMouseButtons[10];
		int						m_Position_Z;
		tagPOINT				m_NewLocalPosition;
		tagPOINT				m_OldLocalPosition;
		int						m_Position_X;
		int						m_Position_Y;
		int						field_44;
		BYTE					m_bShouldBeProcessed;
		IDirectInput8*			m_pDeviceObject;
		IDirectInputDevice8*	m_pDInputDevice;
		BYTE					m_bAcquired;
		DIDEVICEOBJECTDATA**	m_pBuffer;
		unsigned int			m_nBufferSize;

	public:
		Mouse();	//	@43B4E0
		~Mouse();

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		void					_Acquire();	//	@43B370
		void					UnacquireAndReset();	//	@43B390
		void					SetWindowCapture(HWND window);	//	@43B3C0
		void					ReleaseWindowCapture();	//	@43B3F0
		void					ResetButtonsState();	//	@43B410
		const char*				MouseButtonToStr(unsigned int button);	//	@43B440
		const char*				MousePositionTostr(unsigned int axis);	//	@43B4C0
		void					Process();	//	@43B670
	};
}

extern Input::Mouse*			g_InputMouse;
extern const char*				g_MouseButtons[];

static_assert(sizeof(Input::Mouse) == INPUT_MOUSE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Input::Mouse));