#include "TransactionBuffer.h"

#pragma message(TODO_IMPLEMENTATION)
TransactionBuffer::TransactionBuffer(unsigned int bufferSize)
{
	MESSAGE_CLASS_CREATED(TransactionBuffer);

	m_Chunks = bufferSize / 4;
	m_Buffer = (char*)MemoryManager::AllocatorsList[MemoryManager::GetAllocatorByMemoryPointer(this)->m_AllocatorIndex]->Allocate(bufferSize, NULL, NULL);
	field_C = 0;
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