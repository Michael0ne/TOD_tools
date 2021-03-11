#include "SurroundGeometry.h"

unsigned int SurroundGeometry::TotalCreated;

#pragma message(TODO_IMPLEMENTATION)
SurroundGeometry::SurroundGeometry() : Node(NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(SurroundGeometry);

	//m_List_1 = List<int>(0x26300);
	//m_List_2 = List<int>(0x1FB00);
	//m_List_3 = List<int>(0x1FB00);

	field_5A4 = 0;
	field_5A8 = 1;
	field_5AC = -1;

	m_Properbility1 = m_Properbility2 = m_Properbility3 = m_Properbility4 = 1.0f;
	m_FarDistance = 900.0f;
	m_CloseDistance = 400.0f;
	m_Variation = 0.1f;
	m_MaxCount = 50;
	m_Category = 0;
	m_RandomSeed = (*(int (*)(int*))0x9513DD)(NULL);

	++TotalCreated;
}