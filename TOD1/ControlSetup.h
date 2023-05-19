#pragma once
#include "Node.h"
#include <vector>

class Control;
class TextSlot;

class ControlSetup : public Node
{
    struct ControlInfo
    {
        String              m_Name;
        std::vector<Control*>  m_NodesList;

        inline ControlInfo(const char* const name)
        {
            MESSAGE_CLASS_CREATED(ControlInfo);

            m_Name = name;
        }

        ~ControlInfo(); //  @925FB0
    };

protected:
    std::vector<ControlInfo>m_ControlsList;

private:
    inline ControlSetup() : Node(NODE_MASK_EMPTY)
    {
        MESSAGE_CLASS_CREATED(ControlSetup);
    };

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    void                    GetControlId(int* args) const;  //  @7231D0
    const int               GetControlId_Impl(const char* const controlname) const; //  @9250E0

    void                    IsControlDown(int* args) const; //  @610000
    const bool              IsControlDown_Impl(const float controlid) const;  //  @925880
    void                    GetControlPressForce_Impl(const float controlid, float* pressure, float* realpressure, bool* pressed, bool* released) const;    //  @925210
    void                    IsControlPressed(int* args) const;  //  @7231F0
    bool                    IsControlPressed_Impl(float control) const; //  @925370
    void                    IsControlReleased(int* args) const; //  @73D9E0
    bool                    IsControlReleased_Impl(float control) const;    //  @9253C0
    void                    GetControlPressForce(float* args) const;  //  @73DA00
    const float             GetControlPressure_Impl(float control) const;   //  @925410
    void                    GetControlRealPressure(int* args) const;    //  @73DA20
    const float             GetControlRealPressure_Impl(const float control) const; //  @925460
    void                    IsControllerPresent(bool* args) const;   //  @925F50
    void                    GetControlPos(int* args) const; //  @6AE470
    const int               GetControlPos_Impl(const int control) const;  //  @925170
    void                    GetControlDeltaPos(int* args) const;    //  @73DA40
    const int               GetControlDeltaPos_Impl(const int control) const;   //  @9251C0
    void                    SetVibration(int* args) const;  //  @755250
    void                    SetVibration_Impl(const int controller, const float force) const;   //  @924F40
    void                    GetVibration(int* args) const;  //  @755270
    const float             GetVibration_Impl(const int controller) const;  //  @924F80
    void                    AnyControllerButtonPressed(int* args) const;    //  @925F70
    void                    AnyControllerStickMoved(int* args) const;   //  @925F90
    void                    GetKeyPressedString(char** args) const; //  @6D8200
    void                    SetWaitForControllerText(int* args) const;  //  @9254B0
    void                    SetEnableWaitForController(const int* args);    //  @924FE0
    void                    SetCurrentController(const int* args);  //  @924EC0
    void                    Start(const int* args); //  @926280
    void                    Start_Impl(Node* node); //  @925A60

    static const char* const    GetKeyPressedString_Impl();  //  @924ED0
    static bool             AnyControllerStickMoved_Impl(const int controllerindex); //  @925040
    static bool             AnyControllerButtonPressed_Impl(const int controllerindex);  //  @925000

    static ControlSetup*    Create(AllocatorIndex); // @925810

private:
    static void             SetWaitForControllerText_Impl(TextSlot* textslot, const int textindex);  //  @9252F0

public:
    static ControlSetup*    CurrentController; // @A3E16C
    static bool             WaitForController; // @A3E168
    static short*           WaitForControllerText; // @A3E164

    static void             Register(); // @925BA0
};

extern EntityType* tControlSetup; // @A3E15C

ASSERT_CLASS_SIZE(ControlSetup, 96);