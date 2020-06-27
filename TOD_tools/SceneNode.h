#pragma once

#include "stdafx.h"
#include "Types.h"
#include "RewindBuffer.h"
#include "MemoryAllocators.h"

#define SCENENODE_CLASS_SIZE 120

class SceneNode
{
private:
	int* m_pUnkPtr;
	RewindBuffer* m_pRewindBuffer;
	int field_8[5];
	int field_20;
	void* m_pCreator;
	int field_28;
	int field_2C;
	int m_nUnkType;
	int field_34;
	int* m_pUnkBuffer;
	int m_nUnkArraySize;
	int field_40;
	int field_44;
	int* m_pUnkArray2;
	int* field_4C;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int* field_6C;
	int m_nGameTime;
	int m_nNewFrameNumber;
	int field_78;
public:
	SceneNode()
	{
		MESSAGE_CLASS_CREATED(SceneNode);

		m_pUnkPtr = nullptr;
		m_pRewindBuffer = nullptr;
		
		for (int ind = 0; ind < 5; ind++)
			field_8[ind] = 0;

		field_20 = 0;
		m_pCreator = nullptr;
		field_28 = 0;
		field_2C = 0;
		m_nUnkType = 0;
		field_34 = 0;
		m_pUnkBuffer = nullptr;
		m_nUnkArraySize = 0;
		field_40 = 0;
		field_44 = 0;
		m_pUnkArray2 = nullptr;
		field_4C = nullptr;
		field_50 = 0;
		field_54 = 0;
		field_58 = 0;
		field_5C = 0;
		field_60 = 0;
		field_64 = 0;
		field_68 = 0;
		field_6C = nullptr;
		m_nGameTime = 0;
		m_nNewFrameNumber = 0;
		field_78 = 0;
	}

	~SceneNode()
	{
		MESSAGE_CLASS_DESTROYED(SceneNode);
	}

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	void	Init();	//	@874510
	void	RegisterEntity()	//	@86C770
	{
		void(__thiscall * _Register)(void* _this) = (void(__thiscall*)(void*))0x86C770;

		_Register(this);
	}
};

extern SceneNode* g_SceneNode;

static_assert(sizeof(SceneNode) == SCENENODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SceneNode));