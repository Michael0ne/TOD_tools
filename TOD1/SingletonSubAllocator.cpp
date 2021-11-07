#include "SingletonSubAllocator.h"

SingletonSubAllocator::SingletonSubAllocator()
{
    MESSAGE_CLASS_CREATED(SingletonSubAllocator);

    m_Instantiated = false;
    m_InstancePtr = nullptr;
}

void* SingletonSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
    if (size > m_AllocatedSpaceSize)
        return nullptr;

    m_InstancePtr = m_AllocatedSpacePtr;
    m_Instantiated = true;

    return m_InstancePtr;
}

void* SingletonSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
    void* alignedspaceptr = (void*)(~(alignment - 1) & (int)((char*)m_AllocatedSpacePtr + alignment - 1));

    m_InstancePtr = alignedspaceptr;
    m_Instantiated = true;

    if (size > (size_t)((char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize - (char*)alignedspaceptr))
        return nullptr;
    else
        return alignedspaceptr;
}

void SingletonSubAllocator::Free(void* ptr)
{
    m_Instantiated = false;
    m_InstancePtr = nullptr;
}

void SingletonSubAllocator::FreeAligned(void* ptr)
{
    m_Instantiated = false;
    m_InstancePtr = nullptr;
}

void* SingletonSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
    if (newsize <= (size_t)(m_AllocatedSpaceSize + (char*)m_AllocatedSpacePtr - (char*)m_InstancePtr))
        return oldptr;

    if (newsize > m_AllocatedSpaceSize)
        return nullptr;

    m_InstancePtr = m_AllocatedSpacePtr;

    return m_InstancePtr;
}

int SingletonSubAllocator::stub8(int* unk)
{
    return m_AllocatedSpaceSize;
}

void SingletonSubAllocator::stub9()
{
    return;
}

const int SingletonSubAllocator::GetTotalAllocations() const
{
    return m_Instantiated ? m_AllocatedSpaceSize : NULL;
}

const char* const SingletonSubAllocator::GetAllocatorName() const
{
    return "SingletonSubAllocator";
}

const int SingletonSubAllocator::stub19() const
{
    return m_Instantiated;
}

const int SingletonSubAllocator::stub20() const
{
    return !m_Instantiated;
}

const int SingletonSubAllocator::stub21() const
{
    return m_Instantiated ? NULL : m_AllocatedSpaceSize;
}

const int SingletonSubAllocator::GetAvailableMemory() const
{
    return m_Instantiated ? NULL : m_AllocatedSpaceSize;
}