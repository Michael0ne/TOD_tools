#include "MemoryAllocators.h"

namespace Allocators {

	void MemoryAllocators::ReleaseMemory(void* obj, bool _unk)
	{
		debug("Memory release on: %X\n", obj);

		//	TODO: once MemoryAllocator class is finished - make proper implementation!
		void(__cdecl * _Release)(void* _obj, bool __unk) = (void(__cdecl*)(void*, bool))0x4778D0;

		_Release(obj, _unk);
	}

	void MemoryAllocators::DefragmentIfNecessary(int* unk)
	{
		(*(void(__thiscall*)(int*))0x47B780)(unk);
	}

}

inline void PATCH_ALLOCATORS()
{
	//	>> A3AFC0
}