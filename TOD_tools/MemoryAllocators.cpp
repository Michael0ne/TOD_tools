#include "MemoryAllocators.h"

namespace Allocators {

	unsigned int& MemoryAllocators::_A3B0C8 = *(unsigned int*)0xA3B0C8;
	bool Allocator::ms_bSystemAllocatorsVtablePresent = false;
	Allocators::SystemAllocator__vtable* Allocator::ms_pSystemAllocatorsVtable = nullptr;

	MemoryAllocators::MemoryAllocators()
	{
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

			//	TODO: !!!
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

	void MemoryAllocators::ReleaseMemory(void* obj, bool aligned)
	{
		if (Released)
			return;

		EnterCriticalSection(&CriticalSection);

		if (obj < (AllocatorsList + 0x20 + (TotalAllocators * 8) - 1))
			while (obj < (AllocatorsList + 0x20 + (TotalAllocators-- * 8)));

		if (aligned)
			_A3AFEC[TotalAllocators - 1 * 2]->lpVtbl->FreeAligned(obj);
		else
			_A3AFEC[TotalAllocators - 1 * 2]->lpVtbl->Free(obj);

		LeaveCriticalSection(&CriticalSection);
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

	Allocators::SystemAllocator__vtable* Allocator::GetSystemAllocatorsVtable()
	{
		if (Allocator::ms_bSystemAllocatorsVtablePresent)
			return Allocator::ms_pSystemAllocatorsVtable;

		Allocator::ms_bSystemAllocatorsVtablePresent = true;
		Allocator::ms_pSystemAllocatorsVtable = (SystemAllocator__vtable*)0x9B750C;

		return Allocator::ms_pSystemAllocatorsVtable;
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

	void Allocator::Init(Allocator* allocator, unsigned int index, const char* name, int size)
	{
		if (!allocator->m_pSystemAllocators)
			allocator->m_pSystemAllocators = Allocator::GetSystemAllocatorsVtable();	//	NOTE: inline maybe?

		AllocatorsList[index] = allocator;
		AllocatorsList[index]->m_pSystemAllocators = allocator->m_pSystemAllocators;
		allocator->m_nAllocatorIndex = index;

		void* allocatedSpace = AllocatorsList[index]->m_pSystemAllocators->malloc((size + 64) & 0xFFFFFFC0);
		AllocatorBuffers[index] = allocatedSpace;

		allocator->lpVtbl->_SetFields_4_8_20(allocator, allocatedSpace, name, size);	//	TODO: better name

		if (allocator->field_1C)
			(**(void (__stdcall*)(int))allocator->field_1C)(0);	//	TODO: this is ugly! what is 'field_1C'?
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