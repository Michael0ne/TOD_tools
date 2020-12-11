#include "InputMouse.h"
#include "Window.h"
#include "GfxInternal_Dx9.h"

Input::Mouse* g_InputMouse = NULL;
const char* g_MouseButtons[] = {
	"MOUSE_BUTTON_0",
	"MOUSE_BUTTON_1",
	"MOUSE_BUTTON_2",
	"MOUSE_BUTTON_3",
	"MOUSE_BUTTON_4",
	"MOUSE_BUTTON_5",
	"MOUSE_BUTTON_6",
	"MOUSE_BUTTON_7",
	"MOUSE_BUTTON_WHEEL_UP",
	"MOUSE_BUTTON_WHEEL_DOWN"
};

namespace Input {

	Mouse::Mouse()
	{
		MESSAGE_CLASS_CREATED(Mouse);

		m_nMouseButtons[MOUSE_BUTTON_0] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_1] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_2] = NULL;
		m_NewLocalPosition.x = m_NewLocalPosition.y = NULL;
		m_Position_X = m_Position_Y = m_Position_Z = NULL;
		m_OldLocalPosition.x = m_OldLocalPosition.y = NULL;
		field_44 = NULL;
		m_bShouldBeProcessed = true;
		m_pDeviceObject = nullptr;
		m_pDInputDevice = nullptr;

		g_InputMouse = this;

		if (FAILED(DirectInput8Create_Hooked(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDeviceObject, NULL)))
			IncompatibleMachineParameterError(0, false);

		if (FAILED(m_pDeviceObject->CreateDevice(GUID_SysMouse, &m_pDInputDevice, NULL)))
			IncompatibleMachineParameterError(0, false);

		if (FAILED(m_pDInputDevice->SetDataFormat(&c_dfDIMouse)))
			IncompatibleMachineParameterError(0, false);

		if (FAILED(m_pDInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			IncompatibleMachineParameterError(0, false);

		*(int*)&m_pBuffer = (int)Allocators::AllocatorsList[DEFAULT]->Allocate(sizeof(DIDEVICEOBJECTDATA) * INPUT_MOUSE_BUFFERS_COUNT, NULL, NULL);
		memset(m_pBuffer, NULL, sizeof(DIDEVICEOBJECTDATA) * INPUT_MOUSE_BUFFERS_COUNT);

		m_nBufferSize = NULL;

		DIPROPDWORD diProperty;
		diProperty.diph.dwSize = 20;
		diProperty.diph.dwHeaderSize = 16;
		diProperty.diph.dwObj = 0;
		diProperty.diph.dwHow = 0;
		diProperty.dwData = 30;

		if (FAILED(m_pDInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProperty.diph)))
			IncompatibleMachineParameterError(0, false);

		m_bAcquired = false;
		if (SUCCEEDED(m_pDInputDevice->Acquire()))
			m_bAcquired = true;

		tagPOINT clientPos;
		GetCursorPos(&clientPos);
		ScreenToClient(g_Window->m_hWindow, &clientPos);

		m_OldLocalPosition = clientPos;
	}

	Mouse::~Mouse()
	{
		MESSAGE_CLASS_DESTROYED(Mouse);
	}

	void Mouse::_Acquire()
	{
		if (!m_bAcquired &&
			SUCCEEDED(m_pDInputDevice->Acquire()))
			m_bAcquired = true;
	}

	void Mouse::UnacquireAndReset()
	{
		m_pDInputDevice->Unacquire();
		m_bAcquired = false;

		m_nMouseButtons[MOUSE_BUTTON_0] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_1] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_2] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_3] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_4] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_5] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_6] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_7] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_WHEEL_UP] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_WHEEL_DOWN] = NULL;
	}

	void Mouse::SetWindowCapture(HWND window)
	{
		if (++field_44 == 1)
			SetCapture(window ? window : g_Window->m_hWindow);
	}

	void Mouse::ReleaseWindowCapture()
	{
		if (field_44-- == 1)
			ReleaseCapture();

		if (field_44 < 0)
			field_44 = NULL;
	}

	void Mouse::ResetButtonsState()
	{
		m_nMouseButtons[MOUSE_BUTTON_0] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_1] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_2] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_3] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_4] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_5] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_6] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_7] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_WHEEL_UP] = NULL;
		m_nMouseButtons[MOUSE_BUTTON_WHEEL_DOWN] = NULL;

		m_Position_Z = NULL;
	}

	const char* Mouse::MouseButtonToStr(unsigned int button)
	{
		if (button >= 0 &&
			button <= INPUT_MOUSE_MAX_BUTTONS)
			return g_MouseButtons[button];
		else
			return nullptr;
	}

	const char* Mouse::MousePositionTostr(unsigned int axis)
	{
		if (!axis)
			return "MOUSE_POSITION_X";
		if (axis == 1)
			return "MOUSE_POSITION_Y";
		return nullptr;
	}

	void Mouse::Process()
	{
		m_bShouldBeProcessed = g_Window->m_bVisible;

		tagPOINT clientPos;
		GetCursorPos(&clientPos);
		ScreenToClient(g_Window->m_hWindow, &clientPos);
		m_NewLocalPosition = clientPos;

		if (m_bAcquired)
		{
			if (SUCCEEDED(m_pDInputDevice->Acquire()))
				m_bAcquired = true;
			if (!m_bAcquired)
			{
				m_bShouldBeProcessed = false;
				return;
			}
		}

		DIMOUSESTATE* mouseState;
		if (FAILED(m_pDInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState)))
		{
			debug("Failed GetDeviceState\n");

			m_pDInputDevice->Unacquire();
			m_bAcquired = false;

			m_nMouseButtons[MOUSE_BUTTON_0] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_1] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_2] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_3] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_4] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_5] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_6] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_7] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_WHEEL_UP] = NULL;
			m_nMouseButtons[MOUSE_BUTTON_WHEEL_DOWN] = NULL;

			return;
		}

		m_Position_X = mouseState->lX;
		m_Position_Y = mouseState->lY;
		m_Position_Z = mouseState->lZ;

		m_OldLocalPosition.x += mouseState->lX;
		m_OldLocalPosition.y += mouseState->lY;
		
		Vector2<int> screenRes;
		GfxInternal_Dx9::GetScreenResolution(screenRes);

		m_OldLocalPosition.x = m_OldLocalPosition.x < 0 ? 0 : screenRes.x - m_OldLocalPosition.x;
		m_OldLocalPosition.y = m_OldLocalPosition.y < 0 ? 0 : screenRes.y - m_OldLocalPosition.y;

		DWORD buffSize = 30;
		if (HRESULT msdevdatares = m_pDInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), (LPDIDEVICEOBJECTDATA)&m_pBuffer, &buffSize, NULL))
		{
			if (msdevdatares != 1)
			{
				debug("Failed GetDeviceData - %i\n", msdevdatares);
				return;
			}
			debug("WARNING: Mouse buffer overflowed! (size %i, count %i)", 30, buffSize);
		}

		if (buffSize <= 0)
			return;

		if (m_nBufferSize <= buffSize)
			m_nBufferSize = buffSize;

		//	NOTE: why?
		if (buffSize <= 0)
			return;

		for (unsigned int ind = 0; ind < buffSize; ind++)
			switch (m_pBuffer[ind]->dwOfs)
			{
			case 0:
			case 4:
				break;
			case 8:
				if (m_pBuffer[ind]->dwData >= 0)
				{
					if (m_pBuffer[ind]->dwData > 0)
						m_nMouseButtons[MOUSE_BUTTON_WHEEL_UP] |= 6;
				}else
					m_nMouseButtons[MOUSE_BUTTON_WHEEL_DOWN] |= 6;
				break;
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
				m_nMouseButtons[m_pBuffer[ind]->dwOfs - 12] |= (m_pBuffer[ind]->dwData >> 7) & 1 ? 3 : ((m_nMouseButtons[m_pBuffer[ind]->dwOfs - 12] & 0xFFFFFFFE) | 4);
				break;
			default:
				debug("WARNING: Invalid mousebutton %i!", m_pBuffer[ind]->dwOfs);
				break;
			}
	}

}