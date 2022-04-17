#include "RigidBody.h"
#include "NumberType.h"
#include "TruthType.h"
#include "VectorType.h"
#include "QuaternionType.h"

EntityType* tRigidBody;

int RigidBody::CommandDoCollisionGameLogic;
int RigidBody::CommandGetContactAction;

#pragma message(TODO_IMPLEMENTATION)
RigidBody::RigidBody() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(RigidBody);

    m_Orientation_t = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
    m_UnkVec_6 = Orientation(0.0f, 0.0f, 0.0f, 1.0f);

    //m_Unknown_1 = List<int>(0x25300);

    m_CollMass = 10.0f;

    m_Position_t = {};
    m_LinearSpeed = {};
    m_AngularMomentum = {};
    m_LinearVelocity = {};
    m_CollAngularSpeed = {};

    m_UnkVec_1 = Vector4f(1.0f, 1.0f, 1.0f, 0.0f);

    SetBoxInertialMatrix((int*)&m_UnkVec_1);

    field_22D = 0;
    m_Grounded = false;
}

void RigidBody::Instantiate()
{
    Node::Instantiate();

    Vector4f pos;
    GetPos(pos);

    m_Position_t = pos;
    m_UnkVec_7 = pos;
    m_AngularMomentum.x = 0.f;
    m_AngularMomentum.y = 0.f;
    m_AngularMomentum.z = 0.f;
    m_AngularMomentum.a = 0.f;
    m_LinearSpeed.x = 0.f;
    m_LinearSpeed.y = 0.f;
    m_LinearSpeed.z = 0.f;
    m_LinearSpeed.a = 0.f;

    static int _A3E1A4 = 0; //  NOTE: never used.
}

const float RigidBody::GetCollMass() const
{
    return m_CollMass;
}

void RigidBody::SetCollMass(const float mass)
{
    StoreProperty(13, &m_CollMass, tNUMBER);
    m_CollMass = mass;
}

const float RigidBody::GetElasticity() const
{
    return m_Elasticity;
}

void RigidBody::SetElasticity(const float elasticity)
{
    StoreProperty(14, &m_Elasticity, tNUMBER);
    m_Elasticity = elasticity;
}

const bool RigidBody::IsGrounded() const
{
    return m_Grounded;
}

void RigidBody::SetIsGrounded(const bool grounded)
{
    m_Grounded = grounded;
}

void RigidBody::GetAngularMomentum(Vector4f& momentum) const
{
    momentum = m_AngularMomentum;
}

void RigidBody::SetAngularMomentum(const Vector4f& momentum)
{
    StoreProperty(10, &m_AngularMomentum, tVECTOR);
    m_AngularMomentum = momentum;
}

const Vector4f RigidBody::GetLinearVelocity() const
{
    const float invMass = 1.f / m_CollMass;
    return Vector4f(m_LinearSpeed.x * invMass, m_LinearSpeed.y * invMass, m_LinearSpeed.z * invMass, m_LinearSpeed.a);
}

void RigidBody::SetLinearVelocity(Vector4f& speed)
{
    StoreProperty(12, &m_LinearVelocity, tVECTOR);

    if (!tVECTOR->IsValidValueForType(&speed))
    {
        String str;
        tVECTOR->PrintFormattedValue(str, &speed, 0);
        debug("SetLinearVelocity: '%s' values out of range: %s\n", tVECTOR->m_TypeName.m_Str, str.m_Str);
    }

    m_LinearSpeed.x = speed.x * m_CollMass;
    m_LinearSpeed.y = speed.y * m_CollMass;
    m_LinearSpeed.z = speed.z * m_CollMass;
    m_LinearSpeed.a = speed.a;
}

void RigidBody::GetAngularVelocity(Vector4f& speed) const
{
    speed = m_CollAngularSpeed;
}

#pragma message(TODO_IMPLEMENTATION)
void RigidBody::SetAngularVelocity(const Vector4f& pseed)
{
}

void RigidBody::GetOrientationT(Orientation& orient) const
{
    orient = m_Orientation_t;
}

void RigidBody::SetOrientationT(const Orientation& orient)
{
    StoreProperty(15, &m_Orientation_t, tQUATERNION);
    m_Orientation_t = orient;
}

void RigidBody::GetPositionT(Vector4f& pos) const
{
    pos = m_Position_t;
}

void RigidBody::SetPositionT(const Vector4f& pos)
{
    StoreProperty(16, &m_Position_t, tVECTOR);
    m_Position_t = pos;
}


#pragma message(TODO_IMPLEMENTATION)
void RigidBody::TestAndResolveCollision(int* args)
{
}

void RigidBody::GetVelocityAtLocalPos(int* args)
{
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
}

void RigidBody::GetVelocityAtGlobalPos(int* args)
{
    args[0] = 0;
    args[1] = 0;
    args[2] = 0;
}

void RigidBody::Reset(int* args)
{
}

void RigidBody::SetBoxInertialMatrix(int* args)
{
    SetBoxInertialMatrix_Impl({ (float)args[0], (float)args[1], (float)args[2], 0 });
}

void RigidBody::SetEllipsoidInertialMatrix(int* args)
{
    SetEllipsoidInertialMatrix_Impl({ (float)args[0], (float)args[1], (float)args[2], 0 });
}

void RigidBody::Register()
{
    tRigidBody = new EntityType("RigidBody");
    tRigidBody->InheritFrom(tNode);
    tRigidBody->SetCreator((CREATOR)Create);

    tRigidBody->RegisterProperty(tNUMBER, "coll_mass", (EntityGetterFunction)&GetCollMass, (EntitySetterFunction)&SetCollMass, "control=slider|min=0.01|max=100", 13);
    tRigidBody->RegisterProperty(tNUMBER, "Elasticity", (EntityGetterFunction)&GetElasticity, (EntitySetterFunction)&SetElasticity, nullptr, 14);
    tRigidBody->RegisterProperty(tTRUTH, "Grounded", (EntityGetterFunction)&IsGrounded, (EntitySetterFunction)&SetIsGrounded, nullptr);
    tRigidBody->RegisterProperty(tVECTOR, "angularmomentum", (EntityGetterFunction)&GetAngularMomentum, (EntitySetterFunction)&SetAngularMomentum, nullptr, 10);
    tRigidBody->RegisterProperty(tVECTOR, "coll_linear_speed", (EntityGetterFunction)&GetLinearVelocity, (EntitySetterFunction)&SetLinearVelocity, nullptr, 12);
    tRigidBody->RegisterProperty(tVECTOR, "coll_angular_speed", (EntityGetterFunction)&GetAngularVelocity, (EntitySetterFunction)&SetAngularVelocity, nullptr, 11);
    tRigidBody->RegisterProperty(tQUATERNION, "Orientation_t", (EntityGetterFunction)&GetOrientationT, (EntitySetterFunction)&SetOrientationT, nullptr, 15);
    tRigidBody->RegisterProperty(tVECTOR, "Position_t", (EntityGetterFunction)&GetPositionT, (EntitySetterFunction)&SetPositionT, nullptr, 16);

    tRigidBody->RegisterScript("TestAndResolveCollision(number):integer", (EntityFunctionMember)&TestAndResolveCollision);
    tRigidBody->RegisterScript("getvelocityatlocalpos(vector):vector", (EntityFunctionMember)&GetVelocityAtLocalPos);
    tRigidBody->RegisterScript("getvelocityatglobalpos(vector):vector", (EntityFunctionMember)&GetVelocityAtGlobalPos);
    tRigidBody->RegisterScript("reset", (EntityFunctionMember)&Reset);
    tRigidBody->RegisterScript("SetBoxInertiaMatrix(vector)", (EntityFunctionMember)&SetBoxInertialMatrix);
    tRigidBody->RegisterScript("SetEllipsoidInertiaMatrix(vector)", (EntityFunctionMember)&SetEllipsoidInertialMatrix);

    CommandDoCollisionGameLogic = GetCommandId("command_do_collision_game_logic", true);
    CommandGetContactAction = GetCommandId("command_get_contact_action(integer):integer", true);

    tRigidBody->PropagateProperties();
}

#pragma message(TODO_IMPLEMENTATION)
void RigidBody::SetEllipsoidInertialMatrix_Impl(const Vector4f& pos)
{
}

#pragma message(TODO_IMPLEMENTATION)
void RigidBody::SetBoxInertialMatrix_Impl(const Vector4f& pos)
{
}

RigidBody* RigidBody::Create(AllocatorIndex)
{
    return new RigidBody;
}