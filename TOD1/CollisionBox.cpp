#include "CollisionBox.h"

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