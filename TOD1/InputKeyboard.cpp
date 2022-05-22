#include "InputKeyboard.h"
#include "Platform.h"
#include "LogDump.h"

Input::Keyboard* g_InputKeyboard;
const char* Input::Keyboard::KeyboardButtons[] = {
    "-none-", "ESCAPE", "1", "2", "3",
    "4", "5", "6", "7", "8",
    "9", "0", "-", "=", "BACKSPACE",
    "TAB", "Q", "W", "E", "R",
    "T", "Y", "U", "I", "O",
    "P", "[", "]", "RETURN", "LCONTROL",
    "A", "S", "D", "F", "G",
    "H", "J", "K", "L", ";",
    "'", "r", "LSHIFT", "\\", "Z",
    "X", "C", "V", "B", "N",
    "M", ",", ".", "/", "RSHIFT",
    "NUMPAD*", "LALT", "SPACE", "CAPSLOCK", "F1",
    "F2", "F3", "F4", "F5", "F6",
    "F7", "F8", "F9", "F10", "NUMLOCK",
    "SCROLLOCK", "NUMPAD7", "NUMPAD8", "NUMPAD9", "NUMPAD-",
    "NUMPAD4", "NUMPAD5", "NUMPAD6", "NUMPAD+", "NUMPAD1",
    "NUMPAD2", "NUMPAD3", "NUMPAD0", "NUMPAD.", "EM102",
    "F11", "F12", "F13", "F14", "F15",
    "KANA", "CONVERT", "NOCONVERT", "YEN", "NUMPAD=",
    "AT", "COLON", "KANJI", "AX", "UNLABELED",
    "NUMPADENTER", "RCONTROL", "NUMPAD,", "NUMPAD/", "SYSRQ",
    "RALT", "PAUSE", "HOME", "UP", "PAGEUP",
    "LEFT", "RIGHT", "END", "DOWN",
    "PAGEDOWN", "INSERT", "LWIN", "RWIN"
};

namespace Input
{
    void Keyboard::Process()
    {
        ResetKeyboardButtons();

        if (!m_Acquired)
            return;

        DWORD buffSize = INPUT_KEYBOARD_BUFFERS_COUNT;
        HRESULT keybDevDataResult = m_DirectInputDeviceInterface->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_DataBuffer, &buffSize, NULL);
        if (SUCCEEDED(keybDevDataResult))
            if (keybDevDataResult == DI_BUFFEROVERFLOW)
                LogDump::LogA("WARNING: Keyboard buffer overflowed! (size %i, count %i)\n", INPUT_KEYBOARD_BUFFERS_COUNT, buffSize);
            else
                UnacquireAndResetKeyboard();

        if (buffSize)
        {
            if (m_DataBufferSize <= buffSize)
                m_DataBufferSize = buffSize;

            for (unsigned int ind = 0; ind < buffSize; ind++)
                m_ButtonStates[m_DataBuffer[ind].dwOfs] = ((m_DataBuffer[ind].dwData >> 7) & 1) == 0 ? m_ButtonStates[m_DataBuffer[ind].dwOfs] | 2 : m_ButtonStates[m_DataBuffer[ind].dwOfs] | 1;
        }

        if (SUCCEEDED(m_DirectInputDeviceInterface->GetDeviceState(sizeof(m_ButtonStates_1), m_ButtonStates_1)))
            UnacquireAndResetKeyboard();

        if (m_Acquired &&
            (m_ButtonStates_1[56] & 0x80) != NULL &&
            (m_ButtonStates_1[15] & 0x80) != NULL)
        {
            m_ButtonStates_1[15] = NULL;
            m_ButtonStates[15] = NULL;
        }
    }

    void Keyboard::Reset()
    {
        memset(&m_ButtonStates, NULL, sizeof(m_ButtonStates));
    }

    void Keyboard::ResetKeyboardButtons()
    {
        if (!m_Acquired)
        {
            if (!FAILED(m_DirectInputDeviceInterface->Acquire()))
                m_Acquired = true;

            for (int ind = 255; ind != 0; ind--)
                m_ButtonStates_1[ind] = NULL;
        }
    }

    void Keyboard::UnacquireAndResetKeyboard()
    {
        if (m_Acquired)
        {
            m_DirectInputDeviceInterface->Unacquire();
            m_Acquired = false;

            for (int ind = 255; ind != 0; ind--)
                m_ButtonStates_1[ind] = NULL;
        }
    }

    const char* Keyboard::KeyboardButtonToStr(int keyCode)
    {
        keyCode++;
        if (keyCode >= 0 &&
            keyCode <= 118)
            return KeyboardButtons[keyCode];
        else
            return NULL;
    }

    Keyboard::Keyboard()
    {
        MESSAGE_CLASS_CREATED(Keyboard);

        g_InputKeyboard = this;

        m_DirectInputDevice = nullptr;
        m_DirectInputDeviceInterface = nullptr;
        m_Acquired = false;
        m_DataBufferSize = NULL;

        for (int ind = 255; ind != 0; ind--)
            m_ButtonStates_1[ind] = NULL;

#ifndef _EXE
        if (FAILED(DirectInput8Create_Hooked(Platform::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&m_DirectInputDevice, NULL)))
#else
        if (FAILED(DirectInput8Create(Platform::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&m_DirectInputDevice, NULL)))
#endif
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_KEYBOARD, false);
        if (FAILED(m_DirectInputDevice->CreateDevice(GUID_SysKeyboard, &m_DirectInputDeviceInterface, NULL)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_KEYBOARD, false);
        if (FAILED(m_DirectInputDeviceInterface->SetDataFormat(&c_dfDIKeyboard)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_KEYBOARD, false);
        if (FAILED(m_DirectInputDeviceInterface->SetCooperativeLevel(g_Platform->m_WindowHandle, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_KEYBOARD, false);

        m_DataBuffer = new DIDEVICEOBJECTDATA[INPUT_KEYBOARD_BUFFERS_COUNT];

        DIPROPDWORD diProperty;
        diProperty.diph.dwSize = 20;
        diProperty.diph.dwHeaderSize = 16;
        diProperty.diph.dwObj = 0;
        diProperty.diph.dwHow = 0;
        diProperty.dwData = 30;

        if (FAILED(m_DirectInputDeviceInterface->SetProperty(DIPROP_BUFFERSIZE, &diProperty.diph)))
            IncompatibleMachineParameterError(ERRMSG_INCOMPATIBLE_KEYBOARD, false);

        if (m_Acquired)
            return;

        if (SUCCEEDED(m_DirectInputDeviceInterface->Acquire()))
            m_Acquired = true;

        for (int ind = 255; ind != 0; ind--)
            m_ButtonStates_1[ind] = NULL;
    }

    Keyboard::~Keyboard()
    {
        MESSAGE_CLASS_DESTROYED(Keyboard);

        UnacquireAndResetKeyboard();

        RELEASE_SAFE(m_DirectInputDeviceInterface);
        RELEASE_SAFE(m_DirectInputDevice);
    }
}