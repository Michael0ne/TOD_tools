#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>

//	TODO: potentially, make this a DLL instead of compile-time library dependency.
HINSTANCE WindowInstance;
HWND WindowHandle;

void PrintDebug(const char* format, ...)
{
	va_list va;
	va_start(va, format);
	char buffer[2048] = {};
	vsprintf(buffer, format, va);
	OutputDebugString(buffer);
	OutputDebugString("\n");
	va_end(va);
}

BOOL CALLBACK NewSceneDialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

void NewScene_Handler()
{
	HWND DialogHwnd = CreateDialog(WindowInstance, MAKEINTRESOURCE(151), WindowHandle, (DLGPROC)NewSceneDialogProc);
	ShowWindow(DialogHwnd, SW_SHOW);
}

void OpenScene_Handler()
{

}

void (*MenuItemHandlers[])(void) =
{
	NewScene_Handler,
	OpenScene_Handler
};

void CALLBACK ProcessDebugMenuOption(HWND hwnd, HINSTANCE inst, WPARAM itemid)
{
	if (hwnd == NULL ||
		inst == NULL)
	{
		PrintDebug("Empty window handle and/or applcation instance passed!\n");
		return;
	}

	WindowInstance = inst;

	//	NOTE: do whatever you want here when someone clicked on debug menu item.
	switch (itemid)
	{
	case 40001:
		MenuItemHandlers[0]();
		break;
	case 40002:
		MenuItemHandlers[1]();
		break;
	default:
		PrintDebug("Unhandled menu item (%d)!\n", itemid);
		break;
	}
}