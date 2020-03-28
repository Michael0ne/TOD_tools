#pragma once

#include "stdafx.h"

namespace Allocators {

	class MemoryAllocator;

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

	struct MemoryAllocator__vtable {
		void(__thiscall* Release)(MemoryAllocator*, bool FreeMemory);
		void* (__thiscall* CallMethodAtOffset8)(MemoryAllocator*);
		void* (__stdcall* allocate)(signed int, char*, int);	//	@951271
		void(__cdecl* purecall_dup2)();	//	@951271
		void(__cdecl* purecall_dup3)();	//	@951271
		void(__cdecl* purecall_dup4)();	//	@951271
		void(__cdecl* purecall_dup5)();	//	@951271
		void(__cdecl* purecall_dup6)();	//	@951271
		void(__cdecl* purecall_dup7)();	//	@951271
		int(__thiscall* CallMethodAtOffset20)(MemoryAllocator*);	//	@478350

		void(__thiscall* SetField20)(MemoryAllocator*, char);	//	?SetField20
		void(__thiscall* SetAllocatorSpaceParams)(MemoryAllocator*, void* allocatedSpacePtr, char* allocatorName, int allocatedSize);	//	@47AB60	//	TODO: better name?
		void* (__thiscall* GetAllocatedSpacePtr)(MemoryAllocator*);	//	?GetField4
		int (__thiscall* GetAllocatedSpaceSize)(MemoryAllocator*);	//	GetFileName
		int(__cdecl* ReturnZero_1)();	//	//	GetLinesPerPage
		int(__cdecl* ReturnZero_1_dup1)();	//	//	GetLinesPerPage
		const char* (__cdecl* ReturnUnknownStr)();	//	?ReturnUnknownStr
		void(__thiscall* SetFieldC)(MemoryAllocator*, char);	//	@478380
		signed int(__cdecl* ReturnMinusOne_dup1)();	//	?ReturnMinusOne
		signed int(__cdecl* ReturnMinusOne_dup2)();	//	?ReturnMinusOne

		signed int(__cdecl* ReturnMinusOne_dup3)();	//	?ReturnMinusOne
		signed int(__cdecl* ReturnMinusOne_dup4)();	//	?ReturnMinusOne
		void(__cdecl* nullsub)();	//	nullsub_41
		int(__stdcall* ReturnZero_2)(int, int, int, int);	//	?ReturnZero_3
		int(__stdcall* ReturnZero_3)(int, int, int, int, int);	//	?ReturnZero_2
		int(__cdecl* ReturnZero_1_dup2)();	//	GetLinesPerPage
		int(__cdecl* ReturnZero_1_dup3)();	//	//	GetLinesPerPage
		int(__stdcall* ReturnZero_4_dup1)(int);	//	?ReturnZero
		int(__stdcall* ReturnZero_4_dup2)(int);	//	?ReturnZero
		char(__stdcall* ReturnZero_5)(int);	//	?ReturnZero_0

		char(__stdcall* ReturnZero_6)(int, int, int);	//	?ReturnZero_1
		int(__stdcall* ReturnZero_4_dup3)(int);	//	?ReturnZero
		int(__stdcall* ReturnZero_4_dup4)(int);	//	?ReturnZero
		char(__stdcall* ReturnZero_7)(int, int);	//	?ReturnFalse
	};

	/*
	 *------------------------------------------------------------
	 *------------------------------------------------------------
	 *-------------------- Memory allocators ---------------------
	 *------------------------------------------------------------
	 * -----------------------------------------------------------
	*/
	class MemoryAllocator
	{
	private:
		MemoryAllocator__vtable* lpVtbl;
		void* m_pAllocatedSpacePtr;	//	Pointer to space allocated for this allocator
		int m_nAllocatedSpaceSize;	//	In bytes
		int field_C;
		SystemAllocator__vtable* m_pSystemAllocators;
		char* m_szAllocatorName;
		int m_nAllocatorIndex;
		void(__stdcall*** stub1)(int);
		char field_20;
		char field_21;
		char field_22[2];
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
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
	public:
		MemoryAllocator();
	};

	struct BestFitAllocator : MemoryAllocator
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
	};

	struct FirstFitSubAllocator
	{
		MemoryAllocator__vtable* lpVtbl;
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

	struct StackBasedSubAllocator
	{
		MemoryAllocator__vtable* lpVtbl;
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

	struct FrameBasedSubAllocator
	{
		MemoryAllocator__vtable* lpVtbl;
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
	};

	struct PoolSubAllocator
	{
		MemoryAllocator__vtable* lpVtbl;
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
	static int* SystemAllocatorsVtablePresent = (int*)0xA3AF9C;
	static int* Released = (int*)0xA3AFBC;
	static Allocators::MemoryAllocator* AllocatorsList[eAllocatorType::ALLOCATORS_TOTAL] = {
		(MemoryAllocator*)0xA3AFC0,
		(MemoryAllocator*)0xA3AFC4,
		(MemoryAllocator*)0xA3AFC8,
		(MemoryAllocator*)0xA3AFCC,
		(MemoryAllocator*)0xA3AFD0,
		(MemoryAllocator*)0xA3AFD4,
		(MemoryAllocator*)0xA3AFD8,
		(MemoryAllocator*)0xA3AFDC,
		(MemoryAllocator*)0xA3AFE0,
		(MemoryAllocator*)0xA3AFE4
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
	static Allocators::MemoryAllocator* g_pAllocatorDefault = (Allocators::MemoryAllocator*)0xA3B0D0;
	static Allocators::BestFitAllocator* g_pAllocatorDefragmenting = (Allocators::BestFitAllocator*)0xA3B130;
	static Allocators::PoolSubAllocator* g_pAllocatorCollisionCacheEntries = (Allocators::PoolSubAllocator*)0xA3B258;
	static Allocators::StackBasedSubAllocator* g_pAllocatorScratchpad = (Allocators::StackBasedSubAllocator*)0xA3B298;
	static Allocators::BestFitAllocator* g_pAllocatorRenderlist = (Allocators::BestFitAllocator*)0xA3B2D0;
	static Allocators::FirstFitSubAllocator* g_pAllocatorTemp = (Allocators::FirstFitSubAllocator*)0xA3B3F4;
	static Allocators::FrameBasedSubAllocator* g_pAllocatorPlayerData = (Allocators::FrameBasedSubAllocator*)0xA3B430;
	static Allocators::FrameBasedSubAllocator* g_pAllocatorCutsceneOrRewind = (Allocators::FrameBasedSubAllocator*)0xA3B478;
	static Allocators::FrameBasedSubAllocator* g_pAllocatorMissionAssets = (Allocators::FrameBasedSubAllocator*)0xA3B4C0;
	static Allocators::FrameBasedSubAllocator* g_pAllocatorMainAssets = (Allocators::FrameBasedSubAllocator*)0xA3B508;

	//	Actual static class object that holds all MemoryAllocator class and subclasses objects.
	class MemoryAllocators
	{
	private:
		int		field_0;	//	Maybe vtable pointer?

		MemoryAllocator	ALLOCATOR_DEFAULT;
		BestFitAllocator ALLOCATOR_DEFRAGMENTING;
		PoolSubAllocator ALLOCATOR_COLLISION_CACHE_ENTRIES;
		StackBasedSubAllocator ALLOCATOR_SCRATCHPAD;
		BestFitAllocator ALLOCATOR_RENDERLIST;
		FirstFitSubAllocator ALLOCATOR_TEMP;
		FrameBasedSubAllocator ALLOCATOR_PLAYER_DATA;
		FrameBasedSubAllocator ALLOCATOR_CUTSCENE_OR_REWIND;
		FrameBasedSubAllocator ALLOCATOR_MISSION_ASSETS;
		FrameBasedSubAllocator ALLOCATOR_MAIN_ASSETS;
	};

	static MemoryAllocators* g_pAllocators = (MemoryAllocators*)0xA3B0CC
}

static_assert(sizeof(Allocators::MemoryAllocator) == 0x60, MESSAGE_WRONG_CLASS_SIZE("MemoryAllocator"));