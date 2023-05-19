#include "TransactionBuffer.h"

float TransactionBuffer::_A11C90 = 1.f;

TransactionBuffer::TransactionBuffer(const uint32_t bufferSize)
{
    MESSAGE_CLASS_CREATED(TransactionBuffer);

    ChunksTotal = bufferSize / 4;
    BufferData = (uint32_t*)MemoryManager::AllocatorsList[MemoryManager::GetAllocatorByMemoryPointer(this)->AllocatorIndex]->Allocate(bufferSize, __FILE__, __LINE__);
    ReadCompressedBufferData = nullptr;
    Size = 0;
    _f20 = 0;
    List_1.resize(1);
    ListHeadPtr = &List_1.begin();
}

TransactionBuffer::~TransactionBuffer()
{
    MESSAGE_CLASS_DESTROYED(TransactionBuffer);

    MemoryManager::AllocatorsList[MemoryManager::GetAllocatorByMemoryPointer(this)->AllocatorIndex]->Free(BufferData);
}

void TransactionBuffer::_8AA1F0(int** a1)
{
    **a1 = 0x80000000;

    int* a_ = *a1 + 1;
    *a1 = a_;

    _f20 = &a_[-Size] - (int*)BufferData;

    *a1 = (int*)BufferData;
}

#pragma message(TODO_IMPLEMENTATION)
int TransactionBuffer::DeleteChangesInBlocks(const int index, const int oldSize, const int blockMask)
{
    return 0;
}