#include "Control.h"
#include "IntegerType.h"
#include "StringType.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "InputGameController.h"
#ifdef DIRECTX
#include "GfxInternal_Dx9.h"
#endif
#ifdef OPENGL
#include "GfxInternal_OGL.h"
#endif

EntityType* tControl;
unsigned int Control::ActiveControllerIndex;
bool Control::ControllersUsedByEditor[8];

void Control::Register()
{
    tControl = new EntityType("Control");
    tControl->InheritFrom(tNode);
    tControl->SetCreator((CREATOR)Create);

    tControl->RegisterProperty(tINTEGER, "key", (EntityGetterFunction)&GetKey, NULL, NULL, NULL, (EntitySetterFunction)&SetKey, NULL, NULL, NULL, "control=string", NULL, NULL, -1);
    tControl->RegisterProperty(tSTRING, "keystr", (EntityGetterFunction)&GetKeyStr, NULL, NULL, NULL, (EntitySetterFunction)&SetKeyStr, NULL, NULL, NULL, "control=string", NULL, NULL, -1);

    tControl->PropagateProperties();
}

const int Control::MousePositionEngineToString() const
{
    if (m_ControlType != MOUSE)
        return NULL;

    switch (m_Key)
    {
    case 0:
        return g_InputMouse->m_Position_X;
    case 1:
        return g_InputMouse->m_Position_Y;
    default:
        return NULL;
    }
}

const int Control::MousePositionRawToString() const
{
    if (m_ControlType != MOUSE)
        return NULL;

    switch (m_Key)
    {
    case 0:
        return
#ifdef DIRECTX
            g_GfxInternal_Dx9->m_Windowed
#endif
#ifdef OPENGL
            g_GfxInternal_OGL->m_Windowed
#endif
            ? g_InputMouse->m_WindowedMousePosition.x : g_InputMouse->m_FullscreenMousePosition.x;
    case 1:
        return
#ifdef DIRECTX
            g_GfxInternal_Dx9->m_Windowed
#endif
#ifdef OPENGL
            g_GfxInternal_OGL->m_Windowed
#endif
            ? g_InputMouse->m_WindowedMousePosition.y : g_InputMouse->m_FullscreenMousePosition.y;
    default:
        return NULL;
    }
}

void Control::GetStateInfo(float* pressure, float* realpressure, bool* pressed, bool* released)
{
    //  NOTE: original code has 'is acquired' check before getting state of each "button" and I'm not sure why, because the app is single threaded...
    switch (m_ControlType)
    {
        case KEYBOARD:
        {
            if (!g_InputKeyboard->m_Acquired)
                break;

            if ( (g_InputKeyboard->m_ButtonStates_1[m_Key] & 0x80) != 0 )
                *pressure = 1.f;

            if ( (g_InputKeyboard->m_ButtonStates[m_Key] & 1) != 0 )
                *pressed = true;

            if ( ( (g_InputKeyboard->m_ButtonStates[m_Key] >> 1) & 1 ) != 0 )
                *released = true;

            break;
        }
        case MOUSE:
        {
            *pressure = 0.f;
            *pressed = false;
            *released = false;

            break;
        }
        case MOUSE_BUTTON:
        {
            if (!g_InputMouse->m_ShouldBeProcessed)
                break;

            if ( (g_InputMouse->m_MouseButtons[m_Key] & 1) != 0 )
                *pressure = 1.f;

            if ( (g_InputMouse->m_MouseButtons[m_Key] & 2) != 0 )
                *pressed = true;

            if ( ( (g_InputMouse->m_MouseButtons[m_Key] >> 2) & 1 ) != 0 )
                *released = true;

            break;
        }
        case GAMEPAD:
        {
            if (!Input::Gamepad::IsControllerPresent(ActiveControllerIndex) || ControllersUsedByEditor[ActiveControllerIndex])
            {
                *pressure = 0.f;
                *pressed = false;
                *released = false;

                break;
            }

            Input::Gamepad* currentGamepad = Input::Gamepad::GetGameControllerByIndex(ActiveControllerIndex);
            const float buttonPressure = currentGamepad->GetPressure(m_Key);
            *pressure = buttonPressure;

            if (buttonPressure == 0.f)
            {
                if (currentGamepad->IsButtonPressed(m_Key))
                    *pressure = 1.f;
            }

            //  NOTE: difference from the above is unclear.
            if (currentGamepad->KeyPressed(m_Key))
                *pressed = true;

            if (currentGamepad->IsReleased(m_Key))
                *released = true;

            break;
        }
        case DPAD:
        {
            if (!Input::Gamepad::IsControllerPresent(ActiveControllerIndex) || ControllersUsedByEditor[ActiveControllerIndex])
            {
                *pressure = 0.f;
                *pressed = false;
                *released = false;

                break;
            }

            const float pressForce = GetDPadPressForce((DPadKey)m_Key);
            const float normalizedPressure = (pressForce - 0.3f) * 1.4285715f;
            *pressure = normalizedPressure;
            *realpressure = pressForce;

            if (normalizedPressure <= 0.f || m_IsNormalizedPressure)
                *pressed = false;
            else
                *pressed = true;
            
            if (normalizedPressure == 0.f && m_IsNormalizedPressure)
                *released = true;
            else
                *released = false;

            m_IsNormalizedPressure = normalizedPressure > 0.f;

            break;
        }
        default:
            break;
    }
}

const int Control::GetKey() const
{
    return m_Key;
}

void Control::SetKey(const int keyind)
{
    if (keyind >= 0)
        m_Key = keyind;
}

const char* Control::GetKeyStr() const
{
    switch (m_ControlType)
    {
    case NONE:
        return nullptr;
    case KEYBOARD:
        return Input::Keyboard::KeyboardButtonToStr(m_Key);
    case MOUSE:
        return Input::Mouse::MousePositionTostr(m_Key);
    case MOUSE_BUTTON:
        return Input::Mouse::MouseButtonToStr(m_Key);
    case GAMEPAD:
        return Input::Gamepad::GamepadButtonToString(m_Key);
    case DPAD:
        return DPadKeyToStr(m_Key);
    default:
        return nullptr;
    }
}

void Control::SetKeyStr(const char* args)
{
    m_Key = -1;
    m_ControlType = NONE;

    for (unsigned int i = 0; i < 255; ++i)
    {
        if (strcmp(Input::Keyboard::KeyboardButtonToStr(i), args) == NULL)
        {
            m_Key = i;
            m_ControlType = KEYBOARD;

            return;
        }
    }

    for (unsigned int i = 0; i < 2; ++i)
    {
        if (strcmp(Input::Mouse::MousePositionTostr(i), args) == NULL)
        {
            m_Key = i;
            m_ControlType = MOUSE;

            return;
        }
    }

    for (unsigned int i = 0; i < 10; ++i)
    {
        if (strcmp(Input::Mouse::MouseButtonToStr(i), args) == NULL)
        {
            m_Key = i;
            m_ControlType = MOUSE_BUTTON;

            return;
        }
    }

    for (unsigned int i = 0; i < 255; ++i)
    {
        if (strcmp(Input::Gamepad::GamepadButtonToString(i), args) == NULL)
        {
            m_Key = i;
            m_ControlType = GAMEPAD;

            return;
        }
    }

    for (unsigned int i = 0; i < 255; ++i)
    {
        if (strcmp(DPadKeyToStr(i), args) == NULL)
        {
            m_Key = i;
            m_ControlType = DPAD;

            return;
        }
    }

    LogDump::LogA("WARNING: keystr '%s' didn't map to any key\n", args);
}

const char* Control::DPadKeyToStr(const unsigned int keyid) const
{
    switch (keyid)
    {
    case 0:
        return "A_LEFT";
    case 1:
        return "A_RIGHT";
    case 2:
        return "A_UP";
    case 3:
        return "A_DOWN";
    case 4:
        return "C_LEFT";
    case 5:
        return "C_RIGHT";
    case 6:
        return "C_UP";
    case 7:
        return "C_DOWN";
    default:
        return nullptr;
    }
}

float Control::GetDPadPressForce(const DPadKey key) const
{
    const Input::Gamepad* gamepad = Input::Gamepad::GetGameControllerByIndex(ActiveControllerIndex);
    if (!gamepad)
        return 0;

    Vector4f axisposition0, axisposition1;
    gamepad->GetAxisPosition(&axisposition0, 0);
    gamepad->GetAxisPosition(&axisposition1, 1);

    switch (key)
    {
        case A_LEFT:
            return 0 - axisposition0.x < 0 ? 0 : 0 - axisposition0.x;
        case A_RIGHT:
            return axisposition0.x < 0 ? 0 : axisposition0.x;
        case A_UP:
            return 0 - axisposition0.y < 0 ? 0 : 0 - axisposition0.y;
        case A_DOWN:
            return axisposition0.y < 0 ? 0 : axisposition0.y;
        case C_LEFT:
            return 0 - axisposition1.x < 0 ? 0 : 0 - axisposition1.x;
        case C_RIGHT:
            return axisposition1.x < 0 ? 0 : axisposition1.x;
        case C_UP:
            return 0 - axisposition1.y < 0 ? 0 : 0 - axisposition1.y;
        case C_DOWN:
            return axisposition1.y < 0 ? 0 : axisposition1.y;
        default:
            return 0;
    }
}

Control* Control::Create(AllocatorIndex)
{
    return new Control;
}