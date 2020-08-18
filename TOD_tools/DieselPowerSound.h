#pragma once

#include "stdafx.h"

#include "Types.h"
#include "MemoryAllocators.h"

namespace Audio {

#define AUDIO_DIESELPO_CLASS_SIZE 40

	typedef int (*DIESELPOWERCREATEPROC)(int, int, int, int, int, int, int, int, int);

	class DieselPower;

	struct DieselPower__vtable
	{
		int field_0;
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
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int (*field_58)(void);
		int* (*field_5C)(void);
		void(__thiscall* field_60)(DieselPower* _this, signed int);
		void(__stdcall* field_64)(signed int);
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
		int field_80;
		int field_84;
		int field_88;
		char* (*GetName)(void);
		IDirectSound* (*GetDirectSoundPtr)(void);
		int field_94;
		int field_98;
		int field_9C;
		int field_A0;
		int field_A4;
		int field_A8;
		int field_AC;
		int field_B0;
	};

	class DieselPower
	{
	private:
		DieselPower__vtable* lpVtbl;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;

	public:
		DieselPower()
		{
			MESSAGE_CLASS_CREATED(DieselPower);
		}
		~DieselPower()
		{
			MESSAGE_CLASS_DESTROYED(DieselPower);
		}

		void* operator new (size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		static DieselPower* CallFactory(unsigned int versionMajor, unsigned int versionMinor, unsigned int versionBuild, float unk, HWND windowHandle, int unk1, int unk2, int unk3);	//	@940A70

		static const char* RequiredVersion;	//	@A1BA50
		static char* DetectedVersion;	//	@A5E800
		static HMODULE& LibraryHandle;	//	@A5E840
		static int& ErrorCode;	//	@A5E844
		static DIESELPOWERCREATEPROC FactoryFunction;	//	@A5E868
		static char* ErrorMessageBuffer;	//	@A5E700
		static bool& Created;	//	@A5E84C
	};

	extern DieselPower* g_DieselPower;
}

static_assert(sizeof(Audio::DieselPower) == AUDIO_DIESELPO_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(DieselPower));