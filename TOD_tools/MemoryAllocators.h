#pragma once

#include "stdafx.h"

enum KapowAllocatorTypes {
	ALLOCATOR_MAIN_ASSETS,
	ALLOCATOR_MISSION_ASSETS,
	ALLOCATOR_CUTSCENE_OR_REWIND,
	ALLOCATOR_PLAYER_DATA,
	ALLOCATOR_TEMP,
	ALLOCATOR_RENDERLIST,
	ALLOCATOR_SCRATCHPAD,
	ALLOCATOR_COLLISION_CACHE_ENTRIES,
	ALLOCATOR_DEFRAGMENTING,
	ALLOCATORS_TOTAL
};

//	Usage: g_pAllocator[ALLOCATOR_TEMP]->allocate(size, filler_value, unk);
//	TODO: DWORD* should really be replaced with 'MemoryAllocator' class, once it's finished.
static DWORD* g_pAllocator[9] = {
	(DWORD*)0xA3AFC4,
	(DWORD*)0xA3AFC8,
	(DWORD*)0xA3AFCC,
	(DWORD*)0xA3AFD0,
	(DWORD*)0xA3AFD4,
	(DWORD*)0xA3AFD8,
	(DWORD*)0xA3AFDC,
	(DWORD*)0xA3AFE0,
	(DWORD*)0xA3AFE4
};

//	There are 9 'types' of allocator classes.
class MemoryAllocators
{
private:
	//	Possible size is 59 (0x3b) bytes.
public:
	MemoryAllocators();
	~MemoryAllocators();
};

