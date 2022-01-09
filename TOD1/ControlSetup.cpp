#include "ControlSetup.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "InputGameController.h"
#include "Control.h"
#include "TextSlot.h"
#include "TextAsset.h"

EntityType* tControlSetup;

void ControlSetup::GetControlId(int* args) const
{
    *args = GetControlId_Impl((const char* const)args[1]);
}

const int ControlSetup::GetControlId_Impl(const char* const controlname) const
{
    if (!m_ControlsList.size())
        return -1;

    for (unsigned int i = 0; i < m_ControlsList.size(); ++i)
        if (strcmp(m_ControlsList[i].m_Name.m_Str, controlname) == NULL)
            return i;

    return NULL;
}

void ControlSetup::IsControlDown(int* args) const
{
    *args = IsControlDown_Impl((float)args[1]);
}

const bool ControlSetup::IsControlDown_Impl(const float controlid) const
{
    struct ControlState
    {
        bool    m_Released;
        bool    m_Pressed;
        float   m_Pressure;
        float   m_RealPressure;
    } controlstate;

    GetControlPressForce(controlid, &controlstate.m_Pressure, &controlstate.m_RealPressure, &controlstate.m_Released, &controlstate.m_Pressed);

    return controlstate.m_Pressure > 0;
}

#pragma message(TODO_IMPLEMENTATION)
float ControlSetup::GetControlPressForce(const float controlid, float* pressure, float* realpressure, bool* pressed, bool* released) const
{
    return 0.0f;
}

void ControlSetup::IsControlPressed(int* args) const
{
    *args = IsControlPressed_Impl((float)args[1]);
}

bool ControlSetup::IsControlPressed_Impl(float control) const
{
    float pressure, realpressure;
    bool pressed, released;

    GetControlPressForce(control, &pressure, &realpressure, &pressed, &released);

    return pressed;
}

void ControlSetup::IsControlReleased(int* args) const
{
    *args = IsControlReleased_Impl((float)args[1]);
}

bool ControlSetup::IsControlReleased_Impl(float control) const
{
    float pressure, realpressure;
    bool pressed, released;

    GetControlPressForce(control, &pressure, &realpressure, &pressed, &released);

    return released;
}

void ControlSetup::GetControlPressForce(float* args) const
{
    *args = GetControlPressure_Impl(args[1]);
}

const float ControlSetup::GetControlPressure_Impl(float control) const
{
    float pressure, realpressure;
    bool pressed, released;

    GetControlPressForce(control, &pressure, &realpressure, &pressed, &released);

    return pressure;
}

void ControlSetup::GetControlRealPressure(int* args) const
{
    *args = GetControlRealPressure_Impl((float)args[1]);
}

const float ControlSetup::GetControlRealPressure_Impl(const float control) const
{
    float pressure, realpressure;
    bool pressed, released;

    GetControlPressForce(control, &pressure, &realpressure, &pressed, &released);

    return realpressure;
}

void ControlSetup::IsControllerPresent(bool* args) const
{
    *args = Input::Gamepad::IsControllerPresent(Control::ActiveControllerIndex);
}

void ControlSetup::GetControlPos(int* args) const
{
    *args = GetControlPos_Impl(args[1]);
}

const int ControlSetup::GetControlPos_Impl(const int control) const
{
    if (control < 0 || control >= m_ControlsList.size())
        return NULL;

    int pos = NULL;
    for (unsigned int i = 0; i < m_ControlsList[control].m_NodesList.size(); ++i)
        pos = m_ControlsList[control].m_NodesList[i]->MousePositionRawToString();

    return pos;
}

void ControlSetup::GetControlDeltaPos(int* args) const
{
    *args = GetControlDeltaPos_Impl(args[1]);
}

const int ControlSetup::GetControlDeltaPos_Impl(const int control) const
{
    if (control < 0 || control >= m_ControlsList.size())
        return NULL;

    int pos = NULL;
    for (unsigned int i = 0; i < m_ControlsList[control].m_NodesList.size(); ++i)
        pos = m_ControlsList[control].m_NodesList[i]->MousePositionEngineToString();

    return pos;
}

void ControlSetup::SetVibration(int* args) const
{
    SetVibration_Impl(args[0], (float)args[1]);
}

void ControlSetup::SetVibration_Impl(const int controller, const float force) const
{
    if (Input::Gamepad::IsControllerPresent(Control::ActiveControllerIndex) &&
        !Control::_A3E170[Control::ActiveControllerIndex])
        Input::Gamepad::GetGameControllerByIndex(Control::ActiveControllerIndex)->SetControllerVibration(controller, force);
}

void ControlSetup::GetVibration(int* args) const
{
    *args = GetVibration_Impl(args[1]);
}

const float ControlSetup::GetVibration_Impl(const int controller) const
{
    if (!Input::Gamepad::IsControllerPresent(Control::ActiveControllerIndex) ||
        Control::_A3E170[Control::ActiveControllerIndex])
        return NULL;

    return (float)Input::Gamepad::GetGameControllerByIndex(Control::ActiveControllerIndex)->GetControllerVibration(controller);
}

void ControlSetup::AnyControllerButtonPressed(int* args) const
{
    *args = AnyControllerButtonPressed(Control::ActiveControllerIndex);
}

void ControlSetup::AnyControllerStickMoved(int* args) const
{
    *args = AnyControllerStickMoved(Control::ActiveControllerIndex);
}

void ControlSetup::GetKeyPressedString(char** args) const
{
    *args = (char*)GetKeyPressedString_Impl();
}

void ControlSetup::SetWaitForControllerText(int* args) const
{
    if (!args[0] || !((Node*)args[1])->m_ScriptEntity)
    {
        SetWaitForControllerText(nullptr, args[1]);
        return;
    }

    EntityType* argtextslot = ((Node*)args[0])->m_ScriptEntity;
    while (tTextSlot != argtextslot)
    {
        argtextslot = argtextslot->m_Parent;
        if (!argtextslot)
        {
            SetWaitForControllerText(nullptr, args[1]);
            return;
        }
    }

    SetWaitForControllerText((TextSlot*)args[0], args[1]);
}

const char* const ControlSetup::GetKeyPressedString_Impl()
{
    for (unsigned int i = 0; i < 255; ++i)
        if (g_InputKeyboard->m_Acquired && g_InputKeyboard->m_ButtonStates[i] & 1)
            return Input::Keyboard::KeyboardButtonToStr(i);

    for (unsigned int i = 0; i < 10; ++i)
        if (g_InputMouse->m_ShouldBeProcessed && g_InputMouse->m_MouseButtons[i] & 2)
            return Input::Mouse::MouseButtonToStr(i);

    return nullptr;
}

bool ControlSetup::AnyControllerStickMoved(const int controllerindex)
{
    bool movementdetected = false;
    Vector4f axisposition = {};

    for (unsigned int i = 0; i < 2; ++i)
    {
        Input::Gamepad::GetGameControllerByIndex(controllerindex)->GetAxisPosition(&axisposition, i);
        movementdetected =
            axisposition.x < -0.30000001f ||
            axisposition.x > -0.30000001f ||
            axisposition.y < -0.30000001f ||
            axisposition.y > -0.30000001f;
    }

    return movementdetected;
}

bool ControlSetup::AnyControllerButtonPressed(const int controllerindex)
{
    int i = 0;
    bool buttonpressed = false;
    static const int buttonsindexes[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, -1    //  NOTE: last element is only for clarity and does not exist in EXE.
    };

    do
    {
        if (buttonpressed)
            break;

        if (Input::Gamepad::GetGameControllerByIndex(controllerindex)->KeyPressed(buttonsindexes[i]))
            buttonpressed = true;
    } while (buttonsindexes[i++] != -1);

    return buttonpressed;
}

ControlSetup* ControlSetup::Create(AllocatorIndex)
{
    return new ControlSetup;
}

#pragma message(TODO_IMPLEMENTATION)
void ControlSetup::SetWaitForControllerText(TextSlot* textslot, const int textindex)
{
    static short* WaitForControllerText = new short[2048];
    if (!WaitForControllerText)
        TextAsset::CopyCharArrayToGameString(WaitForControllerText, "Please insert controller and press START");
}

#pragma message(TODO_IMPLEMENTATION)
void ControlSetup::Register()
{
    tControlSetup = new EntityType("ControlSetup");
    tControlSetup->InheritFrom(tNode);
    tControlSetup->SetCreator((EntityType::CREATOR)Create);

    tControlSetup->RegisterScript("getcontrolid(string):integer", &GetControlId, NULL, NULL, NULL, nullptr, nullptr);
    tControlSetup->RegisterScript("iscontroldown(integer):truth", &IsControlDown, NULL, NULL, NULL, nullptr, nullptr);
    tControlSetup->RegisterScript("iscontrolpressed(integer):truth", &IsControlPressed, NULL, NULL, NULL, nullptr, nullptr);
    tControlSetup->RegisterScript("iscontrolreleased(integer):truth", &IsControlReleased, NULL, NULL, NULL, nullptr, nullptr);
}