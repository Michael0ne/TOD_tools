#pragma once
#include "Node.h"
#include <vector>

class Control;

class ControlSetup : public Node
{
    struct ControlInfo
    {
        String              m_Name;
        std::vector<Control*>  m_NodesList;
    };

protected:
    std::vector<ControlInfo>m_ControlsList;

private:
    inline ControlSetup() : Node(NODE_MASK_EMPTY)
    {
        MESSAGE_CLASS_CREATED(ControlSetup);
    };

    void                    GetControlId(int* args) const;  //  @7231D0
    const int               GetControlId_Impl(const char* const controlname) const; //  @9250E0

    void                    IsControlDown(int* args) const; //  @610000
    const bool              IsControlDown_Impl(const float controlid) const;  //  @925880
    float                   GetControlPressForce(const float controlid, float* pressure, float* realpressure, bool* pressed, bool* released) const;    //  @925210

    void                    GetKeyPressedString(char** args) const; //  @6D8200

    static const char* const    GetKeyPressedString_Impl();  //  @924ED0
    static bool             AnyControllerStickMoved(const int controllerindex); //  @925040
    static bool             AnyControllerButtonPressed(const int controllerindex);  //  @925000

    static ControlSetup* Create(AllocatorIndex); // @925810

public:
    static ControlSetup* CurrentController; // @A3E16C
    static bool&   WaitForController; // @A3E168
    static short*   WaitForControllerText; // @A3E164

    static void    Register(); // @925BA0
};

extern EntityType* tControlSetup; // @A3E15C

ASSERT_CLASS_SIZE(ControlSetup, 96);