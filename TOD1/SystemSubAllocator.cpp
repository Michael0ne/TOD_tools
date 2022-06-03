#include "SystemSubAllocator.h"

SystemSubAllocator::SystemSubAllocator()
{
    MESSAGE_CLASS_CREATED(SystemSubAllocator);

    AllocationsTotal = NULL;
}

void* SystemSubAllocator::Allocate_A(size_t size, const char* const fileName, const unsigned int fileLineNumber)
{
    ++AllocationsTotal;

    if (ProfilerEnabled)
        stub9();

    return malloc(size);
}

void* SystemSubAllocator::AllocateAligned(size_t size, size_t alignment, const char* const fileName, const unsigned int fileLineNumber)
{
    ++AllocationsTotal;

    if (ProfilerEnabled)
        stub9();

    if (alignment > 1)
#ifdef _DEBUG
        return _aligned_malloc(size, alignment);
#else
        return _aligned_offset_malloc_dbg(size, alignment, fileName);
#endif
    else
        return malloc(size);
}

void SystemSubAllocator::Free(void* ptr)
{
    if (!ptr)
        return;

    --AllocationsTotal;
    free(ptr);
}

void SystemSubAllocator::FreeAligned(void* ptr)
{
    if (!ptr)
        return;

    --AllocationsTotal;
    _aligned_free(ptr);
}

void* SystemSubAllocator::Realloc(void* oldptr, size_t newsize, const char* const fileName, const unsigned int fileLineNumber)
{
    if (oldptr)
    {
        if (ProfilerEnabled)
            stub9();

        if (newsize)
            return realloc(oldptr, newsize);

        free(oldptr);
        --AllocationsTotal;

        return nullptr;
    }
    else
    {
        ++AllocationsTotal;

        if (ProfilerEnabled)
            stub9();

        return malloc(newsize);
    }
}

int SystemSubAllocator::stub8(int* unk)
{
    return NULL;
}

void SystemSubAllocator::stub9()
{
    return;
}

void SystemSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
    return;
}

const int SystemSubAllocator::GetAllocatedSpaceSize() const
{
    MEMORYSTATUSEX memoryStatus;
    GlobalMemoryStatusEx(&memoryStatus);

    return (int)memoryStatus.ullAvailPhys;
}

const int SystemSubAllocator::GetTotalAllocations() const
{
    MEMORYSTATUSEX memoryStatus;
    GlobalMemoryStatusEx(&memoryStatus);

    return memoryStatus.dwMemoryLoad * (memoryStatus.ullAvailPhys / 100UL);
}

const char* const SystemSubAllocator::GetAllocatorName() const
{
    return "SystemSubAllocator";
}

const int SystemSubAllocator::stub19() const
{
    return AllocationsTotal;
}

const int SystemSubAllocator::stub20() const
{
    return -1;
}

const int SystemSubAllocator::stub21() const
{
    return -1;
}

const int SystemSubAllocator::GetAvailableMemory() const
{
    MEMORYSTATUSEX memoryStatus;
    GlobalMemoryStatusEx(&memoryStatus);

    return (int)memoryStatus.ullAvailPhys;
}

int SystemSubAllocator::stub35()
{
    // TODO: original code doesn't put anything to EAX. Is return type correct?
    return NULL;
}