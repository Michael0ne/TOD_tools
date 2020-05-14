#include "AuxQuadTree.h"

AuxQuadTree::AuxQuadTree(const Entity* owner)
{
	m_vPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_bLodFade = -1;
	m_pContactFilter = -1;
	m_pOwner = (Entity*)owner;
	field_4 = 0;
	m_pChildFirst = nullptr;
	m_pChildLast = nullptr;
	field_38 = 0;
	m_nFlags.flag0 = 0x3C;
	m_nFlags.flag1 = 0xB;
	m_nFlags.flag2 = 0x32;
	m_nLodDistance = 0;
	m_bLod = 0;
	m_pContactNode = nullptr;
	m_nUserType = 0x10000000;
	field_4D = field_4D & 0x88 | 8;
	field_30 = 0;
	field_18 = 0;
}