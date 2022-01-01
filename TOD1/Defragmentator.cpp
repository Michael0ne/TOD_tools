#include "Defragmentator.h"
#include "MemoryManager.h"

void DefragmentatorBase::_47BC70(int chunkind)
{
	m_AllocatedSpace[chunkind].m_ChunkId |= 1;

	if (m_AllocatedSpace[chunkind].field_8 != -1)
		m_AllocatedSpace[m_AllocatedSpace[chunkind].field_8].field_4 = m_AllocatedSpace[chunkind].field_4;

	if (m_AllocatedSpace[chunkind].field_4 == -1)
		field_18 = m_AllocatedSpace[chunkind].field_8;
	else
		m_AllocatedSpace[m_AllocatedSpace[chunkind].field_4].field_8 = m_AllocatedSpace[chunkind].field_8;

	--field_1C;
}

void DefragmentatorBase::_47BCD0(int chunkind)
{
	if (field_18 == -1)
		m_AllocatedSpace[field_18].field_4 = chunkind;

	int field_18_old = field_18;
	int chunkid = NULL;

	field_18 = chunkind;
	m_AllocatedSpace[chunkind].m_ChunkId = chunkid & 0xFFFFFFFE;
	m_AllocatedSpace[chunkind].field_4 = -1;
	m_AllocatedSpace[chunkind].field_8 = field_18_old;
	++field_1C;
}

void DefragmentatorBase::_47BD40(int chunkind)
{
	if (m_AllocatedSpace[chunkind].field_4)
	{
		if ((m_AllocatedSpace[chunkind].m_ChunkId & 2) == 0)
			(*(void(__thiscall*)(int*, char))m_AllocatedSpace[chunkind].field_4)((int*)m_AllocatedSpace[chunkind].field_4, 0);
		MemoryManager::ReleaseMemory((void*)m_AllocatedSpace[chunkind].field_4, (m_AllocatedSpace[chunkind].m_ChunkId & 0x3FC) != 0);
		m_AllocatedSpace[chunkind].field_4 = NULL;
		m_AllocatedSpace[chunkind].m_ChunkId &= 0xC00003FF;
	}
}

DefragmentatorBase::DefragmentatorBase(BestFitAllocator* bestfitallocator, char a2, int size)
{
	MESSAGE_CLASS_CREATED(DefragmentatorBase);

	m_Size = size;
	m_DefragmentAllocator = bestfitallocator;
	m_AllocatedSpace = new Defragmentator_Space[size];
	field_20 = a2;
	bestfitallocator->m_Defragmentator = this;
	m_DefragmentAllocator_1 = bestfitallocator;
}

DefragmentatorBase::~DefragmentatorBase()
{
	MESSAGE_CLASS_DESTROYED(DefragmentatorBase);

	unsigned int chunkid = NULL;
	int defrname = NULL;

	m_AllocatedSpace->m_ChunkId = (chunkid | 1) & 0xC0000003;
	m_AllocatedSpace->field_4 = -1;
	m_AllocatedSpace->field_8 = defrname;

	field_14 = NULL;
	field_18 = -1;

	if (m_Size > 1)
	{
		for (unsigned int i = 1; i < m_Size; ++i)
		{
			if (field_18 != -1)
				m_AllocatedSpace[field_18].field_4 = i;
			field_18 = i;
			m_AllocatedSpace[i].m_ChunkId = chunkid & 0xFFFFFFFE;
			m_AllocatedSpace[i].field_4 = -1;
			m_AllocatedSpace[i].field_8 = field_18;

			++field_1C;
		}
	}

	field_1C = m_Size - 1;
}

int DefragmentatorBase::stub2(int size, int a2, int a3, int a4, int alignment, char a6)
{
	if (MemoryManager::Released)
		return NULL;

	void* space = alignment ? MemoryManager::AllocatorsList[m_DefragmentAllocator->m_AllocatorIndex]->AllocateAligned(size, alignment, NULL, NULL) : MemoryManager::AllocatorsList[m_DefragmentAllocator->m_AllocatorIndex]->Allocate_A(size, NULL, NULL);
	if (!space)
		return NULL;

	//if (field_18 == -1)
	//     dummy();
	// NOTE: call to empty function is here. Skipped.

	_47BC70(field_18);

	int dummy = NULL, dummy2 = NULL;
	m_AllocatedSpace[field_18].m_ChunkId = (dummy2 ^ (dummy2 ^ (4 * alignment)) & 0x3FC | 1) & 0xC00003FD | (2 * (a6 & 1 | ((size & 0xFFFFF) << 9)));
	m_AllocatedSpace[field_18].field_4 = (int)space;
	m_AllocatedSpace[field_18].field_8 = dummy;
	field_14 = field_18;

	return field_18;
}

int DefragmentatorBase::stub3(int chunkind, int a2, const char* const a3, int a4, unsigned int size)
{
	void* space;

	if (m_AllocatedSpace[chunkind].field_4)
		space = MemoryManager::Realloc((void*)m_AllocatedSpace[chunkind].field_4, size, true);
	else
		if (MemoryManager::Released)
			space = nullptr;
		else
			space = MemoryManager::AllocatorsList[m_DefragmentAllocator->m_AllocatorIndex]->Allocate_A(size, NULL, NULL);

	if (space)
	{
		m_AllocatedSpace[chunkind].field_4 = (int)space;
		m_AllocatedSpace[chunkind].m_ChunkId ^= (m_AllocatedSpace[chunkind].m_ChunkId ^ (size << 10)) & 0x3FFFFC00;

		return chunkind;
	}
	else
	{
		_47BCD0(chunkind);
		return NULL;
	}
}

char DefragmentatorBase::stub4(int a1, int a2)
{
	return MemoryManager::_477BC0((void*)(m_AllocatedSpace[a1].field_4), a2);
}

void DefragmentatorBase::stub5(int a1)
{
	if (a1)
	{
		_47BD40(a1);
		_47BCD0(a1);
	}
}

int DefragmentatorBase::stub6(int a1)
{
	return (m_AllocatedSpace[a1].m_ChunkId >> 1) & 1;
}

void DefragmentatorBase::stub7()
{
	return;
}

void DefragmentatorBase::stub8()
{
	return;
}

void Defragmentator::stub8()
{
	return;
}