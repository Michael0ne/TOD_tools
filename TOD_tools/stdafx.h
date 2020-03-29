#pragma once
#pragma comment (lib, "Winmm.Lib")

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

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

#define MESSAGE_WRONG_CLASS_SIZE(x) "Wrong size for " #x " class!"

#define INCLUDE_FIXES 1	//	This includes various fixes of the game code.

template <typename T>
struct Vector2 {
	T	x;
	T	y;
};

typedef Vector2<int> ScreenResolution;

template <typename T>
struct Vector3 {
	T	x;
	T	y;
	T	z;
};

template <typename T>
struct Vector4 {
	T	x;
	T	y;
	T	z;
	T	a;
};

typedef Vector4<float> ColorRGB;
typedef Vector4<float> Vector4f;

template<typename T>
struct Quaternion {
	T	w;
	T	x;
	T	y;
	T	z;
};

typedef Quaternion<float> Orientation;

class Window;
class StreamedSoundBuffers;
class GfxInternal_Dx9;
class String;
class SceneNode;
class InputMouse;
class InputKeyboard;
class InputGameController;
class File;
class Builtin;

//	TODO: move these into tod_global_vars.h
static int* nGameTime = (int*)0xA3DCD4;
static __m128* nRealTime = (__m128*)0xA3DCC0;
static DWORD* nFrameNumber = (DWORD*)0xA3DCE0;
static DWORD* nFrameNumberA = (DWORD*)0xA0833C;
//	Used when loading opening zip archive.
static DWORD* nZipSlotId = (DWORD*)0xA35DDC;

static const File * const g_pFilesArray[8] = {
	(File*)0xA35DB8,
	(File*)0xA35DBC,
	(File*)0xA35DC0,
	(File*)0xA35DC4,
	(File*)0xA35DC8,
	(File*)0xA35DCC,
	(File*)0xA35DD0,
	(File*)0xA35DD4
};

static IDirect3DDevice9 * g_pDirect3DDevice9 = (IDirect3DDevice9*)0xA39F34;
static HWND * g_hWnd = (HWND*)0xA35EB8;
static Builtin * g_pBuiltin = (Builtin *)0xA3B578;
static HINSTANCE * g_hInstance = (HINSTANCE*)0xA35EB0;
static Window * g_pWindow = (Window*)0xA35EB8;
static StreamedSoundBuffers * g_pSoundManager = (StreamedSoundBuffers*)0xA35EC0;
static GfxInternal_Dx9 * g_pRenderer = (GfxInternal_Dx9*)0xA39F14;
static LPSTR * g_CmdLine = (LPSTR*)0xA35EB4;
static DWORD * g_pGameConfiguration = (DWORD*)0xA5D5AC;
static InputMouse * g_pInputMouse = (InputMouse*)0xA35EAC;
static InputKeyboard * g_pInputKeyboard = (InputKeyboard*)0xA35E80;
static InputGameController * g_pInputGameController = (InputGameController*)0xA35E7C;

//	TODO: these two below are different. First used in game, second in editor (maybe?).
static SceneNode* g_pSceneNode = (SceneNode*)0xA3DCB8;
static SceneNode* g_pScene = (SceneNode*)0xA3DCBC;

static STICKYKEYS	*sSTICKYKEYS = (STICKYKEYS*)0xA0917C;
static TOGGLEKEYS	*sTOGGLEKEYS = (TOGGLEKEYS*)0xA09184;
static FILTERKEYS	*sFILTERKEYS = (FILTERKEYS*)0xA0918C;

static DWORD* stringResourceBaseAddr = (DWORD*)0xA35E28;

static UINT* uType = (UINT*)0xA091A8;

static bool * bGameDiscFound = (bool*)0xA35E68;

static void (__cdecl* Allocators__ReleaseMemory)(void* ptr, bool unk) = (void (__cdecl*)(void*, bool))0x4778D0;

static void (__thiscall *SoundManager__SetGlobalPause)(StreamedSoundBuffers* _this, bool bPause) = (void (__thiscall*)(StreamedSoundBuffers*, bool))0x43D1D0;
static void (__thiscall *SoundManager__MeasureWaitForSoftPause)(StreamedSoundBuffers* _this) = (void (__thiscall*)(StreamedSoundBuffers*))0x43E800;
static void (__thiscall *SoundManager__DumpStreamedSoundBuffers)(const StreamedSoundBuffers* _this) = (void(__thiscall*)(const StreamedSoundBuffers*))0x43EAD0;

static void (__thiscall *KapowInput__ProcessMouse)(InputMouse* _this) = (void (__thiscall*)(InputMouse*))0x43B670;
static void (__thiscall *KapowInput__ProcessKeyboard)(InputKeyboard* _this) = (void (__thiscall*)(InputKeyboard*))0x43AF60;
static void (__cdecl *KapowInput__ProcessGameControllers)() = (void(__cdecl*)())0x43A190;
static void (__thiscall *KapowInput__ResetMouse)(InputMouse* _this) = (void(__thiscall*)(InputMouse*))0x43B410;
static void (__thiscall *KapowInput__ResetKeyboard)(InputKeyboard* _this) = (void(__thiscall*)(InputKeyboard*))0x43A740;

static void	(__cdecl *Performance__Initialise)() = (void (__cdecl*)())0x4306D0;
static void	(__cdecl *Performance__QueryCounter)() = (void(__cdecl*)())0x4306F0;
static long	(__cdecl *Performance__Measure)() = (long(__cdecl*)())0x430570;
static bool * Performance__bQueryingPerformance = (bool*)0xA08E90;
static bool * Performance__bMeasuringPerformance = (bool*)0xA35E38;

static void (__cdecl *InitialiseGame)(LPSTR lpCmdLine) = (void (__cdecl *)(LPSTR))0x93F680;

static void (__cdecl *FindIdFile)() = (void(__cdecl*)())0x439230;

static DWORD* (__thiscall *KapowStringsPool__Create)(DWORD* _this, const char* szString) = (DWORD* (__thiscall *)(DWORD*, const char*))0x405D90;
static String* (__thiscall *KapowStringsPool__AllocateSpace)(String* _this) = (String* (__thiscall *)(String*))0x4056E0;
static void	(__thiscall *KapowStringsPool__unkAllocation)(String* _this) = (void(__thiscall *)(String*))0x405590;

static void (__thiscall *KapowRenderer__CreateDirect3DDevice)(DWORD* _this, int* unk1, int unk2, int unk3, int unk4, int unk5) = (void(__thiscall *)(DWORD*, int*, int, int, int, int))0x45E620;

static void(__cdecl *PrintBuildNumber)(char* buffer) = (void(__cdecl*)(char*))0x401000;
static void(__cdecl *PrintAuthor)(char* buffer) = (void(__cdecl*)(char*))0x401020;

static void(__thiscall *GfxInternal_Dx9__DumpScreenshot)(GfxInternal_Dx9 *_this, IDirect3DSurface9* pSurface) = (void(__thiscall*)(GfxInternal_Dx9*, IDirect3DSurface9*))0x44E970;

static bool(__cdecl* Scene__Update)() = (bool(__cdecl*)())0x93CEB0;

extern HMODULE DllModuleHandle;

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
extern inline void PATCH_PERF();
extern inline void PATCH_RENDERER();
extern inline void PATCH_CONFIG();