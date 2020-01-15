#include "stdafx.h"

HMODULE DllModuleHandle;
HANDLE hHookThread = NULL;

static FILE * logfile;
void debug (char * message, ...) {
	if (!logfile)
		return;
	SYSTEMTIME	t;
	GetLocalTime (&t);
	fprintf (logfile, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
	va_list	arg;	
	va_start (arg, message);
	vfprintf (logfile, message, arg);
	fflush(logfile);
	va_end (arg); 
}

extern "C"
{
	HRESULT (*_DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion, DWORD riidltf, DWORD ppvOut, DWORD punkOuter);

	__declspec(dllexport) HRESULT DirectInput8Create(HINSTANCE hinst, DWORD dwVersion, DWORD riidltf, DWORD ppvOut, DWORD punkOuter)
	{ return _DirectInput8Create (hinst, dwVersion, riidltf, ppvOut, punkOuter); }
}

HMODULE dinput = NULL;
void HookDInput()
{
	dinput = LoadLibrary("C:\\Windows\\System32\\dinput8.dll");
	if (!dinput) {
		debug("Failed to find dinput8 library!\n");

		return;
	}

	/*
	*	Method offsets.
	*	dinput8 + 0x0	- ConfigureDevices
	*	dinput8 + 0x4	- DirectInput8Create
	*	dinput8 + 0x8	- EnumDevices
	*	dinput8 + 0xC	- EnumDevicesBySemantics
	*	dinput8 + 0x10	- FindDevice
	*	dinput8 + 0x14	- GetDeviceStatus
	*	dinput8 + 0x18	- Initialize
	*	dinput8 + 0x1C	- RunControlPanel
	*/
	_DirectInput8Create = (HRESULT (*)(HINSTANCE, DWORD, DWORD, DWORD, DWORD))GetProcAddress (dinput, "DirectInput8Create");

	debug("DirectInput8 hooked! Lib addr: %0.4x, DirectInput8Create method addr: %0.4x\n", &dinput, &_DirectInput8Create);
}

void openzip(char*	szZipName)
{
	//debug("openzip: %s slot %d\n", zipname, *nZipSlotId);

	DWORD dwFunc = 0x419100;	// actual address of OpenZip(char* zipname);

	_asm {
		mov eax, szZipName
		push eax
		call dwFunc
	}
}

//	NOTE: This is really just a part of LoadingScreenNode class. Move out as soon as class is done.
char _SetLoadscreenEnabled(DWORD unk)
{
	DWORD dwFunc = 0x87C6A0;
	DWORD* dwThis = (DWORD*)0xA3D7E0;
	char result = 0;

	_asm {
		mov eax, unk
		push eax

		mov ecx, dwThis
		push ecx

		call dwFunc
		mov result, al
		add esp, 10h
	}

	return result != 0;
}

double get_asin(float angle)
{
	double result = asin(angle);

	return result;
}

double get_acos(float angle)
{
	double result = acos(angle);

	return result;
}

float __stdcall sqrt_handler(float eq)
{
	float result = sqrtf(eq);

	return result;
}

//	NOTE: Most visible effect when modifying return value - changed FOV.
double get_atan(float angle)
{
	float result = atanf(angle);

	return result;
}

DWORD __stdcall testbits_handler(DWORD* a1)
{
	return *a1 = (a1[1] & a1[2]) != 0;
}

int __stdcall getRandInt()
{
	return rand();
}

long double __cdecl power(float basis, float exponent)
{
	return pow(basis, exponent);
}

void __cdecl dummy()
{
	return;
}

DWORD WINAPI HookThread(LPVOID lpParam)
{
	debug("HookThread started...\n");

	int nLastKeyPress = 0;
	const int nInterval = 100;

	while (true) {
		if (GetAsyncKeyState(VK_TAB) && (*nGameTime) > nLastKeyPress + nInterval) {
			//debug("gameTime=%d\n", *nGameTime);
			debug("0xA5D5AC=%d \n", (DWORD*)0xA5D5AC);
			nLastKeyPress = *nGameTime;
		}
	}

	debug("HookThread has finished!\n");

	return 0;
}

//	C3	retn
//	90	nop
void MemoryHook()
{
	//	Insert all hooks here.

	//	Redirect all logs into our file.
	//	TODO: this replaces calls to 'log' function. Calls to 'PrintNewFrameInfo' and 'OutputDebugString' should also be hooked.
	hook(0x40C9D0, &debug, PATCH_JUMP);

	//	Have control over loading zip's.
	hook(0x4195B7, &openzip, PATCH_CALL);
	hook(0x4198A3, &openzip, PATCH_CALL);

	//	Move to BuiltinTypes.h when class is ready.
	hook(0x465C90, &get_asin, PATCH_JUMP);
	hook(0x465C80, &get_acos, PATCH_JUMP);
	hook(0x487D20, &sqrt_handler, PATCH_JUMP);
	hook(0x465CA0, &get_atan, PATCH_JUMP);
	hook(0x487D90, &testbits_handler, PATCH_JUMP);
	//hook(0x484F80, &getRandInt, PATCH_JUMP);
	hook(0x465E90, &power, PATCH_JUMP);

	//	Apply reversed and implemented classes.
	PATCH_WINDOW();
	PATCH_SOUND_MANAGER();
	PATCH_ALLOCATORS();
	PATCH_STRING_BUFFER();
}

//=========================================================================
//
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)	{
	case DLL_PROCESS_ATTACH:
		//	Begin tracing.
		fopen_s(&logfile, "tod_trace.log", "ab");
		if (!logfile)
			return false;
		debug("Log Started!\n");

		//	Replace DirectInput8 methods with stubs.
		HookDInput();

		if (!dinput)
			return false;

		// Unlock memory.
		register DWORD dwValue;
		if (!VirtualProtect ((LPVOID)0x401000, 0x5B1394, PAGE_EXECUTE_READWRITE, &dwValue))
			exit(0);
		else
			debug("Execution memory unlocked\n");

		if (!VirtualProtect((LPVOID)0x9B330C, 0x54CF4, PAGE_EXECUTE_READWRITE, &dwValue))
			exit(0);
		else
			debug("Variables memory unlocked\n");

		//	Replace this dll handle with game's handle.
		DllModuleHandle = hModule;

		//	Install memory hooks.
		MemoryHook();

		//	Make separate thread.
		DWORD dwThreadId;
		hHookThread = CreateThread(NULL, 0, HookThread, NULL, 0, &dwThreadId);
		if (hHookThread == NULL)
			debug("Failed to create HookThread!\n");
		else
			debug("Hook thread created with id %d\n", dwThreadId);

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		//	Terminate separate thread.
		if (hHookThread) {
			CloseHandle(hHookThread);
			debug("Hook thread terminated\n");
			hHookThread = NULL;
		}

		//	Stop tracing.
		if (logfile) {
			debug("Log Closed!\n\n");
			fclose (logfile);
			logfile = NULL;
		}

		//	Free DirectInput8 library.
		if (dinput)
			FreeLibrary(dinput);

		break;
	}
	return TRUE;
}

