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

void Allocator::scalar_destructor(bool freeMemory)
{
	if (freeMemory)
		Allocators::ReleaseMemory(this, false);
}

void* Allocator::Allocate(size_t size, int filler, int unk)
{
	return Allocate_A(size, filler, unk);
}

int Allocator::stub8(int unk)
{
	return -1;
}

void Allocator::CallMethodAtOffset20()
{
	stub9();
}

void Allocator::SetField21(char unk)
{
	field_21 = unk;
}

void Allocator::SetNameAndAllocatedSpaceParams(void* _allocbufferptr, const char* _allocname, int _allocsize)
{
	m_AllocatedSpacePtr = _allocbufferptr;
	m_AllocatedSpaceSize = _allocsize;
	m_AllocatorName = _allocname;
}

void* Allocator::GetAllocatedSpacePtr()
{
	return m_AllocatedSpacePtr;
}

int Allocator::GetAllocatedSpaceSize()
{
	return m_AllocatedSpaceSize;
}

int Allocator::GetTotalAllocations()
{
	return 0;
}

int Allocator::stub16()
{
	return 0;
}

const char* Allocator::GetAllocatorName()
{
	return "Unknown";
}

void Allocator::SetFieldC(char unk)
{
	field_C = unk;
}

int Allocator::stub19()
{
	return -1;
}

int Allocator::stub20()
{
	return -1;
}

int Allocator::stub21()
{
	return -1;
}

int Allocator::GetAvailableMemory()
{
	return -1;
}

void Allocator::Dump()
{
	return;
}

int Allocator::stub23(int, int, int, int)
{
	return 0;
}

int Allocator::stub24(int, int, int, int, int)
{
	return 0;
}

int Allocator::stub25()
{
	return 0;
}

int Allocator::stub26()
{
	return 0;
}

int Allocator::stub27(int)
{
	return 0;
}

int Allocator::stub28(int)
{
	return 0;
}

char Allocator::stub29(int)
{
	return 0;
}

char Allocator::stub30(int, int, int)
{
	return 0;
}

int Allocator::stub31(int)
{
	return 0;
}

int Allocator::stub32(int)
{
	return 0;
}

char Allocator::stub33(int, int)
{
	return 0;
}

void Allocator::stub34()
{
	return;
}

SystemSubAllocator::SystemSubAllocator()
{
	MESSAGE_CLASS_CREATED(SystemSubAllocator);

	m_AllocationsTotal = NULL;
}

void* SystemSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	++m_AllocationsTotal;

	if (field_C)
		stub9();

	return malloc(size);
}

void* SystemSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	++m_AllocationsTotal;

	if (field_C)
		stub9();

	if (alignment > 1)
		return (*(void* (__cdecl*)(size_t, size_t, size_t))0x952ED4)(size, alignment, NULL);
	else
		return malloc(size);
}

void SystemSubAllocator::Free(void* ptr)
{
	if (!ptr)
		return;

	--m_AllocationsTotal;
	free(ptr);
}

void SystemSubAllocator::FreeAligned(void* ptr)
{
	if (!ptr)
		return;

	--m_AllocationsTotal;
	free((void*)(((unsigned int)ptr & 0xFFFFFFFC) - 4));
}

void* SystemSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	if (oldptr)
	{
		if (field_C)
			stub9();

		if (newsize)
			return realloc(oldptr, newsize);

		free(oldptr);
		--m_AllocationsTotal;

		return nullptr;
	}else{
		++m_AllocationsTotal;

		if (field_C)
			stub9();

		return malloc(newsize);
	}
}

int SystemSubAllocator::stub8(int unk)
{
	return -1;
}

void SystemSubAllocator::stub9()
{
	return;
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

void* FirstFitSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 4, filler, unk);
}

//	TODO: implementation!
void* FirstFitSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	return (*(void* (__thiscall*)(FirstFitSubAllocator*, size_t, size_t, int, int))0x479C40)(this, size, alignment, filler, unk);
}

//	TODO: implementation!
void FirstFitSubAllocator::Free(void* ptr)
{
	(*(void(__thiscall*)(FirstFitSubAllocator*, void*))0x479A10)(this, ptr);
}

void FirstFitSubAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

//	TODO: implementation!
void* FirstFitSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	return (*(void* (__thiscall*)(FirstFitSubAllocator*, void*, size_t, int, int))0x479DA0)(this, oldptr, newsize, filler, unk);
}

//	TODO: implementation!
int FirstFitSubAllocator::stub8(int unk)
{
	return (*(int(__thiscall*)(FirstFitSubAllocator*, int))0x479AA0)(this, unk);
}

void FirstFitSubAllocator::stub9()
{
	return;
}

FrameBasedSubAllocator::FrameBasedSubAllocator()
{
	MESSAGE_CLASS_CREATED(FrameBasedSubAllocator);

	field_40 = nullptr;
}

void* FrameBasedSubAllocator::Allocate(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 4, filler, unk);
}

void* FrameBasedSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 4, filler, unk);
}

//	TODO: implementation!
void* FrameBasedSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	return (*(void* (__thiscall*)(FrameBasedSubAllocator*, size_t, size_t, int, int))0x479F60)(this, size, alignment, filler, unk);
}

//	TODO: implementation!
void FrameBasedSubAllocator::Free(void* ptr)
{
	(*(void(__thiscall*)(FrameBasedSubAllocator*, void*))0x479FF0)(this, ptr);
}

void FrameBasedSubAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

void* FrameBasedSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	return ((SequentialSubAllocator*)this)->Realloc(oldptr, newsize, filler, unk);
}

int FrameBasedSubAllocator::stub8(int unk)
{
	return 0;
}

void FrameBasedSubAllocator::stub9()
{
	((SequentialSubAllocator*)this)->stub9();
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

		_f0 = _f0 * (float)1.5f;
	}

	field_10C = 0x7FFFFFFF;
	field_114 = nullptr;
	field_118 = field_11C = field_120 = 0;

	g_Allocators.m_BestFitAllocator_UnknownValue = 16;
}

void* BestFitAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 8, filler, unk);
}

//	TODO: implementation!
void* BestFitAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	return (*(void* (__thiscall*)(BestFitAllocator*, size_t, size_t, int, int))0x478820)(this, size, alignment, filler, unk);
}

//	TODO: implementation!
void BestFitAllocator::Free(void* ptr)
{
	(*(void(__thiscall*)(BestFitAllocator*, void*))0x479210)(this, ptr);
}

void BestFitAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

//	TODO: implementation!
void* BestFitAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	return (*(void* (__thiscall*)(BestFitAllocator*, void*, size_t, int, int))0x4795C0)(this, oldptr, newsize, filler, unk);
}

int BestFitAllocator::stub8(int unk)
{
	//	NOTE: this is reconstructed assembly code.
	if (!unk)
		return unk;

	int* v1 = (int*)(unk - 2);
	int result = *v1;

	if (result < (int)v1)
		result = m_AllocatedSpaceSize + (int)m_AllocatedSpacePtr;
	else
		result = (result - (int)v1) - 8;

	return result;
}

void BestFitAllocator::stub9()
{
	return;
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

PoolSubAllocator::PoolSubAllocator()
{
	MESSAGE_CLASS_CREATED(PoolSubAllocator);
}

//	TODO: implementation!
void* PoolSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return (*(void* (__thiscall*)(PoolSubAllocator*, size_t, int, int))0x47A1F0)(this, size, filler, unk);
}

void* PoolSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	return alignment == field_30 ? Allocate_A(size, filler, unk) : nullptr;
}

//	TODO: implementation!
void PoolSubAllocator::Free(void* ptr)
{
	(*(void(__thiscall*)(PoolSubAllocator*, void*))0x47A260)(this, ptr);
}

void PoolSubAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

void* PoolSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	stub9();

	if (newsize)
		return oldptr;

	Free(oldptr);

	return nullptr;
}

int PoolSubAllocator::stub8(int unk)
{
	return field_3C;
}

void PoolSubAllocator::stub9()
{
	return;
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

void* StackBasedSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 8, filler, unk);
}

//	TODO: implementation!
void* StackBasedSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	return (*(void* (__thiscall*)(StackBasedSubAllocator*, size_t, size_t, int, int))0x47A930)(this, size, alignment, filler, unk);
}

void StackBasedSubAllocator::Free(void* ptr)
{
	//	NOTE: this is reconstructed assembly code.
	if (!ptr)
		return;

	--m_ElementsInStack;
	int* v1 = (int*)m_StackBeginPtr;
	v1 = (int*)(*v1);
	m_StackBeginPtr = v1;
	v1[1] = NULL;
}

void StackBasedSubAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

//	TODO: implementation!
void* StackBasedSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	return (*(void* (__thiscall*)(StackBasedSubAllocator*, void*, size_t, int, int))0x47AA20)(this, oldptr, newsize, filler, unk);
}

int StackBasedSubAllocator::stub8(int unk)
{
	//	NOTE: this is reconstructed assembly code.
	int result = field_34;
	int* v1 = (int*)unk;
	v1 = v1 - field_34;
	result = v1[1] - (int)v1 - 3;

	return result;
}

//	TODO: implementation!
void StackBasedSubAllocator::stub9()
{
	(*(void(__thiscall*)(StackBasedSubAllocator*))0x47AAC0)(this);
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

Defragmentator::Defragmentator()
{
	MESSAGE_CLASS_CREATED(Defragmentator);
}

SingletonSubAllocator::SingletonSubAllocator()
{
	MESSAGE_CLASS_CREATED(SingletonSubAllocator);

	field_24 = NULL;
	field_28 = NULL;
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
	(*(void(__cdecl*)())0x4776A0)();
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

//	NOTE: this is an abstract function to free memory by pointer using according allocator method.
void Allocators::ReleaseMemory(void* ptr, bool aligned)
{
	if (Released)
		return;

	EnterCriticalSection(&AllocatorsCriticalSection);

	int allocInd = TotalAllocators - 1;
	void* _alloc = nullptr;

	if (ptr < AllocatorsList[TotalAllocators]->field_1C)
	{
		do {
			_alloc = AllocatorsList[allocInd--]->field_1C;
		} while (ptr < _alloc);
	}

	if (aligned)
		_A3AFE8[allocInd].m_Allocator->FreeAligned(ptr);
	else
		_A3AFE8[allocInd].m_Allocator->Free(ptr);

	LeaveCriticalSection(&AllocatorsCriticalSection);
}

Allocator* Allocators::GetAllocatorByMemoryPointer(void* ptr)
{
	int allocInd = TotalAllocators - 1;

	if (ptr < AllocatorsList[TotalAllocators]->field_1C)
		while (ptr < AllocatorsList[allocInd--]->field_1C);

	return _A3AFE8[allocInd].m_Allocator;
}

Allocator* Allocators::AllocatorsList[TOTAL] = {
	(Allocator*)0xA3AFC0,
	(Allocator*)0xA3AFC4,
	(Allocator*)0xA3AFC8,
	(Allocator*)0xA3AFCC,
	(Allocator*)0xA3AFD0,
	(Allocator*)0xA3AFD4,
	(Allocator*)0xA3AFD8,
	(Allocator*)0xA3AFDC,
	(Allocator*)0xA3AFE0,
	(Allocator*)0xA3AFE4
};

int& Allocators::TotalAllocators = *(int*)0xA3B098;
void* Allocators::BufferPtr = (void*)0xA3B09C;
float& Allocators::_A3B0C8 = *(float*)0xA3B0C8;
RTL_CRITICAL_SECTION& Allocators::AllocatorsCriticalSection = *(RTL_CRITICAL_SECTION*)0xA3AFA0;
int& Allocators::_A3AFB8 = *(int*)0xA3AFB8;
bool& Allocators::Released = *(bool*)0xA3AFBC;