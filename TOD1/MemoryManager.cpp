#include "MemoryManager.h"

#include "SystemSubAllocator.h"
#include "FrameBasedSubAllocator.h"
#include "FirstFitSubAllocator.h"
#include "BestFitAllocator.h"
#include "StackBasedSubAllocator.h"
#include "PoolSubAllocator.h"
#include "Defragmentator.h"

void* MemoryManager::BufferPtr;
void* MemoryManager::BuffersPtr[TOTAL];
float MemoryManager::_A3B0C8;
int MemoryManager::TotalAllocators;
int MemoryManager::_A3AFB8;
RTL_CRITICAL_SECTION MemoryManager::AllocatorsCriticalSection;
bool MemoryManager::Released;
Allocator* MemoryManager::AllocatorsList[TOTAL];
MemoryManager::Allocator_Struct2 MemoryManager::_A3AFE8[22];
MemoryManager MemoryManager::g_Allocators;

void MemoryManager::CreateAllocators()
{
	static FrameBasedSubAllocator ALLOCATOR_MAIN_ASSETS;
	static FrameBasedSubAllocator ALLOCATOR_MISSION_ASSETS;
	static FrameBasedSubAllocator ALLOCATOR_CUTSCENE_OR_REWIND;
	static FrameBasedSubAllocator ALLOCATOR_PLAYER_DATA;
	static FirstFitSubAllocator ALLOCATOR_TEMP;
	static BestFitAllocator ALLOCATOR_RENDERLIST;
	static StackBasedSubAllocator ALLOCATOR_SCRATCHPAD;
	static PoolSubAllocator ALLOCATOR_COLLISION_CACHE_ENTRIES(48, 4);
	static BestFitAllocator ALLOCATOR_DEFRAGMENTING;
	static Defragmentator DEFRAGMENTATOR(&ALLOCATOR_DEFRAGMENTING, 1, 100000);

	InitAllocator(ALLOCATOR_CUTSCENE_OR_REWIND, CUTSCENE_OR_REWIND, "ALLOCATOR_CUTSCENE_OR_REWIND", 0x200000);
	InitAllocator(ALLOCATOR_PLAYER_DATA, PLAYER_DATA, "ALLOCATOR_PLAYER_DATA", 0x300000);
	InitAllocator(ALLOCATOR_MISSION_ASSETS, MISSION_ASSETS, "ALLOCATOR_MISSION_ASSETS", 0x700000);
	InitAllocator(ALLOCATOR_MAIN_ASSETS, MAIN_ASSETS, "ALLOCATOR_MAIN_ASSETS", 0x5A00000);
	InitAllocator(ALLOCATOR_COLLISION_CACHE_ENTRIES, COLLISION_CACHE_ENTRIES, "ALLOCATOR_COLLISION_CACHE_ENTRIES", 0x3E800);
	InitAllocator(ALLOCATOR_DEFRAGMENTING, DEFRAGMENTING, "ALLOCATOR_DEFRAGMENTING", 0x96000);
	InitAllocator(ALLOCATOR_RENDERLIST, RENDERLIST, "ALLOCATOR_RENDERLIST", 0xA00000);
	InitAllocator(ALLOCATOR_TEMP, TEMP, "ALLOCATOR_TEMP", 0x40000);
	InitAllocator(ALLOCATOR_SCRATCHPAD, SCRATCHPAD, "ALLOCATOR_SCRATCHPAD", 0x3FC0);

	InitAllocatorsBuffers();

	ALLOCATOR_RENDERLIST.field_20 = NULL;
	ALLOCATOR_COLLISION_CACHE_ENTRIES.field_20 = NULL;
	ALLOCATOR_SCRATCHPAD.field_20 = NULL;
}

void MemoryManager::InitAllocator(Allocator& alloc, AllocatorIndex allocind, const char* const allocname, unsigned int allocsize)
{
	alloc.m_SystemAllocators = alloc.GetSystemAllocatorsTable();

	AllocatorsList[allocind] = &alloc;
	alloc.m_AllocatorIndex = allocind;
	BuffersPtr[allocind] = alloc.m_SystemAllocators._malloc((allocsize + 64) & 0xFFFFFFC0);
	alloc.SetNameAndAllocatedSpaceParams(BuffersPtr[allocind], allocname, allocsize);

	if (alloc.m_Defragmentator)
		delete alloc.m_Defragmentator;
}

void MemoryManager::InitAllocatorsBuffers()
{
	TotalAllocators = 0;
	unsigned char v19[12] = {};
	char* v0 = nullptr;

	for (int ind = DEFRAGMENTING; ind != 1; --ind)
	{
		void* v1 = (void*)-1;
		int v2 = -1;
		for (int currallocator = 1; currallocator < TOTAL; ++currallocator)
		{
			void* v4 = AllocatorsList[currallocator]->GetAllocatedSpacePtr();
			if (!v19[currallocator] && v4 < v1)
			{
				v1 = v4;
				v2 = currallocator;
			}
		}

		if (v1 > (void*)(((unsigned int)v0 + 7) & 0xFFFFFFF8))
		{
			_A3AFE8[TotalAllocators].m_AllocatedSpacePtr = v0;
			_A3AFE8[TotalAllocators].m_Allocator = AllocatorsList[DEFAULT];
			TotalAllocators++;
		}

		_A3AFE8[TotalAllocators].m_AllocatedSpacePtr = AllocatorsList[v2]->GetAllocatedSpacePtr();
		_A3AFE8[TotalAllocators].m_Allocator = AllocatorsList[v2];
		TotalAllocators++;

		v0 = (char*)AllocatorsList[v2]->GetAllocatedSpacePtr() + AllocatorsList[v2]->GetAllocatedSpaceSize();

		v19[v2] = 1;
	}

	_A3AFE8[TotalAllocators].m_AllocatedSpacePtr = v0;
	_A3AFE8[TotalAllocators].m_Allocator = AllocatorsList[DEFAULT];
	TotalAllocators++;
}

MemoryManager::MemoryManager()
{
	MESSAGE_CLASS_CREATED(MemoryManager);

	static SystemSubAllocator ALLOCATOR_DEFAULT;

	ALLOCATOR_DEFAULT.m_AllocatorName = "ALLOCATOR_DEFAULT";
	ALLOCATOR_DEFAULT.field_20 = NULL;
	ALLOCATOR_DEFAULT.m_AllocatorIndex = DEFAULT;

	AllocatorsList[DEFAULT] = &ALLOCATOR_DEFAULT;
	AllocatorsList[MAIN_ASSETS] = nullptr;
	AllocatorsList[MISSION_ASSETS] = nullptr;
	AllocatorsList[CUTSCENE_OR_REWIND] = nullptr;
	AllocatorsList[PLAYER_DATA] = nullptr;
	AllocatorsList[TEMP] = nullptr;
	AllocatorsList[RENDERLIST] = nullptr;
	AllocatorsList[SCRATCHPAD] = nullptr;
	AllocatorsList[COLLISION_CACHE_ENTRIES] = nullptr;
	AllocatorsList[DEFRAGMENTING] = nullptr;

	BuffersPtr[MAIN_ASSETS] = nullptr;
	BuffersPtr[MISSION_ASSETS] = nullptr;
	BuffersPtr[CUTSCENE_OR_REWIND] = nullptr;
	BuffersPtr[PLAYER_DATA] = nullptr;
	BuffersPtr[TEMP] = nullptr;
	BuffersPtr[RENDERLIST] = nullptr;
	BuffersPtr[SCRATCHPAD] = nullptr;
	BuffersPtr[COLLISION_CACHE_ENTRIES] = nullptr;
	BuffersPtr[DEFRAGMENTING] = nullptr;

	BufferPtr = malloc(1024);

	CreateAllocators();

	_A3AFB8 = 0xABCDEF;

	InitializeCriticalSection(&AllocatorsCriticalSection);
}

MemoryManager::~MemoryManager()
{
	MESSAGE_CLASS_DESTROYED(MemoryManager);

	if (BufferPtr)
		free(BufferPtr);

	for (unsigned int ind = 0; ind < 9; ++ind)
		if (AllocatorsList[ind + 1])
			AllocatorsList[ind + 1]->m_SystemAllocators._free(BuffersPtr[ind + 1]);

	Released = true;
}

char MemoryManager::_477BC0(void* ptr, int a2)
{
	if (a2 <= NULL)
		return 1;

	return GetAllocatorByMemoryPointer(ptr)->stub34((int*)ptr, a2);
}

DefragmentatorBase* MemoryManager::_4777F0(AllocatorIndex allocind)
{
	return AllocatorsList[allocind]->m_Defragmentator;
}

void MemoryManager::ReleaseMemory(void* ptr, bool aligned)
{
	if (Released)
		return;

	EnterCriticalSection(&AllocatorsCriticalSection);

	//	NOTE: figure out which allocator has allocated this memory and use it's method to free memory.
	int ind = TotalAllocators - 1;
	void* _allocspace = nullptr;
	if (ptr < _A3AFE8[TotalAllocators].m_AllocatedSpacePtr)
	{
		do
		{
			_allocspace = _A3AFE8[ind--].m_AllocatedSpacePtr;
		} while (ptr < _allocspace);
	}

	if (aligned)
		_A3AFE8[ind].m_Allocator->FreeAligned(ptr);
	else
		_A3AFE8[ind].m_Allocator->Free(ptr);

	LeaveCriticalSection(&AllocatorsCriticalSection);
}

Allocator* MemoryManager::GetAllocatorByMemoryPointer(void* ptr)
{
	int allocInd = TotalAllocators - 1;
	void* _spaceptr = nullptr;

	if (ptr < _A3AFE8[TotalAllocators].m_AllocatedSpacePtr)
	{
		do
		{
			_spaceptr = _A3AFE8[allocInd--].m_AllocatedSpacePtr;
		} while (ptr < _spaceptr);
	}

	return _A3AFE8[allocInd].m_Allocator;
}

void* MemoryManager::Realloc(void* oldptr, size_t newsize, bool a3)
{
	if (Released)
		return nullptr;

	EnterCriticalSection(&AllocatorsCriticalSection);

	Allocator* _allocator = GetAllocatorByMemoryPointer(oldptr);
	void* newptr = _allocator->Realloc(oldptr, newsize, NULL, NULL);
	if (!newptr && newsize && (_allocator->field_20 || !a3))
	{
		if (BufferPtr)
		{
			delete BufferPtr;
			BufferPtr = nullptr;
		}
	}

	LeaveCriticalSection(&AllocatorsCriticalSection);

	return newptr;
}

void* MemoryManager::AllocateByType(AllocatorIndex allocind, size_t size)
{
	if (Released)
		return nullptr;
	else
		return AllocatorsList[allocind]->Allocate_A(size, NULL, NULL);
}