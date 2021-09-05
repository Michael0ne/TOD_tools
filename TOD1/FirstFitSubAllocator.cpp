#include "FirstFitSubAllocator.h"
#include "LogDump.h"

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

void* FirstFitSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
 return AllocateAligned(size, 8, filler, unk);
}

#pragma message(TODO_IMPLEMENTATION)
void* FirstFitSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
 return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void FirstFitSubAllocator::Free(void* ptr)
{
 if (m_ProfilerEnabled)
  stub9();

 if (!ptr)
  return;
}

void FirstFitSubAllocator::FreeAligned(void* ptr)
{
 Free(ptr);
}

#pragma message(TODO_IMPLEMENTATION)
void* FirstFitSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
 if (!oldptr)
  return Allocate_A(newsize, filler, unk);

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

 m_Space = (FirstFitElement*)(((int)((char*)m_AllocatedSpacePtr + field_24 + 7) & 0xFFFFFFF8) - field_24);
 m_Space->m_SpacePtr = nullptr;

 m_Space->m_Next = m_Space;
 m_Space_1 = m_Space;
 m_Space_1 = m_Space;
 m_SpaceOccupied = NULL;
 m_FreeRegions = 1;
 m_UsedRegions = 0;
 
 if (m_ProfilerEnabled)
  memset((int*)m_Space + 2, 0xAB, (char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize - (char*)m_Space->m_SpacePtr);
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
   result = spaceelement->m_Next <= spaceelement ? (int)((char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize - field_24 - (int)spaceelement) : (int)((char*)spaceelement->m_Next - field_24 - (int)spaceelement);

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
   spacesize = (int)((char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize);

  LogDump::LogA("%d\t: %x\t : %s\t : %d\n", i, spaceel, spaceel->m_SpacePtr ? "used" : "free", spacesize - field_24 - (int)spaceel);

  spaceel = spaceel->m_Next;
 }

 LogDump::LogA("-----------------------------\n");
}