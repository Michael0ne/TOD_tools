#include "ControlSetup.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "InputGameController.h"
#include "Control.h"
#include "TextSlot.h"
#include "TextAsset.h"

EntityType* tControlSetup;

bool ControlSetup::ControllersUsedByEditor[8];

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

    GetControlPressForce_Impl(controlid, &controlstate.m_Pressure, &controlstate.m_RealPressure, &controlstate.m_Released, &controlstate.m_Pressed);

    return controlstate.m_Pressure > 0;
}

float ControlSetup::GetControlPressForce_Impl(const float controlid, float* pressure, float* realpressure, bool* pressed, bool* released) const
{
    *pressure = 0;
    *realpressure = 0;
    *pressed = false;
    *released = false;

    if (controlid < 0 || controlid < m_ControlsList.size())
        return;

    if (m_ControlsList.begin() + controlid == m_ControlsList.end())
        return;

    auto& controlInfo = m_ControlsList[controlid].m_NodesList;
    for (size_t i = 0; i < controlInfo.size(); ++i)
    {
        float controlPressure = 0, controlRealPressure = 0;
        bool controlPressed = false, controlReleased = false;
        controlInfo[i]->GetStateInfo(&controlPressure, &controlRealPressure, &controlPressed, &controlReleased);
        if (controlPressure > *pressure)
            *pressure = controlPressure;
        if (controlRealPressure > *realpressure)
            *realpressure = controlRealPressure;

        *pressed |= controlPressed;
        *released |= controlReleased;
    }
}

void ControlSetup::IsControlPressed(int* args) const
{
    *args = IsControlPressed_Impl((float)args[1]);
}

bool ControlSetup::IsControlPressed_Impl(float control) const
{
    float pressure, realpressure;
    bool pressed, released;

    GetControlPressForce_Impl(control, &pressure, &realpressure, &pressed, &released);

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

    GetControlPressForce_Impl(control, &pressure, &realpressure, &pressed, &released);

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

    GetControlPressForce_Impl(control, &pressure, &realpressure, &pressed, &released);

    return pressure;
}

void ControlSetup::GetControlRealPressure(int* args) const
{
    *args = (int)GetControlRealPressure_Impl((float)args[1]);
}

const float ControlSetup::GetControlRealPressure_Impl(const float control) const
{
    float pressure, realpressure;
    bool pressed, released;

    GetControlPressForce_Impl(control, &pressure, &realpressure, &pressed, &released);

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
    *args = (int)GetVibration_Impl(args[1]);
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
    *args = AnyControllerButtonPressed_Impl(Control::ActiveControllerIndex);
}

void ControlSetup::AnyControllerStickMoved(int* args) const
{
    *args = AnyControllerStickMoved_Impl(Control::ActiveControllerIndex);
}

void ControlSetup::GetKeyPressedString(char** args) const
{
    *args = (char*)GetKeyPressedString_Impl();
}

void ControlSetup::SetWaitForControllerText(int* args) const
{
    if (!args[0] || !((Node*)args[1])->m_ScriptEntity)
    {
        SetWaitForControllerText_Impl(nullptr, args[1]);
        return;
    }

    EntityType* argtextslot = ((Node*)args[0])->m_ScriptEntity;
    while (tTextSlot != argtextslot)
    {
        argtextslot = argtextslot->m_Parent;
        if (!argtextslot)
        {
            SetWaitForControllerText_Impl(nullptr, args[1]);
            return;
        }
    }

    SetWaitForControllerText_Impl((TextSlot*)args[0], args[1]);
}

void ControlSetup::SetEnableWaitForController(const int* args)
{
    WaitForController = args[0] != false;
}

void ControlSetup::SetCurrentController(const int* args)
{
    CurrentController = (ControlSetup*)args[0];
}

void ControlSetup::Start(const int* args)
{
    m_ControlsList.clear();
    Start_Impl(this);
}

void ControlSetup::Start_Impl(Node* node)
{
    if (node)
    {
        Node* controlNode = nullptr;
        EntityType* script = node->m_ScriptEntity;
        if (script)
        {
            while (tControl != script)
            {
                script = script->m_Parent;
                if (!script)
                    break;
            }

            if (!script)
                controlNode = nullptr;
            else
                controlNode = node;
        }
        else
        {
            controlNode = nullptr;
        }

        if (controlNode)
        {
            int controlId = GetControlId_Impl(controlNode->m_Name);
            if (controlId < 0)
            {
                m_ControlsList.push_back(ControlInfo(controlNode->m_Name));
                controlId = m_ControlsList.size() - 1;
            }

            m_ControlsList[controlId].m_NodesList.push_back((Control*)controlNode);
        }
    }

    for (Node* child = node->m_FirstChild; child; child = child->m_NextSibling)
        Start_Impl(child);
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

bool ControlSetup::AnyControllerStickMoved_Impl(const int controllerindex)
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

bool ControlSetup::AnyControllerButtonPressed_Impl(const int controllerindex)
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

void ControlSetup::SetWaitForControllerText_Impl(TextSlot* textslot, const int textindex)
{
    static unsigned short* WaitForControllerText = new unsigned short[2048];
    if (!WaitForControllerText)
        TextAsset::CopyCharArrayToGameString((short*)WaitForControllerText, "Please insert controller and press START");

    int textIndexActual = textindex < 0 ? 0 : textindex;
    auto& texIndiciesList = ((TextAsset*)textslot->m_TextAsset.m_AssetPtr)->m_TextIndicies;
    if (textslot->m_TextAsset)
        textIndexActual = textIndexActual >= texIndiciesList.size() ? texIndiciesList.size() - 1 : textIndexActual;

    WaitForControllerText[0] = NULL;

    if (textslot->m_TextAsset)
    {
        if (textIndexActual < texIndiciesList.size())
            ((TextAsset*)textslot->m_TextAsset.m_AssetPtr)->GetGameString(texIndiciesList[textIndexActual], WaitForControllerText, 255, true);
        else
            WaitForControllerText[0] = NULL;
    }
}

void ControlSetup::Register()
{
    tControlSetup = new EntityType("ControlSetup");
    tControlSetup->InheritFrom(tNode);
    tControlSetup->SetCreator((CREATOR)Create);

    tControlSetup->RegisterScript("getcontrolid(string):integer", (EntityFunctionMember)&GetControlId);
    tControlSetup->RegisterScript("iscontroldown(integer):truth", (EntityFunctionMember)&IsControlDown);
    tControlSetup->RegisterScript("iscontrolpressed(integer):truth", (EntityFunctionMember)&IsControlPressed);
    tControlSetup->RegisterScript("iscontrolreleased(integer):truth", (EntityFunctionMember)&IsControlReleased);
    tControlSetup->RegisterScript("getcontrolpressure(integer):number", (EntityFunctionMember)&GetControlPressForce);
    tControlSetup->RegisterScript("getcontrolrealpressure(integer):number", (EntityFunctionMember)&GetControlRealPressure);
    tControlSetup->RegisterScript("iscontrollerpresent:truth", (EntityFunctionMember)&IsControllerPresent);
    tControlSetup->RegisterScript("getcontrolpos(integer):integer", (EntityFunctionMember)&GetControlPos);
    tControlSetup->RegisterScript("getcontroldeltapos(integer):integer", (EntityFunctionMember)&GetControlDeltaPos);
    tControlSetup->RegisterScript("setvibration(integer,number)", (EntityFunctionMember)&SetVibration);
    tControlSetup->RegisterScript("getvibration(integer):number", (EntityFunctionMember)&GetVibration);
    tControlSetup->RegisterScript("AnyControllerButtonPressed:truth", (EntityFunctionMember)&AnyControllerButtonPressed);
    tControlSetup->RegisterScript("AnyControllerStickMoved:truth", (EntityFunctionMember)&AnyControllerStickMoved);
    tControlSetup->RegisterScript("GetKeyPressedString:string", (EntityFunctionMember)&GetKeyPressedString);
    tControlSetup->RegisterScript("SetWaitForControllerText(entity,integer)", (EntityFunctionMember)&SetWaitForControllerText);
    tControlSetup->RegisterScript("SetEnableWaitForController(truth)", (EntityFunctionMember)&SetEnableWaitForController);
    tControlSetup->RegisterScript("SetCurrentController", (EntityFunctionMember)&SetCurrentController);
    tControlSetup->RegisterScript("start", (EntityFunctionMember)&Start);

    tControlSetup->PropagateProperties();
}

ControlSetup::ControlInfo::~ControlInfo()
{
    MESSAGE_CLASS_DESTROYED(ControlInfo);
}