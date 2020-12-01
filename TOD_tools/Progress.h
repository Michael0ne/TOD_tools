#pragma once

#include "List.h"
#include "Performance.h"

#define PROGRESS_CLASS_SIZE 112
#define PROGRESS_BASE_CLASS_SIZE 72

class ProgressBase
{
private:
	int field_4;
	List<String> m_StatesStringsList;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int m_TimeStartHi;
	int m_TimeStartLo;

public:
	virtual void	stub1(int) {};	//	NOTE: not used anywhere and actual function has nothing in it.
	virtual void	UpdateProgress(float time, bool) {};

	ProgressBase(INT64 timeStart);	//	@40E900
	~ProgressBase();
};

class Progress : public ProgressBase
{
private:
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	class Entity* m_LoadScreenSprite;
	class GfxInternal_Dx9_Texture* m_Texture;
	int	m_SpriteWidth;
	int m_SpriteHeight;
	bool m_Enabled;
	int field_6C;

public:
	virtual void	UpdateProgress(float time, bool);	//	@87B830

	Progress();		//	@87B720
	~Progress();

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}
};

extern Progress* g_Progress;	//	@A3D7D0
extern ProgressBase* g_ProgressBase;	//	@A35B88

static_assert(sizeof(ProgressBase) == PROGRESS_BASE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(ProgressBase));
static_assert(sizeof(Progress) == PROGRESS_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Progress));