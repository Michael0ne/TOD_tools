#pragma once
#include "stdafx.h"

enum AllocatorIndex
{
	DEFAULT = 0,
	MAIN_ASSETS = 1,
	MISSION_ASSETS = 2,
	CUTSCENE_OR_REWIND = 3,
	PLAYER_DATA = 4,
	TEMP = 5,
	RENDERLIST = 6,
	SCRATCHPAD = 7,
	COLLISION_CACHE_ENTRIES = 8,
	DEFRAGMENTING = 9,

	TOTAL = 10
};

class Allocator
{
	friend class MemoryManager;

	struct SystemAllocatorsTable
	{
		void*					(*_malloc)(size_t size);
		void					(*_free)(void* ptr);

		SystemAllocatorsTable();
	};
protected:
	void*						m_AllocatedSpacePtr;
	int							m_AllocatedSpaceSize;
	bool						m_ProfilerEnabled;
	SystemAllocatorsTable&		m_SystemAllocators;
	const char*					m_AllocatorName;
public:
	AllocatorIndex				m_AllocatorIndex;
	class DefragmentatorBase*	m_Defragmentator;
	char						field_20;
	char						field_21;

private:
	SystemAllocatorsTable&		GetSystemAllocatorsTable() const;	//	@4775C0

public:
	Allocator();	//	@47AB30
	virtual						~Allocator();	//	@478410

	virtual void*				Allocate(size_t size, int filler, int unk);	//	@478340
	virtual void*				Allocate_A(size_t size, int filler, int unk) = 0;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) = 0;

	virtual void				Free(void* ptr) = 0;
	virtual void				FreeAligned(void* ptr) = 0;

	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) = 0;
	virtual int					stub8(int* unk) = 0;
	virtual void				stub9() = 0;
	virtual void				CallMethodAtOffset20();	//	@478350
	virtual void				SetField21(char);	//	@478360
	virtual void				SetNameAndAllocatedSpaceParams(void* bufferptr, const char* const name, int size);	//	@47AB60
	virtual void*				GetAllocatedSpacePtr() const;	//	@478370
	virtual const int			GetAllocatedSpaceSize() const;	//	@419C40
	virtual const int			GetTotalAllocations() const;
	virtual const int			GetAllocatedElementsTotal() const;
	virtual const char* const	GetAllocatorName() const;	//	@47AB80
	virtual void				SetProfilerEnabled(bool);	//	@478380
	virtual const int			stub19() const;
	virtual const int			stub20() const;
	virtual const int			stub21() const;
	virtual const int			GetAvailableMemory() const;
	virtual void				Dump() const;
	virtual int					stub24(int, int, int, int);
	virtual int					stub25(int, int, int, int, int);
	virtual int*				stub26() const;
	virtual const int			stub27() const;
	virtual int					stub28(int*) const;
	virtual const int			stub29(int*) const;
	virtual char				stub30(int*) const;
	virtual char				stub31(int*, int, int) const;
	virtual int					stub32(int) const;
	virtual int					stub33(int) const;
	virtual char				stub34(int*, int);
	virtual int					stub35();
	virtual void				stub36();
};

ASSERT_CLASS_SIZE(Allocator, 36);