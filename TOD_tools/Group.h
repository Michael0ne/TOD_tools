#pragma once

#include "Node.h"

class Group : public Node
{
protected:

public:
	Group() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(Group);

		m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x80000000;
	}
};

ASSERT_CLASS_SIZE(Group, 80);