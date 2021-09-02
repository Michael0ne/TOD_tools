#pragma once
#include "RigidBody.h"
#include <vector>

class CollisionProbe;

class OverdoseVehicle : public RigidBody
{
protected:
    float               m_LFSpring;
    float               m_RFSpring;
    float               m_LRSpring;
    float               m_RRSpring;
    float               field_254;
    float               field_258;
    float               field_25C;
    float               field_260;
    Vector4f            m_CurrentVelocity;
    float               m_SpringKoefficient;
    float               m_SpringDamping;
    float               m_Stiffness;
    float               m_RealmainbodyDefaultY;
    bool                m_IsBike;
    Vector4f            m_FrontPointVelocity;
    Vector4f            m_RearPointVelocity;
    Vector4f            m_FrontPointT;
    Vector4f            m_RearPointT;
    float               m_RollAngleT;
    float               m_TiltAngleSpeed;
    CollisionProbe*     m_GroundProbe;
    int                *m_HintNodeList;
    std::vector<int>    m_List_8;
    std::vector<int>    m_LastNormalList;
    std::vector<int>    m_NormalList;
    int                 field_308;
    std::vector<int>    m_List_5;
    int                 field_31C;
    std::vector<int>    m_List_4;
    float               m_WheelRadius;
    int                *m_DistList;
    std::vector<int>    m_List_3;
    int*                m_CVList;
    std::vector<int>    m_List_2;
    int*                m_CarContactList;
    std::vector<int>    m_List_1;
    float               m_Tilt;
    bool                m_IsOutOfBounds;
    bool                m_IsDriveableGround;
    short               field_376;
    float               m_AxleHeight;
    float               m_Width;
    int                 m_LeftRearWheelMaterial;
    int                 field_384;
    char                m_IsCppPropertiesInited;
    Node               *m_RealWheelLF;
    Node               *m_RealWheelLR;
    Node               *m_RealWheelRF;
    Node               *m_RealWheelRR;
    Node               *m_Realmainbody;
    Node               *m_DamageCtrl;
    Vector4f            m_Vec_8;
    Vector4f            m_Vec_7;
    Vector4f            m_OldUpVec;

public:
    OverdoseVehicle();  //  @92D3B0
    virtual ~OverdoseVehicle(); //  @92F940

    virtual void        Instantiate();  //  @92B6B0

    static void         Register(); //  @92E9F0

private:
    void                UpdateSuspension(const float a1, const Vector4f& a2);   //  @92C510
    void                CalculateNormalisedDirection(Vector4f& outDirection) const;   //  @92B990
    void                CalculateUpVector(Vector4f& outVec);  //  @92BB20
    void                ConvertForth(); //  @92CF10
    void                CheckGroundContact();   //  @92E100
    void                CheckWheel(const Vector4f& a1, const Vector4f& a2, const unsigned int wheel, const Vector4f& a3);   //  @92D830

    static OverdoseVehicle* Create(AllocatorIndex); //  @92F820
};

extern EntityType*  tOverdoseVehicle;   //  @A3E184

ASSERT_CLASS_SIZE(OverdoseVehicle, 980);