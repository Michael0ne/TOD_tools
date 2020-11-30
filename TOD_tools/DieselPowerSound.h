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
		//	TODO: this is done so the linker doesn't complain about undefined functions.
		virtual void stub1() {};
		virtual void stub2() {};
		virtual void stub3() {};
		virtual void stub4() {};
		virtual void stub5() {};
		virtual void stub6() {};
		virtual void stub7() {};
		virtual void stub8() {};
		virtual void stub9() {};
		virtual void stub10() {};
		virtual void stub11() {};
		virtual void stub12() {};
		virtual void stub13() {};
		virtual void stub14() {};
		virtual void stub15() {};
		virtual int* stub16(DWORD, signed int, LPWAVEFORMATEX*) { return nullptr; };
		virtual void stub17() {};
		virtual void stub18() {};
		virtual void stub19() {};
		virtual void stub20() {};
		virtual int* stub21(DWORD, LPWAVEFORMATEX) { return nullptr; };
		virtual void stub22() {};
		virtual int* stub23() { return nullptr; };
		virtual int* stub24() { return nullptr; };
		virtual void stub25(signed int) {};
		virtual void stub26(signed int) {};
		virtual void stub27() {};
		virtual void stub28() {};
		virtual void stub29() {};
		virtual void stub30() {};
		virtual void stub31() {};
		virtual void stub32() {};
		virtual void stub33() {};
		virtual void stub34() {};
		virtual void stub35() {};
		virtual const char* GetSystemName() { return nullptr; };
		virtual IDirectSound* GetDirectSound() { return nullptr; };
		virtual void stub38() {};
		virtual void Release() {};
		virtual void stub40() {};
		virtual void stub41() {};
		virtual void stub42() {};
		virtual void stub43() {};
		virtual void stub44() {};
		virtual void stub45() {};

		DieselPower();
		~DieselPower();

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