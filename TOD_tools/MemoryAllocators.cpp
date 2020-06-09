#include "MemoryAllocators.h"

namespace Allocators {

	unsigned int& MemoryAllocators::_A3B0C8 = *(unsigned int*)0xA3B0C8;

	MemoryAllocators::MemoryAllocators()
	{
		//	This is a global instance of a class.
		_A3B0C8 = 0;

		//	NOTE: default constructor for ALLOCATOR_DEFAULT is called from here in code.
		ALLOCATOR_DEFAULT.m_szAllocatorName = "ALLOCATOR_DEFAULT";
		ALLOCATOR_DEFAULT.field_20 = 0;
		AllocatorsList[eAllocatorType::ALLOCATOR_DEFAULT] = &ALLOCATOR_DEFAULT;
		ALLOCATOR_DEFAULT.m_nAllocatorIndex = eAllocatorType::ALLOCATOR_DEFAULT;

		AllocatorsList[eAllocatorType::ALLOCATOR_MAIN_ASSETS] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_MISSION_ASSETS] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_CUTSCENE_OR_REWIND] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_PLAYER_DATA] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_TEMP] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_RENDERLIST] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_SCRATCHPAD] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_COLLISION_CACHE_ENTRIES] = nullptr;
		AllocatorsList[eAllocatorType::ALLOCATOR_DEFRAGMENTING] = nullptr;

		AllocatorBuffers[eAllocatorType::ALLOCATOR_MAIN_ASSETS] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_MISSION_ASSETS] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_CUTSCENE_OR_REWIND] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_PLAYER_DATA] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_TEMP] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_RENDERLIST] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_SCRATCHPAD] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_COLLISION_CACHE_ENTRIES] = nullptr;
		AllocatorBuffers[eAllocatorType::ALLOCATOR_DEFRAGMENTING] = nullptr;

		BufferPtr = malloc(0x400);

		//	NOTE:	478440 inlined!
		ALLOCATOR_MAIN_ASSETS.field_44 = 0;

		if (!((unsigned char)ALLOCATOR_MAIN_ASSETS.field_44 & 1)) {
			ALLOCATOR_MAIN_ASSETS.field_44 |= 1;

			
		}

		_A3AFB8 = 0xABCDEF;
		
		InitializeCriticalSection(&CriticalSection);
	}

	MemoryAllocators::~MemoryAllocators()
	{
		if (BufferPtr)
			free(BufferPtr);

		unsigned int index = 0;
		do {
			if (AllocatorsList[index + 1])
				AllocatorsList[index + 1]->m_pSystemAllocators->free(AllocatorBuffers[index + 1]);
			++index;
		} while (index < 9);

		Released = true;
	}

	void MemoryAllocators::ReleaseMemory(void* obj, bool _unk)
	{
		debug("Memory release on: %X\n", obj);

		//	TODO: once MemoryAllocator class is finished - make proper implementation!
		void(__cdecl * _Release)(void* _obj, bool __unk) = (void(__cdecl*)(void*, bool))0x4778D0;

		_Release(obj, _unk);
	}

	void MemoryAllocators::DefragmentIfNecessary(void* unk)
	{
		(*(void(__thiscall*)(void*))0x47B780)(unk);
	}

	SystemSubAllocator::SystemSubAllocator()
	{
		//	Since it's global static object, constructor for parent class will be called here.
		m_nAllocationsTotal = 0;
		memcpy(&lpVtbl, (const void*)0x9B7CD8, 4);	//	NOTE: this will be fixed automatically once proper inheritance model is ready.
	}

	Allocator::Allocator()
	{
		lpVtbl = (Allocator__vtable*)0x9B75C8;
		m_pAllocatedSpacePtr = nullptr;
		m_nAllocatedSpaceSize = 0;
		m_pSystemAllocators = (SystemAllocator__vtable*)GetSystemAllocatorsVtable();
		field_1C = 0;
		field_21 = 0;
		field_20 = 1;
	}

	void* GetSystemAllocatorsVtable()
	{
		if (SystemAllocatorsVtableLoaded & 1)
			return (void*)SystemAllocatorsVtablePtr;

		SystemAllocatorsVtableLoaded |= 1;
		SystemAllocatorsVtablePtr = (SystemAllocator__vtable*)0x9B750C;

		return (void*)SystemAllocatorsVtablePtr;
	}

	FrameBasedSubAllocator::FrameBasedSubAllocator()
	{
		//	field_64 = 0;
	}

	SequentialSubAllocator::SequentialSubAllocator()
	{
		field_24 = field_28 = field_2C = field_30 = field_34 = field_38 = field_3C = 0;
	}

}

inline void PATCH_ALLOCATORS()
{}