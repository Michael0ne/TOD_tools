#include "CollisionBox.h"
#include "IntegerType.h"
#include "NumberType.h"

EntityType* tCollisionBox;

CollisionBox::CollisionBox() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(CollisionBox);

    m_Width = 1;
    m_Height = 1;
    m_Length = 1;

    m_CollisionFlags.Solid = true;
    m_CollisionFlags.BulletProof = true;
    m_CollisionFlags.Opaque = true;
    m_CollisionFlags.CameraBlock = true;
    m_CollisionFlags.Trigger = false;
    m_CollisionFlags.NavigationBlock = false;
    m_CollisionFlags.Physics = true;
    m_CollisionFlags.ActivateTriggers = false;

    m_List = new CollBoxList;
    m_QuadTree->_8A3810(m_List);
}

CollisionBox::~CollisionBox()
{
    MESSAGE_CLASS_DESTROYED(CollisionBox);

    delete m_List;
}

const float CollisionBox::GetWidth() const
{
    return m_Width;
}

void CollisionBox::SetWidth(const float width)
{
    m_Width = width;

    Instantiate();
}

const float CollisionBox::GetHeight() const
{
    return m_Height;
}

void CollisionBox::SetHeight(const float height)
{
    m_Height = height;

    Instantiate();
}

const float CollisionBox::GetLength() const
{
    return m_Length;
}

void CollisionBox::SetLength(const float length)
{
    m_Length = length;
}

const char CollisionBox::GetCollisionFlags() const
{
    return m_CollisionFlags.m_Flags;
}

void CollisionBox::SetCollisionFlags(const char flags)
{
    SetParam(10, &m_CollisionFlags.m_Flags, tINTEGER);
    m_CollisionFlags.m_Flags = flags;

    Instantiate();
}

void CollisionBox::Register()
{
    tCollisionBox = new EntityType("CollisionBox");
    tCollisionBox->InheritFrom(tNode);
    tCollisionBox->SetCreator((EntityType::CREATOR)Create);

    tCollisionBox->RegisterProperty(tNUMBER, "width", &GetWidth, NULL, NULL, NULL, &SetWidth, NULL, NULL, NULL, "control=slider|min=0|max=20", NULL, NULL, -1);
    tCollisionBox->RegisterProperty(tNUMBER, "height", &GetHeight, NULL, NULL, NULL, &SetHeight, NULL, NULL, NULL, "control=slider|min=0|max=20", NULL, NULL, -1);
    tCollisionBox->RegisterProperty(tNUMBER, "length", &GetLength, NULL, NULL, NULL, &SetLength, NULL, NULL, NULL, "control=slider|min=0|max=20", NULL, NULL, -1);
    tCollisionBox->RegisterProperty(tINTEGER, "CollisionFlags", &GetCollisionFlags, NULL, NULL, NULL, &SetCollisionFlags, NULL, NULL, NULL, "control=flags|flag0=Solid|flag1=BulletProof|flag2=Opaque|flag3=CameraBlock|flag4=Trigger|flag5=NavigationBlock|flag6=Physics|flag7=Activate triggers", NULL, NULL, 10);

    tCollisionBox->PropagateProperties();
}

CollisionBox* CollisionBox::Create(AllocatorIndex)
{
    return new CollisionBox;
}