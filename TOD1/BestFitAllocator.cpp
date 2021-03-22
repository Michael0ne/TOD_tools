#include "BestFitAllocator.h"
#include "LogDump.h"

unsigned int BestFitAllocator::MinimumSize;

const int BestFitAllocator::_479780(const int* const a1) const
{
	unsigned int i = 19, j = 0;
	for (; i > (j + 1);)
	{
		int t = j + (i - j) / 2;
		if (a1 <= field_24[t].field_0)
			i = j + (i - j) / 2;
		if (a1 > field_24[t].field_0)
			j = t;
	}

	return a1 > field_24[j].field_0 ? i : j;
}

BestFitAllocator::BestFitAllocator()
{
	MESSAGE_CLASS_CREATED(BestFitAllocator);

	unsigned int f0 = 32;

	for (unsigned int i = 0; i < 20; ++i)
	{
		field_24[i].field_0 = nullptr;
		field_24[i].field_4 = f0;
		field_24[i].field_8 = NULL;

		f0 = (int)((float)f0 * 1.5f);
	}

	field_24[19].field_4 = 0x7FFFFFFF;
	m_Contents = nullptr;
	field_118 = NULL;
	field_11C = NULL;
	field_120 = NULL;

	MinimumSize = 16;
}

void* BestFitAllocator::Allocate_A(size_t size, int filler, int unk)
{
	return AllocateAligned(size, 8, filler, unk);
}

#pragma message(TODO_IMPLEMENTATION)
void* BestFitAllocator::AllocateAligned(size_t size, size_t alignment, int filler, int unk)
{
	stub9();

	alignment = alignment < 8 ? 8 : alignment;
	size = size < MinimumSize ? MinimumSize : size;
	int v = _479780((int*)((size + 3) & 0xFFFFFFFC));	//	NOTE: align size by 4 bytes.
	int v_ = v;

	if (v >= 20)
	{
		stub9();
		return NULL;
	}

	BestFitAllocInfo* allocinfo = &field_24[v];
	int* al = nullptr, *al2 = nullptr;

	while (true)
	{
		al = allocinfo->field_0;
		if (allocinfo->field_0)
			break;

		v_ = ++v;
		++allocinfo;

		if (v >= 20)
		{
			stub9();
			return NULL;
		}
	}

	while (true)
	{
		al2 = (int*)stub25((int)al, (size + 3) & 0xFFFFFFFC, filler, unk, alignment);
		if (al2)
			break;

		al = (int*)al[2];
		if (!al)
		{
			++allocinfo;
			if (++v_ >= 20)
			{
				stub9();
				return NULL;
			}
		}
	}

	stub9();

	return al2;
}

#pragma message(TODO_IMPLEMENTATION)
void BestFitAllocator::Free(void* ptr)
{
}

void BestFitAllocator::FreeAligned(void* ptr)
{
	Free(ptr);
}

#pragma message(TODO_IMPLEMENTATION)
void* BestFitAllocator::Realloc(void* oldptr, size_t newsize, int filler, int unk)
{
	return nullptr;
}

int BestFitAllocator::stub8(int* unk)
{
	if (!unk)
		return (int)unk;
	
	if ((int*)(unk - 2) <= unk - 2)
		return ((int*)((char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize) - unk);
	else
		return ((int*)(unk - 2) - unk);
}

void BestFitAllocator::stub9()
{
	return;
}

void BestFitAllocator::CallMethodAtOffset20()
{
	return;
}

void BestFitAllocator::SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size)
{
	Allocator::SetNameAndAllocatedSpaceParams(bufferptr, name, size);

	int* uninitspaceptr = (int*)((char*)((int)((char*)m_AllocatedSpacePtr + 15) & 0xFFFFFFF8) - 8);	//	NOTE: this is obnoxious.
	*uninitspaceptr = (int)uninitspaceptr;
	uninitspaceptr[1] = ((int)uninitspaceptr >> 2) & 0x3FFFFFFF | 0x80000000;
	uninitspaceptr[2] = NULL;
	uninitspaceptr[3] = NULL;
	uninitspaceptr[4] = NULL;
	uninitspaceptr[5] = NULL;

	int v = _479780((int*)((char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize - (char*)uninitspaceptr - 8));
	field_24[v].field_0 = uninitspaceptr;
	field_24[v].field_8 = 1;

	field_118 = NULL;
	field_120 = NULL;
	m_Contents = uninitspaceptr;
	field_11C = 1;
}

const int BestFitAllocator::GetTotalAllocations() const
{
	return field_118;
}

const int BestFitAllocator::GetAllocatedElementsTotal() const
{
	return field_120;
}

const int BestFitAllocator::stub19() const
{
	return field_120;
}

const int BestFitAllocator::stub20() const
{
	return field_11C;
}

#pragma message(TODO_IMPLEMENTATION)
const int BestFitAllocator::stub21() const
{
	return NULL;
}

#pragma message(TODO_IMPLEMENTATION)
const int BestFitAllocator::GetAvailableMemory() const
{
	return NULL;
}

void BestFitAllocator::Dump() const
{
	LogDump::LogA("BestFitAllocator contents:\n");
	LogDump::LogA("-----------------------------\n");

	int* cont = m_Contents;
	for (int i = 0; i < (field_11C + field_120); ++i)
	{
		LogDump::LogA("%d\t: %x\t : %s\t : %d\n", i, m_Contents, cont[1] & 0x40000000 ? "used" : "free", (int*)*cont <= cont ? ((char*)m_AllocatedSpacePtr + m_AllocatedSpaceSize - (char*)cont - 8) : ((char*)*cont - (char*)cont - 8));
		cont = (int*)*cont;
	}

	LogDump::LogA("-----------------------------\n");
}

int BestFitAllocator::stub24(int a1, int a2, int a3, int a4)
{
	return stub25(a1, a2, a3, a4, 8);
}

#pragma message(TODO_IMPLEMENTATION)
int BestFitAllocator::stub25(int, int, int, int, int)
{
	return NULL;
}

int* BestFitAllocator::stub26() const
{
	return m_Contents;
}

const int BestFitAllocator::stub27() const
{
	return m_Contents[1] * 4;
}

int BestFitAllocator::stub28(int* a1) const
{
	return (int*)*a1 == m_Contents ? NULL : *a1;
}

const int BestFitAllocator::stub29(int* a1) const
{
	return m_Contents ? NULL : (4 * a1[1]);
}

char BestFitAllocator::stub30(int* a1) const
{
	return (a1[1] >> 30) & 1;
}

#pragma message(TODO_IMPLEMENTATION)
char BestFitAllocator::stub31(int* a1, int a2, int a3) const
{
	return NULL;
}

int BestFitAllocator::stub32(int a1) const
{
	return a1 + 8;
}

int BestFitAllocator::stub33(int a1) const
{
	return a1 - 8;
}

#pragma message(TODO_IMPLEMENTATION)
char BestFitAllocator::stub34(int*, int)
{
	return NULL;
}

int BestFitAllocator::stub35()
{
	return field_11C + field_120;
}