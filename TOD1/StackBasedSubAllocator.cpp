#include "StackBasedSubAllocator.h"

StackBasedSubAllocator::StackBasedSubAllocator()
{
	MESSAGE_CLASS_CREATED(StackBasedSubAllocator);

	m_StackSpace = nullptr;
	m_StackSpace_1 = nullptr;
	m_StackEndPtr = nullptr;
	m_ElementsInStack = NULL;
	field_34 = 8;
}

void* StackBasedSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 8, filler, unk);
}

void* StackBasedSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	char* alignedspace = (char*)(~(alignment - 1) & (int)((char*)m_StackSpace->m_ActualSpacePtr + alignment - 1));
	if ((char*)m_StackEndPtr - alignedspace < size)
		return nullptr;

	StackElement* newelement = (StackElement*)(alignedspace + (int)m_StackSpace - (char*)m_StackSpace->m_ActualSpacePtr);
	newelement->m_PreviousElement = m_StackSpace->m_PreviousElement;
	newelement->m_NextElement = m_StackSpace->m_NextElement;
	newelement->m_ActualSpacePtr = m_StackSpace->m_ActualSpacePtr;
	m_StackSpace = newelement;

	if (m_StackSpace->m_PreviousElement)
		m_StackSpace->m_PreviousElement->m_NextElement = m_StackSpace;
	else
		m_StackSpace_1 = m_StackSpace;

	newelement = (StackElement*)((char*)m_StackSpace->m_ActualSpacePtr + ((size + 3) & 0xFFFFFFFC));
	newelement->m_NextElement = nullptr;
	newelement->m_PreviousElement = m_StackSpace;
	m_StackSpace->m_NextElement = newelement;
	m_StackSpace = newelement;

	m_ElementsInStack++;

	return newelement->m_PreviousElement->m_ActualSpacePtr;
}

void StackBasedSubAllocator::Free(void* ptr)
{
	if (!ptr)
		return;

	--m_ElementsInStack;
	m_StackSpace->m_NextElement = nullptr;
	m_StackSpace = m_StackSpace->m_PreviousElement;
}

void StackBasedSubAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

void* StackBasedSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	if (!oldptr)
		return Allocate_A(newsize, filler, unk);

	if (!newsize)
	{
		Free(oldptr);
		return nullptr;
	}

	--m_ElementsInStack;
	m_StackSpace->m_PreviousElement->m_NextElement = nullptr;
	m_StackSpace = m_StackSpace->m_PreviousElement;

	StackElement* newelementptr = (StackElement*)((char*)m_StackSpace->m_ActualSpacePtr + ((newsize + 3) & 0xFFFFFFFC));
	newelementptr->m_NextElement = nullptr;
	newelementptr->m_PreviousElement = m_StackSpace;
	m_StackSpace->m_NextElement = newelementptr;
	m_StackSpace = newelementptr;
	m_ElementsInStack++;

	return newelementptr->m_PreviousElement->m_ActualSpacePtr;
}

int StackBasedSubAllocator::stub8(int* unk)
{
	return (unk - field_34 + 4) - (unk - field_34) - 12;
}

void StackBasedSubAllocator::stub9()
{
	if (!m_StackSpace_1)
		return;

	//	TODO: maybe this function should have return type?
	StackElement* prevelement = m_StackSpace_1;
	for (prevelement = prevelement->m_PreviousElement; prevelement; prevelement = prevelement->m_PreviousElement);
}

void StackBasedSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
	Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

	m_StackSpace = (StackElement*)m_AllocatedSpacePtr;
	m_StackSpace_1 = (StackElement*)m_AllocatedSpacePtr;
	m_StackEndPtr = (char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize;
	*(char*)m_AllocatedSpacePtr = NULL;
	m_StackSpace->m_NextElement = nullptr;
}

const int StackBasedSubAllocator::GetTotalAllocations() const
{
	return (int)m_StackSpace - (int)m_StackSpace_1->m_ActualSpacePtr;
}

const char* const StackBasedSubAllocator::GetAllocatorName() const
{
	return "StackBasedSubAllocator";
}

const int StackBasedSubAllocator::stub19() const
{
	return m_ElementsInStack;
}

const int StackBasedSubAllocator::stub20() const
{
	return 1;
}

const int StackBasedSubAllocator::stub21() const
{
	return GetAvailableMemory();
}

const int StackBasedSubAllocator::GetAvailableMemory() const
{
	return (int)m_StackEndPtr - ((int)((char*)m_StackSpace + 15) & 0xFFFFFFF8);
}