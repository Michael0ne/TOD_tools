#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <unordered_map>

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

void CALLBACK NewScene_Handler(HWND hWnd, HINSTANCE hInst, WPARAM wItemId)
{
	HWND DialogHwnd = CreateDialog(hInst, MAKEINTRESOURCE(151), hWnd, (DLGPROC)NewSceneDialogProc);
	ShowWindow(DialogHwnd, SW_SHOW);
}

void CALLBACK OpenScene_Handler(HWND hWnd, HINSTANCE hInst, WPARAM wItemId)
{
}

static const std::unordered_map<uint64_t, void (__stdcall*)(HWND hWnd, HINSTANCE hInst, WPARAM wItemId)> MenuItemHandlers = {
	{ (uint64_t)40001, NewScene_Handler },
	{ (uint64_t)40002, OpenScene_Handler }
};

void CALLBACK ProcessDebugMenuOption(HWND hwnd, HINSTANCE inst, WPARAM itemid)
{
	if (hwnd == NULL || inst == NULL)
	{
		PrintDebug("Empty window handle and/or applcation instance passed!\n");
		return;
	}

	if (!itemid)
	{
		PrintDebug("Empty itemid passed into a function!\n");
		return;
	}

	const auto& handlerRef = MenuItemHandlers.find(itemid);
	if (handlerRef == MenuItemHandlers.cend())
	{
		PrintDebug("Unhandled menu item (%d)!", itemid);
		return;
	}

	const auto handler = handlerRef->second;
	handler(hwnd, inst, itemid);
}