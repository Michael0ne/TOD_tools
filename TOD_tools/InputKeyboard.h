#pragma once

#include "stdafx.h"

#include "MemoryAllocators.h"

namespace Input {

#define INPUT_KEYBOARD_CLASS_SIZE 532
#define INPUT_KEYBOARD_BUFFER_SIZE 600

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

		void	CreateDevice();

	public:
		Keyboard()
		{
			MESSAGE_CLASS_CREATED(Keyboard);

			memset(&m_nButtonStates, 0, sizeof(m_nButtonStates));
			memset(&m_nButtonStates1, 0, sizeof(m_nButtonStates1));

			m_bAcquired = 0;
			_pad[0] = 0;
			_pad[1] = 0;
			_pad[2] = 0;

			m_pDInputDevice = nullptr;
			m_pDeviceObject = nullptr;
			m_pBuffer = nullptr;

			m_nBufferSize = 0;
		}
		~Keyboard()
		{
			MESSAGE_CLASS_DESTROYED(Keyboard);
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		void Init();	//	@43AD80
		void Process();	//	@43AF60
		void Reset();	//	@43A740
	};
}

extern Input::Keyboard * g_InputKeyboard;

static_assert(sizeof(Input::Keyboard) == INPUT_KEYBOARD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Input::Keyboard));