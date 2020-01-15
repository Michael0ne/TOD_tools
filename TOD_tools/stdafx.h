#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>		//	For general windows types.
#include <xmmintrin.h>		//	For __m128 type.
#include "stdio.h"			//	For general standart input/output routines.
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>			//	For mathematical functions.

#include <WinUser.h>		//	For Windows-specific objects.
#include "d3d9types.h"		//	For directX 9 types.

//	TODO: move these into tod_global_vars.h
class KapowWindow;
class SoundManager;
class MemoryAllocators;
class KapowStringBuffer;

static char* aShellLaunched = (char*)0x9C59E4;
static HWND* g_hWnd = (HWND*)0xA35EB8;
static HINSTANCE* g_hInstance = (HINSTANCE*)0xA35EB0;
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

static KapowWindow * g_kapowWindow = (KapowWindow*)0xA35EB8;
typedef MemoryAllocators KapowAllocators;
static KapowAllocators * g_kapowAllocators = (KapowAllocators*)0xA3AFC0;

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