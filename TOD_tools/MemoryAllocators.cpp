#include "MemoryAllocators.h"

namespace Allocators {

	void MemoryAllocators::ReleaseMemory(void* obj, bool _unk)
	{
		debug("Memory release on: %x %c\n", obj, _unk);

		//	TODO: once MemoryAllocator class is finished - make proper implementation!
		void(__cdecl * _Release)(void* _obj, bool __unk) = (void(__cdecl*)(void*, bool))0x4778D0;

		_Release(obj, _unk);
	}
}

inline void PATCH_ALLOCATORS()
{
	//	>> A3AFC0
}