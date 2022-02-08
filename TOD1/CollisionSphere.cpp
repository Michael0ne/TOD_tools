#include "CollisionSphere.h"
#include "BuiltinType.h"
#include "NumberType.h"
#include "IntegerType.h"

EntityType* tCollisionSphere;

CollisionSphere::CollisionSphere() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(CollisionSphere);

    m_Radius = 1.0f;
    m_Extrusion = BuiltinType::ZeroVector;
    m_CollisionFlags.Solid = true;
    m_CollisionFlags.BulletProof = true;
    m_CollisionFlags.Opaque = true;
    m_CollisionFlags.CameraBlock = true;
    m_CollisionFlags.Physics = true;

    field_60 = new char[20];
    m_QuadTree->_8A3810(field_60);
}

const float CollisionSphere::GetRadius() const
{
    return m_Radius;
}

void CollisionSphere::SetRadius(const float radius)
{
    SetParam(10, &m_Radius, tNUMBER);
    m_Radius = radius;

    Instantiate();
}

const float CollisionSphere::GetExtrusion() const
{
    return m_Extrusion.z;
}

void CollisionSphere::SetExtrusion(const float extrusion)
{
    m_Extrusion.z = extrusion;

    Instantiate();
}

const char CollisionSphere::GetCollisionFlags() const
{
    return m_CollisionFlags.m_CollisionFlags;
}

void CollisionSphere::SetCollisionFlags(const char flags)
{
    SetParam(12, &m_CollisionFlags, tINTEGER);
    m_CollisionFlags.m_CollisionFlags = flags;

    Instantiate();
}

void CollisionSphere::Register()
{
    tCollisionSphere = new EntityType("CollisionSphere");
    tCollisionSphere->InheritFrom(tNode);
    tCollisionSphere->SetCreator((CREATOR)Create);

    tCollisionSphere->RegisterProperty(tNUMBER, "radius", (EntityGetterFunction)&GetRadius, NULL, NULL, NULL, (EntitySetterFunction)&SetRadius, NULL, NULL, NULL, "control=slider|min=0|max=20", NULL, NULL, 10);
    tCollisionSphere->RegisterProperty(tNUMBER, "extrusion", (EntityGetterFunction)&GetExtrusion, NULL, NULL, NULL, (EntitySetterFunction)&SetExtrusion, NULL, NULL, NULL, "control=slider|min=0|max=20", NULL, NULL, 11);
    tCollisionSphere->RegisterProperty(tINTEGER, "CollisionFlags", (EntityGetterFunction)&GetCollisionFlags, NULL, NULL, NULL, (EntitySetterFunction)&SetCollisionFlags, NULL, NULL, NULL, "control=flags|flag0=Solid|flag1=BulletProof|flag2=Opaque|flag3=CameraBlock|flag4=Trigger|flag5=NavigationBlock|flag6=Physics|flag7=Activate triggers", NULL, NULL, 12);

    tCollisionSphere->PropagateProperties();
}

CollisionSphere* CollisionSphere::Create(AllocatorIndex)
{
    return new CollisionSphere;
}