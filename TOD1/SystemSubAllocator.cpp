#include "SystemSubAllocator.h"

SystemSubAllocator::SystemSubAllocator()
{
	MESSAGE_CLASS_CREATED(SystemSubAllocator);

	m_AllocationsTotal = NULL;
}

void* SystemSubAllocator::Allocate_A(size_t size, int filler, int unk)
{
#ifdef VERBOSELOG
	LogDump::LogA("Allocate %d bytes!\n", size);
#endif

	++m_AllocationsTotal;

	if (m_ProfilerEnabled)
		stub9();

#ifdef INCLUDE_FIXES
	void* space = malloc(size);
	if (!space)
		return nullptr;
	memset(space, filler, size);
	return space;
#endif

	return malloc(size);
}

void* SystemSubAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int)
{
#ifdef VERBOSELOG
	LogDump::LogA("Allocate %d bytes aligned %d!\n", size, alignment);
#endif

	++m_AllocationsTotal;

	if (m_ProfilerEnabled)
		stub9();

#ifdef INCLUDE_FIXES
	void* space = alignment > 1 ? _aligned_offset_malloc(size, alignment, NULL) : malloc(size);
	if (!space)
		return nullptr;
	memset(space, filler, size);
	return space;
#endif

	if (alignment > 1)
		return _aligned_offset_malloc(size, alignment, NULL);
	else
		return malloc(size);
}

void SystemSubAllocator::Free(void* ptr)
{
#ifdef VERBOSELOG
	LogDump::LogA("Freeing %x!\n", ptr);
#endif

	if (!ptr)
		return;

	--m_AllocationsTotal;
	free(ptr);
}

void SystemSubAllocator::FreeAligned(void* ptr)
{
#ifdef VERBOSELOG
	LogDump::LogA("Freeing aligned %x!\n", ptr);
#endif

	if (!ptr)
		return;

	--m_AllocationsTotal;
	_aligned_free(ptr);
}

void* SystemSubAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
#ifdef VERBOSELOG
	LogDump::LogA("Reallocation %x with new size %d\n", oldptr, newsize);
#endif

	if (oldptr)
	{
		if (m_ProfilerEnabled)
			stub9();

		if (newsize)
			// TODO: only fill new space if filler argument isn't null?
			return realloc(oldptr, newsize);

		free(oldptr);
		--m_AllocationsTotal;

		return nullptr;
	}
	else
	{
		++m_AllocationsTotal;

		if (m_ProfilerEnabled)
			stub9();

#ifdef INCLUDE_FIXES
		void* space = malloc(newsize);
		if (!space)
			return nullptr;
		memset(space, filler, newsize);
		return space;
#endif

		return malloc(newsize);
	}
}

int SystemSubAllocator::stub8(int* unk)
{
	return NULL;
}

void SystemSubAllocator::stub9()
{
	return;
}

void SystemSubAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
	return;
}

const int SystemSubAllocator::GetAllocatedSpaceSize() const
{
	MEMORYSTATUS memoryStatus;
	GlobalMemoryStatus(&memoryStatus);

	return memoryStatus.dwTotalPhys;
}

const int SystemSubAllocator::GetTotalAllocations() const
{
	MEMORYSTATUS memoryStatus;
	GlobalMemoryStatus(&memoryStatus);

	return memoryStatus.dwMemoryLoad * (memoryStatus.dwTotalPhys / 100);
}

const char* const SystemSubAllocator::GetAllocatorName() const
{
	return "SystemSubAllocator";
}

const int SystemSubAllocator::stub19() const
{
	return m_AllocationsTotal;
}

const int SystemSubAllocator::stub20() const
{
	return -1;
}

const int SystemSubAllocator::stub21() const
{
	return -1;
}

const int SystemSubAllocator::GetAvailableMemory() const
{
	MEMORYSTATUS memoryStatus;
	GlobalMemoryStatus(&memoryStatus);

	return memoryStatus.dwAvailPhys;
}

int SystemSubAllocator::stub35()
{
	// TODO: original code doesn't put anything to EAX. Is return type correct?
	return NULL;
}