#include "MemoryAllocators.h"

void Allocator::RetrieveSystemAllocators()
{
	m_SystemAllocators->_malloc	= malloc;
	m_SystemAllocators->_free	= free;
}

Allocator::Allocator()
{
	MESSAGE_CLASS_CREATED(Allocator);

	m_AllocatedSpacePtr = nullptr;
	m_AllocatedSpaceSize = NULL;
	RetrieveSystemAllocators();

	field_1C = nullptr;
	field_20 = NULL;
	field_21 = NULL;
}

SystemSubAllocator::SystemSubAllocator()
{
	MESSAGE_CLASS_CREATED(SystemSubAllocator);

	m_AllocationsTotal = NULL;
}

FirstFitSubAllocator::FirstFitSubAllocator()
{
	MESSAGE_CLASS_CREATED(FirstFitSubAllocator);

	field_24 = 8;
	field_28 = NULL;
	field_2C = NULL;
	field_30 = NULL;
	field_34 = NULL;
	field_38 = NULL;
}

FrameBasedSubAllocator::FrameBasedSubAllocator()
{
	MESSAGE_CLASS_CREATED(FrameBasedSubAllocator);

	field_40 = nullptr;
}

BestFitAllocator::BestFitAllocator()
{
	MESSAGE_CLASS_CREATED(BestFitAllocator);

	int _f0 = 32;

	for (int ind = 0; ind < 20; ind++)
	{
		field_28[ind].field_0 = _f0;
		field_28[ind].field_4 = NULL;
		field_28[ind].field_8 = NULL;

		_f0 = _f0 * 1.5f;
	}

	field_10C = 0x7FFFFFFF;
	field_114 = nullptr;
	field_118 = field_11C = field_120 = 0;

	g_Allocators.m_BestFitAllocator_UnknownValue = 16;
}

SequentialSubAllocator::SequentialSubAllocator()
{
	MESSAGE_CLASS_CREATED(SequentialSubAllocator);

	field_24 = field_28 = field_2C = NULL;
	field_30 = field_34 = field_38 = field_3C = NULL;
}

PoolSubAllocator::PoolSubAllocator(int unk1, int unk2)
{
	MESSAGE_CLASS_CREATED(PoolSubAllocator);

	field_34 = unk1;
	field_30 = (unk2 + 7) & 0xFFFFFFF8;
	field_2C = NULL;
}

StackBasedSubAllocator::StackBasedSubAllocator()
{
	MESSAGE_CLASS_CREATED(StackBasedSubAllocator);

	m_StackBeginPtr = nullptr;
	field_28 = nullptr;
	m_StackEndPtr = nullptr;
	m_ElementsInStack = NULL;
	field_34 = 8;
}

Defragmentator::Defragmentator(BestFitAllocator* bestfitallocator, char unk1, int size)
{
	MESSAGE_CLASS_CREATED(Defragmentator);

	m_Size = size;
	m_DefragmentAllocator = bestfitallocator;
	m_AllocatedSpace = malloc(12 * size);
	field_20 = unk1;
	bestfitallocator->field_1C = this;
	m_DefragmentAllocator_1 = bestfitallocator;
}

void Allocators::CreateAllocators()
{
	ALLOCATOR_MAIN_ASSETS = FrameBasedSubAllocator();
	ALLOCATOR_MISSION_ASSETS = FrameBasedSubAllocator();
	ALLOCATOR_CUTSCENE_OR_REWIND = FrameBasedSubAllocator();
	ALLOCATOR_PLAYER_DATA = FrameBasedSubAllocator();
	ALLOCATOR_TEMP = FirstFitSubAllocator();
	ALLOCATOR_RENDERLIST = BestFitAllocator();
	ALLOCATOR_SCRATCHPAD = StackBasedSubAllocator();
	ALLOCATOR_COLLISION_CACHE_ENTRIES = PoolSubAllocator(48, 4);
	ALLOCATOR_DEFRAGMENTING = BestFitAllocator();
	m_Defragmentator = Defragmentator(&ALLOCATOR_DEFRAGMENTING, 1, 10000);
	ALLOCATOR_DEFRAGMENTING.field_20 = NULL;
	m_Defragmentator.field_28 = NULL;
	m_Defragmentator.field_2C = NULL;

	InitAllocator(&ALLOCATOR_CUTSCENE_OR_REWIND, CUTSCENE_OR_REWIND, "ALLOCATOR_CUTSCENE_OR_REWIND", 0x200000);
	InitAllocator(&ALLOCATOR_PLAYER_DATA, PLAYER_DATA, "ALLOCATOR_PLAYER_DATA", 0x300000);
	InitAllocator(&ALLOCATOR_MISSION_ASSETS, MISSION_ASSETS, "ALLOCATOR_MISSION_ASSETS", 0x700000);
	InitAllocator(&ALLOCATOR_MAIN_ASSETS, MAIN_ASSETS, "ALLOCATOR_MAIN_ASSETS", 0x5A00000);
	InitAllocator(&ALLOCATOR_COLLISION_CACHE_ENTRIES, COLLISION_CACHE_ENTRIES, "ALLOCATOR_COLLISION_CACHE_ENTRIES", 0x3E800);
	InitAllocator(&ALLOCATOR_DEFRAGMENTING, DEFRAGMENTING, "ALLOCATOR_DEFRAGMENTING", 0x96000);
	InitAllocator(&ALLOCATOR_RENDERLIST, RENDERLIST, "ALLOCATOR_RENDERLIST", 0xA00000);
	InitAllocator(&ALLOCATOR_TEMP, TEMP, "ALLOCATOR_TEMP", 0x40000);
	InitAllocator(&ALLOCATOR_SCRATCHPAD, SCRATCHPAD, "ALLOCATOR_SCRATCHPAD", 0x3FC0);

	_4776A0();

	ALLOCATOR_RENDERLIST.field_20 = NULL;
	ALLOCATOR_COLLISION_CACHE_ENTRIES.field_20 = NULL;
	ALLOCATOR_SCRATCHPAD.field_20 = NULL;
}

void Allocators::InitAllocator(Allocator* _alloc, int _allocindex, const char* _allocname, int _allocsize)
{
	if (!_alloc->m_SystemAllocators)
		_alloc->RetrieveSystemAllocators();

	AllocatorsList[_allocindex] = _alloc;
	_alloc->m_AllocatorIndex = _allocindex;
	BuffersPtr[_allocindex] = _alloc->m_SystemAllocators->_malloc((_allocsize + 63) & 0xFFFFFFC0);
	_alloc->SetNameAndAllocatedSpaceParams(BuffersPtr[_allocindex], _allocname, _allocsize);

	if (_alloc->field_1C)
		((Allocator*)(_alloc->field_1C))->scalar_destructor(false);

}

//	TODO: implementation!
void Allocators::_4776A0()
{
	(*(void (*)())0x4776A0)();
}

Allocators::Allocators()
{
	MESSAGE_CLASS_CREATED(Allocators);

	ALLOCATOR_DEFAULT = SystemSubAllocator();

	ALLOCATOR_DEFAULT.m_AllocatorName = "ALLOCATOR_DEFAULT";
	ALLOCATOR_DEFAULT.field_20 = NULL;
	ALLOCATOR_DEFAULT.m_AllocatorIndex = ALLOCATOR_INDEX::DEFAULT;

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

	BuffersPtr[DEFAULT] = &ALLOCATOR_DEFAULT;
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

Allocators::~Allocators()
{
	MESSAGE_CLASS_DESTROYED(Allocators);

	if (BufferPtr)
		free(BufferPtr);

	for (int ind = 0; ind < 9; ind++)
		if (AllocatorsList[ind + 1])
			AllocatorsList[ind]->m_SystemAllocators->_free(BuffersPtr[ind + 1]);

	Released = true;
}