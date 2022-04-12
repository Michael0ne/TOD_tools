#pragma once
#include "Node.h"

class RigidBody : public Node
{
protected:
    float               m_CollMass;
    D3DMATRIX           m_Matrix;
    float               field_94;
    float               field_98;
    float               field_9C;
    float               field_A0;
    float               field_A4;
    float               field_A8;
    float               field_AC;
    float               field_B0;
    float               field_B4;
    float               field_B8;
    float               field_BC;
    float               field_C0;
    float               field_C4;
    float               field_C8;
    float               field_CC;
    float               field_D0;
    float               m_Elasticity;
    int                 field_D8;
    Vector4f            m_UnkVec_1;
    Vector4f            m_Position_t;
    Orientation         m_Orientation_t;
    Vector4f            m_LinearSpeed;
    Vector4f            m_AngularMomentum;
    Vector4f            m_UnkVec_7;
    Orientation         m_UnkVec_6;
    int                 field_14C;
    int                 field_150;
    int                 field_154;
    int                 field_158;
    int                 field_15C;
    int                 field_160;
    int                 field_164;
    int                 field_168;
    Vector4f            m_UnkVec_8;
    Vector4f            m_UnkVec_9;
    Vector4f            m_UnkVec_10;
    int                 field_19C;
    int                 field_1A0;
    int                 field_1A4;
    int                 field_1A8;
    int                 field_1AC;
    int                 field_1B0;
    int                 field_1B4;
    int                 field_1B8;
    int                 field_1BC;
    int                 field_1C0;
    int                 field_1C4;
    int                 field_1C8;
    int                 field_1CC;
    int                 field_1D0;
    int                 field_1D4;
    int                 field_1D8;
    int                 field_1DC;
    int                 field_1E0;
    int                 field_1E4;
    int                 field_1E8;
    Vector4f            m_LinearVelocity;
    Vector4f            m_CollAngularSpeed;
    float               field_20C;
    float               field_210;
    float               field_214;
    int                 field_218;
    float               field_21C;
    float               field_220;
    float               field_224;
    int                 field_228;
    char                field_22C;
    char                field_22D;
    short               field_22E;
    int                 m_Unknown_1[4];
    bool                m_Grounded;

public:
    RigidBody(); // @9376E0
    virtual void Instantiate(); //  @931900

    const float         GetCollMass() const;    //  @89A880
    void                SetCollMass(const float mass);  //  @4CEF70
    const float         GetElasticity() const;  //  @931700
    void                SetElasticity(const float elasticity);  //  @4CF080
    const bool          IsGrounded() const; //  @931680
    void                SetIsGrounded(const bool grounded);    //  @4CEF90
    void                GetAngularMomentum(Vector4f& momentum) const;   //  @931690
    void                SetAngularMomentum(const Vector4f& momentum);   //  @9316C0
    const Vector4f      GetLinearVelocity() const; //  @4CEEF0
    void                SetLinearVelocity(Vector4f& speed);  //  @500C20
    void                GetAngularVelocity(Vector4f& speed) const;    //  @932530
    void                SetAngularVelocity(const Vector4f& pseed);  //  @932560
    void                GetOrientationT(Orientation& orient) const; //  @4CEFA0
    void                SetOrientationT(const Orientation& orient); //  @4CEFD0
    void                GetPositionT(Vector4f& pos) const;  //  @4CF010
    void                SetPositionT(const Vector4f& pos);  //  @4CF040

    void                TestAndResolveCollision(int* args);  //  @938DF0
    void                GetVelocityAtLocalPos(int* args);   //  @938CB0
    void                GetVelocityAtGlobalPos(int* args);  //  @938CB0
    void                Reset(int* args);   //  @883EC0
    void                SetBoxInertialMatrix(int* args); // @9332C0
    void                SetEllipsoidInertialMatrix(int* args);  //  @938D40

    static void         Register(); //  @9388C0

    static int          CommandDoCollisionGameLogic;    //  @A3E19C
    static int          CommandGetContactAction;    //  @A3E1A0

private:
    void                SetEllipsoidInertialMatrix_Impl(const Vector4f& pos);    //  @9335D0
    void                SetBoxInertialMatrix_Impl(const Vector4f& pos); //  @9332C0

    static RigidBody*   Create(AllocatorIndex); //  @938D80
};

extern EntityType* tRigidBody;  //  @A3E198

ASSERT_CLASS_SIZE(RigidBody, 580);