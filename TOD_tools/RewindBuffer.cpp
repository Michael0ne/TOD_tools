#include "RewindBuffer.h"

#pragma message(TODO_IMPLEMENTATION)
RewindBuffer::RewindBuffer(const unsigned int bufferSize)
{
	MESSAGE_CLASS_CREATED(RewindBuffer);

	m_UnknownList.field_4 = (*(void* (*)())0x8AB060)();
	m_UnknownList.field_8 = 0;
	field_1C = 0;
	m_nChunks = bufferSize / 4;
	m_pBuffer = Allocators::AllocatorsList[Allocators::GetAllocatorByMemoryPointer(this)->m_AllocatorIndex]->Allocate_A(4 * m_nChunks, NULL, NULL);
	field_C = 0;
	m_UnknownList._8751D0();
	m_nSize = 0;
	field_20 = 0;
	field_1C = m_UnknownList.field_4;
}

#pragma message(TODO_IMPLEMENTATION)
void RewindBuffer_List::_8751D0()
{
	(*(void(__thiscall*)(RewindBuffer_List*))0x8751D0)(this);
}