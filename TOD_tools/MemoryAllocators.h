#pragma once

#include "stdafx.h"

#define ALLOCATOR_CLASS_SIZE 36

class Allocator
{
	friend class Defragmentator;
	friend class Allocators;
protected:
	void*						m_AllocatedSpacePtr;
	int							m_AllocatedSpaceSize;
	char						field_C;

	struct SystemAllocators
	{
		void*	(*_malloc)(size_t size);
		void	(*_free)(void* ptr);
	} *m_SystemAllocators;

	const char*					m_AllocatorName;
	unsigned int				m_AllocatorIndex;
	void*						field_1C;
	char						field_20;
	char						field_21;

	void						RetrieveSystemAllocators();	//	@4775C0

public:
	Allocator();	//	@47AB30
	~Allocator();

	unsigned int				GetAllocatorIndex()
	{
		return m_AllocatorIndex;
	}

	virtual void				scalar_destructor(bool freeMemory);

	virtual void*				Allocate(size_t size, int filler, int unk);
	virtual void*				Allocate_A(size_t size, int filler, int unk) = 0;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) = 0;

	virtual void				Free(void* ptr) = 0;
	virtual void				FreeAligned(void* ptr) = 0;

	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) = 0;
	virtual int					stub8(int unk) = 0;
	virtual void				stub9() = 0;
	virtual void				CallMethodAtOffset20();
	virtual void				SetField21(char unk);
	virtual void				SetNameAndAllocatedSpaceParams(void* _allocbufferptr, const char* _allocname, int _allocsize);
	virtual void*				GetAllocatedSpacePtr();
	virtual int					GetAllocatedSpaceSize();
	virtual int					GetTotalAllocations();
	virtual int					stub16();
	virtual const char*			GetAllocatorName();
	virtual void				SetFieldC(char unk);
	virtual int					stub19();
	virtual int					stub20();
	virtual int					stub21();
	virtual int					GetAvailableMemory();
	virtual void				Dump();
	virtual int					stub23(int, int, int, int);
	virtual int					stub24(int, int, int, int, int);
	virtual int					stub25();
	virtual int					stub26();
	virtual int					stub27(int);
	virtual int					stub28(int);
	virtual char				stub29(int);
	virtual char				stub30(int, int, int);
	virtual int					stub31(int);
	virtual int					stub32(int);
	virtual char				stub33(int, int);
	virtual void				stub34();

};

#define SYSTEMSUBALLOCATOR_CLASS_SIZE 40

class SystemSubAllocator : public Allocator
{
protected:
	unsigned int				m_AllocationsTotal;

public:
	SystemSubAllocator();	//	@47AB90

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;
	virtual void				Free(void* ptr) override;
	virtual void				FreeAligned(void* ptr) override;
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;
	virtual int					stub8(int unk) override;
	virtual void				stub9() override;
};

#define FIRSTFITSUBALLOCATOR_CLASS_SIZE 60

class FirstFitSubAllocator : public Allocator
{
protected:
	int							field_24;
	int							field_28;
	int							field_2C;
	int							field_30;
	int							field_34;
	int*						field_38;

public:
	FirstFitSubAllocator();	//	@4797F0

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;
	virtual void				Free(void* ptr) override;
	virtual void				FreeAligned(void* ptr) override;
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;
	virtual int					stub8(int unk) override;
	virtual void				stub9() override;
};

#define SEQUENTIALSUBALLOCATOR_CLASS_SIZE 64

class SequentialSubAllocator : public Allocator
{
protected:
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;

public:
	SequentialSubAllocator();	//	@47A420
};

#define FRAMEBASEDSUBALLOCATOR_CLASS_SIZE 68

class FrameBasedSubAllocator : public SequentialSubAllocator
{
protected:
	int*						field_40;

public:
	FrameBasedSubAllocator();	//	@479EE0

	virtual void*				Allocate(size_t size, int filler, int unk) override;
	virtual void*				Allocate_A(size_t size, int filler, int unk) override;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;
	virtual void				Free(void* ptr) override;
	virtual void				FreeAligned(void* ptr) override;
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;
	virtual int					stub8(int unk) override;
	virtual void				stub9() override;
};

#define BESTFITALLOCATOR_CLASS_SIZE 292

class BestFitAllocator : public Allocator
{
protected:
	int field_24;

	struct {
		int field_0;
		int field_4;
		int field_8;
	}	field_28[19];

	signed int field_10C;
	int field_110;
	int* field_114;
	int field_118;
	int field_11C;
	int field_120;

public:
	BestFitAllocator();

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;
	virtual void				Free(void* ptr) override;
	virtual void				FreeAligned(void* ptr) override;
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;
	virtual int					stub8(int unk) override;
	virtual void				stub9() override;
};

#define POOLSUBALLOCATOR_CLASS_SIZE 64

class PoolSubAllocator : public Allocator
{
protected:
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;

public:
	PoolSubAllocator();
	PoolSubAllocator(int unk1, int unk2);	//	@47A1B0

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;
	virtual void				Free(void* ptr) override;
	virtual void				FreeAligned(void* ptr) override;
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;
	virtual int					stub8(int unk) override;
	virtual void				stub9() override;
};

#define STACKBASEDSUBALLOCATOR_CLASS_SIZE 56

class StackBasedSubAllocator : public Allocator
{
protected:
	void* m_StackBeginPtr;
	int* field_28;
	void* m_StackEndPtr;
	int m_ElementsInStack;
	int field_34;

public:
	StackBasedSubAllocator();	//	@47A820

	virtual void*				Allocate_A(size_t size, int filler, int unk) override;
	virtual void*				AllocateAligned(size_t size, size_t alignment, int filler, int unk) override;
	virtual void				Free(void* ptr) override;
	virtual void				FreeAligned(void* ptr) override;
	virtual void*				Realloc(void* oldptr, size_t newsize, int filler, int unk) override;
	virtual int					stub8(int unk) override;
	virtual void				stub9() override;
};

#define DEFRAGMENTATOR_CLASS_SIZE 52

class Defragmentator
{
	friend class Allocators;
protected:
	BestFitAllocator* m_DefragmentAllocator;
	BestFitAllocator* m_DefragmentAllocator_1;;
	int m_Size;
	void* m_AllocatedSpace;
	int field_14;
	int field_18;
	int field_1C;
	char field_20;
	int field_24;
	int field_28;
	char field_2C;
	int field_30;

	virtual void	_47BDD0(int unk);	//	@47BDD0
	virtual void	_47BE60(int unk1, int unk2, int unk3, int unk4, int unk5, char unk6, int unk7);	//	@47BE60
	virtual int		_47BF20(int unk1, int unk2, const char* unk3, int unk4, int unk5);	//	@47BF20
	virtual bool	_47BD20(int unk1, int unk2);	//	@47BD20
	virtual void	_47BDB0(int unk1);	//	@47BDB0
	virtual int		_4783F0(int unk1);	//	@4783F0
	virtual void	nullsub_1();
	virtual void	nullsub_2();

public:
	Defragmentator();
	Defragmentator(BestFitAllocator* bestfitallocator, char unk1, int size);	//	@47BBD0
};

#define SINGLETONSUBALLOCATOR_CLASS_SIZE 44

class SingletonSubAllocator : public Allocator
{
protected:
	char	field_24;
	int		field_28;

public:
	SingletonSubAllocator();	//	@47A690
	~SingletonSubAllocator();
};

#define ALLOCATORS_CLASS_SIZE 1160

enum ALLOCATOR_INDEX
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

struct Allocator_Struct2
{
	void*					m_AllocatedSpacePtr;
	Allocator*				m_Allocator;
};

class Allocators
{
	friend class BestFitAllocator;
protected:
	int						field_0;	//	NOTE: never used.
	SystemSubAllocator		ALLOCATOR_DEFAULT;
	int						m_AllocatorDefaultInitialized;

	Defragmentator			m_Defragmentator;
	BestFitAllocator		ALLOCATOR_DEFRAGMENTING;
	int						_pad;

	PoolSubAllocator		ALLOCATOR_COLLISION_CACHE_ENTRIES;
	StackBasedSubAllocator	ALLOCATOR_SCRATCHPAD;
	BestFitAllocator		ALLOCATOR_RENDERLIST;
	FirstFitSubAllocator	ALLOCATOR_TEMP;
	FrameBasedSubAllocator	ALLOCATOR_PLAYER_DATA;
	int						_pad_1;

	FrameBasedSubAllocator	ALLOCATOR_CUTSCENE_OR_REWIND;
	int						_pad_2;

	FrameBasedSubAllocator	ALLOCATOR_MISSION_ASSETS;
	int						_pad_3;

	FrameBasedSubAllocator	ALLOCATOR_MAIN_ASSETS;

	short					m_AllocatorsInitialized;
	int						m_BestFitAllocator_UnknownValue;

	void					CreateAllocators();	//	@478440
	void					InitAllocator(Allocator* _alloc, int _allocindex, const char* _allocname, int _allocsize);	//	@477630
	void					_4776A0();

public:
	Allocators();	//	@478040
	~Allocators();	//	@9B1AF0

	static void				ReleaseMemory(void* ptr, bool aligned);	//	@4778D0
	static Allocator*		GetAllocatorByMemoryPointer(void* ptr);	//	@4777B0

	static	RTL_CRITICAL_SECTION&	AllocatorsCriticalSection;	//	@A3AFA0
	static	int&			_A3AFB8;	//	@A3AFB8
	static	bool&			Released;	//	@A3AFBC
	static	Allocator*		AllocatorsList[TOTAL];	//	@A3AFC0
	static	Allocator_Struct2*	_A3AFE8[22];	//	@A3AFE8
	static	Allocator*		_A3AFEC[TOTAL];	//	@A3AFEC
	static	int&			TotalAllocators;	//	@A3B098
	static	void*			BufferPtr;	//	@A3B09C
	static	void*			BuffersPtr[TOTAL];	//	@A3B0A0
	static	float&			_A3B0C8;	//	@A3B0C8
};

static Allocators g_Allocators;	//	@A3B0CC

static_assert(sizeof(Allocator) == ALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Allocator));
static_assert(sizeof(SystemSubAllocator) == SYSTEMSUBALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SystemSubAllocator));
static_assert(sizeof(FirstFitSubAllocator) == FIRSTFITSUBALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(FirstFitSubAllocator));
static_assert(sizeof(SequentialSubAllocator) == SEQUENTIALSUBALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(SequentialSubAllocator));
static_assert(sizeof(FrameBasedSubAllocator) == FRAMEBASEDSUBALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(FrameBasedSubAllocator));
static_assert(sizeof(BestFitAllocator) == BESTFITALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(BestFitAllocator));
static_assert(sizeof(PoolSubAllocator) == POOLSUBALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(PoolSubAllocator));
static_assert(sizeof(StackBasedSubAllocator) == STACKBASEDSUBALLOCATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(StackBasedSubAllocator));
static_assert(sizeof(Defragmentator) == DEFRAGMENTATOR_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Defragmentator));