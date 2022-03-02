#pragma once
#include "Camera.h"

class FrameBuffer;

// NOTE: actual path to header file "/Editor/EditorCamera.h".
class EditorCamera : public Camera
{
    friend class Scene;
protected:
    char            field_D4;
    char            field_D5;
    char            field_D6;
    char            field_D7;
    float           field_D8;
    float           field_DC;
    bool            m_HasParentNode;
    Vector4f        m_ParentNodePosition;
    int             field_F4;
    int             field_F8;
    int             field_FC;
    int             field_100;
    int             field_104;
    char            field_108;
    Orientation     m_Orient_1;
    Orientation     m_Orient_2;
    int             field_12C;
    int             m_GamepadIndex;
    FrameBuffer*    m_FrameBuffer;

public:
    EditorCamera(); // @484B20
    virtual         ~EditorCamera();    //  @484E20
    virtual void    Update() override;  //  @4845C0

    static void     Register(); //  @484C60

private:
    void            AddToFov(const float val);  //  @87D9B0

    static EditorCamera*    Create(AllocatorIndex); //  @484DE0
};

extern EntityType* tEditorCamera; // @A3B55C

ASSERT_CLASS_SIZE(EditorCamera, 312);