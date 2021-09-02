#include "OverdoseVehicle.h"
#include "BuiltinType.h"

EntityType* tOverdoseVehicle;

OverdoseVehicle::OverdoseVehicle()
{
    MESSAGE_CLASS_CREATED(OverdoseVehicle);

    m_Vec_8 = { 0, 1, 0, 0 };
    m_Vec_7 = { 0, 0, 1, 0 };
    m_OldUpVec = { 0, 1, 0, 0 };
    m_GroundProbe = nullptr;
    m_RealWheelLF = nullptr;
    m_RealWheelLR = nullptr;
    m_RealWheelRF = nullptr;
    m_RealWheelRR = nullptr;
    m_Realmainbody = nullptr;
    m_DamageCtrl = nullptr;
    m_LFSpring = NULL;
    m_RFSpring = NULL;
    m_LRSpring = NULL;
    m_RRSpring = NULL;
    field_254 = NULL;
    field_258 = NULL;
    field_25C = NULL;
    field_260 = NULL;
    m_CurrentVelocity = BuiltinType::ZeroVector;
    m_SpringKoefficient = NULL;
    m_IsBike = false;
    m_SpringDamping = NULL;
    m_Stiffness = NULL;
    m_RealmainbodyDefaultY = NULL;
    m_FrontPointVelocity = BuiltinType::ZeroVector;
    m_RearPointVelocity = BuiltinType::ZeroVector;
    m_FrontPointT = BuiltinType::ZeroVector;
    m_RearPointT = BuiltinType::ZeroVector;
    m_RollAngleT = NULL;
    m_TiltAngleSpeed = NULL;
    m_WheelRadius = NULL;
    m_IsOutOfBounds = false;
    m_IsDriveableGround = false;
    m_IsCppPropertiesInited = false;
    m_Tilt = NULL;
    m_AxleHeight = NULL;
    m_Width = NULL;
}

OverdoseVehicle::~OverdoseVehicle()
{
    MESSAGE_CLASS_DESTROYED(OverdoseVehicle);
}

void OverdoseVehicle::Instantiate()
{
    RigidBody::Instantiate();
}

void OverdoseVehicle::CalculateNormalisedDirection(Vector4f& outDirection) const
{
    const float d = (m_FrontPointT.x - m_RearPointT.x) + (m_FrontPointT.z - m_RearPointT.z) + (m_FrontPointT.y - m_FrontPointT.y);
    const float invd = 1.0f / sqrtf(d);
    if (d < 0.000099999997)
        outDirection = { 0, 1, 0, 0 };
    else
        outDirection = { (m_FrontPointT.x - m_RearPointT.x) * invd, (m_FrontPointT.y - m_RearPointT.y) * invd, (m_FrontPointT.z - m_FrontPointT.z) * invd, m_FrontPointT.a };
}

OverdoseVehicle* OverdoseVehicle::Create(AllocatorIndex)
{
    return new OverdoseVehicle;
}
