/*
 *
 *	Hello...
 *
 *	If you're here and willing to help, please, take time to explore the code already written
 *	and also reference IDB file before any changes.
 *
 *	Some terms used in code:
 *	TODO -- things that need to be done sometime (sooner the better);
 *	NOTE -- a note, obviously, explaining why stuff is like that;
 *	FIXME -- uh-oh, this code MUST be fixed as soon as possible.
 *
*/
#pragma once
#pragma comment (lib, "Winmm.Lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dsound.lib")

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>		//	For general windows types.
#include <xmmintrin.h>		//	For __m128 type.
#include <cstdio>			//	For general standart input/output routines.
#include <stdlib.h>			//	Standard library functions.
#include <stdarg.h>
#include <math.h>			//	For mathematical functions.
#include <psapi.h>			//	For processes.
#include <ShlObj.h>			//	For registry operations.
#include <timeapi.h>		//	For timeGetTime.

#include <WinUser.h>		//	For Windows-specific objects.
#include <d3d9.h>			//	For all directX related stuff.
#include <list>				//	For lists, obviously.
#include <dinput.h>			//	For DirectInput stuff.
#include <mmsystem.h>		//	Necessary for dsound.
#include <dsound.h>			//	For DirectSound stuff.

#define MESSAGE_WRONG_CLASS_SIZE(x) "Wrong size for " #x " class!"
#define MESSAGE_CLASS_CREATED(x) debug(#x " created at %X\n", this)
#define MESSAGE_CLASS_DESTROYED(x) debug(#x " destroyed!\n")

extern HMODULE g_DirectInput;
typedef HRESULT(__stdcall* DINPUT8CREATEORIGINAL)(HINSTANCE, DWORD, const IID&, LPVOID*, LPUNKNOWN);
extern DINPUT8CREATEORIGINAL DirectInput8Create_Hooked;

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

// by SilentPL (https://github.com/CookiePLMonster)
#define PATCH_NOTHING	0x00
#define PATCH_CALL		0xE8
#define PATCH_JUMP_SHORT 0xEB
#define PATCH_JUMP		0xE9

//	TODO: add version for 2 byte "jmp near short".
static void hook (DWORD address, void * function, BYTE type) {
	BYTE * patch = (BYTE *)address;
	if (type) *patch = type;	// JMP
//	if (type != PATCH_JUMP_SHORT)
	*(DWORD*)(patch + 1) = ((DWORD)function - (address + 5));
//	else
//		if (((int)function - address) > (char)-127 && ((int)function - address) < (char)127)
//			*(char*)(patch + 1) = (char)((int)function - address);
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
extern inline void PATCH_PERF();
extern inline void PATCH_RENDERER();
extern inline void PATCH_CONFIG();
extern inline void PATCH_INPUT_MOUSE();
extern inline void PATCH_INPUT_KEYBOARD();
extern inline void PATCH_INPUT_GAMEPAD();