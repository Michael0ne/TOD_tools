#include "InputMouse.h"
#include "Window.h"

Input::Mouse* g_InputMouse = NULL;

namespace Input {

	void Mouse::Init()
	{
		patch(0xA35EAC, this, 4);

		CreateDevice();
	}

	void Mouse::CreateDevice()
	{
		m_bShouldBeProcessed = true;

		//	TODO: maybe this needs more detailed error code report?
		HRESULT result;
		if ((result = DirectInput8Create_Hooked(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&m_pDeviceObject, NULL)) < 0)
			IncompatibleMachineParameterError(0, 0);
		else
			debug("Input::Mouse DirectInput8Create returned: %i\n", result);

		if ((result = m_pDeviceObject->CreateDevice(GUID_SysMouse, &m_pDInputDevice, NULL)) < 0)
			IncompatibleMachineParameterError(0, 0);
		else
			debug("Input::Mouse CreateDevice returned: %i\n", result);

		if ((result = m_pDInputDevice->SetDataFormat(&c_dfDIMouse)) < 0)
			IncompatibleMachineParameterError(0, 0);
		else
			debug("Input::Mouse SetDataFormat returned: %i\n", result);

		if ((result = m_pDInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)) < 0)
			IncompatibleMachineParameterError(0, 0);
		else
			debug("Input::Mouse SetCooperativeLevel returned: %i\n", result);

		if (!Allocators::Released)
			if (m_pBuffer = (DIDEVICEOBJECTDATA*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(INPUT_MOUSE_BUFFER_SIZE))
				memset(m_pBuffer, 0, INPUT_MOUSE_BUFFER_SIZE);

		m_nBufferSize = 0;

		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = 20;
		dipdw.diph.dwHeaderSize = 16;
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = 0;
		dipdw.dwData = 30;

		if ((result = m_pDInputDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)) < 0)
			IncompatibleMachineParameterError(0, 0);
		else
			debug("Input::Mouse SetProperty DIPROP_BUFFERSIZE returned: %i\n", result);

		m_bAcquired = false;

		//	TODO: this returns 0x80070005 - E_ACCESSDENIED. Why?
		if ((result = m_pDInputDevice->Acquire()) >= 0)
			m_bAcquired = true;
		else
			debug("Input::Mouse Acquire returned: %x\n", result);

		tagPOINT point;

		GetCursorPos(&point);
		ScreenToClient(g_Window->m_hWindow, &point);

		m_OldLocalPosition.x = point.x;
		m_OldLocalPosition.y = point.y;
	}

	void Mouse::Process()
	{
	}

	void Mouse::Reset()
	{
	}
}

inline void PATCH_INPUT_MOUSE()
{
	void* dwFunc;

	_asm	mov	eax, offset Input::Mouse::Init
	_asm	mov dwFunc, eax
	//	Override InputMouse::Init function.
	hook(0x93E02F, dwFunc, PATCH_NOTHING);
}