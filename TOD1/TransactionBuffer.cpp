#include "TransactionBuffer.h"

float TransactionBuffer::_A11C90 = 1.f;

#pragma message(TODO_IMPLEMENTATION)
TransactionBuffer::TransactionBuffer(unsigned int bufferSize)
{
    MESSAGE_CLASS_CREATED(TransactionBuffer);

    m_Chunks = bufferSize / 4;
    m_Buffer = (int*)MemoryManager::AllocatorsList[MemoryManager::GetAllocatorByMemoryPointer(this)->m_AllocatorIndex]->Allocate(bufferSize, NULL, NULL);
    m_ReadCompressedBufferData = nullptr;
    m_Size = 0;
    field_20 = 0;
    m_List_1.resize(1);
    field_1C = (void*)&m_List_1.begin();
}

TransactionBuffer::~TransactionBuffer()
{
    MESSAGE_CLASS_DESTROYED(TransactionBuffer);

    MemoryManager::AllocatorsList[MemoryManager::GetAllocatorByMemoryPointer(this)->m_AllocatorIndex]->Free(m_Buffer);
}

void TransactionBuffer::_8AA1F0(int** a1)
{
    **a1 = 0x80000000;
    *a1++;
    field_20 = (Entity*)(&(*a1)[-m_Size] - (int*)m_Buffer);
    *a1 = (int*)m_Buffer;
}

#pragma message(TODO_IMPLEMENTATION)
int TransactionBuffer::DeleteChangesInBlocks(const int index, const int oldSize, const int blockMask)
{
    return 0;
}