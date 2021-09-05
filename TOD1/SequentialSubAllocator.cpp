#include "SequentialSubAllocator.h"

int SequentialSubAllocator::_47A4A0(int a1)
{
 return (~(a1 - 1) & (field_3C + (int)m_RegionBegin + a1 - 1)) - field_3C;
}

SequentialSubAllocator::SequentialSubAllocator()
{
 MESSAGE_CLASS_CREATED(SequentialSubAllocator);

 m_RegionBegin = nullptr;
 m_RegionBegin_1 = nullptr;
 field_24 = NULL;
 m_AllocationsTotal = NULL;
 m_ValidRegionEnd = nullptr;
 field_38 = NULL;
 field_3C = NULL;
}

void* SequentialSubAllocator::Allocate(size_t size, int filler, int unk)
{
 return AllocateAligned(size, 4, filler, unk);
}

void* SequentialSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
 return AllocateAligned(size, 8, filler, unk);
}

void* SequentialSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
 int res = (~(alignment - 1) & ((int)m_RegionBegin + field_3C + alignment - 1));

 if ((((size + 3) & 0xFFFFFFFC) + res + field_3C) > (unsigned int)m_ValidRegionEnd)
  return nullptr;

 if (m_RegionBegin == m_RegionBegin_1)
  m_RegionBegin_1 = (char*)(res - field_3C);
 field_38 = res - field_3C;
 m_RegionBegin = (char*)(res + ((size + 3) & 0xFFFFFFFC));
 ++m_AllocationsTotal;

 return (void*)res;
}

void SequentialSubAllocator::Free(void* ptr)
{
 m_AllocationsTotal--;

 if ((int)ptr == field_38)
  field_38 = NULL;

 if (!m_AllocationsTotal)
  stub35();
}

void SequentialSubAllocator::FreeAligned(void* ptr)
{
 Free(ptr);
}

void* SequentialSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
 if (!oldptr)
  Allocate_A(newsize, filler, unk);

 if (newsize > 0 && ((char*)oldptr + newsize) <= m_ValidRegionEnd)
 {
  m_RegionBegin = (char*)oldptr + newsize;
  return oldptr;
 }
 else
 {
  Free(oldptr);
  return nullptr;
 }
}

int SequentialSubAllocator::stub8(int* unk)
{
 return NULL;
}

void SequentialSubAllocator::stub9()
{
 return;
}

void SequentialSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
 Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);
 m_ValidRegionEnd = (char*)bufferptr + size;
 m_RegionBegin_1 = (char*)bufferptr;
 m_RegionBegin = (char*)bufferptr;

 stub36();
}

const int SequentialSubAllocator::GetTotalAllocations() const
{
 return m_RegionBegin - m_RegionBegin_1;
}

const int SequentialSubAllocator::GetAllocatedElementsTotal() const
{
 return m_AllocationsTotal;
}

const char* const SequentialSubAllocator::GetAllocatorName() const
{
 return "SequentialSubAllocator";
}

const int SequentialSubAllocator::stub19() const
{
 return m_AllocationsTotal;
}

int SequentialSubAllocator::stub35()
{
 m_RegionBegin_1 = (char*)m_AllocatedSpacePtr;
 m_RegionBegin = (char*)m_AllocatedSpacePtr;
 field_38 = NULL;
 stub36();
 field_24 = NULL;
 m_AllocationsTotal = NULL;

 // FIXME: original code doesn't return anything, but this virtual function SHOULD return something.
 return NULL;
}

void SequentialSubAllocator::stub36()
{
 return;
}