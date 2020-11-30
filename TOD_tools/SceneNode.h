#pragma once

#include "MemoryAllocators.h"

#define SCENENODE_CLASS_SIZE 120

class SceneNode
{
private:
	int field_0;
	class RewindBuffer* m_RewindBuffer_1;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int* field_38;
	int field_3C;
	int field_40;
	int field_44;
	int* field_48;
	class RewindBuffer* m_pSavePointSummary;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	class Entity* field_6C;
	int m_nGameTime;
	int m_nNewFrameNumber;

public:
	SceneNode();	//	@874510
	~SceneNode()
	{
		MESSAGE_CLASS_DESTROYED(SceneNode);
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void		_874940();	//	@874940
};

extern SceneNode* tSceneNode;

static_assert(sizeof(SceneNode) == SCENENODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SceneNode));