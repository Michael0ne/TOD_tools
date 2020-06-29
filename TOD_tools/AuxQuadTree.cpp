#include "AuxQuadTree.h"

AuxQuadTree::AuxQuadTree(const Entity* owner)
{
	MESSAGE_CLASS_CREATED(AuxQuadTree);

	m_vPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_bLodFade = -1;
	m_pContactFilter = -1;
	m_pOwner = (Entity*)owner;
	field_4 = 0;
	m_pChildFirst = nullptr;
	m_pChildLast = nullptr;
	field_38 = 0;
	field_3C = 60;
	field_3D = 11;
	field_3E = 50;
	m_nLodDistance = 0;
	m_bLod = 0;
	m_pContactNode = nullptr;
	m_nUserType = 0x10000000;
	field_4D = field_4D & 0x88 | 8;
	field_30 = 0;
	field_18 = 0;
}