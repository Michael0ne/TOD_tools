#include "FrameBasedSubAllocator.h"

FrameBasedSubAllocator::FrameBasedSubAllocator()
{
	MESSAGE_CLASS_CREATED(FrameBasedSubAllocator);

	m_AllocSpaceInfo = nullptr;
}

void* FrameBasedSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 8, filler, unk);
}

void* FrameBasedSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	if (m_AllocSpaceInfo->m_RegionPtr == m_RegionBegin)
		m_AllocSpaceInfo->m_RegionPtr += _47A4A0(alignment) - (int)m_RegionBegin;
	
	void* space = SequentialSubAllocator::AllocateAligned(size, alignment, filler, unk);

	if (space)
		++m_AllocSpaceInfo->field_0;

	return space;
}

void FrameBasedSubAllocator::Free(void* ptr)
{
	if (ptr >= m_AllocSpaceInfo->m_RegionPtr)
	{
		--m_AllocSpaceInfo->field_0;
		SequentialSubAllocator::Free(ptr);
		
		return;
	}

	AllocatorRegionInfo* allocregion = m_AllocSpaceInfo;
	while (true)
	{
		if (ptr < allocregion->m_RegionPtr && allocregion->m_PreviousRegionPtr && ptr >= allocregion->m_PreviousRegionPtr->m_RegionPtr)
				break;

		allocregion = allocregion->m_PreviousRegionPtr;
		if (!allocregion)
		{
			allocregion = nullptr;	//	TODO: wtf?
			--allocregion->field_0;
			
			SequentialSubAllocator::Free(ptr);
			return;
		}
	}

	--allocregion->m_PreviousRegionPtr->field_0;
	SequentialSubAllocator::Free(ptr);
}

void FrameBasedSubAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

void* FrameBasedSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	return SequentialSubAllocator::Realloc(oldptr, newsize, filler, unk);
}

int FrameBasedSubAllocator::stub8(int* unk)
{
	return NULL;
}

void FrameBasedSubAllocator::stub9()
{
	SequentialSubAllocator::stub9();
}

void FrameBasedSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
	SequentialSubAllocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);
	m_AllocSpaceInfo->field_0 = NULL;
	m_AllocSpaceInfo->m_PreviousRegionPtr = nullptr;
	m_AllocSpaceInfo->m_RegionPtr = m_RegionBegin;
}

const char* const FrameBasedSubAllocator::GetAllocatorName() const
{
	return "FrameBasedSubAllocator";
}

const int FrameBasedSubAllocator::stub21() const
{
	return m_ValidRegionEnd - m_RegionBegin;
}

const int FrameBasedSubAllocator::GetAvailableMemory() const
{
	return stub21();
}

int FrameBasedSubAllocator::stub35()
{
	if (m_AllocSpaceInfo)
	{
		AllocatorRegionInfo* allocspaceinfo = m_AllocSpaceInfo;
		while (true)
		{
			AllocatorRegionInfo* prevregion = allocspaceinfo->m_PreviousRegionPtr;
			if (!prevregion)
				break;
			Free(allocspaceinfo);
			allocspaceinfo = prevregion;
		}
	}

	SequentialSubAllocator::stub35();
	m_AllocSpaceInfo->field_0 = NULL;
	m_AllocSpaceInfo->m_PreviousRegionPtr = nullptr;
	m_AllocSpaceInfo->m_RegionPtr = m_RegionBegin;

	return NULL;
}

void FrameBasedSubAllocator::stub36()
{
	*(char**)&m_AllocSpaceInfo = m_RegionBegin;
	m_RegionBegin += 48;
	m_RegionBegin_1 = m_RegionBegin;
}

void FrameBasedSubAllocator::_47A120()
{
	AllocatorRegionInfo* allocatedspace = (AllocatorRegionInfo*)Allocate_A(sizeof(AllocatorRegionInfo), NULL, NULL);
	
	allocatedspace->field_0 = NULL;
	allocatedspace->m_RegionPtr = m_RegionBegin;
	allocatedspace->m_PreviousRegionPtr = m_AllocSpaceInfo;

	m_AllocSpaceInfo = allocatedspace;
}