/*
 *
 * Hello...
 *
 * If you're here and willing to help, please, take time to explore the code already written
 * and also reference IDB file before any changes.
 *
 * Some terms used in code:
 * TODO -- things that need to be done sometime (sooner the better);
 * NOTE -- a note, obviously, explaining why stuff is like that;
 * FIXME -- uh-oh, this code MUST be fixed as soon as possible.
 *
 * Preprocessor defines and their usage:
 * INCLUDE_FIXES -- includes fixes to obvious bugs and improvements;
 * VERBOSELOG  -- verbose logging for certain functions;
 * PLATFORM_PS2 -- use PS2-specific code;
 * PLATFORM_LINUX -- use linux-specific code;
 * If none of the 2 defined code compilation assumes Windows is being used as target platform.
 *
*/
#pragma once
#pragma comment (lib, "Winmm.Lib")

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>  // For general windows types.
#include <xmmintrin.h>  // For __m128 type.
#include <cstdio>   // For general standart input/output routines.
#include <stdlib.h>   // Standard library functions.
#include <stdarg.h>
#include <math.h>   // For mathematical functions.
#include <psapi.h>   // For processes.
#include <ShlObj.h>   // For registry operations.
#include <timeapi.h>  // For timeGetTime.
#include "time.h"   // For time stuff.

#include <WinUser.h>  // For Windows-specific objects.
#include <d3d9.h>   // For all directX related stuff.
#include <list>    // For lists, obviously.
#include <dinput.h>   // For DirectInput stuff.
#include <mmsystem.h>  // Necessary for dsound.
#include <dsound.h>   // For DirectSound stuff.

#define KAPOW_ENGINE_VERSION_MAJOR 34
#define KAPOW_ENGINE_VERSION_MINOR 7
#define KAPOW_ENGINE_VERSION_BUILD 1925
#define KAPOW_ENGINE_BUILDBY "Kasper.Fauerby"

#define MESSAGE_NOT_IMPLEMENTED(x) debug(#x " is not implemented!\n")
#define MESSAGE_WRONG_CLASS_SIZE(x) "Wrong size for " #x " class!"
#define MESSAGE_CLASS_CREATED(x) debug(#x " created at %X\n", this)
#define MESSAGE_CLASS_DESTROYED(x) debug(#x " destroyed!\n")
#define ASSERT_CLASS_SIZE(x, size) static_assert(sizeof(x) == size, MESSAGE_WRONG_CLASS_SIZE(x))

#define ALIGN_4BYTES(x) ((int)(x) & 0xFFFFFFFC)
#define ALIGN_4BYTESUP(x) ((int)(x + 3) & 0xFFFFFFFC)
#define D3DCOLOR_DWORD(r, g, b, a) (DWORD)((unsigned char)(b * 255.f) | (((unsigned char)(g * 255.f) | (((unsigned char)(r * 255.f) | ((unsigned char)(a * 255.f) << 8)) << 8)) << 8))
#define DEG2RAD(deg) (0.017453292f * deg)

// To be used with DirectX interfaces.
#define RELEASE_SAFE(p) \
if (p) \
{ \
 p->Release(); \
 p = nullptr; \
} \

#ifdef INCLUDE_FIXES
#define Stringify( L ) #L
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringify, __LINE__ )
#define TODO_IMPLEMENTATION __FILE__ "(" $Line "): TODO: implementation!"
#else
#define TODO_IMPLEMENTATION 
#endif

#define DECLARE_SCRIPT_ENTITY_CLASS(className, baseClassName) \
 class className : public baseClassName \
 { \
 protected: \
  virtual ~##className(); \
  ##className(); \

#define DECLARE_SCRIPT_ENTITY_PROPERTY(propName, propType, propScriptType) \
  private: \
   propScriptType propName; \
  \
  public: \
   propType Get_ ## propName(); \
   void  Set_ ## propName(propType); \

#define DECLARE_SCRIPT_ENTITY_CLASS_END(className, classSize) \
 public: \
  static void Register(); \
  static className* Create(AllocatorIndex); \
 }; \
 \
 extern EntityType* t##className; \
 \
 ASSERT_CLASS_SIZE(className, classSize);

extern HMODULE g_DirectInput;
typedef HRESULT(__stdcall* DINPUT8CREATEORIGINAL)(HINSTANCE, DWORD, const IID&, LPVOID*, LPUNKNOWN);
extern DINPUT8CREATEORIGINAL DirectInput8Create_Hooked;

// --------------------------------------------------------
// Useful functions.
// by SilentPL (https://github.com/CookiePLMonster)

#define nop(a, s) _nop((void*)(a), (s))
static void _nop(void* pAddress, DWORD size)
{
    DWORD dwAddress = (DWORD)pAddress;
    if (size % 2)
    {
        *(BYTE*)pAddress = 0x90;
        dwAddress++;
    }
    if (size - (size % 2))
    {
        DWORD sizeCopy = size - (size % 2);
        do
        {
            *(WORD*)dwAddress = 0xFF8B;
            dwAddress += 2;
            sizeCopy -= 2;
        } while (sizeCopy);
    }
}

#define PATCH_NOTHING 0x00
#define PATCH_CALL  0xE8
#define PATCH_JUMP_SHORT 0xEB
#define PATCH_JUMP  0xE9

// TODO: add version for 2 byte "jmp near short".
static void hook(DWORD address, void* function, BYTE type) {
    BYTE* patch = (BYTE*)address;
    if (type) *patch = type; // JMP
   // if (type != PATCH_JUMP_SHORT)
    *(DWORD*)(patch + 1) = ((DWORD)function - (address + 5));
    // else
    //  if (((int)function - address) > (char)-127 && ((int)function - address) < (char)127)
    //   *(char*)(patch + 1) = (char)((int)function - address);
}

extern void debug(char* message, ...);

template <typename T>
static inline T clamp(T val, T min, T max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    else
        return val;
}

// --------------------------------------------------------

// Each class applies it's patches in .cpp file, to be called in dllmain.cpp.
extern inline void PATCH_WINDOW();