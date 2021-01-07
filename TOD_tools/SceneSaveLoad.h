#pragma once

#include "MemoryAllocators.h"

#define SCENESAVELOAD_CLASS_SIZE 120

enum ScenePlayMode
{
	MODE_NONE = 0,
	MODE_1 = 1,
	MODE_2 = 2,
	MODE_3 = 3
};

class SceneSaveLoad
{
private:
	int* field_0;
	class RewindBuffer* m_RewindBuffer_1;
	class Node* m_CurrentUndo[6];
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	ScenePlayMode m_SavedPlayMode;
	int field_34;
	int** field_38;
	int field_3C;
	int field_40;
	int field_44;
	class RewindBuffer* m_RewindBuffer_2;
	class RewindBuffer* m_RewindBuffer_3;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int m_SavedGameTimeMs;
	int m_SavedFrameNumber;
	int field_74;

public:
	SceneSaveLoad();	//	@874510
	~SceneSaveLoad();

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
	void		ResetSavedPlayMode();	//	@873B90
};

extern SceneSaveLoad* g_SceneSaveLoad;

static_assert(sizeof(SceneSaveLoad) == SCENESAVELOAD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SceneSaveLoad));