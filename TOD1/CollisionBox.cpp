#include "CollisionBox.h"

CollisionBox::CollisionBox() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
 MESSAGE_CLASS_CREATED(CollisionBox);

 m_Width = m_Height = m_Length = 1.0f;
 m_CollisionFlags = 79;

 m_QuadTree->_8A3810((m_Unknown = new char[20]));
}