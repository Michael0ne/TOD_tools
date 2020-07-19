#pragma once

#include "Node.h"

#define GROUP_CLASS_SIZE 80

class Group : public Node
{
protected:

public:
	Group() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(Group);

		m_QuadTree->m_nUserType = m_QuadTree->m_nUserType & 0xFFFFFF | m_QuadTree->m_nUserType & 0xFF000000 | 0x80000000;
	}
};

static_assert(sizeof(Group) == GROUP_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Group));