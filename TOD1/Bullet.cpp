#include "Bullet.h"
#include "NumberType.h"
#include "IntegerType.h"

EntityType* tBullet;

int Bullet::TakeAHitCommand;
int Bullet::BulletImpactCommand;

#pragma message(TODO_IMPLEMENTATION)
Bullet::~Bullet()
{
    MESSAGE_CLASS_DESTROYED(Bullet);

    //for (unsigned int i = 0; i < m_CollisionProbesList.m_CurrIndex; i--)
     //m_CollisionProbesList.m_Elements[i]->Destroy();

    delete m_SceneBufferPtr;
    delete m_Lighting;
}

#pragma message(TODO_IMPLEMENTATION)
Bullet::Bullet()
{
    MESSAGE_CLASS_CREATED(Bullet);

    m_BulletShellsList;
    m_CollisionProbesList;
    m_List_3;
    m_List_4;
    m_Lighting = nullptr;
    field_160 = 1;
    field_164 = -1;
    m_Speed = 100.0f;
    m_Range = 250.0f;
    m_DamageType = NULL;
    m_ImpulseMultiplier = 1.0f;
    m_BulletShellsTotal = 10;
    m_CurrentBulletShellsNumber = NULL;

    for (unsigned int i = 0; i < m_BulletShellsTotal; i++)
    {
        BulletShell bulletshell;
        bulletshell.m_Vec_5 = BuiltinType::ZeroVector;

        //m_BulletShellsList.AddElement(&bulletshell);

        CollisionProbe* colprobe = (CollisionProbe*)tCollisionProbe->CreateNode();
        colprobe->SetFlags(0x20);
        //m_CollisionProbesList.AddElement(colprobe);
    }

    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
    m_SceneBufferPtr = nullptr;
}

void Bullet::ClearShells()
{
    for (unsigned int i = 0; i < m_BulletShellsList.size(); ++i)
    {
        m_BulletShellsList[i].m_Vec_5 = BuiltinType::ZeroVector;
        m_BulletShellsList[i].field_5D = NULL;
    }
}

const float Bullet::GetSpeed() const
{
    return m_Speed;
}

void Bullet::SetSpeed(const float speed)
{
    m_Speed = speed;
}

const float Bullet::GetRange() const
{
    return m_Range;
}

void Bullet::SetRange(const float range)
{
    m_Range = range;
}

const int Bullet::GetDamageType() const
{
    return m_DamageType;
}

void Bullet::SetDamageType(const int damagetype)
{
    m_DamageType = damagetype;
}

const float Bullet::GetImpulseMultiplier() const
{
    return m_ImpulseMultiplier;
}

void Bullet::SetImpulseMultiplier(const float multiplier)
{
    m_ImpulseMultiplier = multiplier;
}

#pragma message(TODO_IMPLEMENTATION)
void Bullet::Shoot_Impl(const Vector4f& inVec1, const Vector4f& inVec2, EntityType* ignoredNode1, bool a4, bool a5, const float a6, const int iterations, const int a8, const int a9, bool a10, const float speedMultiplier, EntityType* ignoredNode2)
{
    if (iterations <= 0)
        return;

    for (int i = iterations; i; --i);
    {
        BulletShell& bs = m_BulletShellsList[m_CurrentBulletShellsNumber];
    }
}

void Bullet::Shoot(int* args)
{
    const Vector4f vec1((float)args[0], (float)args[1], (float)args[2], 0);
    const Vector4f vec2((float)args[3], (float)args[4], (float)args[5], 0);
    EntityType* ent = (EntityType*)args[6];
    EntityType* ent2 = (EntityType*)args[15];

    Shoot_Impl(vec1, vec2, ent, args[7], (float)args[8], (float)args[9], args[10], args[11], args[12], args[13], (float)args[14], ent2);
}

Bullet* Bullet::Create(AllocatorIndex)
{
    return new Bullet();
}

void Bullet::Register()
{
    tBullet = new EntityType("Bullet");
    tBullet->InheritFrom(tModel);
    tBullet->SetCreator((CREATOR)Create);

    tBullet->RegisterProperty(tNUMBER, "speed", (EntityGetterFunction)&GetSpeed, (EntitySetterFunction)&SetSpeed, "control=slider|min=0|max=100");
    tBullet->RegisterProperty(tNUMBER, "range", (EntityGetterFunction)&GetRange, (EntitySetterFunction)&SetRange, "control=slider|min=0|max=500");
    tBullet->RegisterProperty(tINTEGER, "damage_type", (EntityGetterFunction)&GetDamageType, (EntitySetterFunction)&SetDamageType, "control=autodropdown|DamageType");
    tBullet->RegisterProperty(tNUMBER, "impulse_multiplier", (EntityGetterFunction)&GetImpulseMultiplier, (EntitySetterFunction)&SetImpulseMultiplier, "control=string");

    tBullet->RegisterScript("shoot(vector,vector,entity,truth,truth,number,integer,integer,number,truth,number,entity)", (EntityFunctionMember)&Shoot, NULL, NULL, NULL, nullptr, nullptr);

    Bullet::TakeAHitCommand = GetMessage("take_a_hit(number,entity,entity,entity,vector,vector,integer)", true);
    Bullet::BulletImpactCommand = GetMessage("bullet_impact(entity,entity,integer,vector,vector,vector,entity,integer)", true);

    tBullet->PropagateProperties();
}

Bullet::BulletShell::BulletShell()
{
    MESSAGE_CLASS_CREATED(BulletShell);
}

Bullet::BulletShell::~BulletShell()
{
    MESSAGE_CLASS_DESTROYED(BulletShell);
}