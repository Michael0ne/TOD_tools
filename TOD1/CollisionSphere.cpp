#include "CollisionSphere.h"

CollisionSphere::CollisionSphere() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
 MESSAGE_CLASS_CREATED(CollisionSphere);

 m_Radius = 1.0f;
 m_Extrusion = Vector4f();
 m_CollisionFlags = (char)79;

 m_QuadTree->_8A3810((m_Unknown = new char[20]));
}