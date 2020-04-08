#include "SceneNode.h"

#include "MemoryAllocators.h"

SceneNode* g_SceneNode = NULL;

void SceneNode::Init()
{
	patch(0xA3D7B8, this, 4);

	field_34 = 0;
	field_44 = 0;
	m_nUnkArraySize = 0;
	field_40 = 0;
	field_4C = 0;

	if (!Allocators::Released)
		if (m_pRewindBuffer = (RewindBuffer*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(sizeof(RewindBuffer)))
			m_pRewindBuffer->Init(REWINDBUFFER_DEFAULT_SIZE);
}