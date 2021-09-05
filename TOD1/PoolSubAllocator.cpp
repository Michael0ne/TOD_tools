#include "PoolSubAllocator.h"

PoolSubAllocator::PoolSubAllocator(int objectsize, int alignment)
{
 MESSAGE_CLASS_CREATED(PoolSubAllocator);

 m_ObjectSize = objectsize;
 m_ObjectSizeAligned = (alignment + 7) & 0xFFFFFFF8;
 m_ObjectSpace = nullptr;
}

void* PoolSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
 stub9();

 if (!m_ObjectSpace)
  return nullptr;

 void* result = m_ObjectSpace;
 m_ObjectSpace = (int*)*((int*)m_ObjectSpace);
 ++m_ObjectsInPool;
 m_TotalOccupiedSpace += field_3C;

 stub9();

 return result;
}

void* PoolSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
 return alignment == m_ObjectSizeAligned ? Allocate_A(size, filler, unk) : nullptr;
}

void PoolSubAllocator::Free(void* ptr)
{
 stub9();

 m_TotalOccupiedSpace -= field_3C;
 m_ObjectsInPool--;

 *(int*)ptr = (int)m_ObjectSpace;
 m_ObjectSpace = ptr;

 if (m_ProfilerEnabled)
  memset((int*)ptr + 1, 0xAB, (field_3C - 4) >> 2);

 stub9();
}

void PoolSubAllocator::FreeAligned(void* ptr)
{
 Free(ptr);
}

void* PoolSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
 stub9();

 if (!newsize)
 {
  Free(oldptr);
  return nullptr;
 }

 return oldptr;
}

int PoolSubAllocator::stub8(int* unk)
{
 return field_3C;
}

void PoolSubAllocator::stub9()
{
 return;
}

#pragma message(TODO_IMPLEMENTATION)
void PoolSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
 Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);
 m_ObjectSpace = (void*)(~(m_ObjectSizeAligned - 1) & (int)((char*)bufferptr + m_ObjectSizeAligned - 1));
 field_38 = NULL;
}

const int PoolSubAllocator::GetTotalAllocations() const
{
 return m_TotalOccupiedSpace;
}

const int PoolSubAllocator::GetAllocatedElementsTotal() const
{
 return m_ObjectsInPool;
}

const char* const PoolSubAllocator::GetAllocatorName() const
{
 return "PoolSubAllocator";
}

const int PoolSubAllocator::stub19() const
{
 return m_ObjectsInPool;
}