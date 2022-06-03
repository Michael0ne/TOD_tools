#include "Defragmentator.h"
#include "MemoryManager.h"
#include "Timer.h"
#include "LogDump.h"
#include "ScriptThread.h"

void DefragmentatorBase::MarkSpaceAsOccupied(int chunkind)
{
    m_AllocatedSpace[chunkind].m_Flags.Occupied = true;

    if (m_AllocatedSpace[chunkind].field_8 != -1)
        m_AllocatedSpace[m_AllocatedSpace[chunkind].field_8].m_ScriptThreadPtr = m_AllocatedSpace[chunkind].m_ScriptThreadPtr;

    if (m_AllocatedSpace[chunkind].m_ScriptThreadPtr == (ScriptThread*) - 1)
        m_LatestUsedIndex = m_AllocatedSpace[chunkind].field_8;
    else
        m_AllocatedSpace[(int)m_AllocatedSpace[chunkind].m_ScriptThreadPtr].field_8 = m_AllocatedSpace[chunkind].field_8;

    --m_ChunksAvailable;
}

void DefragmentatorBase::MarkSpaceAsUnoccupied(int chunkind)
{
    if (m_LatestUsedIndex == -1)
        m_AllocatedSpace[m_LatestUsedIndex].m_ScriptThreadPtr = (ScriptThread*)chunkind;

    int field_18_old = m_LatestUsedIndex;
    int chunkid = NULL;

    m_LatestUsedIndex = chunkind;
    m_AllocatedSpace[chunkind].m_Flags.Occupied = false;
    m_AllocatedSpace[chunkind].m_ScriptThreadPtr = (ScriptThread*) - 1;
    m_AllocatedSpace[chunkind].field_8 = field_18_old;
    ++m_ChunksAvailable;
}

void DefragmentatorBase::FreeChunk(int chunkind)
{
    if (m_AllocatedSpace[chunkind].m_ScriptThreadPtr)
    {
        if (m_AllocatedSpace[chunkind].m_Flags._1 == 0)
            delete m_AllocatedSpace[chunkind].m_ScriptThreadPtr;

        MemoryManager::ReleaseMemory((void*)m_AllocatedSpace[chunkind].m_ScriptThreadPtr, m_AllocatedSpace[chunkind].m_Flags.Alignment != 0);
        m_AllocatedSpace[chunkind].m_ScriptThreadPtr = nullptr;
        m_AllocatedSpace[chunkind].m_Flags.Size = NULL;
    }
}

DefragmentatorBase::DefragmentatorBase(BestFitAllocator* allocator, char a2, int size)
{
    MESSAGE_CLASS_CREATED(DefragmentatorBase);

    m_Size = size;
    m_Allocator = allocator;
    m_AllocatedSpace = new Defragmentator_Space[size];
    field_20 = a2;
    allocator->Defragmentator = this;
    m_DefaultSelectedAllocator = allocator;
}

DefragmentatorBase::~DefragmentatorBase()
{
    MESSAGE_CLASS_DESTROYED(DefragmentatorBase);

    unsigned int chunkid = NULL;
    int defrname = NULL;

    m_AllocatedSpace->m_Flags.FlagsDw = 0xC0000003;
    m_AllocatedSpace->m_ScriptThreadPtr = (ScriptThread*) - 1;
    m_AllocatedSpace->field_8 = defrname;

    field_14 = NULL;
    m_LatestUsedIndex = -1;

    if (m_Size > 1)
    {
        for (unsigned int i = 1; i < m_Size; ++i)
        {
            if (m_LatestUsedIndex != -1)
                m_AllocatedSpace[m_LatestUsedIndex].m_ScriptThreadPtr = (ScriptThread*)i;
            m_LatestUsedIndex = i;
            m_AllocatedSpace[i].m_Flags.Occupied = false;
            m_AllocatedSpace[i].m_ScriptThreadPtr = (ScriptThread*) - 1;
            m_AllocatedSpace[i].field_8 = m_LatestUsedIndex;

            ++m_ChunksAvailable;
        }
    }

    m_ChunksAvailable = m_Size - 1;
}

int DefragmentatorBase::Allocate(int size, int a2, int a3, int a4, int alignment, char a6)
{
    if (MemoryManager::Released)
        return NULL;

    void* space = alignment ? MemoryManager::AllocatorsList[m_Allocator->AllocatorIndex]->AllocateAligned(size, alignment, NULL, NULL) : MemoryManager::AllocatorsList[m_Allocator->AllocatorIndex]->Allocate_A(size, NULL, NULL);
    if (!space)
        return NULL;

    MarkSpaceAsOccupied(m_LatestUsedIndex);

    m_AllocatedSpace[m_LatestUsedIndex].m_Flags.Occupied = true;
    m_AllocatedSpace[m_LatestUsedIndex].m_Flags.Alignment = alignment * 4;
    m_AllocatedSpace[m_LatestUsedIndex].m_Flags.Size = size;
    m_AllocatedSpace[m_LatestUsedIndex].m_ScriptThreadPtr = (ScriptThread*)space;
    m_AllocatedSpace[m_LatestUsedIndex].field_8 = NULL;
    field_14 = m_LatestUsedIndex;

    return m_LatestUsedIndex;
}

int DefragmentatorBase::Reallocate(int chunkind, int a2, const char* const a3, int a4, unsigned int size)
{
    void* space;

    if (m_AllocatedSpace[chunkind].m_ScriptThreadPtr)
        space = MemoryManager::Realloc((void*)m_AllocatedSpace[chunkind].m_ScriptThreadPtr, size, true);
    else
        if (MemoryManager::Released)
            space = nullptr;
        else
            space = MemoryManager::AllocatorsList[m_Allocator->AllocatorIndex]->Allocate_A(size, NULL, NULL);

    if (space)
    {
        m_AllocatedSpace[chunkind].m_ScriptThreadPtr = (ScriptThread*)space;
        m_AllocatedSpace[chunkind].m_Flags.Size = size;

        return chunkind;
    }
    else
    {
        MarkSpaceAsUnoccupied(chunkind);
        return NULL;
    }
}

char DefragmentatorBase::CanExpand(const int chunkindex, const int newsize)
{
    return MemoryManager::CheckIfCanExpandMemoryBySize((void*)(m_AllocatedSpace[chunkindex].m_ScriptThreadPtr), newsize);
}

void DefragmentatorBase::Deallocate(int chunkind)
{
    if (chunkind)
    {
        FreeChunk(chunkind);
        MarkSpaceAsUnoccupied(chunkind);
    }
}

int DefragmentatorBase::CheckFlag(int chunkind)
{
    return m_AllocatedSpace[chunkind].m_Flags._1 == 1;
}

void DefragmentatorBase::stub6()
{
    return;
}

void DefragmentatorBase::stub7()
{
    return;
}

void Defragmentator::Defragment()
{
    stub6();
    LogDump::LogA("Defragmenting memory...\n");
    LogDump::LogA("Before: Largest block: %d, Free mem: %d, Spilled allocs: %d\n", m_Allocator->stub21(), m_Allocator->GetAvailableMemory(), m_SpilledAllocs);

    DefragmentMemory();

    LogDump::LogA("After: Largest block: %d, Free mem: %d, Spilled allocs: %d\n", m_Allocator->stub21(), m_Allocator->GetAvailableMemory(), m_SpilledAllocs);
    stub6();
}

#pragma message(TODO_IMPLEMENTATION)
void Defragmentator::DefragmentMemory()
{
}

int Defragmentator::Allocate(int size, int a2, int a3, int a4, int alignment, char a6)
{
    int chunkind = DefragmentatorBase::Allocate(size, a2, a3, a4, alignment, a6);
    if (!chunkind && size > 0)
    {
        Allocator* selectedAllocator = m_Allocator;
        m_Allocator = MemoryManager::AllocatorsList[DEFAULT];

        chunkind = DefragmentatorBase::Allocate(size, a2, a3, a4, alignment, a6);
        m_SpilledAllocs++;

        m_Allocator = selectedAllocator;
    }

    return chunkind;
}

int Defragmentator::Reallocate(int chunkind, int a2, const char* const a3, int a4, unsigned int newsize)
{
    Allocator* alloc = MemoryManager::GetAllocatorByMemoryPointer((void*)m_AllocatedSpace[chunkind].m_ScriptThreadPtr);
    if (alloc != m_Allocator || CanExpand(chunkind, newsize))
    {
        int newchunkind = DefragmentatorBase::Reallocate(chunkind, a2, a3, a4, newsize);
        if (chunkind && alloc != m_Allocator && newchunkind != chunkind)
            m_SpilledAllocs--;

        return newchunkind;
    }
    else
    {
        Allocator* selectedAllocator = m_Allocator;
        m_Allocator = MemoryManager::AllocatorsList[DEFAULT];

        int newchunkind = DefragmentatorBase::Allocate(newsize, a2, (int)a3, a4, 0, true);
        m_SpilledAllocs++;

        m_Allocator = selectedAllocator;

        if (chunkind)
        {
            memcpy(m_AllocatedSpace[newchunkind].m_ScriptThreadPtr, m_AllocatedSpace[chunkind].m_ScriptThreadPtr, m_AllocatedSpace[chunkind].m_Flags.Size);
            Deallocate(chunkind);
        }

        return newchunkind;
    }
}

void Defragmentator::Deallocate(int chunkind)
{
    if (chunkind <= 0)
        return;

    Allocator* alloc = MemoryManager::GetAllocatorByMemoryPointer((void*)m_AllocatedSpace[chunkind].m_ScriptThreadPtr);
    DefragmentatorBase::Deallocate(chunkind);

    if (alloc != m_Allocator)
        m_SpilledAllocs--;
}

void Defragmentator::stub7()
{
}

void Defragmentator::DefragmentIfNecessary()
{
    if (field_2C || m_SpilledAllocs > 0 &&
        (Timer::GetMilliseconds() - m_LastDefragmentationTime) > 10000)
    {
        Defragment();
        field_2C = NULL;
    }
}

int Defragmentator::FindScriptThreadSpaceIndex(ScriptThread* scriptThread) const
{
    if (m_AllocatedSpace[field_14].m_ScriptThreadPtr == scriptThread)
        return field_14;

    if (m_Size <= 0)
        return 0;

    unsigned int spaceIndex;
    for (spaceIndex = 0; m_AllocatedSpace[spaceIndex].m_ScriptThreadPtr != scriptThread; ++spaceIndex);

    return spaceIndex >= m_Size ? 0 : spaceIndex;
}