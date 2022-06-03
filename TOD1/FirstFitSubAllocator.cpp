#include "FirstFitSubAllocator.h"
#include "LogDump.h"

bool FirstFitSubAllocator::_4798D0(FirstFitElement* element)
{
    bool result = false;

    while (!element->m_Next->m_SpacePtr)
    {
        if (element >= element->m_Next)
            break;

        result = true;

        if (m_Space_1 == element->m_Next)
            m_Space_1 = element;

        FirstFitElement* nextElement = element->m_Next;
        element->m_Next = element->m_Next->m_Next;

        if (ProfilerEnabled)
            memset(nextElement, 0xAB, field_24);

        --m_FreeRegions;
    }

    if (m_Space == element->m_Next && !m_Space->m_SpacePtr && element != m_Space)
        _4798D0(m_Space);

    return result;
}

FirstFitSubAllocator::FirstFitSubAllocator()
{
    MESSAGE_CLASS_CREATED(FirstFitSubAllocator);

    field_24 = 8;
    m_Space = NULL;
    m_SpaceOccupied = NULL;
    m_FreeRegions = NULL;
    m_UsedRegions = NULL;
    m_Space_1 = NULL;
}

void* FirstFitSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    return AllocateAligned(size, 8, fileName, fileLineNumber);
}

#pragma message(TODO_IMPLEMENTATION)
void* FirstFitSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    if (ProfilerEnabled)
        stub9();

    if (alignment < AlignmentDefault)
        alignment = AlignmentDefault;

    int firstFreeElement = -1;
    FirstFitElement* originalElement = m_Space_1;
    while (true)
    {
        FirstFitElement* element1 = m_Space_1;
        if (!element1->m_SpacePtr)
        {
            if (_4798D0(m_Space_1))
            {
                if (m_Space_1->m_Next <= m_Space)
                {
                    if (originalElement > m_Space_1)
                        originalElement = m_Space;
                }
                else
                    if (originalElement > m_Space_1 && originalElement < m_Space_1->m_Next)
                        originalElement = m_Space_1;
            }

            element1 = m_Space_1;
            FirstFitElement* nextElement = element1->m_Next;

            if (element1->m_Next <= element1)
                nextElement = (FirstFitElement*)((char*)AllocatedSpacePtr + AllocatedSpaceSize);

            firstFreeElement = nextElement - field_24 - element1;
        }
    }
}

#pragma message(TODO_IMPLEMENTATION)
void FirstFitSubAllocator::Free(void* ptr)
{
    if (ProfilerEnabled)
        stub9();

    if (!ptr)
        return;
}

void FirstFitSubAllocator::FreeAligned(void* ptr)
{
    Free(ptr);
}

#pragma message(TODO_IMPLEMENTATION)
void* FirstFitSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    if (!oldptr)
        return Allocate_A(newsize, fileName, fileLineNumber);

    if (!newsize)
    {
        Free(oldptr);
        return nullptr;
    }

    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
int FirstFitSubAllocator::stub8(int* unk)
{
    return NULL;
}

void FirstFitSubAllocator::stub9()
{
    return;
}

void FirstFitSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

    m_Space = (FirstFitElement*)(((int)((char*)AllocatedSpacePtr + field_24 + 7) & 0xFFFFFFF8) - field_24);
    m_Space->m_SpacePtr = nullptr;

    m_Space->m_Next = m_Space;
    m_Space_1 = m_Space;
    m_Space_1 = m_Space;
    m_SpaceOccupied = NULL;
    m_FreeRegions = 1;
    m_UsedRegions = 0;

    if (ProfilerEnabled)
        memset((int*)m_Space + 2, 0xAB, (char*)AllocatedSpacePtr + AllocatedSpaceSize - (char*)m_Space->m_SpacePtr);
}

const int FirstFitSubAllocator::GetTotalAllocations() const
{
    return m_SpaceOccupied;
}

const char* const FirstFitSubAllocator::GetAllocatorName() const
{
    return "FirstFitSubAllocator";
}

const int FirstFitSubAllocator::stub19() const
{
    return m_UsedRegions;
}

const int FirstFitSubAllocator::stub20() const
{
    return m_FreeRegions;
}

#pragma message(TODO_IMPLEMENTATION)
const int FirstFitSubAllocator::stub21() const
{
    return NULL;
}

const int FirstFitSubAllocator::GetAvailableMemory() const
{
    FirstFitElement* spaceelement = m_Space;
    int result = NULL;

    do
    {
        if (!spaceelement->m_SpacePtr)
            result = spaceelement->m_Next <= spaceelement ? (int)((char*)AllocatedSpacePtr + AllocatedSpaceSize - field_24 - (int)spaceelement) : (int)((char*)spaceelement->m_Next - field_24 - (int)spaceelement);

        spaceelement = spaceelement->m_Next;
    } while (m_Space != spaceelement);

    return result;
}

void FirstFitSubAllocator::Dump() const
{
    LogDump::LogA("FirstFitAllocator contents:\n");
    LogDump::LogA("-----------------------------\n");

    FirstFitElement* spaceel = m_Space;
    for (int i = 0; i < (m_FreeRegions + m_UsedRegions); ++i)
    {
        int spacesize = (int)spaceel->m_Next;
        if (spaceel->m_Next <= spaceel)
            spacesize = (int)((char*)AllocatedSpacePtr + AllocatedSpaceSize);

        LogDump::LogA("%d\t: %x\t : %s\t : %d\n", i, spaceel, spaceel->m_SpacePtr ? "used" : "free", spacesize - field_24 - (int)spaceel);

        spaceel = spaceel->m_Next;
    }

    LogDump::LogA("-----------------------------\n");
}