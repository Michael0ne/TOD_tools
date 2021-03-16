#include "TransactionBuffer.h"

#pragma message(TODO_IMPLEMENTATION)
TransactionBuffer::TransactionBuffer(unsigned int bufferSize)
{
	MESSAGE_CLASS_CREATED(TransactionBuffer);

	m_Chunks = bufferSize / 4;
	m_Buffer = Allocators::AllocatorsList[Allocators::GetAllocatorByMemoryPointer(this)->m_AllocatorIndex]->Allocate_A(4 * m_Chunks, NULL, NULL);
	field_C = 0;
	m_Size = 0;
	field_20 = 0;
	field_1C = (void*)&m_List_1.cbegin();
}

#pragma message(TODO_IMPLEMENTATION)
TransactionBuffer::~TransactionBuffer()
{
	MESSAGE_CLASS_DESTROYED(TransactionBuffer);
}