#include "MemoryAllocators.h"

namespace Allocators {

	MemoryAllocator::MemoryAllocator()
	{
		debug("Allocator constructor!\n");
	}
}

inline void PATCH_ALLOCATORS()
{
	//	>> A3AFC0
}