#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>		//	For general windows types.
#include <xmmintrin.h>		//	For __m128 type.
#include "stdio.h"			//	For general standart input/output routines.
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>			//	For mathematical functions.
#include <psapi.h>			//	For processes.
#include <ShlObj.h>

#include <WinUser.h>		//	For Windows-specific objects.
#include "d3d9types.h"		//	For directX 9 types.

template <typename T>
struct Vector2 {
	T	x;
	T	y;
};

template<typename T>
struct Vector3 {
	T	x;
	T	y;
	T	z;
};

template<typename T>
struct Quaternion {
	T	w;
	T	x;
	T	y;
	T	z;
};

//	TODO: move these into tod_global_vars.h
class KapowWindow;
class SoundManager;
class MemoryAllocators;
struct  KapowStringBuffer;

static char* aShellLaunched = (char*)0x9C59E4;
static int* nGameTime = (int*)0xA3DCD4;
static __m128* nRealTime = (__m128*)0xA3DCC0;
static DWORD* nFrameNumber = (DWORD*)0xA3DCE0;
static DWORD* nFrameNumberA = (DWORD*)0xA0833C;
//	Used when loading opening zip archive.
static DWORD* nZipSlotId = (DWORD*)0xA35DDC;

//	RegisterWindowClass function
static ATOM (__thiscall *_RegisterWindowClass)(void* _this, int menuresid, UINT16 iconresid) = reinterpret_cast<ATOM (__thiscall*)(void*, int, UINT16)>(0x43C570);
//	WndProc function
static LRESULT (CALLBACK * _WndProc)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = reinterpret_cast<LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM)>(0x43C320);

static HWND * g_hWnd = (HWND*)0xA35EB8;
static HINSTANCE * g_hInstance = (HINSTANCE*)0xA35EB0;
static KapowWindow * g_kapowWindow = (KapowWindow*)0xA35EB8;
typedef MemoryAllocators KapowAllocators;
static KapowAllocators * g_kapowAllocators = (KapowAllocators*)0xA3AFC0;
static SoundManager * g_soundManager = (SoundManager*)0xA35EC0;
static DWORD * g_unkGlobalWindowObject = (DWORD*)0xA39F14;
static DWORD * g_unkMouseDevice = (DWORD*)0xA35EAC;
static DWORD * g_unkKeyboardDevice = (DWORD*)0xA35E80;
static LPSTR * g_CmdLine = (LPSTR*)0xA35EB4;
static DWORD * g_GameConfiguration = (DWORD*)0xA5D5AC;

static STICKYKEYS	*sSTICKYKEYS = (STICKYKEYS*)0xA0917C;
static TOGGLEKEYS	*sTOGGLEKEYS = (TOGGLEKEYS*)0xA09184;
static FILTERKEYS	*sFILTERKEYS = (FILTERKEYS*)0xA0918C;

static DWORD* stringResourceBaseAddr = (DWORD*)0xA35E28;

static UINT* uType = (UINT*)0xA091A8;

static bool * bGameDiscFound = (bool*)0xA35E68;

static void (__thiscall *SoundManager__SetGlobalPause)(DWORD* _this, bool bPause) = (void (__thiscall*)(DWORD*, bool))0x43D1D0;
static void (__thiscall *SoundManager__MeasureWaitForSoftPause)(DWORD* _this) = (void (__thiscall*)(DWORD*))0x43E800;

static void(__thiscall *KapowInput__ProcessMouseInput)(DWORD* _this) = (void (__thiscall*)(DWORD*))0x43B670;
static void(__thiscall *KapowInput__ProcessKeyboardInput)(DWORD* _this) = (void (__thiscall*)(DWORD*))0x43AF60;
static void(__thiscall *KapowInput__ProcessControllers)() = (void(__thiscall*)())0x43A190;
static void(__thiscall *KapowInput_43B410)(DWORD* _this) = (void(__thiscall*)(DWORD*))0x43B410;
static void(__thiscall *KapowInput_43A740)(DWORD* _this) = (void(__thiscall*)(DWORD*))0x43A740;

static void(__cdecl *Performance__Initialise)() = (void (__cdecl*)())0x4306D0;
static void(__cdecl *Performance__QueryCounter)() = (void(__cdecl*)())0x4306F0;

static void(__cdecl *KapowEngine__InitialiseEngine)() = (void (__cdecl *)())0x93F680;

static void(__cdecl *FindIdFile)() = (void(__cdecl*)())0x439230;

extern HMODULE DllModuleHandle;

// Possibly a pointer to sound manager object.
//	_soundManager + 112		=	number of allocated Stream Databuffer's.
//	_soundManager + 16		=	muted?
static DWORD* _soundManager = (DWORD*)0xA35EC0;

// --------------------------------------------------------
// Useful functions.

#define nop(a, s) _nop((void*)(a), (s))
static void _nop(void* pAddress, DWORD size)
{
	DWORD dwAddress = (DWORD)pAddress;
	if ( size % 2 )
	{
		*(BYTE*)pAddress = 0x90;
		dwAddress++;
	}
	if ( size - ( size % 2 ) )
	{
		DWORD sizeCopy = size - ( size % 2 );
		do
		{
			*(WORD*)dwAddress = 0xFF8B;
			dwAddress += 2;
			sizeCopy -= 2;
		}
		while ( sizeCopy );	
	}
}

// by SilentPL
#define PATCH_NOTHING	0x00
#define PATCH_CALL		0xE8
#define PATCH_JUMP		0xE9

static void hook (DWORD address, void * function, BYTE type) {
	BYTE * patch = (BYTE *)address;
	if (type) *patch = type;	// JMP
	*(DWORD *)(patch+1) = ((DWORD)function-(address+5));	
}

#define patch(addr, data, size) _patch((void*)(addr), (DWORD)(data), (size))
static void _patch(void* pAddress, DWORD data, DWORD iSize)
{ memcpy(pAddress, &data, iSize); }

extern void debug (char * message, ...);

// --------------------------------------------------------

//	Each class applies it's patches in .cpp file, to be called in dllmain.cpp.
extern inline void PATCH_WINDOW();
extern inline void PATCH_SOUND_MANAGER();
extern inline void PATCH_ALLOCATORS();
extern inline void PATCH_STRING_BUFFER();
extern inline void PATCH_FILEPROC();