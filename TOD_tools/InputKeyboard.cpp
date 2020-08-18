#include "InputKeyboard.h"
#include "Window.h"

Input::Keyboard* g_InputKeyboard = NULL;

namespace Input
{
	void Keyboard::Init()
	{
		patch(0xA35E80, this, 4);

		CreateDevice();
	}

	void Keyboard::CreateDevice()
	{
		HRESULT result;
		if ((result = DirectInput8Create_Hooked(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&m_pDeviceObject, NULL)) < 0)
			IncompatibleMachineParameterError(1, 0);
		else
			debug("Input::Keyboard DirectInput8Create returned: %i\n", result);

		if ((result = m_pDeviceObject->CreateDevice(GUID_SysKeyboard, &m_pDInputDevice, NULL)) < 0)
			IncompatibleMachineParameterError(1, 0);
		else
			debug("Input::Keyboard CreateDevice returned: %i\n", result);

		if ((result = m_pDInputDevice->SetDataFormat(&c_dfDIKeyboard)) < 0)
			IncompatibleMachineParameterError(1, 0);
		else
			debug("Input::Keyboard SetDataFormat returned: %i\n", result);

		if ((result = m_pDInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_FOREGROUND | DISCL_EXCLUSIVE)) < 0)
			IncompatibleMachineParameterError(1, 0);
		else
			debug("Input::Keyboard SetCooperativeLevel returned: %i\n", result);

		if (m_pBuffer = (DIDEVICEOBJECTDATA*)Allocators::AllocatorsList[DEFAULT]->Allocate(INPUT_KEYBOARD_BUFFER_SIZE, NULL, NULL))
			memset(m_pBuffer, 0, INPUT_KEYBOARD_BUFFER_SIZE);

		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = 20;
		dipdw.diph.dwHeaderSize = 16;
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = 0;
		dipdw.dwData = 30;

		if ((result = m_pDInputDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)) < 0)
			IncompatibleMachineParameterError(1, 0);
		else
			debug("Input::Keyboard SetProperty DIPROP_BUFFERSIZE returned: %i\n", result);

		if (m_bAcquired)
			return;

		if ((result = m_pDInputDevice->Acquire()) >= 0)
			m_bAcquired = true;
		else
			debug("Input::Keyboard Acquire returned: %i\n", result);

		memset(&m_nButtonStates1, 0, sizeof(m_nButtonStates1));
	}

	void Keyboard::Process()
	{

	}

	void Keyboard::Reset()
	{

	}
}

void inline PATCH_INPUT_KEYBOARD()
{}