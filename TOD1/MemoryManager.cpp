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
int MemoryManager::AllocatorsBuffersTotal;
int MemoryManager::_A3AFB8;
RTL_CRITICAL_SECTION MemoryManager::AllocatorsCriticalSection;
bool MemoryManager::Released;
Allocator* MemoryManager::AllocatorsList[TOTAL];
MemoryManager::AllocatorBufferData MemoryManager::AllocatorsBuffersData[22];
MemoryManager MemoryManager::Instance;

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
    const uint32_t alignedSize = ALIGN_64BYTESUP(allocsize);
    alloc.LowLevelAllocators = alloc.GetSystemAllocatorsTable();

    AllocatorsList[allocind] = &alloc;
    alloc.AllocatorIndex = allocind;
    BuffersPtr[allocind] = alloc.LowLevelAllocators._malloc(alignedSize);

#ifdef INCLUDE_FIXES
    memset(BuffersPtr[allocind], NULL, alignedSize);
#endif

    alloc.SetNameAndAllocatedSpaceParams(BuffersPtr[allocind], allocname, allocsize);
}

void MemoryManager::InitAllocatorsBuffers()
{
    bool allocatorChecked[TOTAL] = {};
    uint32_t* spacePtr = nullptr;
    uint32_t allocatorIndex = 9;

    AllocatorsBuffersTotal = 0;

    do
    {
        uint32_t* lowestPtr = (uint32_t*)-1;
        int32_t index = -1;

        for (uint32_t i = 1; i < TOTAL; ++i)
        {
            uint32_t* allocatorBufferPtr = (uint32_t*)AllocatorsList[i]->GetAllocatedSpacePtr();
            if (!allocatorChecked[i] && allocatorBufferPtr < lowestPtr)
            {
                lowestPtr = allocatorBufferPtr;
                index = i;
            }
        }

        if ((uint32_t)lowestPtr > ALIGN_8BYTESUP(spacePtr))
        {
            AllocatorsBuffersData[AllocatorsBuffersTotal].m_AllocatedSpacePtr = spacePtr;
            AllocatorsBuffersData[AllocatorsBuffersTotal].m_Allocator = AllocatorsList[DEFAULT];

            AllocatorsBuffersTotal++;
        }

        AllocatorsBuffersData[AllocatorsBuffersTotal].m_AllocatedSpacePtr = AllocatorsList[index]->GetAllocatedSpacePtr();
        AllocatorsBuffersData[AllocatorsBuffersTotal].m_Allocator = AllocatorsList[index];

        AllocatorsBuffersTotal++;

        spacePtr = (uint32_t*)((uint8_t*)AllocatorsList[index]->GetAllocatedSpacePtr() + AllocatorsList[index]->GetAllocatedSpaceSize());

        allocatorChecked[index] = true;
    } while (allocatorIndex-- != 1);

    AllocatorsBuffersData[AllocatorsBuffersTotal].m_AllocatedSpacePtr = spacePtr;
    AllocatorsBuffersData[AllocatorsBuffersTotal].m_Allocator = AllocatorsList[DEFAULT];

    AllocatorsBuffersTotal++;
}

MemoryManager::MemoryManager()
{
    MESSAGE_CLASS_CREATED(MemoryManager);

    static SystemSubAllocator ALLOCATOR_DEFAULT;

    ALLOCATOR_DEFAULT.AllocatorName = "ALLOCATOR_DEFAULT";
    ALLOCATOR_DEFAULT.field_20 = NULL;
    ALLOCATOR_DEFAULT.AllocatorIndex = DEFAULT;

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
            AllocatorsList[ind + 1]->LowLevelAllocators._free(BuffersPtr[ind + 1]);

    Released = true;
}

char MemoryManager::CheckIfCanExpandMemoryBySize(void* ptr, int a2)
{
    if (a2 <= NULL)
        return 1;

    return GetAllocatorByMemoryPointer(ptr)->TryExpandBy((uint8_t*)ptr, a2);
}

DefragmentatorBase* MemoryManager::GetDefragmentator(AllocatorIndex allocind)
{
    //  NOTE: this is always called with the 'DEFRAGMENTATOR' allocator index.
    return AllocatorsList[allocind]->Defragmentator;
}

void MemoryManager::ReleaseMemory(void* ptr, bool aligned)
{
    if (Released)
        return;

#ifdef INCLUDE_FIXES
    if (!ptr)
        return;
#endif

    EnterCriticalSection(&AllocatorsCriticalSection);

    // NOTE: figure out which allocator has allocated this memory and use it's method to free memory.
    int ind = AllocatorsBuffersTotal - 1;
    void* _allocspace = nullptr;
    if (ptr < AllocatorsBuffersData[ind].m_AllocatedSpacePtr)
    {
        do
        {
            _allocspace = AllocatorsBuffersData[--ind].m_AllocatedSpacePtr;
        } while (ptr < _allocspace);
    }

    if (aligned)
        AllocatorsBuffersData[ind].m_Allocator->FreeAligned(ptr);
    else
        AllocatorsBuffersData[ind].m_Allocator->Free(ptr);

    LeaveCriticalSection(&AllocatorsCriticalSection);
}

Allocator* MemoryManager::GetAllocatorByMemoryPointer(void* ptr)
{
    int allocInd = AllocatorsBuffersTotal - 1;
    void* _spaceptr = nullptr;

    if (ptr < AllocatorsBuffersData[AllocatorsBuffersTotal].m_AllocatedSpacePtr)
    {
        do
        {
            _spaceptr = AllocatorsBuffersData[allocInd--].m_AllocatedSpacePtr;
        } while (ptr < _spaceptr);
    }

    return AllocatorsBuffersData[allocInd].m_Allocator;
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