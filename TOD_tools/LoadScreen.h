#pragma once

#include "stdafx.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"

#include "Entity.h"

#define LOADSCREEN_CLASS_SIZE 120
#define LOADSCREEN_CURRENT_CLASS_SIZE 28

class LoadScreen : public Entity
{
};

//	NOTE: not sure this class really exists.
class CurrentLoadScreen
{
public:
	void* lpVtbl;
	String m_sTexturePath;
	BYTE m_bEnabled;
	BYTE _pad1[3];
	int m_nEnabledTime;

public:
	CurrentLoadScreen()
	{
		MESSAGE_CLASS_CREATED(CurrentLoadScreen);

		lpVtbl = nullptr;
		m_sTexturePath = String();
		m_bEnabled = false;
		_pad1[0] = 0;
		_pad1[1] = 0;
		_pad1[2] = 0;
		m_nEnabledTime = 0;
	}

	~CurrentLoadScreen()
	{
		MESSAGE_CLASS_DESTROYED(CurrentLoadScreen);
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

	void	Init(const char* szTexturePath);	//	@87C230
};

extern LoadScreen* g_LoadScreen;	//	@A3D7E4
extern CurrentLoadScreen* g_CurrentLoadScreen;	//	@A3D7E0

static_assert(sizeof(LoadScreen) == LOADSCREEN_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(LoadScreen));
static_assert(sizeof(CurrentLoadScreen) == LOADSCREEN_CURRENT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(CurrentLoadScreen));