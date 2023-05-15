#include "OverdoseVehicle.h"
#include "BuiltinType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "VectorType.h"

EntityType* tOverdoseVehicle;

int OverdoseVehicle::UpdateAccelerationCommand;
int OverdoseVehicle::IsPlayerCarCommand;
DataType* OverdoseVehicle::VectorListType;

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

void OverdoseVehicle::GetCurrentVelocity(Vector4f& outVelocity) const
{
    outVelocity = m_CurrentVelocity;
}

void OverdoseVehicle::SetCurrentVelocity(const Vector4f& velocity)
{
    m_CurrentVelocity = velocity;
}

const float OverdoseVehicle::GetSpringCoefficient() const
{
    return m_SpringKoefficient;
}

void OverdoseVehicle::SetSpringCoefficient(const float koef)
{
    StoreProperty(26, &m_SpringKoefficient, tNUMBER);
    m_SpringKoefficient = koef;
}

const float OverdoseVehicle::GetSpringDamping() const
{
    return m_SpringDamping;
}

void OverdoseVehicle::SetSpringDamping(const float damping)
{
    StoreProperty(25, &m_SpringDamping, tNUMBER);
    m_SpringDamping = damping;
}

const float OverdoseVehicle::GetStiffness() const
{
    return m_Stiffness;
}

void OverdoseVehicle::SetStiffness(const float stiffness)
{
    StoreProperty(27, &m_Stiffness, tNUMBER);
    m_Stiffness = stiffness;
}

const float OverdoseVehicle::GetRealmainbodyDefaultY() const
{
    return m_RealmainbodyDefaultY;
}

void OverdoseVehicle::SetRealmainbodyDefaultY(const float defy)
{
    StoreProperty(24, &m_RealmainbodyDefaultY, tNUMBER);
    m_RealmainbodyDefaultY = defy;
}

const float OverdoseVehicle::GetLFSpring() const
{
    return m_LFSpring;
}

void OverdoseVehicle::SetLFSpring(const float lf)
{
    m_LFSpring = lf;
}

const float OverdoseVehicle::GetRFSpring() const
{
    return m_RFSpring;
}

void OverdoseVehicle::SetRFSpring(const float rf)
{
    m_RFSpring = rf;
}

const float OverdoseVehicle::GetRRSpring() const
{
    return m_RRSpring;
}

void OverdoseVehicle::SetRRSpring(const float rr)
{
    m_RRSpring = rr;
}

const float OverdoseVehicle::GetLRSpring() const
{
    return m_LRSpring;
}

void OverdoseVehicle::SetLRSpring(const float lr)
{
    m_LRSpring = lr;
}

const int OverdoseVehicle::GetLeftRearWheelMaterial() const
{
    return m_RearWheelsMaterialID;
}

void OverdoseVehicle::SetLeftRearWheelMaterial(const int material)
{
    m_RearWheelsMaterialID = material;
}

const int OverdoseVehicle::GetRightRearWheelMaterial() const
{
    return m_RearWheelsMaterialID;
}

void OverdoseVehicle::SetRightRearWheelMaterial(const int material)
{
    m_RearWheelsMaterialID = material;
}

void OverdoseVehicle::GetFrontPointVelocity(Vector4f& outVelocity) const
{
    outVelocity = m_FrontPointVelocity;
}

void OverdoseVehicle::SetFrontPointVelocity(const Vector4f& velocity)
{
    StoreProperty(21, &m_FrontPointVelocity, tVECTOR);
    m_FrontPointVelocity = velocity;
}

void OverdoseVehicle::GetRearPointVelocity(Vector4f& outVelocity) const
{
    outVelocity = m_RearPointVelocity;
}

void OverdoseVehicle::SetRearPointVelocity(const Vector4f& velocity)
{
    StoreProperty(22, &m_RearPointVelocity, tVECTOR);
    m_RearPointVelocity = velocity;
}

const bool OverdoseVehicle::IsBike() const
{
    return m_IsBike;
}

void OverdoseVehicle::SetIsBike(const bool isbike)
{
    m_IsBike = isbike;
}

Node* OverdoseVehicle::GetRealWheelLF() const
{
    return m_RealWheelLF;
}

void OverdoseVehicle::SetRealWheelLF(Node* wheel)
{
    m_RealWheelLF = wheel;
}

Node* OverdoseVehicle::GetRealWheelRF() const
{
    return m_RealWheelRF;
}

void OverdoseVehicle::SetRealWheelRF(Node* wheel)
{
    m_RealWheelRF = wheel;
}

Node* OverdoseVehicle::GetRealWheelRR() const
{
    return m_RealWheelRR;
}

void OverdoseVehicle::SetRealWheelRR(Node* wheel)
{
    m_RealWheelRR = wheel;
}

Node* OverdoseVehicle::GetRealWheelLR() const
{
    return m_RealWheelLR;
}

void OverdoseVehicle::SetRealWheelLR(Node* wheel)
{
    m_RealWheelLR = wheel;
}

Node* OverdoseVehicle::GetRealmainbody() const
{
    return m_Realmainbody;
}

void OverdoseVehicle::SetRealmainbody(Node* realmainbody)
{
    m_Realmainbody = realmainbody;
}

Node* OverdoseVehicle::GetDamageCtrl() const
{
    return m_DamageCtrl;
}

void OverdoseVehicle::SetDamageCtrl(Node* damagectrl)
{
    m_DamageCtrl = damagectrl;
}

void* OverdoseVehicle::GetCarContactList() const
{
    return (void*)&m_CarContactList;
}

void OverdoseVehicle::SetCarContactList(const std::vector<bool>& carcontactlist)
{
    m_CarContactList = carcontactlist;
}

void* OverdoseVehicle::GetCVList() const
{
    return (void*)&m_CVList;
}

void OverdoseVehicle::SetCVList(const std::vector<int>& cvlist)
{
    m_CVList = cvlist;
}

void* OverdoseVehicle::GetDistList() const
{
    return (void*)&m_DistList;
}

void OverdoseVehicle::SetDistList(const std::vector<float>& distlist)
{
    m_DistList = distlist;
}

void* OverdoseVehicle::GetNormalList()
{
    if (m_CachedNormalList.size() < m_NormalList.size())
        m_CachedNormalList.resize(m_NormalList.size());

    for (size_t i = 0; i < m_NormalList.size(); ++i)
        m_CachedNormalList[i] = { m_NormalList[i].x, m_NormalList[i].y, m_NormalList[i].z };

    return (void*)&m_CachedNormalList;
}

void OverdoseVehicle::SetNormalList(const std::vector<Vector3f>& normallist)
{
    if (m_NormalList.size() < normallist.size())
        m_NormalList.resize(normallist.size());

    for (size_t i = 0; i < m_NormalList.size(); ++i)
    {
        m_NormalList[i] = { normallist[i].x, normallist[i].y, normallist[i].z, 0 };
    }
}

void* OverdoseVehicle::GetLastNormalList()
{
    if (m_LastNormalList.size() > m_CachedLastNormalList.size())
        m_CachedLastNormalList.resize(m_LastNormalList.size());

    for (size_t i = 0; i < m_LastNormalList.size(); ++i)
        m_CachedLastNormalList[i] = { m_LastNormalList[i].Vec1.x, m_LastNormalList[i].Vec1.y, m_LastNormalList[i].Vec1.z };

    return (void*)&m_CachedLastNormalList;
}

void OverdoseVehicle::SetLastNormalList(const std::vector<Vector3f>& lastnormallist)
{
    void* currentLastNormalList = GetLastNormalList();
    StoreProperty(23, &currentLastNormalList, VectorListType);

    if (lastnormallist.size() > m_LastNormalList.size())
        m_LastNormalList.resize(lastnormallist.size());

    for (size_t i = 0; i < m_LastNormalList.size(); ++i)
        m_LastNormalList[i].Vec1 = { lastnormallist[i].x, lastnormallist[i].y, lastnormallist[i].z, 0 };
}

void* OverdoseVehicle::GetHintnodeList() const
{
    return (void*)&m_HintNodeList;
}

void OverdoseVehicle::SetHintnodeList(const std::vector<Node*> hintnodelist)
{
    m_HintNodeList = hintnodelist;
}

Node* OverdoseVehicle::GetGroundProbe() const
{
    return (Node*)m_GroundProbe;
}

void OverdoseVehicle::SetGroundProbe(Node* node)
{
    m_GroundProbe = (CollisionProbe*)node;
}

const float OverdoseVehicle::GetWheelRadius() const
{
    return m_WheelRadius;
}

void OverdoseVehicle::SetWheelRadius(const float radius)
{
    StoreProperty(29, &m_WheelRadius, tNUMBER);
    m_WheelRadius = radius;
}

const float OverdoseVehicle::GetTilt() const
{
    return m_Tilt;
}

void OverdoseVehicle::SetTilt(const float tilt)
{
    m_Tilt = tilt;
}

const bool OverdoseVehicle::IsOutOfBounds() const
{
    return m_IsOutOfBounds;
}

void OverdoseVehicle::SetIsOutOfBounds(const bool oob)
{
    m_IsOutOfBounds = oob;
}

const bool OverdoseVehicle::IsDriveableGround() const
{
    return m_IsDriveableGround;
}

void OverdoseVehicle::SetIsDriveableGround(const bool dg)
{
    m_IsDriveableGround = dg;
}

void OverdoseVehicle::GetFrontPointT(Vector4f& outPoint) const
{
    outPoint = m_FrontPointT;
}

void OverdoseVehicle::SetFrontPointT(const Vector4f& pointT)
{
    StoreProperty(17, &m_FrontPointT, tVECTOR);
    m_FrontPointT = pointT;
}

void OverdoseVehicle::GetRearPointT(Vector4f& outPoint) const
{
    outPoint = m_RearPointT;
}

void OverdoseVehicle::SetRearPointT(const Vector4f& pointT)
{
    StoreProperty(18, &m_RearPointT, tVECTOR);
    m_RearPointT = pointT;
}

const float OverdoseVehicle::GetRollAngleT() const
{
    return m_RollAngleT;
}

void OverdoseVehicle::SetRollAngleT(const float angle)
{
    StoreProperty(19, &m_RollAngleT, tNUMBER);
    m_RollAngleT = angle;
}

const float OverdoseVehicle::GetTiltAngleSpeed() const
{
    return m_TiltAngleSpeed;
}

void OverdoseVehicle::SetTiltAngleSpeed(const float tiltanglespeed)
{
    StoreProperty(28, &m_TiltAngleSpeed, tNUMBER);
    m_TiltAngleSpeed = tiltanglespeed;
}

const float OverdoseVehicle::GetAxleHeight() const
{
    return m_AxleHeight;
}

void OverdoseVehicle::SetAxleHeight(const float axh)
{
    StoreProperty(20, &m_AxleHeight, tNUMBER);
    m_AxleHeight = axh;
}

const float OverdoseVehicle::GetWidth() const
{
    return m_Width;
}

void OverdoseVehicle::SetWidth(const float width)
{
    StoreProperty(30, &m_Width, tNUMBER);
    m_Width = width;
}

const bool OverdoseVehicle::CppPropertiesInited() const
{
    return m_IsCppPropertiesInited;
}

void OverdoseVehicle::SetCppPropertiesInited(const bool inited)
{
    m_IsCppPropertiesInited = inited;
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
    tOverdoseVehicle->RegisterProperty(tTRUTH, "bike", (EntityGetterFunction)&IsBike, (EntitySetterFunction)&SetIsBike, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_LF", (EntityGetterFunction)&GetRealWheelLF, (EntitySetterFunction)&SetRealWheelLF, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_RF", (EntityGetterFunction)&GetRealWheelRF, (EntitySetterFunction)&SetRealWheelRF, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_RR", (EntityGetterFunction)&GetRealWheelRR, (EntitySetterFunction)&SetRealWheelRR, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "RealWheel_LR", (EntityGetterFunction)&GetRealWheelLR, (EntitySetterFunction)&SetRealWheelLR, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "realmainbody", (EntityGetterFunction)&GetRealmainbody, (EntitySetterFunction)&SetRealmainbody, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "damagectrl", (EntityGetterFunction)&GetDamageCtrl, (EntitySetterFunction)&SetDamageCtrl, nullptr);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(truth)"), "car_contact_list", (EntityGetterFunction)&GetCarContactList, (EntitySetterFunction)&SetCarContactList, nullptr);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(integer)"), "CV_List", (EntityGetterFunction)&GetCVList, (EntitySetterFunction)&SetCVList, nullptr);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(number)"), "dist_List", (EntityGetterFunction)&GetDistList, (EntitySetterFunction)&SetDistList, nullptr);
    VectorListType = DataType::LoadScript("list(vector)");
    tOverdoseVehicle->RegisterProperty(VectorListType, "normal_List", (EntityGetterFunction)&GetNormalList, (EntitySetterFunction)&SetNormalList, nullptr);
    tOverdoseVehicle->RegisterProperty(VectorListType, "LastNormal_list", (EntityGetterFunction)&GetLastNormalList, (EntitySetterFunction)&SetLastNormalList, nullptr, 23);
    tOverdoseVehicle->RegisterProperty(DataType::LoadScript("list(entity)"), "Hintnode_list", (EntityGetterFunction)&GetHintnodeList, (EntitySetterFunction)&SetHintnodeList, nullptr);
    tOverdoseVehicle->RegisterProperty(tEntity, "Groundprobe", (EntityGetterFunction)&GetGroundProbe, (EntitySetterFunction)&SetGroundProbe, nullptr);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "wheel_radius", (EntityGetterFunction)&GetWheelRadius, (EntitySetterFunction)&SetWheelRadius, nullptr, 29);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "tilt", (EntityGetterFunction)&GetTilt, (EntitySetterFunction)&SetTilt, nullptr);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "is_out_of_bounds", (EntityGetterFunction)&IsOutOfBounds, (EntitySetterFunction)&SetIsOutOfBounds, nullptr);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "driveable_ground", (EntityGetterFunction)&IsDriveableGround, (EntitySetterFunction)&SetIsDriveableGround, nullptr);
    tOverdoseVehicle->RegisterProperty(tVECTOR, "front_point_t", (EntityGetterFunction)&GetFrontPointT, (EntitySetterFunction)&SetFrontPointT, nullptr, 17);
    tOverdoseVehicle->RegisterProperty(tVECTOR, "rear_point_t", (EntityGetterFunction)&GetRearPointT, (EntitySetterFunction)&SetRearPointT, nullptr, 18);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "roll_angle_t", (EntityGetterFunction)&GetRollAngleT, (EntitySetterFunction)&SetRollAngleT, nullptr, 19);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "tilt_angle_speed", (EntityGetterFunction)&GetTiltAngleSpeed, (EntitySetterFunction)&SetTiltAngleSpeed, nullptr, 28);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "axle_height", (EntityGetterFunction)&GetAxleHeight, (EntitySetterFunction)&SetAxleHeight, nullptr, 20);
    tOverdoseVehicle->RegisterProperty(tNUMBER, "width", (EntityGetterFunction)&GetWidth, (EntitySetterFunction)&SetWidth, nullptr, 30);
    tOverdoseVehicle->RegisterProperty(tTRUTH, "cpppropertiesinited", (EntityGetterFunction)&CppPropertiesInited, (EntitySetterFunction)&SetCppPropertiesInited, nullptr);

    tOverdoseVehicle->RegisterScript("check_wheel(vector,vector,integer,vector)", (EntityFunctionMember)&CheckWheel);
    tOverdoseVehicle->RegisterScript("update_suspension(number,vector)", (EntityFunctionMember)&UpdateSuspension);
    tOverdoseVehicle->RegisterScript("calc_normalized_dir_vec:vector", (EntityFunctionMember)&CalcNormalizedDir);
    tOverdoseVehicle->RegisterScript("calc_up_vec:vector", (EntityFunctionMember)&CalculateUpVector);
    tOverdoseVehicle->RegisterScript("convert_forth", (EntityFunctionMember)&ConvertForth);
    tOverdoseVehicle->RegisterScript("check_ground_contact", (EntityFunctionMember)&CheckGroundContact);
    tOverdoseVehicle->RegisterScript("StoreOldUpVector", (EntityFunctionMember)&StoreOldUpVector);

    UpdateAccelerationCommand = GetCommand("update_acceleration(vector,number)", true);
    IsPlayerCarCommand = GetCommand("command_IsPlayer_car:truth", true);

    tOverdoseVehicle->PropagateProperties();
}

#pragma message(TODO_IMPLEMENTATION)
void OverdoseVehicle::UpdateSuspension(const float a1, const Vector4f& a2)
{
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

#pragma message(TODO_IMPLEMENTATION)
void OverdoseVehicle::CalculateUpVector(Vector4f& outVec)
{
}

#pragma message(TODO_IMPLEMENTATION)
void OverdoseVehicle::ConvertForth()
{
}

#pragma message(TODO_IMPLEMENTATION)
void OverdoseVehicle::CheckGroundContact()
{
}

#pragma message(TODO_IMPLEMENTATION)
void OverdoseVehicle::CheckWheel_Impl(const Vector4f& a1, const Vector4f& a2, const unsigned int wheel, const Vector4f& a3)
{
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