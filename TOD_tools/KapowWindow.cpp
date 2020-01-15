#include "KapowWindow.h"

void KapowWindow::unkClipCursor(bool bUnk)
{
	return;
}

int	KapowWindow::GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type)
{
	return 1;
}

signed int KapowWindow::GetSystemLanguageCode()
{
	return 1;
}

void* KapowWindow::FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize)
{
	return 0;
}

void KapowWindow::IncompatibleMachineParameterError(int messageID, char bWarningIcon)
{
	return;
}

void KapowWindow::SetAccessibilityFeatures(bool bCollect)
{
	return;
}

void KapowWindow::_DestroyWindow()
{
	return;
}

void KapowWindow::_CreateWindow(UINT16 nIconResourceId)
{
	return;
}

LRESULT CALLBACK KapowWindow::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

ATOM KapowWindow::RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId)
{
	//_RegisterWindowClass(_this, nMenuResourceId, nIconResourceId);
	WNDCLASSA WndClass;

	nMenuResourceId = 103;

	debug("Creating menu with resource ID: %d\n", nMenuResourceId);

	WndClass.hCursor = 0;
	WndClass.hIcon = LoadIconA(*g_hInstance, MAKEINTRESOURCE(nIconResourceId));
	WndClass.lpszMenuName = MAKEINTRESOURCE(nMenuResourceId);
	WndClass.lpszClassName = m_szClassName;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hInstance = *g_hInstance;
	WndClass.style = 3;
	WndClass.lpfnWndProc = _WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;

	debug("Registering window class...\n");

	return RegisterClassA(&WndClass);
}

void KapowWindow::ProcessInputDevices(int(*unkProcPtr)(void))
{
	return;
}

void KapowWindow::InitEnvironment(const char* wndClassName, int unkParam1, UINT16 nMenuResourceId, int unkParam2, UINT16 nIconResourceId)
{
	return;
}

bool KapowWindow::IsProcessAGameProcess(DWORD dwProcessId, int unk1, const char* unkProcName, int unk2, char unk3)
{
	return true;
}

void KapowWindow::GetUserDocumentsDir(DWORD unk1)
{
	return;
}

int CALLBACK KapowWindow::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return 1;
}

void KapowWindow::unkSetter4(int unk)
{
	return;
}

void KapowWindow::unkSetter3(int unk)
{
	return;
}

void KapowWindow::unkSetter2(int unk)
{
	return;
}

void KapowWindow::unkSetter1(int unk)
{
	return;
}

//	Apply patches specific to this class.
inline void PATCH_WINDOW()
{
	void * dwFunc;
	_asm
	{
		mov		eax, offset KapowWindow::RegisterWindowClass
		mov		dwFunc, eax
	}

	//	Override register window function.
	hook(0x43C767, dwFunc, PATCH_NOTHING);
}
