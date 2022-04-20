#pragma once
#include "Node.h"

class Control : public Node
{
    friend class ControlSetup;

    enum ControlType
    {
        NONE = 0,
        KEYBOARD = 1,
        MOUSE = 2,
        MOUSE_BUTTON = 3,
        GAMEPAD = 4,
        DPAD = 5
    };

    enum DPadKey
    {
        A_LEFT = 0,
        A_RIGHT,
        A_UP,
        A_DOWN,
        C_LEFT,
        C_RIGHT,
        C_UP,
        C_DOWN
    };

protected:
    int    m_Key;
    char   m_IsNormalizedPressure;
    ControlType  m_ControlType;

public:
    inline Control() : Node(NODE_MASK_EMPTY) // NOTE: no constructor.
    {
        MESSAGE_CLASS_CREATED(Control);

        m_Key = -1;
        m_IsNormalizedPressure = 0;
        m_ControlType = KEYBOARD;
    }

    static void Register(); // @924A30

    static unsigned int     ActiveControllerIndex;  //  @A3E160
    static bool             ControllersUsedByEditor[8]; //  @A3E170 //  NOTE: index - controller id. If true, then controller with this index is being used with the editor.

protected:
    const int   MousePositionEngineToString() const; // @924600
    const int   MousePositionRawToString() const; // @9245A0
    void        GetStateInfo(float* pressure, float* realpressure, bool* pressed, bool* released);  //  @924B40

private:
    const int  GetKey() const; // @8F92C0
    void   SetKey(const int keyind); // @924590

    const char* GetKeyStr() const; // @9249C0
    void   SetKeyStr(const char* args); // @9247B0

    const char* DPadKeyToStr(const unsigned int keyid) const; // @924730
    float       GetDPadPressForce(const DPadKey key) const; //  @924630

    static Control* Create(AllocatorIndex); // @924E60
};

extern EntityType* tControl; // @A3E158

ASSERT_CLASS_SIZE(Control, 92);