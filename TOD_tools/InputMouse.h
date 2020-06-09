#pragma once

#include "stdafx.h"
#include "Globals.h"

#include "MemoryAllocators.h"

namespace Input {

#define INPUT_MOUSE_CLASS_SIZE 96
#define INPUT_MOUSE_BUFFER_SIZE 600

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
	class Mouse
	{
	private:
		unsigned int			m_nMouseButtons[10];
		int						field_44;
		tagPOINT				m_NewLocalPosition;
		tagPOINT				m_OldLocalPosition;
		DIMOUSESTATE* m_MouseState;
		int						field_a;
		int						field_b;
		BYTE					m_bShouldBeProcessed;
		BYTE					field_c[3];
		IDirectInput8* m_pDeviceObject;
		IDirectInputDevice8* m_pDInputDevice;
		BYTE					m_bAcquired;
		BYTE					field_d[3];
		DIDEVICEOBJECTDATA* m_pBuffer;
		int						m_nBufferSize;

		void CreateDevice();

	public:
		Mouse()
		{
			debug("Input::Mouse created at %X\n", this);

			for (int ind = 0; ind < 10; ind++)
				m_nMouseButtons[ind] = 0;

			field_44 = 0;
			m_NewLocalPosition = {};
			m_OldLocalPosition = {};
			m_MouseState = nullptr;
			field_a = 0;
			field_b = 0;
			m_bShouldBeProcessed = false;
			field_c[0] = 0;
			field_c[1] = 0;
			field_c[2] = 0;

			m_pDeviceObject = nullptr;
			m_pDInputDevice = nullptr;

			m_bAcquired = false;
			field_d[0] = 0;
			field_d[1] = 0;
			field_d[2] = 0;
			m_pBuffer = nullptr;
			m_nBufferSize = 0;
		}

		~Mouse()
		{
			debug("Input::Mouse destroyed!\n");
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}

		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}

		void Init();	//	@43B4E0
		void Process();	//	@43B670
		void Reset();	//	@43B410
	};
}

extern Input::Mouse* g_InputMouse;

static_assert(sizeof(Input::Mouse) == INPUT_MOUSE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Input::Mouse));