#pragma once
#include "Node.h"

struct CameraMatrix
{
    Vector4f    m_Vec[6];

    CameraMatrix(); // @4087C0

    void        SetupMatrix(const Vector4f& pos, const Orientation& orient, const float fov, const float nearclip, const float farclip); // @406E60
};

// NOTE: actual path to header file "/Kernel/Scenegraph/Camera.h".
class Camera : public Node
{
public:
    CameraMatrix    m_CameraMatrix;
    float           m_Offset;
    float           m_Fov;
    float           m_NearClip;
    float           m_FarClip;
    float           m_DynlightCullRange;
    Vector4f        field_4C;
public:
    Camera(); // @87D8F0
    virtual void    Update() override;  //  @87E420

    void            GetMatrix(DirectX::XMMATRIX& outmat) const; // @87BC60

    float           GetOffset() const; // @91CE60
    void            SetOffset(const float offset); // @4843E0

    float           GetNearClip() const; // @87D810
    void            SetNearClip(const float nearclip); // @4B3FD0

    float           GetFarClip() const; // @87D850
    void            SetFarClip(const float farclip); // @87D820

    float           GetFov() const; // @4ADB40
    void            SetFov(const float fov); // @4ADB10

    float           GetDynlightCullRange() const; // @87D8C0
    void            SetDynlightCullRange(const float dynlightcullrange); // @87D8D0

    void            GetCameraPos(Vector3f* pos); // @87D860
    void            Project(float* params); // @87E4F0
    void            Project_Impl(Vector2f& projectedPos, const Vector4f& inpos) const;  //  @87DA10

    void            UpdateCameraMatrix();   //  @87DE40

    static Vector4f ActiveCameraPosition; // @A3D898

    static void  StoreActiveCameraPosition(); // @87E160

    static void  Register(); // @87E1E0
    static Camera* Create(AllocatorIndex); // @87E560
};

extern EntityType* tCamera; // @A3D80C

ASSERT_CLASS_SIZE(Camera, 212);