#pragma once
#include "MemoryManager.h"

class TransactionBuffer
{
public:
 char*           m_Buffer;
 int             m_Chunks;
 int             m_Size;
 int             field_C;
 std::list<int>  m_List_1;
 void*           field_1C;
 class Entity*   field_20;

public:
 TransactionBuffer(unsigned int bufferSize); // @8AADA0
 ~TransactionBuffer(); // @875210

 void* operator new(size_t size)
 {
  return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
 }
 void operator delete(void* ptr)
 {
  if (ptr)
   MemoryManager::ReleaseMemory(ptr, false);
  ptr = nullptr;
 }

 void            _8AA1F0(int** a1); // @8AA1F0
};

ASSERT_CLASS_SIZE(TransactionBuffer, 36);