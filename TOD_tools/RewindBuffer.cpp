#include "RewindBuffer.h"

#include "MemoryAllocators.h"

void RewindBuffer::Init(unsigned int size)
{
	m_pUnkPtrToUnkStr = nullptr;
	m_nChunks = size / 4;

	//	TODO: allocate 'pUnkStr'
	m_pUnkStr.m_pUnkBuffer_Size32 = (*(void*(__cdecl*)())0x8AB060)();
	m_pUnkStr.field_8 = 0;

	//	NOTE: figure out allocator number?
	if (!Allocators::Released)
		m_pBuffer = (void*)Allocators::AllocatorsList->ALLOCATOR_DEFAULT->allocate(size);

	m_pUnkStr.Init();
	m_pUnkPtrToUnkStr = m_pUnkStr.m_pUnkBuffer_Size32;

	field_C = 0;
	m_nSize = 0;
	field_20 = 0;
}

void RewindBuffer_UnkStr::Init()
{
	void(__thiscall * _Init)(RewindBuffer_UnkStr * _this) = (void(__thiscall*)(RewindBuffer_UnkStr*))0x8751D0;

	_Init(this);
}