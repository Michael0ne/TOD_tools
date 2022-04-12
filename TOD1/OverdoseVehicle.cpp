#include "OverdoseVehicle.h"
#include "BuiltinType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "VectorType.h"

EntityType* tOverdoseVehicle;

int OverdoseVehicle::UpdateAccelerationCommand;
int OverdoseVehicle::IsPlayerCarCommand;

OverdoseVehicle::OverdoseVehicle()
{
    MESSAGE_CLASS_CREATED(OverdoseVehicle);

    m_Vec_8 = { 0, 1, 0, 0 };
    m_Vec_7 = { 0, 0, 1, 0 };
    m_LocalDirectionVector = { 0, 1, 0, 0 };
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
    m_KoefAdjustedLFSpring = NULL;
    m_KoefAdjustedRFSpring = NULL;
    m_KoefAdjustedLRSpring = NULL;
    m_KoefAdjustedRRSpring = NULL;
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

void OverdoseVehicle::Register()
{
    tOverdoseVehicle = new EntityType("OverdoseVehicle");
    tOverdoseVehicle->InheritFrom(tRigidBody);
    tOverdoseVehicle->SetCreator((CREATOR)Create);

    tOverdoseVehicle->RegisterProperty(tVECTOR, "current_velocity", (EntityGetterFunction)&GetCurrentVelocity, (EntitySetterFunction)&SetCurrentVelocity, nullptr);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "spring_k", (EntityGetterFunction)&GetSpringCoefficient, (EntitySetterFunction)&SetSpringCoefficient, nullptr, 26);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "spring_damping", (EntityGetterFunction)&GetSpringDamping, (EntitySetterFunction)&SetSpringDamping, nullptr, 25);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "stiffness", (EntityGetterFunction)&GetStiffness, (EntitySetterFunction)&SetStiffness, nullptr, 27);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "realmainbody_default_y", (EntityGetterFunction)&GetRealmainbodyDefaultY, (EntitySetterFunction)&SetRealmainbodyDefaultY, nullptr, 24);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "LF_spring", (EntityGetterFunction)&GetLFSpring, (EntitySetterFunction)&SetLFSpring, nullptr);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "RF_spring", (EntityGetterFunction)&GetRFSpring, (EntitySetterFunction)&SetRFSpring, nullptr);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "RR_spring", (EntityGetterFunction)&GetRRSpring, (EntitySetterFunction)&SetRRSpring, nullptr);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "LR_spring", (EntityGetterFunction)&GetLRSpring, (EntitySetterFunction)&SetLRSpring, nullptr);
    tOverdoseVehicle->RegisterProperty(tINTEGER, "LeftRearWheelMaterial", (EntityGetterFunction)&GetLeftRearWheelMaterial, (EntitySetterFunction)&SetLeftRearWheelMaterial, nullptr);
    tOverdoseVehicle->RegisterProperty(tINTEGER, "RightRearWheelMaterial", (EntityGetterFunction)&GetRightRearWheelMaterial, (EntitySetterFunction)&SetRightRearWheelMaterial, nullptr);
    tOverdoseVehicle->RegisterProperty(tVECTOR, "front_point_velocity", (EntityGetterFunction)&GetFrontPointVelocity, (EntitySetterFunction)&SetFrontPointVelocity, nullptr, 21);
    tOverdoseVehicle->RegisterProperty(tVECTOR, "rear_point_velocity", (EntityGetterFunction)&GetRearPointVelocity, (EntitySetterFunction)&SetRearPointVelocity, nullptr, 22);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "bike", (EntityGetterFunction)&GetIsBike, (EntitySetterFunction)&SetIsBike, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_LF", (EntityGetterFunction)&GetRealWheelLF, (EntitySetterFunction)&SetRealWheelLF, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_RF", (EntityGetterFunction)&GetRealWheelRF, (EntitySetterFunction)&SetRealWheelRF, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_RR", (EntityGetterFunction)&GetRealWheelRR, (EntitySetterFunction)&SetRealWheelRR, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_LR", (EntityGetterFunction)&GetRealWheelLR, (EntitySetterFunction)&SetRealWheelLR, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "realmainbody", (EntityGetterFunction)&GetRealmainbody, (EntitySetterFunction)&SetRealmainbody, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "damagectrl", (EntityGetterFunction)&GetDamageCtrl, (EntitySetterFunction)&SetDamageCtrl, nullptr);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(truth)"), "car_contact_list", (EntityGetterFunction)&GetCarContactList, (EntitySetterFunction)&SetCarContactList, nullptr);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(integer)"), "CV_List", (EntityGetterFunction)&GetCVList, (EntitySetterFunction)&SetCVList, nullptr);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(number)"), "dist_List", (EntityGetterFunction)&GetDistList, (EntitySetterFunction)&SetDistList, nullptr);
    static DataType* dtListVector = DataType::LoadScript("list(vector)");
    tOverdoseVehicle->RegisterProperty(dtListVector, "normal_List", (EntityGetterFunction)&GetNormalList, (EntitySetterFunction)&SetNormalList, nullptr);
    tOverdoseVehicle->RegisterProperty(dtListVector, "LastNormal_list", (EntityGetterFunction)&GetLastNormalList, (EntitySetterFunction)&SetLastNormalList, nullptr, 23);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(entity)"), "Hintnode_list", (EntityGetterFunction)&GetHintNodeList, (EntitySetterFunction)&SetHintNodeList, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "Groundprobe", (EntityGetterFunction)&GetGroundProbe, (EntitySetterFunction)&SetGroundProbe, nullptr);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "wheel_radius", (EntityGetterFunction)&GetWheelRadius, (EntitySetterFunction)&SetWheelRadius, nullptr, 29);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "tilt", (EntityGetterFunction)&GetTilt, (EntitySetterFunction)&SetTilt, nullptr);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "is_out_of_bounds", (EntityGetterFunction)&IsOutOfBounds, (EntitySetterFunction)&SetIsOutOfBounds, nullptr);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "driveable_ground", (EntityGetterFunction)&GetIsDriveableGround, (EntitySetterFunction)&SetIsDriveableGround, nullptr);
    tOverdoseVehicle->RegisterProperty(tVECTOR, "front_point_t", (EntityGetterFunction)&GetFrontPointT, (EntitySetterFunction)&SetFrontPointT, nullptr, 17);
    tOverdoseVehicle->RegisterProperty(tVECTOR, "rear_point_t", (EntityGetterFunction)&GetRearPointT, (EntitySetterFunction)&SetRearPointT, nullptr, 18);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "roll_angle_t", (EntityGetterFunction)&GetRollAngleT, (EntitySetterFunction)&SetRollAngleT, nullptr, 19);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "tilt_angle_speed", (EntityGetterFunction)&GetTiltAngleSpeed, (EntitySetterFunction)&SetTiltAngleSpeed, nullptr, 28);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "axle_height", (EntityGetterFunction)&GetAxleHeight, (EntitySetterFunction)&SetAxleHeight, nullptr, 20);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "width", (EntityGetterFunction)&GetWidth, (EntitySetterFunction)&SetWidth, nullptr, 30);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "cpppropertiesinited", (EntityGetterFunction)&IsCppPropertiesInited, (EntitySetterFunction)&SetCppPropertiesInited, nullptr);

    tOverdoseVehicle->RegisterScript("check_wheel(vector,vector,integer,vector)", (EntityFunctionMember)&CheckWheel);
    tOverdoseVehicle->RegisterScript("update_suspension(number,vector)", (EntityFunctionMember)&UpdateSuspension);
    tOverdoseVehicle->RegisterScript("calc_normalized_dir_vec:vector", (EntityFunctionMember)&CalcNormalizedDir);
    tOverdoseVehicle->RegisterScript("calc_up_vec:vector", (EntityFunctionMember)&CalculateUpVector);
    tOverdoseVehicle->RegisterScript("convert_forth", (EntityFunctionMember)&ConvertForth);
    tOverdoseVehicle->RegisterScript("check_ground_contact", (EntityFunctionMember)&CheckGroundContact);
    tOverdoseVehicle->RegisterScript("StoreOldUpVector", (EntityFunctionMember)&StoreOldUpVector);

    UpdateAccelerationCommand = RegisterGlobalCommand("update_acceleration(vector,number)", true);
    IsPlayerCarCommand = RegisterGlobalCommand("command_IsPlayer_car:truth", true);

    tOverdoseVehicle->PropagateProperties();
}

void OverdoseVehicle::CalculateNormalisedDirection(Vector4f& outDirection) const
{
    const float d = (m_FrontPointT.x - m_RearPointT.x) + (m_FrontPointT.z - m_RearPointT.z) + (m_FrontPointT.y - m_FrontPointT.y);
    const float invd = 1.0f / sqrtf(d);
    if (d < 0.000099999997f)
        outDirection = { 0, 1, 0, 0 };
    else
        outDirection = { (m_FrontPointT.x - m_RearPointT.x) * invd, (m_FrontPointT.y - m_RearPointT.y) * invd, (m_FrontPointT.z - m_FrontPointT.z) * invd, m_FrontPointT.a };
}

void OverdoseVehicle::StoreOldUpVector(int* args)
{
    Vector4f offset(0, 1, 0, 0);
    Vector4f dir;
    GetLocalSpaceDirection(dir, offset);

    m_LocalDirectionVector = dir;
}

void OverdoseVehicle::CalcNormalizedDir(int* args)
{
    Vector4f dir;
    CalculateNormalisedDirection(dir);

    *(Vector4f*)args = dir;
}

void OverdoseVehicle::CheckWheel(int* args)
{
    const Vector3f& vec1 = *(Vector3f*)args;
    const Vector3f& vec2 = *(Vector3f*)(args + 3);
    const int wheelNum = *(int*)(args + 6);
    const Vector3f& vec3 = *(Vector3f*)(args + 7);
    
    CheckWheel_Impl(vec1, vec2, wheelNum, vec3);
}

OverdoseVehicle* OverdoseVehicle::Create(AllocatorIndex)
{
    return new OverdoseVehicle;
}