#pragma once

#include "stdafx.h"

enum KapowAllocatorTypes {
	UNSET,
	ALLOCATOR_MAIN_ASSETS,
	ALLOCATOR_MISSION_ASSETS,
	ALLOCATOR_CUTSCENE_OR_REWIND,
	ALLOCATOR_PLAYER_DATA,
	ALLOCATOR_TEMP,
	ALLOCATOR_RENDERLIST,
	ALLOCATOR_SCRATCHPAD,
	ALLOCATOR_COLLISION_CACHE_ENTRIES,
	ALLOCATOR_DEFRAGMENTING
};

class MemoryAllocators
{
private:
	//	Possible size is 59 (0x3b) bytes.
public:
	MemoryAllocators();
	~MemoryAllocators();
};

