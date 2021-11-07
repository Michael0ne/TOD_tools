#include "InputMouse.h"
#include "Window.h"
#include "GfxInternal_Dx9.h"
#include "LogDump.h"

Input::Mouse* g_InputMouse = NULL;
const char* g_MouseButtons[] = {
    "MOUSE_BUTTON_0", "MOUSE_BUTTON_1",
    "MOUSE_BUTTON_2", "MOUSE_BUTTON_3",
    "MOUSE_BUTTON_4", "MOUSE_BUTTON_5",
    "MOUSE_BUTTON_6", "MOUSE_BUTTON_7",
    "MOUSE_BUTTON_WHEEL_UP", "MOUSE_BUTTON_WHEEL_DOWN"
};

namespace Input {

    Mouse::Mouse()
    {
        MESSAGE_CLASS_CREATED(Mouse);

        m_MouseButtons[MOUSE_BUTTON_0] = NULL;
        m_MouseButtons[MOUSE_BUTTON_1] = NULL;
        m_MouseButtons[MOUSE_BUTTON_2] = NULL;
        m_FullscreenMousePosition.x = m_FullscreenMousePosition.y = NULL;
        m_Position_X = m_Position_Y = m_Position_Z = NULL;
        m_WindowedMousePosition.x = m_WindowedMousePosition.y = NULL;
        field_44 = NULL;
        m_ShouldBeProcessed = true;
        m_DirectInputInterface = nullptr;
        m_DirectInputDevice = nullptr;

        g_InputMouse = this;

#ifndef _EXE
        if (FAILED(DirectInput8Create_Hooked(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInputInterface, NULL)))
#else
        if (FAILED(DirectInput8Create(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInputInterface, NULL)))
#endif
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_MOUSE, false);

        if (FAILED(m_DirectInputInterface->CreateDevice(GUID_SysMouse, &m_DirectInputDevice, NULL)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_MOUSE, false);

        if (FAILED(m_DirectInputDevice->SetDataFormat(&c_dfDIMouse)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_MOUSE, false);

        if (FAILED(m_DirectInputDevice->SetCooperativeLevel(g_Window->m_WindowHandle, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_MOUSE, false);

        m_Buffer = new DIDEVICEOBJECTDATA[INPUT_MOUSE_BUFFERS_COUNT];

        m_BufferSize = NULL;

        DIPROPDWORD diProperty;
        diProperty.diph.dwSize = 20;
        diProperty.diph.dwHeaderSize = 16;
        diProperty.diph.dwObj = 0;
        diProperty.diph.dwHow = 0;
        diProperty.dwData = 30;

        if (FAILED(m_DirectInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProperty.diph)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_MOUSE, false);

        m_Acquired = false;
        if (SUCCEEDED(m_DirectInputDevice->Acquire()))
            m_Acquired = true;

        tagPOINT clientPos;
        GetCursorPos(&clientPos);
        ScreenToClient(g_Window->m_WindowHandle, &clientPos);

        m_WindowedMousePosition = clientPos;
    }

    Mouse::~Mouse()
    {
        MESSAGE_CLASS_DESTROYED(Mouse);
    }

    void Mouse::_Acquire()
    {
        if (!m_Acquired &&
            SUCCEEDED(m_DirectInputDevice->Acquire()))
            m_Acquired = true;
    }

    void Mouse::UnacquireAndReset()
    {
        m_DirectInputDevice->Unacquire();
        m_Acquired = false;

        m_MouseButtons[MOUSE_BUTTON_0] = NULL;
        m_MouseButtons[MOUSE_BUTTON_1] = NULL;
        m_MouseButtons[MOUSE_BUTTON_2] = NULL;
        m_MouseButtons[MOUSE_BUTTON_3] = NULL;
        m_MouseButtons[MOUSE_BUTTON_4] = NULL;
        m_MouseButtons[MOUSE_BUTTON_5] = NULL;
        m_MouseButtons[MOUSE_BUTTON_6] = NULL;
        m_MouseButtons[MOUSE_BUTTON_7] = NULL;
        m_MouseButtons[MOUSE_BUTTON_WHEEL_UP] = NULL;
        m_MouseButtons[MOUSE_BUTTON_WHEEL_DOWN] = NULL;
    }

    void Mouse::SetWindowCapture(HWND window)
    {
        if (++field_44 == 1)
            SetCapture(window ? window : g_Window->m_WindowHandle);
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
        m_MouseButtons[MOUSE_BUTTON_0] = NULL;
        m_MouseButtons[MOUSE_BUTTON_1] = NULL;
        m_MouseButtons[MOUSE_BUTTON_2] = NULL;
        m_MouseButtons[MOUSE_BUTTON_3] = NULL;
        m_MouseButtons[MOUSE_BUTTON_4] = NULL;
        m_MouseButtons[MOUSE_BUTTON_5] = NULL;
        m_MouseButtons[MOUSE_BUTTON_6] = NULL;
        m_MouseButtons[MOUSE_BUTTON_7] = NULL;
        m_MouseButtons[MOUSE_BUTTON_WHEEL_UP] = NULL;
        m_MouseButtons[MOUSE_BUTTON_WHEEL_DOWN] = NULL;

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
        switch (axis)
        {
        case 0:
            return "MOUSE_POSITION_X";
        case 1:
            return "MOUSE_POSITION_Y";
        default:
            return nullptr;
        }
    }

    void Mouse::Process()
    {
        m_ShouldBeProcessed = g_Window->m_Visible;

        tagPOINT clientPos;
        GetCursorPos(&clientPos);
        ScreenToClient(g_Window->m_WindowHandle, &clientPos);
        m_FullscreenMousePosition = clientPos;

        if (m_Acquired)
        {
            if (SUCCEEDED(m_DirectInputDevice->Acquire()))
                m_Acquired = true;

            if (!m_Acquired)
            {
                m_ShouldBeProcessed = false;
                return;
            }
        }

        DIMOUSESTATE* mouseState;
        if (FAILED(m_DirectInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState)))
        {
            // FIXME: removed to suppress 'failed' messages spam for now. Restore in future.
            //LogDump::LogA("Failed GetDeviceState\n");

            m_DirectInputDevice->Unacquire();
            m_Acquired = false;

            m_MouseButtons[MOUSE_BUTTON_0] = NULL;
            m_MouseButtons[MOUSE_BUTTON_1] = NULL;
            m_MouseButtons[MOUSE_BUTTON_2] = NULL;
            m_MouseButtons[MOUSE_BUTTON_3] = NULL;
            m_MouseButtons[MOUSE_BUTTON_4] = NULL;
            m_MouseButtons[MOUSE_BUTTON_5] = NULL;
            m_MouseButtons[MOUSE_BUTTON_6] = NULL;
            m_MouseButtons[MOUSE_BUTTON_7] = NULL;
            m_MouseButtons[MOUSE_BUTTON_WHEEL_UP] = NULL;
            m_MouseButtons[MOUSE_BUTTON_WHEEL_DOWN] = NULL;

            return;
        }

        m_Position_X = mouseState->lX;
        m_Position_Y = mouseState->lY;
        m_Position_Z = mouseState->lZ;

        m_WindowedMousePosition.x += mouseState->lX;
        m_WindowedMousePosition.y += mouseState->lY;

        Vector2<unsigned int> screenRes;
        GfxInternal_Dx9::GetScreenResolution(screenRes);

        m_WindowedMousePosition.x = m_WindowedMousePosition.x < 0 ? 0 : screenRes.x - m_WindowedMousePosition.x;
        m_WindowedMousePosition.y = m_WindowedMousePosition.y < 0 ? 0 : screenRes.y - m_WindowedMousePosition.y;

        DWORD buffSize = 30;
        HRESULT devicedata = m_DirectInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_Buffer, &buffSize, NULL);
        if (SUCCEEDED(devicedata))
        {
            if (devicedata != 1)
            {
                LogDump::LogA("Failed GetDeviceData - %i\n", devicedata);
                return;
            }
            LogDump::LogA("WARNING: Mouse buffer overflowed! (size %i, count %i)", 30, buffSize);
        }

        if (buffSize <= 0)
            return;

        if (m_BufferSize <= buffSize)
            m_BufferSize = buffSize;

        for (unsigned int ind = 0; ind < buffSize; ind++)
        {
            switch (m_Buffer[ind].dwOfs)
            {
            case 0:
            case 4:
                break;
            case 8:
                if (m_Buffer[ind].dwData >= 0)
                {
                    if (m_Buffer[ind].dwData > 0)
                        m_MouseButtons[MOUSE_BUTTON_WHEEL_UP] |= 6;
                }
                else
                    m_MouseButtons[MOUSE_BUTTON_WHEEL_DOWN] |= 6;
                break;
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
                m_MouseButtons[m_Buffer[ind].dwOfs - 12] |= (m_Buffer[ind].dwData >> 7) & 1 ? 3 : ((m_MouseButtons[m_Buffer[ind].dwOfs - 12] & 0xFFFFFFFE) | 4);
                break;
            default:
                LogDump::LogA("WARNING: Invalid mousebutton %i!", m_Buffer[ind].dwOfs);
                break;
            }
        }
    }
}