#include "Cloth.h"

#pragma message(TODO_IMPLEMENTATION)
Cloth::Cloth() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(Cloth);

	m_ResourceInfo = nullptr;
	field_68 = 1;
	field_A8 = NULL;
	field_6C = nullptr;
	field_AC = 1;
	field_B0 = -1;

	m_List_1 = List<int>(0x1FB00);
	m_List_2 = List<int>(0x1FB00);
}