#pragma once

#include "stdafx.h"

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *-------------------- Memory allocators ---------------------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
namespace Allocators {

	class Allocator;
	class SystemSubAllocator;
	class BestFitAllocator;

	enum eAllocatorType {
		ALLOCATOR_DEFAULT,
		ALLOCATOR_MAIN_ASSETS,
		ALLOCATOR_MISSION_ASSETS,
		ALLOCATOR_CUTSCENE_OR_REWIND,
		ALLOCATOR_PLAYER_DATA,
		ALLOCATOR_TEMP,
		ALLOCATOR_RENDERLIST,
		ALLOCATOR_SCRATCHPAD,
		ALLOCATOR_COLLISION_CACHE_ENTRIES,
		ALLOCATOR_DEFRAGMENTING,
		ALLOCATORS_TOTAL
	};

	struct SystemAllocator__vtable {
		void *(__stdcall* malloc)(size_t size);
		void(__stdcall* free)(void* ptr);
	};

	struct SystemSubAllocator__vtable {
		void(__thiscall* Release)(SystemSubAllocator* _this, bool releasememory);	//	@478410
		void*(__thiscall* _CallMethodAtOffset8)(SystemSubAllocator* _this);	//	@478340
		void*(__thiscall* Allocate)(SystemSubAllocator* _this, size_t size);	//	@47AC00
		void*(__thiscall* AllocateAligned)(SystemSubAllocator* _this, size_t size, size_t alignment);	//	@47AC30
		void(__thiscall* Free)(SystemSubAllocator* _this, void* objptr);	//	@47AC90
		void(__thiscall* FreeAligned)(SystemSubAllocator* _this, void* objptr);	//	@47AC70
		void*(__thiscall* Realloc)(SystemSubAllocator* _this, void* objptr, size_t newsize);	//	@47ACB0	//	NOTE: if newsize is not set (NULL, 0) then memory is free'd!
		signed int(__stdcall* _ReturnMinusOne)();	//	@47AD60
		void(__cdecl* nullsub_1)(void);	//	@47ADE0
		void(__thiscall* CallNullsub_1)(SystemSubAllocator* _this);	//	@478350
		void(__thiscall* _SetField21)(SystemSubAllocator* _this, char val);	//	@478360
		void(__cdecl* nullsub_2)(void);	//	@47ADF0
		void*(__thiscall* GetAllocatedSpacePtr)(SystemSubAllocator* _this);	//	@478370
		int(__cdecl* GetTotalPhysicalMemory)();	//	@47ADA0
		int(__cdecl* GetUsedPhysicalMemory)();	//	@47AD30
		int(__cdecl* _ReturnZero)();	//	@4783C0
		const char*(__cdecl* GetName)();	//	@47AE10
		void(__thiscall* _SetFieldC)(SystemSubAllocator* _this, char val);	//	@478380
		int(__thiscall* GetTotalAllocations)(SystemSubAllocator* _this);	//	@47AD70
		signed int(__cdecl* _ReturnMinusOne_1)();	//	@47AD80
		signed int(__cdecl* _ReturnMinusOne_2)();	//	@47AD90
		int(__cdecl* GetAvailablePhysicalMemory)();	//	@47ADC0
		void(__cdecl* nullsub_3)(void);	//	@8CB190
		int(__stdcall* _ReturnZero_1)(int, int, int, int);	//	@4783A0
		int(__stdcall* _ReturnZero_2)(int, int, int, int, int);	//	@4783B0
		int(__cdecl* _ReturnZero_3)();	//	@4783C0
		int(__cdecl* _ReturnZero_4)();	//	@4783C0
		int(__stdcall* _ReturnZero_5)(int);	//	@993660
		int(__stdcall* _ReturnZero_6)(int);	//	@993660
		char(__stdcall* _ReturnZero_7)(int);	//	@4783D0
		char(__stdcall* _ReturnZero_8)(int, int, int);	//	@4783E0
		int(__stdcall* _ReturnZero_9)(int);	//	@993660
		int(__stdcall* _ReturnZero_10)(int);	//	@993660
		char(__stdcall* _ReturnZero_11)(int, int);	//	@484DB0
		void(__cdecl* nullsub_4)();	//	@47AE00
	};

	struct SystemSubAllocator_Unk__vtable {
		void(__thiscall* _47BDD0)(SystemSubAllocator* _this);	//	@47BDD0
		void(__thiscall* _47AE80)(SystemSubAllocator* _this);	//	@47AE80
		void(__thiscall* _47AEF0)(SystemSubAllocator* _this);	//	@47AEF0
		void(__thiscall* _47BD20)(SystemSubAllocator* _this);	//	@47BD20
		void(__thiscall* _47AFE0)(SystemSubAllocator* _this);	//	@47AFE0
		void(__thiscall* _4783F0)(SystemSubAllocator* _this);	//	@4783F0
		void(__cdecl* nullsub_1)();	//	@8CB190
		void(__cdecl* nullsub_2)();	//	@47AE70
	};

	struct Allocator__vtable {
		void(__thiscall* Release)(Allocator* _this, bool freememory);	//	@478410
		void*(__thiscall* Allocate)(Allocator* _this);	//	@478340	//	NOTE: This just calls next method (below).
		void(__thiscall* stub3)(Allocator* _this);	//	@951271 pure virtual	//	NOTE: this calls next method (below).
		void(__thiscall* stub4)(Allocator* _this);	//	@951271 pure virtual	//	NOTE: this methods seems to actually allocate stuff.
		void(__thiscall* stub5)(Allocator* _this);	//	@951271 pure virtual
		void(__thiscall* stub6)(Allocator* _this);	//	@951271 pure virtual
		void(__thiscall* stub7)(Allocator* _this);	//	@951271 pure virtual
		void(__thiscall* stub8)(Allocator* _this);	//	@951271 pure virtual
		void(__thiscall* stub9)(Allocator* _this);	//	@951271 pure virtual
		void(__thiscall* _CallMethodAtOffset20)(Allocator* _this);	//	@478350
		void(__thiscall* _SetField21)(Allocator* _this, char unk);	//	@478360
		void(__thiscall* _SetFields_4_8_20)(Allocator* _this, void* ptr, char* name, int size);	//	@47AB60
		int(__thiscall* _GetField4)(Allocator* _this);	//	@478370
		char*(__thiscall* _GetField8)(Allocator* _this);	//	@419C40
		int(__cdecl* _ReturnZero_1)();	//	@4783C0	//	NOTE: maybe pure virtual function?
		int(__cdecl* _ReturnZero_2)();	//	@4783C0	//	NOTE: maybe pure virtual function?
		const char*(__cdecl* GetAllocatorName)();	//	@47AB80
		void(__thiscall* _SetFieldC)(Allocator* _this, char unk);	//	@478380
		signed int(__cdecl* _ReturnMinusOne_1)();	//	@478390
		signed int(__cdecl* _ReturnMinusOne_2)();	//	@478390
		signed int(__cdecl* _ReturnMinusOne_3)();	//	@478390
		signed int(__cdecl* _ReturnMinusOne_4)();	//	@478390
		void(__cdecl* nullsub_1)();	//	@8CB190
		int(__stdcall* _ReturnZero_3)(int unk_1, int unk_2, int unk_3, int ukn_4);	//	@4783A0
		int(__stdcall* _ReturnZero_4)(int unk_1, int unk_2, int unk_3, int unk_4, int unk_5);	//	@4783B0
		int(__cdecl* _ReturnZero_5)();	//	@4783C0	//	NOTE: maybe pure virtual function?
		int(__cdecl* _ReturnZero_6)();	//	@4783C0	//	NOTE: maybe pure virtual function?
		int(__stdcall* _ReturnZero_7)(int unk_1);	//	@993660
		int(__stdcall* _ReturnZero_8)(int unk_1);	//	@993660
		char(__stdcall* _ReturnZero_9)(int unk_1);	//	@4783D0
		char(__stdcall* _ReturnZero_10)(int unk_1, int unk_2, int unk_3);	//	@4783E0
		int(__stdcall* _ReturnZero_11)(int unk_1);	//	@993660
		int(__stdcall* _ReturnZero_12)(int unk_1);	//	@993660
		char(__stdcall* _ReturnZero_13)(int unk_1, int unk_2);	//	@484DB0
	};

	class Allocator
	{
	public:
		Allocator__vtable* lpVtbl;
		void* m_pAllocatedSpacePtr;	//	Pointer to space allocated for this allocator
		int m_nAllocatedSpaceSize;	//	In bytes
		char field_C[4];
		SystemAllocator__vtable* m_pSystemAllocators;
		char* m_szAllocatorName;
		int m_nAllocatorIndex;
		void* field_1C;		//	Pointer to ALLOCATOR_DEFAULT.
		char field_20;
		char field_21;
		char field_22[2];
		int m_nAllocationsTotal;
		int m_nInitialised;

	public:
		Allocator();	//	@47AB30

		void* allocate(size_t size)
		{
			return ((SystemSubAllocator__vtable*)lpVtbl)->Allocate((SystemSubAllocator*)this, size);	//	FIXME: this is wrong. Don't cast vtable directly, it's replaced when instantiating object.
		}
	};

	class SystemSubAllocator : public Allocator
	{
	public:
		SystemSubAllocator_Unk__vtable* lpVtbl_Unk;
		BestFitAllocator* m_pDefragmentingAllocator_1;
		BestFitAllocator* m_pDefragmentingAllocator_2;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int field_58;
		int field_5C;

		SystemSubAllocator();	//	@47AB90
	public:
		void* allocate(size_t size) {
			//	TODO: proper virtual functions implementation!
			return ((SystemSubAllocator__vtable*)lpVtbl)->Allocate(this, size);
		}
	};

	class BestFitAllocator : public SystemSubAllocator
	{
		int field_60;
		int field_64;
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;
		int field_90;
		int field_94;
		int field_98;
		int field_9C;
		int field_A0;
		int field_A4;
		int field_A8;
		int field_AC;
		int field_B0;
		int field_B4;
		int field_B8;
		int field_BC;
		int field_C0;
		int field_C4;
		int field_C8;
		int field_CC;
		int field_D0;
		int field_D4;
		int field_D8;
		int field_DC;
		int field_E0;
		int field_E4;
		int field_E8;
		int field_EC;
		int field_F0;
		int field_F4;
		int field_F8;
		int field_FC;
		int field_100;
		int field_104;
		int field_108;
		int field_10C;
		int field_110;
		int field_114;
		int field_118;
		int field_11C;
		int field_120;
		int field_124;
	};

	class FirstFitSubAllocator
	{
		void* lpVtbl;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
	};

	class StackBasedSubAllocator
	{
		void* lpVtbl;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
	};

	//	TODO: don't really know size for this right now, but it's derived from Allocator.
	class SequentialSubAllocator : public Allocator
	{
		int	field_0;
		int	field_4;
		int	field_8;
		int	field_C;
		int	field_10;
		int	field_14;
		int	field_18;
		int	field_1C;
		int	field_20;
		int	field_24;
		int	field_28;
		int	field_2C;
		int	field_30;
		int	field_34;
		int	field_38;
		int field_3C;

	public:
		SequentialSubAllocator();	//	@47A420
	};

	class FrameBasedSubAllocator : public SequentialSubAllocator
	{
	public:
		void* lpVtbl;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
	public:
		FrameBasedSubAllocator();	//	@479EE0
	};

	class PoolSubAllocator
	{
		void* lpVtbl;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
	};

	static SystemAllocator__vtable* SystemAllocatorsVtablePtr = (SystemAllocator__vtable*)0xA3AF98;
	static int&		SystemAllocatorsVtableLoaded = *(int*)0xA3AF9C;
	static bool&		Released = *(bool*)0xA3AFBC;
	static int&		TotalInitialised = *(int*)0xA3B54C;
	static void*	GetSystemAllocatorsVtable();	//	@4775C0
	static void*	BufferPtr;	//	@A3B09C
	static int&		_A3AFB8 = *(int*)0xA3AFB8;	//	@A3AFB8
	static CRITICAL_SECTION& CriticalSection = *(CRITICAL_SECTION*)0xA3AFA0;	//	@A3AFA0

	struct AllocatorsPtrsList {
		SystemSubAllocator*	ALLOCATOR_DEFAULT;
		FrameBasedSubAllocator* ALLOCATOR_MAIN_ASSETS;
		FrameBasedSubAllocator* ALLOCATOR_MISSION_ASSETS;
		FrameBasedSubAllocator* ALLOCATOR_CUTSCENE_OR_REWIND;
		FrameBasedSubAllocator* ALLOCATOR_PLAYER_DATA;
		FirstFitSubAllocator* ALLOCATOR_TEMP;
		BestFitAllocator* ALLOCATOR_RENDERLIST;
		StackBasedSubAllocator* ALLOCATOR_SCRATCHPAD;
		PoolSubAllocator* ALLOCATOR_COLLISION_CACHE_ENTRIES;
		BestFitAllocator* ALLOCATOR_DEFRAGMENTING;
	};

	static Allocator* AllocatorsList[10] = {
		(Allocator*)0xA3AFC0,	//	ALLOCATOR_DEFAULT
		(Allocator*)0xA3AFC4,	//	ALLOCATOR_MAIN_ASSETS
		(Allocator*)0xA3AFC8,	//	ALLOCATOR_MISSION_ASSETS
		(Allocator*)0xA3AFCC,	//	ALLOCATOR_CUTSCENE_OR_REWIND
		(Allocator*)0xA3AFD0,	//	ALLOCATOR_PLAYER_DATA
		(Allocator*)0xA3AFD4,	//	ALLOCATOR_TEMP
		(Allocator*)0xA3AFD8,	//	ALLOCATOR_RENDERLIST
		(Allocator*)0xA3AFDC,	//	ALLOCATOR_SCRATCHPAD
		(Allocator*)0xA3AFE0,	//	ALLOCATOR_COLLISION_CACHE_ENTRIES
		(Allocator*)0xA3AFE4	//	ALLOCATOR_DEFRAGMENTING
	};

	static void* AllocatorBuffers[eAllocatorType::ALLOCATORS_TOTAL] = {
		(void*)0xA3B0A0,
		(void*)0xA3B0A4,
		(void*)0xA3B0A8,
		(void*)0xA3B0AC,
		(void*)0xA3B0B0,
		(void*)0xA3B0B4,
		(void*)0xA3B0B8,
		(void*)0xA3B0BC,
		(void*)0xA3B0C0,
		(void*)0xA3B0C4
	};

	//	Actual static class object that holds all MemoryAllocator class and subclasses objects.
	class MemoryAllocators
	{
	private:
		int		field_0;	//	Maybe vtable pointer?

		SystemSubAllocator	ALLOCATOR_DEFAULT;
		BestFitAllocator ALLOCATOR_DEFRAGMENTING;
		int		_pad;
		PoolSubAllocator ALLOCATOR_COLLISION_CACHE_ENTRIES;
		StackBasedSubAllocator ALLOCATOR_SCRATCHPAD;
		BestFitAllocator ALLOCATOR_RENDERLIST;
		FirstFitSubAllocator ALLOCATOR_TEMP;
		FrameBasedSubAllocator ALLOCATOR_PLAYER_DATA;
		FrameBasedSubAllocator ALLOCATOR_CUTSCENE_OR_REWIND;
		FrameBasedSubAllocator ALLOCATOR_MISSION_ASSETS;
		FrameBasedSubAllocator ALLOCATOR_MAIN_ASSETS;

		int		BestFitAllocatorValue;	//	Initialized to 16 and never changed.

		static	unsigned int&	_A3B0C8;	//	@A3B0C8
	public:
		MemoryAllocators();	//	@478040
		~MemoryAllocators();	//	@9B1AF0

		static void ReleaseMemory(void* obj, bool);
		static void DefragmentIfNecessary(void* unk);	//	@47B780
	};

	static  MemoryAllocators* g_Allocators = (MemoryAllocators*)0xA3B0CC;
}

//static_assert(sizeof(Allocators::MemoryAllocator) == 0x60, MESSAGE_WRONG_CLASS_SIZE("MemoryAllocator"));	//	TODO: what size check should be performed?