#include "Group.h"

Group::Group() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(Group);

	m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x80000000;
}