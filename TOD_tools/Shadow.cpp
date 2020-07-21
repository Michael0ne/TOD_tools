#include "Shadow.h"

int& Shadow::TotalShadows = *(int*)0xA3E094;	//	@A3E094

Shadow::Shadow() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(Shadow);

	m_ResourceInfo = nullptr;
	field_5C = 1;
	m_BoneName = String();
	field_50 = field_54 = 0;
	m_ShadowRadiusX = m_ShadowRadiusY = 1.0f;
	m_Flags = m_Flags & 0xFFFFF0FF | 0xFF;
	m_Bone = nullptr;
	++TotalShadows;
	m_Flags = m_Flags & 0xFFFFEFFF;
	m_Unknown = Vector4f(1000.0f, 1000.0f, 1000.0f, 0.0f);

	//	TODO: initialize lots more stuff here.
}