#pragma once
#include "RigidBody.h"
#include <vector>

class CollisionProbe;

struct DoubleVector4f
{
    Vector4f    Vec1;
    Vector4f    Vec2;
};

class OverdoseVehicle : public RigidBody
{
protected:
    float               m_LFSpring;
    float               m_RFSpring;
    float               m_LRSpring;
    float               m_RRSpring;
    float               m_KoefAdjustedLFSpring;
    float               m_KoefAdjustedRFSpring;
    float               m_KoefAdjustedLRSpring;
    float               m_KoefAdjustedRRSpring;
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
    std::vector<Node*>    m_HintNodeList;
    std::vector<DoubleVector4f>    m_LastNormalList;
    std::vector<Vector4f>    m_NormalList;
    std::vector<Vector3f>    m_CachedLastNormalList;
    std::vector<Vector3f>    m_CachedNormalList;
    float               m_WheelRadius;
    std::vector<float>  m_DistList;
    std::vector<int>    m_CVList;
    std::vector<bool>   m_CarContactList;
    float               m_Tilt;
    bool                m_IsOutOfBounds;
    bool                m_IsDriveableGround;
    short               field_376;
    float               m_AxleHeight;
    float               m_Width;
    int                 m_RearWheelsMaterialID;
    int                 m_GroundMaterialID;
    char                m_IsCppPropertiesInited;
    Node               *m_RealWheelLF;
    Node               *m_RealWheelLR;
    Node               *m_RealWheelRF;
    Node               *m_RealWheelRR;
    Node               *m_Realmainbody;
    Node               *m_DamageCtrl;
    Vector4f            m_Vec_8;
    Vector4f            m_Vec_7;
    Vector4f            m_LocalDirectionVector;

public:
    OverdoseVehicle();  //  @92D3B0
    virtual ~OverdoseVehicle(); //  @92F940

    virtual void        Instantiate();  //  @92B6B0

    void                GetCurrentVelocity(Vector4f& outVelocity) const;    //  @4C8AC0
    void                SetCurrentVelocity(const Vector4f& velocity);   //  @4CF0C0
    const float         GetSpringCoefficient() const;   //  @92B610
    void                SetSpringCoefficient(const float koef); //  @4CF0F0
    const float         GetSpringDamping() const;   //  @92B630
    void                SetSpringDamping(const float damping);  //  @4CF120
    const float         GetStiffness() const;   //  @4CF150
    void                SetStiffness(const float stiffness);    //  @4CF160
    const float         GetRealmainbodyDefaultY() const;    //  @4CF1D0
    void                SetRealmainbodyDefaultY(const float defy);  //  @4CF1E0
    const float         GetLFSpring() const;    //  @4CF210
    void                SetLFSpring(const float lf);    //  @4CF220
    const float         GetRFSpring() const;    //  @4CF240
    void                SetRFSpring(const float rf);    //  @4CF250
    const float         GetRRSpring() const;    //  @4CF270
    void                SetRRSpring(const float rr);    //  @4CF280
    const float         GetLRSpring() const;    //  @4CF2A0
    void                SetLRSpring(const float lr);    //  @4CF2B0
    const int           GetLeftRearWheelMaterial() const;   //  @826170
    void                SetLeftRearWheelMaterial(const int material);   //  @92B620
    const int           GetRightRearWheelMaterial() const;  //  @826170
    void                SetRightRearWheelMaterial(const int material);  //  @92B620
    void                GetFrontPointVelocity(Vector4f& outVelocity) const;   //  @4CF390
    void                SetFrontPointVelocity(const Vector4f& velocity);    //  @4CF3C0
    void                GetRearPointVelocity(Vector4f& outVelocity) const;    //  @4CF400
    void                SetRearPointVelocity(const Vector4f& velocity); //  @4CF430
    const bool          IsBike() const; //  @4CF550
    void                SetIsBike(const bool isbike);   //  @4CF560
    Node*               GetRealWheelLF() const; //  @543BC0
    void                SetRealWheelLF(Node* wheel);    //  @4CF570
    Node*               GetRealWheelRF() const; //  @4CF580
    void                SetRealWheelRF(Node* wheel);    //  @4CF590
    Node*               GetRealWheelRR() const; //  @4CF5A0
    void                SetRealWheelRR(Node* wheel);    //  @4CF5B0
    Node*               GetRealWheelLR() const; //  @92B640
    void                SetRealWheelLR(Node* wheel);    //  @4CF5C0
    Node*               GetRealmainbody() const;    //  @4CF5D0
    void                SetRealmainbody(Node* realmainbody);    //  @4CF5E0
    Node*               GetDamageCtrl() const;  //  @92B650
    void                SetDamageCtrl(Node* damagectrl);    //  @4CF5F0
    void*               GetCarContactList() const;    //  @4CF6C0
    void                SetCarContactList(const std::vector<bool>& carcontactlist); //  @92F800
    void*               GetCVList() const;  //  @4CF690
    void                SetCVList(const std::vector<int>& cvlist);  //  @92F7F0
    void*               GetDistList() const;    //  @4CF680
    void                SetDistList(const std::vector<float>& distlist);    //  @92F7E0
    void*               GetNormalList();  //  @500EC0
    void                SetNormalList(const std::vector<Vector3f>& normallist); //  @500D50
    void*               GetLastNormalList();    //  @500DF0
    void                SetLastNormalList(const std::vector<Vector3f>& lastnormallist); //  @500F90
    void*               GetHintnodeList() const;    //  @4CF6D0
    void                SetHintnodeList(const std::vector<Node*> hintnodelist);  //  @92F810
    Node*               GetGroundProbe() const; //  @4CF620
    void                SetGroundProbe(Node* node); //  @4CF630
    const float         GetWheelRadius() const; //  @4CF640
    void                SetWheelRadius(const float radius); //  @4CF650
    const float         GetTilt() const;    //  @92B680
    void                SetTilt(const float tilt);  //  @92B690
    const bool          IsOutOfBounds() const;  //  @4CF600
    void                SetIsOutOfBounds(const bool oob);   //  @4CF610
    const bool          IsDriveableGround() const;  //  @92B660
    void                SetIsDriveableGround(const bool dg);    //  @92B670
    void                GetFrontPointT(Vector4f& outPoint) const;   //  @4CF470
    void                SetFrontPointT(const Vector4f& pointT); //  @4CF4A0
    void                GetRearPointT(Vector4f& outPoint) const;    //  @4CF4E0
    void                SetRearPointT(const Vector4f& pointT);  //  @4CF510
    const float         GetRollAngleT() const;  //  @4CF190
    void                SetRollAngleT(const float angle);   //  @4CF1A0
    const float         GetTiltAngleSpeed() const;  //  @4CF350
    void                SetTiltAngleSpeed(const float tiltanglespeed);  //  @4CF360
    const float         GetAxleHeight() const;  //  @4CF310
    void                SetAxleHeight(const float axh); //  @4CF320
    const float         GetWidth() const;   //  @4CF2D0
    void                SetWidth(const float width);    //  @4CF2E0
    const bool          CppPropertiesInited() const;    //  @4CF6A0
    void                SetCppPropertiesInited(const bool inited);  //  @4CF6B0

    static void         Register(); //  @92E9F0

private:
    void                UpdateSuspension(const float a1, const Vector4f& a2);   //  @92C510
    void                CalculateNormalisedDirection(Vector4f& outDirection) const;   //  @92B990
    void                CalculateUpVector(Vector4f& outVec);  //  @92BB20
    void                ConvertForth(); //  @92CF10
    void                CheckGroundContact();   //  @92E100
    void                CheckWheel_Impl(const Vector4f& a1, const Vector4f& a2, const unsigned int wheel, const Vector4f& a3);   //  @92D830
    void                StoreOldUpVector(int* args);    //  @92F720
    void                CalcNormalizedDir(int* args);   //  @92F6C0
    void                CheckWheel(int* args);  //  @92F860

    static OverdoseVehicle* Create(AllocatorIndex); //  @92F820

    static int          UpdateAccelerationCommand;  //  @A3E188
    static int          IsPlayerCarCommand; //  @A3E18C
    static DataType*    VectorListType; //  @A3E190
};

extern EntityType*  tOverdoseVehicle;   //  @A3E184

//  TODO: uncomment, once vectors's sizes are consistent.
//ASSERT_CLASS_SIZE(OverdoseVehicle, 980);
ASSERT_CLASS_SIZE(OverdoseVehicle, 964);