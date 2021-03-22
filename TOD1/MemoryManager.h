#pragma once
#include "AllocatorBase.h"

//	NOTE: not an actual class since no methods present.
class MemoryManager
{
	struct Allocator_Struct2
	{
		void* m_AllocatedSpacePtr;
		Allocator* m_Allocator;
	};
protected:
	bool						m_Initialized;

	static void					CreateAllocators();	//	@478440
	static void					InitAllocator(Allocator& alloc, AllocatorIndex allocind, const char* const allocname, unsigned int allocsize);	//	@477630
	static void					InitAllocatorsBuffers();	//	@4776A0

	static void*				BufferPtr;	//	@A3B09C
	static void*				BuffersPtr[TOTAL];	//	@A3B0A0
	static float				_A3B0C8;	//	@A3B0C8
	static int					TotalAllocators;	//	@A3B098
	static int					_A3AFB8;	//	@A3AFB8
	static RTL_CRITICAL_SECTION	AllocatorsCriticalSection;	//	@A3AFA0

public:
	MemoryManager();	//	@478040
	~MemoryManager();	//	@9B1AF0

	static char					_477BC0(void* ptr, int a2);	//	@477BC0
	static class DefragmentatorBase* _4777F0(AllocatorIndex allocind);	//	@4777F0
	static void					ReleaseMemory(void* ptr, bool aligned);	//	@4778D0
	static Allocator*			GetAllocatorByMemoryPointer(void* ptr);	//	@4777B0
	static void*				Realloc(void* oldptr, size_t newsize, bool);	//	@477940
	static void*				AllocateByType(AllocatorIndex allocind, size_t size);	//	@478180

	static bool					Released;	//	@A3AFBC
	static Allocator*			AllocatorsList[TOTAL];	//	@A3AFC0
	static Allocator_Struct2	_A3AFE8[22];	//	@A3AFE8

	static MemoryManager		g_Allocators;	//	@A3B0CC
};