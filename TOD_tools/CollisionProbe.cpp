#include "CollisionProbe.h"

List<Node>& CollisionProbe::ms_List = *(List<Node>*)0xA3DD4C;	//	@A3DD4C

CollisionProbe::CollisionProbe(int unk1, float unk2) : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(CollisionProbe);

	m_TouchingNodes = nullptr;
	m_Nodes = nullptr;

	m_List_1 = List<int>(0x1CB00);
	m_IgnoredNodes = List<Node>(0x1CB00);
	m_List_3 = List<int>(0x1CB00);

	field_AC = 0x2007C00;
	field_D0 = 0;

	Reset();

	field_FC = 1;
	ms_List.AddElement(this);
	field_D0 = 0;

	Reset();
}

#pragma message(TODO_IMPLEMENTATION)
void CollisionProbe::Reset()
{
	(*(void(__thiscall*)(CollisionProbe*))0x8B61D0)(this);
}