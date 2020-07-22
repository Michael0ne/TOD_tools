#include "Cloth.h"

Cloth::Cloth() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(Cloth);

	m_ResourceInfo = nullptr;
	field_68 = 1;
	field_A8 = field_60 = 0;
	field_6C = nullptr;
	field_AC = 1;
	field_B0 = -1;

	m_List_1 = List<int>(0x1FB00);
	m_List_2 = List<int>(0x1FB00);

	//	TODO: initialize more stuff!
}