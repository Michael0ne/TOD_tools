#pragma once

#include "Globals.h"

namespace Audio {

#define AUDIO_DIESELPO_CLASS_SIZE 40

	typedef int (*DIESELPOWERCREATEPROC)(int, int, int, int, int, int, int, int, int);

	class DieselPower
	{
	private:
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
		virtual int stub1();
		virtual int stub2();
		virtual int stub3();
		virtual int stub4();
		virtual int stub5();
		virtual int stub6();
		virtual int stub7();
		virtual int stub8();
		virtual int stub9();
		virtual int stub10();
		virtual int stub11();
		virtual int stub12();
		virtual int stub13();
		virtual int stub14();
		virtual int stub15();
		virtual int* stub16(DWORD, signed int, LPWAVEFORMATEX*);
		virtual int stub17();
		virtual int stub18();
		virtual int stub19();
		virtual int stub20();
		virtual int* stub21(DWORD, LPWAVEFORMATEX);
		virtual int stub22();
		virtual int* stub23();
		virtual int* stub24();
		virtual void stub25(signed int);
		virtual void stub26(signed int);
		virtual int stub27();
		virtual int stub28();
		virtual int stub29();
		virtual int stub30();
		virtual int stub31();
		virtual int stub32();
		virtual int stub33();
		virtual int stub34();
		virtual int stub35();
		virtual const char* GetSystemName();
		virtual IDirectSound* GetDirectSound();
		virtual int stub38();
		virtual int Release();
		virtual int stub40();
		virtual int stub41();
		virtual int stub42();
		virtual int stub43();
		virtual int stub44();
		virtual int stub45();

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