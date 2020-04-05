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

		//	FIXME: this now crashes the game!
		if (!DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&m_pDeviceObject, NULL))
			IncompatibleMachineParameterError(0, 0);

		if (!m_pDeviceObject->CreateDevice(GUID_SysMouse, &m_pDInputDevice, NULL))
			IncompatibleMachineParameterError(0, 0);

		if (!m_pDInputDevice->SetDataFormat(&c_dfDIMouse))
			IncompatibleMachineParameterError(0, 0);

		if (!m_pDInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))
			IncompatibleMachineParameterError(0, 0);

		//	TODO: finish!
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