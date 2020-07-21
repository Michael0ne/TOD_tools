#include "DecalManager.h"

DecalManager::DecalManager() : Node(NODE_MASK_QUADTREE)
{
	MESSAGE_CLASS_CREATED(DecalManager);

	m_ResourceInfo = nullptr;
	field_58 = 1;
	field_60 = new char[20];	//	TODO: implementation!
	m_BufferSize = 30;
	m_PerFrameFillSize = 2;
	field_50 = 0;

	m_QuadTree->m_nUserType = m_QuadTree->m_nUserType & 0xFFFFFF | m_QuadTree->m_nUserType & 0xFF000000 | 0x8000000;
}