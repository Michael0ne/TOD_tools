#include "MockupBox.h"

int& MockupBox::TotalCreated = *(int*)0xA3DFD8;	//	@A3DFD8

MockupBox::MockupBox() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(MockupBox);

	m_ResourceInfo = nullptr;
	field_68 = 1;

	m_List_1 = List<int>(0x1FB00);
	m_List_2 = List<int>(0x1FB00);

	field_B8 = 0;
	field_70 = 0;
	field_BC = 1;
	field_C0 = -1;

	m_Left = -1.5f;
	m_Front = -1.5f;
	m_Height = 3.0f;
	m_Right = 1.5f;
	m_Back = 1.5f;
	m_Opacity = 1.0f;

	++TotalCreated;

	//	TODO: lots of stuff initialized here.
}
