#include "Node.h"
#include "Position.h"
#include "AuxQuadTree.h"
#include "Fragment.h"

Node::Node(unsigned char allocationBitmask)
{
	MESSAGE_CLASS_CREATED(Node);

	m_Flags = m_Flags & 0xC000F000 | 0x40000000;
	field_4C = field_34 = field_38 = 0;
	((Entity*)this)->field_10 = 0;
	m_Parent = nullptr;
	m_Fragment = nullptr;
	m_Position = nullptr;
	m_QuadTree = nullptr;
	field_44 = nullptr;

	if (allocationBitmask & NODE_MASK_POSITION)
		m_Position = new Position(this);

	if (allocationBitmask & NODE_MASK_QUADTREE)
		m_QuadTree = new AuxQuadTree(this);

	if (allocationBitmask & NODE_MASK_FRAGMENT)
		m_Fragment = new Fragment(this);

	//	NOTE: this just changes sign?
	if (m_Fragment)
		m_Flags = m_Flags | 0x80000000;
	else
		m_Flags = m_Flags & 0x7FFFFFFF;

	field_2C = field_2C | 0xFFFFFFFF;
	m_Flags = m_Flags | 0xF000;
}