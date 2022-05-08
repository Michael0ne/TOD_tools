#include "InputGameController.h"
#include "ScriptDatabase.h"
#include "LogDump.h"

Input::Gamepad* g_InputGamepad[INPUT_GAMEPAD_MAX_GAMEPADS] = {};

const char* g_GamepadButtons[INPUT_GAMEPAD_MAX_BUTTONS] = {
 "SELECT",
 "L3",
 "R3",
 "START",
 "D_UP",
 "D_RIGHT",
 "D_DOWN",
 "D_LEFT",
 "L2",
 "R2",
 "L1",
 "R1",
 "TRIANGLE",
 "CIRCLE",
 "CROSS",
 "SQUARE",
};

namespace Input
{
    unsigned int Gamepad::ControllersCreated = NULL;
    LPDIENUMDEVICESCALLBACK Gamepad::EnumCallback;
    int Gamepad::DirectInputGamepadsFound = -1;
    Gamepad** Gamepad::GamepadsArray = nullptr;
    LPDIRECTINPUT8 Gamepad::DirectInput8Interface = nullptr;
    unsigned int Gamepad::GamepadNormalButtons[] =
    {
        8, 9, 10, 11
    };
    unsigned int Gamepad::SmartJoyButtons[] =
    {
        9, 10, 11, 8
    };

    Gamepad::Gamepad(int controllerIndex, int unk1)
    {
        MESSAGE_CLASS_CREATED(Gamepad);

        field_B4 = unk1;
        m_ForceFeedbackAvailable = false;
        m_IsVibrating = false;
        field_E1 = NULL;
        m_DirectInputEffect = nullptr;
        field_D4 = NULL;
        m_VibrationForceDefault = 0.0f;
        m_VibrationForce = 0.0f;
        m_DirectInputDevice = nullptr;

        if (!ControllersCreated)
        {
#ifndef _EXE
            DirectInput8Create_Hooked(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&DirectInput8Interface, NULL);
#else
            DirectInput8Create(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&DirectInput8Interface, NULL);
#endif
            EnumGameControllers();
        }

        ++ControllersCreated;
        g_InputGamepad[controllerIndex] = this;

        if (controllerIndex >= DirectInputGamepadsFound)
            return;

        GamepadsArray = nullptr;

        int _pvRef = controllerIndex;
        if (Script::ForceFeedback &&
            SUCCEEDED(DirectInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)&EnumControllersCallback, &_pvRef, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY)) &&
            GamepadsArray)
        {
            LogDump::LogA("CONTROLLER: Found a force-feedback device!");
            m_ForceFeedbackAvailable = true;

            if (!m_DirectInputDevice)
            {
                LogDump::LogA("CONTROLLER: Error - could not create input device!");
                return;
            }
        }

        if (SUCCEEDED(DirectInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)&EnumControllersCallback, &_pvRef, DIEDFL_ALLDEVICES)) && GamepadsArray)
        {
            LogDump::LogA("CONTROLLER: Using standard game device");

            if (!m_DirectInputDevice)
            {
                LogDump::LogA("CONTROLLER: Error - could not create input device!");
                return;
            }

            m_DirectInputDevice->Acquire();

            memset(&m_ButtonsStates, NULL, sizeof(m_ButtonsStates));

            LogDump::LogA("CONTROLLER: Model of game device = %s", m_ModelName.m_Str);

#ifdef INCLUDE_FIXES
            m_SmartJoyGamepad = strncmp(m_ModelName.m_Str, "SmartJoy", 8) == NULL;
#else
            // NOTE: this is stupid, so disabled by default.
            String sModelName;
            AllocateGamepadNameStringBuffer(&sModelName, m_ModelName.m_Str, 8);

            m_SmartJoyGamepad = strncmp(sModelName.m_Str, "SmartJoy", 8) == NULL;
#endif

            if (!m_ForceFeedbackAvailable)
                return;

            DIPROPDWORD DIPropAutoCenterDisable;

            DIPropAutoCenterDisable.diph.dwSize = sizeof(DIPropAutoCenterDisable);
            DIPropAutoCenterDisable.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            DIPropAutoCenterDisable.diph.dwObj = NULL;
            DIPropAutoCenterDisable.diph.dwHow = DIPH_DEVICE;
            DIPropAutoCenterDisable.dwData = DIPROPAUTOCENTER_OFF;

            if (FAILED(m_DirectInputDevice->SetProperty(DIPROP_AUTOCENTER, &DIPropAutoCenterDisable.diph)))
                LogDump::LogA("CONTROLLER: Could not disable auto-center");

            // TODO: these parameters are NOT what they are in exe.
            DWORD dwAxes[2] = { DIJOFS_X, DIJOFS_Y };
            long lDirection[2] = { NULL, NULL };

            DIPERIODIC diPeriodic;
            DIENVELOPE diEnvelope;
            DIEFFECT diEffect;

            diPeriodic.dwMagnitude = DI_FFNOMINALMAX;
            diPeriodic.lOffset = 0;
            diPeriodic.dwPhase = 0;
            diPeriodic.dwPeriod = (DWORD)(0.05f * DI_SECONDS);

            diEnvelope.dwSize = sizeof(DIENVELOPE);
            diEnvelope.dwAttackLevel = 0;
            diEnvelope.dwAttackTime = (DWORD)(0.5f * DI_SECONDS);
            diEnvelope.dwFadeLevel = 0;
            diEnvelope.dwFadeTime = (DWORD)(1.0f * DI_SECONDS);

            diEffect.dwSize = sizeof(DIEFFECT);
            diEffect.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
            diEffect.dwDuration = (2 * DI_SECONDS);

            diEffect.dwSamplePeriod = 0;
            diEffect.dwGain = DI_FFNOMINALMAX;
            diEffect.dwTriggerButton = DIJOFS_BUTTON0;
            diEffect.dwTriggerRepeatInterval = 0;
            diEffect.cAxes = 2;
            diEffect.rgdwAxes = dwAxes;
            diEffect.rglDirection = &lDirection[0];
            diEffect.lpEnvelope = &diEnvelope;
            diEffect.cbTypeSpecificParams = sizeof(diPeriodic);
            diEffect.lpvTypeSpecificParams = &diPeriodic;

            LPDIRECTINPUTEFFECT lpdiRumbleEffect;

            if (FAILED(m_DirectInputDevice->CreateEffect(GUID_ConstantForce, &diEffect, &lpdiRumbleEffect, NULL)))
                LogDump::LogA("CONTROLLER: Could not create rumble effect for some reason");

            return;
        }

        LogDump::LogA("CONTROLLER: WARNING - Could not create an input device");
    }

    Gamepad::~Gamepad()
    {
        MESSAGE_CLASS_DESTROYED(Gamepad);

        --ControllersCreated;

        if (m_DirectInputDevice)
        {
            m_DirectInputDevice->Unacquire();
            RELEASE_SAFE(m_DirectInputDevice);
        }

        if (!ControllersCreated && DirectInput8Interface)
        {
            RELEASE_SAFE(DirectInput8Interface);
        }
    }

    Gamepad* Gamepad::GetGameControllerByIndex(signed int controllerIndex)
    {
        if (controllerIndex >= 0 ||
            controllerIndex < INPUT_GAMEPAD_MAX_GAMEPADS)
            return g_InputGamepad[controllerIndex];
        else
            return nullptr;
    }

    BOOL Gamepad::DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
    {
        ++DirectInputGamepadsFound;

        return TRUE;
    }

    signed int Gamepad::ReportFoundEffectProperties(int* unk1, int* unk2)
    {
        LogDump::LogA("Found effect: %s\n", (const char*)(unk1 + 8));
        *unk2 = unk1[1];
        unk2[1] = unk1[2];
        unk2[2] = unk1[3];
        unk2[3] = unk1[4];
        return 1;
    }

    signed int Gamepad::ReportFoundObjectName(int* unk1, int unk2)
    {
        LogDump::LogA("Found object: %s\n", (const char*)(unk1 + 8));
        return 1;
    }

    const char* Gamepad::GamepadButtonToString(int buttonNumber)
    {
        if (buttonNumber >= 0 &&
            buttonNumber < INPUT_GAMEPAD_MAX_BUTTONS)
            return g_GamepadButtons[buttonNumber];
        else
            return "<none>";
    }

    void Gamepad::EnumGameControllers()
    {
        DirectInputGamepadsFound = NULL;
        DirectInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)DIEnumDevicesCallback, NULL, NULL);

        //g_InputGamepad = (Gamepad**)(new unsigned int[DirectInputGamepadsFound]);
    }

    int Gamepad::NumberDirectInputDevices()
    {
        if (DirectInputGamepadsFound != -1)
            return DirectInputGamepadsFound;

        bool bInterfaceCreated = false;

        if (!DirectInput8Interface)
        {
            DirectInput8Create(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&DirectInput8Interface, NULL);
            bInterfaceCreated = true;
        }

        EnumGameControllers();

        if (bInterfaceCreated)
        {
            RELEASE_SAFE(DirectInput8Interface);
        }

        return DirectInputGamepadsFound;
    }

    void Gamepad::Process()
    {
        for (int ind = 0; ind < INPUT_GAMEPAD_MAX_GAMEPADS; ind++)
            if (ind < DirectInputGamepadsFound && g_InputGamepad[ind] && g_InputGamepad[ind]->m_DirectInputDevice)
                g_InputGamepad[ind]->ProcessController();
    }

    BOOL CALLBACK Gamepad::EnumControllersCallback(LPCDIDEVICEINSTANCE devInst, Gamepad** pvRef)
    {
        if (*GamepadsArray != *pvRef)
        {
            GamepadsArray++;
            return DIENUM_CONTINUE;
        }

        pvRef[1]->m_GUID.Data1 = devInst->guidInstance.Data1;
        pvRef[1]->m_GUID.Data2 = devInst->guidInstance.Data2;
        *(int*)(pvRef[1]->m_GUID.Data4) = *(int*)(devInst->guidInstance.Data4);
        pvRef[1]->m_GUID.Data4[4] = pvRef[1]->m_GUID.Data4[4];

        pvRef[1]->m_ModelName = devInst->tszProductName;
        DirectInput8Interface->CreateDevice(devInst->guidInstance, &pvRef[1]->m_DirectInputDevice, NULL);

        if (FAILED(pvRef[1]->m_DirectInputDevice->SetDataFormat(&c_dfDIJoystick)))
        {
            MessageBox(g_Window->m_WindowHandle, "Unable to set game controller data format", "Error", MB_OK);

            if (!pvRef[1]->m_DirectInputDevice)
                return DIENUM_STOP;
            RELEASE_SAFE(pvRef[1]->m_DirectInputDevice);

            return DIENUM_STOP;
        }

        if (Script::ForceFeedback && FAILED(pvRef[1]->m_DirectInputDevice->SetCooperativeLevel(g_Window->m_WindowHandle, DISCL_EXCLUSIVE | DISCL_BACKGROUND)))
        {
            MessageBox(g_Window->m_WindowHandle, "Unable to set game controller cooperative level", "Error", MB_OK);

            if (!pvRef[1]->m_DirectInputDevice)
                return DIENUM_STOP;
            RELEASE_SAFE(pvRef[1]->m_DirectInputDevice);

            return DIENUM_STOP;
        }

        GamepadsArray++;

        return DIENUM_STOP;
    }

    String* Gamepad::AllocateGamepadNameStringBuffer(String* outString, char* inGamepadName, signed int length)
    {
        *outString = inGamepadName;

        return outString;
    }

    Window* Gamepad::GetWindow()
    {
        return g_Window;
    }

    void Gamepad::_Acquire()
    {
        if (m_DirectInputDevice)
            m_DirectInputDevice->Acquire();
    }

    void Gamepad::_Unacquire()
    {
        if (m_DirectInputDevice)
            m_DirectInputDevice->Unacquire();
    }

    Vector4f* Gamepad::GetAxisPosition(Vector4f* outPos, int axisNumber) const
    {
        if (!m_DirectInputDevice)
        {
            outPos->x = outPos->y = outPos->z = outPos->a = 0.0f;
            return outPos;
        }

        if (field_B4 != 1 &&
            field_B4 != 2)
        {
            if (!axisNumber)
            {
                *outPos = m_AxisPosition;
                return outPos;
            }

            outPos->x = outPos->y = outPos->z = outPos->a = 0.0f;
            return outPos;
        }

        if (axisNumber)
        {
            if (axisNumber != 1)
            {
                outPos->x = outPos->y = outPos->z = outPos->a = 0.0f;
                return outPos;
            }

            outPos->y = m_AxisRotation.z;
            outPos->x = m_AxisPosition.z;
        }
        else {
            outPos->y = m_AxisPosition.y;
            outPos->x = m_AxisPosition.x;
        }

        outPos->z = 0.0f;
        outPos->a = 0.0f;
        return outPos;
    }

    Orientation* Gamepad::GetAxisRotation(Orientation* outRot, int unk2)
    {
        if (!m_DirectInputDevice)
        {
            outRot->x = outRot->y = outRot->z = outRot->w = 0.0f;
            return outRot;
        }

        if (unk2)
        {
            outRot->x = outRot->y = outRot->z = outRot->w = 0.0f;
            return outRot;
        }

        *outRot = m_AxisRotation;
        return outRot;
    }

    void Gamepad::_439840()
    {
        for (int ind = 31; ind != 0; ind--)
            m_ButtonsStates[ind] &= 2;
    }

    int Gamepad::TranslateToActualGamepadButton(int unk1)
    {
        if (field_B4 != 1)
            return unk1;

        if (!m_SmartJoyGamepad)
            return GamepadNormalButtons[unk1];

        if (m_SmartJoyGamepad == 1)
            return SmartJoyButtons[unk1];

        return unk1;
    }

    bool Gamepad::KeyPressed(const unsigned int button)
    {
        if (!m_DirectInputDevice)
            return false;

        if (field_B4 != 1)
            return (m_ButtonsStates[button] & 2) && (m_ButtonsStates[button] & 1);

        if (!m_SmartJoyGamepad)
            return (m_ButtonsStates[GamepadNormalButtons[button]] & 2) && (m_ButtonsStates[GamepadNormalButtons[button]] & 1);

        if (m_SmartJoyGamepad != 1)
            return (m_ButtonsStates[button] & 2) && (m_ButtonsStates[button] & 1);

        return (m_ButtonsStates[SmartJoyButtons[button]] & 2) && (m_ButtonsStates[SmartJoyButtons[button]] & 1);
    }

    bool Gamepad::IsReleased(int button)
    {
        if (!m_DirectInputDevice)
            return false;

        if (field_B4 != 1)
            return (~m_ButtonsStates[button] & 2) && (m_ButtonsStates[button] & 1);

        if (!m_SmartJoyGamepad)
            return (~m_ButtonsStates[GamepadNormalButtons[button]] & 2) && (m_ButtonsStates[GamepadNormalButtons[button]] & 1);

        if (m_SmartJoyGamepad != 1)
            return (~m_ButtonsStates[button] & 2) && (m_ButtonsStates[button] & 1);

        return (~m_ButtonsStates[SmartJoyButtons[button]] & 2) && (m_ButtonsStates[SmartJoyButtons[button]] & 1);
    }

    char Gamepad::IsButtonPressed(int button)
    {
        if (!m_DirectInputDevice)
            return NULL;

        if (field_B4 != 1)
            return (m_ButtonsStates[button] >> 1) & 1;
        if (!m_SmartJoyGamepad)
            return (m_ButtonsStates[GamepadNormalButtons[button]] >> 1) & 1;
        if (m_SmartJoyGamepad != 1)
            return (m_ButtonsStates[button] >> 1) & 1;

        return (m_ButtonsStates[SmartJoyButtons[button]] >> 1) & 1;
    }

    double Gamepad::GetPressure(int button)
    {
        if (!IsButtonPressed(button))
            return 0.0;
        else
            return 1.0;
    }

    bool Gamepad::IsDInputDeviceFound()
    {
        return m_DirectInputDevice != nullptr;
    }

    bool Gamepad::IsControllerPresent(signed int controllerIndex)
    {
        if (controllerIndex < 0 ||
            controllerIndex >= 4)
            return false;

        if (controllerIndex < DirectInputGamepadsFound &&
            g_InputGamepad &&
            g_InputGamepad[controllerIndex] &&
            GetGameControllerByIndex(controllerIndex)->m_DirectInputDevice)
            return true;

        return false;
    }

    void Gamepad::SetControllerVibration(signed int controllerIndex, float force)
    {
        if (!m_ForceFeedbackAvailable)
        {
            if (controllerIndex)
            {
                if (controllerIndex == 1)
                    m_VibrationForce = force;
            }
            else
                m_VibrationForceDefault = force;

            return;
        }

        DIEFFECT dEffect;

        memset(&dEffect, NULL, sizeof(DIEFFECT));
        int _force = (int)(force * 10000.0f);

        dEffect.dwSize = sizeof(DIEFFECT);
        dEffect.cbTypeSpecificParams = 4;
        dEffect.lpvTypeSpecificParams = &_force;

        // NOTE: wtf?
        if (controllerIndex)
        {
            if (controllerIndex)
            {
                if (controllerIndex == 1)
                    m_VibrationForce = force;
            }
            else
                m_VibrationForceDefault = force;

            return;
        }

        if (!m_DirectInputEffect)
        {
            if (controllerIndex)
            {
                if (controllerIndex == 1)
                    m_VibrationForce = force;
            }
            else
                m_VibrationForceDefault = force;

            return;
        }

        if (force > 0.0f && !m_IsVibrating)
        {
            m_DirectInputEffect->Start(1, DIES_NODOWNLOAD);
            m_IsVibrating = true;
        }

        if (force == 0.0f)
        {
            if (!m_IsVibrating)
            {
                if (controllerIndex)
                {
                    if (controllerIndex == 1)
                        m_VibrationForce = force;
                }
                else
                    m_VibrationForceDefault = force;

                return;
            }

            m_DirectInputEffect->Stop();
            m_IsVibrating = false;
        }

        if (m_IsVibrating)
            m_DirectInputEffect->SetParameters(&dEffect, DIEP_TYPESPECIFICPARAMS);

        if (controllerIndex)
        {
            if (controllerIndex == 1)
                m_VibrationForce = force;
        }
        else
            m_VibrationForceDefault = force;
    }

    double Gamepad::GetControllerVibration(int unk1)
    {
        if (!unk1)
            return m_VibrationForceDefault;
        if (unk1 == 1)
            return m_VibrationForce;

        return 0.0;
    }

    void Gamepad::StartRumbleEffect()
    {
        LogDump::LogA("rumble, rumble, rumble...\n");

        if (m_DirectInputEffect)
            m_DirectInputEffect->Start(1, DIES_NODOWNLOAD);
    }

    const char* Gamepad::GetModelNameString() const
    {
        return m_ModelName.m_Str;
    }

    void Gamepad::ProcessController()
    {
        if (m_DirectInputDevice)
        {
            DIJOYSTATE joyState;
            memset(&joyState, NULL, sizeof(DIJOYSTATE));

            m_DirectInputDevice->Poll();
            HRESULT devstate = m_DirectInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState);

            if (devstate == DIERR_INPUTLOST || devstate == DIERR_NOTACQUIRED)
                return;
            if (m_DirectInputDevice)
                m_DirectInputDevice->Acquire();
            if (FAILED(m_DirectInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState)))
                return;

            // TODO: poll device buttons and axis rotations and fill in structures.
        }

        if (FAILED(DirectInput8Interface->CreateDevice(m_GUID, &m_DirectInputDevice, NULL)))
        {
            m_DirectInputDevice = nullptr;
            return;
        }

        if (FAILED(m_DirectInputDevice->SetDataFormat(&c_dfDIJoystick)))
        {
            MessageBox(g_Window->m_WindowHandle, "Unable to set game controller data format", "Error", MB_OK);
            RELEASE_SAFE(m_DirectInputDevice);
        }

        if (FAILED(m_DirectInputDevice->SetCooperativeLevel(g_Window->m_WindowHandle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
        {
            MessageBox(g_Window->m_WindowHandle, "Unable to set game controller cooperative level", "Error", MB_OK);
            RELEASE_SAFE(m_DirectInputDevice);
        }
    }
    const int Gamepad::TranslateToActualGamepadButton(const unsigned int buttonIndex) const
    {
        if (field_B4 != 1)
            return buttonIndex;

        if (!m_SmartJoyGamepad)
            return GamepadNormalButtons[buttonIndex];
        if (m_SmartJoyGamepad)
            return SmartJoyButtons[buttonIndex];

        return buttonIndex;
    }
}