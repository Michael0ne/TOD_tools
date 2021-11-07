#include "ControlSetup.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "InputGameController.h"

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
    *args = IsControlDown_Impl(args[1]);
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

void ControlSetup::GetKeyPressedString(char** args) const
{
    *args = (char*)GetKeyPressedString_Impl();
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
    }while (i++ != -1);

    return buttonpressed;
}

ControlSetup* ControlSetup::Create(AllocatorIndex)
{
    return new ControlSetup;
}

#pragma message(TODO_IMPLEMENTATION)
void ControlSetup::Register()
{
    tControlSetup = new EntityType("ControlSetup");
    tControlSetup->InheritFrom(tNode);
    tControlSetup->SetCreator((EntityType::CREATOR)Create);

    tControlSetup->RegisterScript("getcontrolid(string):integer", &GetControlId, NULL, NULL, NULL, nullptr, nullptr);
    tControlSetup->RegisterScript("iscontroldown(integer):truth", &IsControlDown, NULL, NULL, NULL, nullptr, nullptr);
}