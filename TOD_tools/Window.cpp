#include "Window.h"
#include "GfxInternal_Dx9.h"
#include "StringsPool.h"
#include "Config.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "InputGameController.h"
#include "StreamedSoundBuffers.h"
#include "Performance.h"
#include "Scene.h"
#include "LogDump.h"
#include "ScriptDatabase.h"

Window* g_Window = nullptr;
unsigned int Window::MessageBoxType[] = {
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0
};
const char Window::RegistryKey[] = "Software\\Eidos\\Total Overdose";
HINSTANCE Window::WindowInstanceHandle = nullptr;
STICKYKEYS Window::StickyKeysFeature = { 8, 0 };
TOGGLEKEYS Window::ToggleKeysFeature = { 8, 0 };
FILTERKEYS Window::FilterKeysFeature = { 24, 0, 0, 0, 0, 0 };
bool Window::GameDiscFound = false;
String WorkingDirectory;
String GameWorkingDirectory;

bool Window::ProcessMessages()
{
	tagMSG	Msg;

	if (!PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
		return true;

	while (Msg.message != WM_QUIT) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);

		if (!PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
			return true;
	}

	return 0;
}

void Window::SetWindowResolutionRaw(const D3DDISPLAYMODE& resolution)
{
	SetWindowPos(m_hWindow, 0, 0, 0, resolution.Width, resolution.Height, SWP_NOMOVE);
}

void Window::SetWindowResolutionDontMove(const Vector2<float>& resolution)
{
	tagRECT		Rect = { 0, 0, (LONG)resolution.x, (LONG)resolution.y };
	DWORD		dwMenuName = GetClassLongA(m_hWindow, GCL_MENUNAME);
	DWORD		dwStyle = GetWindowLongA(m_hWindow, GWL_STYLE);

	AdjustWindowRect(&Rect, dwStyle, dwMenuName);
	SetWindowPos(m_hWindow, 0, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE);
}

void Window::_GetWindowRect(Vector2<LONG>& outRect)
{
	tagRECT		WindowRect;

	if (m_hWindow) {
		GetWindowRect(m_hWindow, &WindowRect);

		m_nWindowLeft	= WindowRect.left;
		m_nWindowTop	= WindowRect.top;
	}

	if (m_nWindowLeft < 0 || m_nWindowTop < 0) {
		m_nWindowLeft	= 0;
		m_nWindowTop	= 0;
	}

	outRect.x	= m_nWindowLeft;
	outRect.y	= m_nWindowTop;
}

void Window::GetTopCorner(Vector2<LONG>& outRect)
{
	tagRECT		Rect;

	GetClientRect(m_hWindow, &Rect);

	outRect.x	= Rect.left;
	outRect.y	= Rect.top;
}

void Window::GetWindowCenterRelative(Vector2<LONG>& outRect)
{
	tagRECT		Rect;

	GetWindowRect(m_hWindow, &Rect);

	outRect.x = Rect.right - Rect.left;
	outRect.y = Rect.bottom - Rect.top;
}

void Window::GetClientCenterRelative(Vector2<LONG>& outRect)
{
	tagRECT		Rect;

	GetClientRect(m_hWindow, &Rect);

	outRect.x	= Rect.right - Rect.left;
	outRect.y	= Rect.bottom - Rect.top;
}

void Window::_SetWindowPos(Vector2<int>& pos)
{
	SetWindowPos(m_hWindow, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

void Window::SetWindowPosNoCopyBits(tagPOINT* newPos)
{
	tagRECT		WindowRect;
	tagRECT		NewRect;

	DWORD	dwMenuName = GetClassLongA(m_hWindow, GCL_MENUNAME);
	DWORD	dwStyle = GetWindowLongA(m_hWindow, GWL_STYLE);

	GetWindowRect(m_hWindow, &WindowRect);

	NewRect.left = newPos->x;
	NewRect.top = newPos->y;
	NewRect.right = WindowRect.right - WindowRect.left + newPos->x;
	NewRect.bottom = WindowRect.bottom - WindowRect.top + newPos->y;

	AdjustWindowRect(&NewRect, dwStyle, dwMenuName);
	SetWindowPos(m_hWindow, 0, NewRect.left, NewRect.top, 0, 0, SWP_NOCOPYBITS);
}

void Window::UpdateVisibility()
{
	bool	bWindowVisible = true;

	WINDOWPLACEMENT	windowPlacement;
	windowPlacement.length	= 44;
	GetWindowPlacement(m_hWindow, &windowPlacement);

	if ((windowPlacement.showCmd == SW_MINIMIZE || windowPlacement.showCmd == SW_SHOWMINIMIZED) && !(m_unkFlags & 4))
		bWindowVisible = false;

	if (m_bVisible != bWindowVisible) {
		m_bVisible = bWindowVisible;

		if (bWindowVisible) {
			g_StreamedSoundBuffers->SetGlobalPause(false);
		}else{
			g_StreamedSoundBuffers->SetGlobalPause(true);
			g_StreamedSoundBuffers->MeasureWaitForSoftPause();
		}
	}
}

void Window::SetCursorReleased(bool bReleased)
{
	m_bCursorReleased = bReleased;

	ShowCursor(bReleased);

	if (bReleased) {
		ClipCursor(0);

		return;
	}

	tagRECT		WindowRect;
	tagPOINT	Point = { 0, 0 };
	RECT		ClipRect;

	SetActiveWindow(m_hWindow);
	GetClientRect(m_hWindow, &WindowRect);
	ClientToScreen(m_hWindow, &Point);

	ClipRect.left	= Point.x;
	ClipRect.top	= Point.y;
	ClipRect.bottom	= WindowRect.bottom;
	ClipRect.right	= WindowRect.right;

	ClipCursor(&ClipRect);
}

int	Window::GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type)
{
	switch (MessageBox(m_hWindow, lpText, lpCaption, MessageBoxType[type])) {
	case IDCANCEL:
		return 2;
		break;
	case IDYES:
		return 4;
		break;
	case IDNO:
		return 8;
		break;
	default:
		return 1;
		break;
	}
}

int CALLBACK GetSystemLanguageCode()
{
	CHAR	LocaleData;
	int		nLanguageCode = 0;
	bool	bUnk;

	GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, &LocaleData, 5);
	int result = sscanf(&LocaleData, "%x", &nLanguageCode);

	if (result == EOF)
		return -1;

	debug("Language code is: %d\n", nLanguageCode);

	if (nLanguageCode > 5132)
	{
		if (nLanguageCode > 13322)
		{
			if (nLanguageCode > 17418)
			{
				if (nLanguageCode == 18442 || nLanguageCode == 19466)
					return 4;
				bUnk = nLanguageCode == 20490;
			}
			else
			{
				if (nLanguageCode == 17418 || nLanguageCode == 14346 || nLanguageCode == 15370)
					return 4;
				bUnk = nLanguageCode == 16394;
			}
		}
		else
		{
			if (nLanguageCode == 13322)
				return 4;
			if (nLanguageCode > 9226)
			{
				if (nLanguageCode == 10250 || nLanguageCode == 11274)
					return 4;
				nLanguageCode = nLanguageCode == 12298;
			}
			else
			{
				if (nLanguageCode == 9226)
					return 4;
				if (nLanguageCode <= 7178)
				{
					if (nLanguageCode == 7178 || nLanguageCode == 6154)
						return 4;
					if (nLanguageCode == 6156)
						return 1;
					return 0;
				}
				bUnk = nLanguageCode == 8202;
			}
		}

		if (bUnk)
			return 4;
		return 0;
	}
	if (nLanguageCode == 5132)
		return 1;
	if (nLanguageCode > 3079)
	{
		if (nLanguageCode <= 4106)
		{
			switch (nLanguageCode)
			{
			case 4106:
			case 3082:
				return 4;
			case 3084:
				return 1;
			case 4103:
				return 3;
			}
			return 0;
		}
		if (nLanguageCode == 4108)
			return 1;
		if (nLanguageCode == 5127)
			return 3;
		bUnk = nLanguageCode == 5130;

		if (bUnk)
			return 4;
		return 0;
	}
	if (nLanguageCode == 3079)
		return 3;
	if (nLanguageCode <= 2055)
	{
		if (nLanguageCode != 2055)
		{
			switch (nLanguageCode)
			{
			case 1031:
				return 3;
			case 1034:
				return 4;
			case 1036:
				return 1;
			case 1040:
				return 2;
			default:
				return 0;
			}
			return 0;
		}
		return 3;
	}
	if (nLanguageCode != 2058)
	{
		if (nLanguageCode != 2060)
		{
			if (nLanguageCode == 2064)
				return 2;
			return 0;
		}
		return 1;
	}
	return 4;

	//	2 - Italian
	//	4 - Spanish
	//	3 - German
	//	1 - French
	//	0 - English
}

void FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize)
{
	int	unk1 = nBaseStringResourcesAddr & 0xF;
	*outString = 0;

	HRSRC strResHandle = FindResourceExA(NULL, RT_STRING, (LPCSTR)((nBaseStringResourcesAddr >> 4) + 1), 0);

	if (!strResHandle)
		return;

	HGLOBAL stringResourceData = LoadResource(NULL, strResHandle);

	if (!stringResourceData)
		return;

	LPVOID pStringResource = LockResource(stringResourceData);

	if (pStringResource) {
		if (nBaseStringResourcesAddr & 0xF) {
			do {
				unk1--;
				pStringResource = (char*)pStringResource + 2 * *(UINT16*)pStringResource + 2;
			} while (unk1);
		}

		signed int length = *(UINT16*)pStringResource;
		if (nMaxsize - 1 < length)
			length = nMaxsize - 1;

		wcsncpy(outString, (const wchar_t*)pStringResource + 1, length);

		length = *(UINT16*)pStringResource;
		if (nMaxsize - 1 < length)
			length = nMaxsize - 1;

		outString[length] = 0;
	}
}

void IncompatibleMachineParameterError(int messageID, char bWarningIcon)
{
	signed int nMessageId;

	switch (messageID) {
	case 0:
		nMessageId = 230;
		break;
	case 1:
		nMessageId = 240;
		break;
	case 2:
		nMessageId = 250;
		break;
	case 3:
		nMessageId = 280;
		break;
	case 4:
		nMessageId = 260;
		break;
	case 5:
		nMessageId = 270;
		break;
	case 6:
		nMessageId = 290;
		break;
	default:
		nMessageId = 0;
		break;
	}

	WCHAR	Caption, Text;
	FindStringResource(Script::LanguageStringsOffset + 220, &Caption, 256);
	FindStringResource(Script::LanguageStringsOffset + nMessageId, &Text, 512);

	MessageBoxW(g_Window->m_hWindow, &Text, &Caption, bWarningIcon ? MB_ICONWARNING : MB_ICONERROR);

	//	TODO: this calls all destructors for all global objects. Use with caution.
	exit(1);
}

void SetAccessibilityFeatures(bool bCollect)
{
	if (bCollect) {
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);

		return;
	}

	if (!(Window::StickyKeysFeature.dwFlags & SKF_STICKYKEYSON)) {
		Window::StickyKeysFeature.dwFlags &= 0xFFFFFFF3;
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
	}

	if (!(Window::ToggleKeysFeature.dwFlags & TKF_TOGGLEKEYSON)) {
		Window::ToggleKeysFeature.dwFlags &= 0xFFFFFFF3;
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
	}

	if (!(Window::FilterKeysFeature.dwFlags & FKF_FILTERKEYSON)) {
		Window::FilterKeysFeature.dwFlags &= 0xFFFFFFF3;
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);
	}
}

void Window::_CreateWindow(UINT16 nIconResourceId)
{
	DWORD windowStyle = (m_unkFlags & 1) != 0 ? WS_CAPTION | WS_MAXIMIZE : WS_CAPTION | WS_MINIMIZE;
	if (nIconResourceId)
		windowStyle |= WS_SYSMENU;

	m_hWindow = CreateWindowExA(WS_EX_APPWINDOW, m_sWindowTitle.m_szString, m_sWindowTitle.m_szString, windowStyle, 0, 0, 0x80000000, 0x80000000, 0, 0, WindowInstanceHandle, 0);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Vector2<LONG>	wndRect;

	if (Msg <= WM_SETCURSOR) {
		if (Msg != WM_SETCURSOR) {
			if (Msg == WM_DESTROY) {
				g_Window->_GetWindowRect(wndRect);
				g_Window->m_bCursorReleased = true;
				ShowCursor(1);
				ClipCursor(0);
				PostQuitMessage(0);
				g_Window->m_hWindow = NULL;

				return 0;
			}
			if (Msg != WM_ERASEBKGND) {
				if (Msg == WM_ACTIVATEAPP && !wParam && g_RendererDx)
					if (g_RendererDx->m_Windowed)
						ShowWindow(hWnd, SW_MINIMIZE);
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}
			return 1;
		}
		if (!g_Window->m_bCursorReleased) {
			SetCursor(0);
			return 0;
		}
		SetCursor(g_Window->m_hCursor);

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (Msg != WM_NCHITTEST) {
		if (Msg == WM_COMMAND) {
			if (g_Window->m_pMenuItemClickedCallback)
				g_Window->m_pMenuItemClickedCallback(wParam);
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
		if (Msg == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) && g_RendererDx->m_Windowed)
			return 1;
		
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (!g_RendererDx->m_Windowed)
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	return 1;
}

void Window::Process(bool(*GameLoop)(void))
{
	ShowWindow(m_hWindow, SW_SHOW);

	while (true) {
		if (m_bQuitRequested)
			SendMessage(m_hWindow, WM_CLOSE, 0, 0);

		if (!ProcessMessages())
			break;

		UpdateVisibility();

		if (m_bVisible) {
			if (g_InputMouse)
				g_InputMouse->Process();

			if (g_InputKeyboard)
				g_InputKeyboard->Process();

			Input::Gamepad::Process();

			if (!GameLoop())
				return;

			if (g_InputMouse)
				g_InputMouse->ResetButtonsState();

			if (g_InputKeyboard)
				g_InputKeyboard->Reset();
		}
		else
			WaitMessage();
	}
}

ATOM Window::RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId)
{
	WNDCLASSA WndClass;

	debug("Creating menu with resource ID: %d\n", nMenuResourceId);

	WndClass.hCursor = 0;
	WndClass.hIcon = LoadIcon(WindowInstanceHandle, MAKEINTRESOURCE(nIconResourceId));
	WndClass.lpszMenuName = MAKEINTRESOURCE(nMenuResourceId);
	WndClass.lpszClassName = m_sWindowTitle.m_szString;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hInstance = WindowInstanceHandle;
	WndClass.style = CS_VREDRAW | CS_HREDRAW;
	WndClass.lpfnWndProc = WindowProc;
	WndClass.cbClsExtra = NULL;
	WndClass.cbWndExtra = NULL;

	return RegisterClassA(&WndClass);
}

#ifdef INCLUDE_FIXES
int CALLBACK MenuClickCallback(WPARAM wParam)
{
	if (wParam == 40006) {
		//	ID_EXIT
		g_Window->m_bQuitRequested = true;

		return 0;
	}

	char szBuffer[8];
	memset(szBuffer, 0, sizeof(szBuffer));

	int written = sprintf(szBuffer, "%i", wParam);

	szBuffer[written] = NULL;

	MessageBoxA(g_Window->m_hWindow, szBuffer, g_Window->m_sWindowTitle.m_szString, MB_ICONEXCLAMATION | MB_OK);

	return 1;
}
#endif

//	NOTE: szFileSystem is passed in, but never used.
Window::Window(const char* wndClassName, int unkParam1, UINT16 nMenuResourceId, char* szFileSystem, UINT16 nIconResourceId)
{
	MESSAGE_CLASS_CREATED(Window);

	HKEY				phkResult;
	char				szDesktopPath[MAX_PATH];
	MEMORYSTATUSEX		memoryStatus;

	patch(0xa35eb8, this, 4);

	m_unkFlags = unkParam1;
	m_bVisible = true;
#ifndef INCLUDE_FIXES
	m_pMenuItemClickedCallback = NULL;
#else
	m_pMenuItemClickedCallback = MenuClickCallback;
#endif
	m_bCursorReleased = true;
	m_bQuitRequested = false;

	m_sWindowTitle = String(wndClassName);

	if (!RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\", 0, KEY_QUERY_VALUE, &phkResult)) {
		DWORD	nMaxValueSize = MAX_PATH;
		RegQueryValueEx(phkResult, "Desktop", NULL, NULL, (byte*)szDesktopPath, &nMaxValueSize);
		RegCloseKey(phkResult);
	}

	m_sUserDesktopPath = String(szDesktopPath);

	RegisterWindowClass(nMenuResourceId, nIconResourceId);
	_CreateWindow(nMenuResourceId);

	m_hCursor = LoadCursor(NULL, IDC_ARROW);

	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);

	SetAccessibilityFeatures(false);

	memoryStatus.dwLength = 64;
	GlobalMemoryStatusEx(&memoryStatus);

	if (memoryStatus.ullTotalPhys < 0xFB00000) {
		WCHAR	Caption, Text;

		FindStringResource(Script::LanguageStringsOffset + 220, &Caption, 256);
		FindStringResource(Script::LanguageStringsOffset + 290, &Text, 512);

		MessageBoxW(m_hWindow, &Text, &Caption, MB_ICONEXCLAMATION);
	}
}

Window::~Window()
{
	MESSAGE_CLASS_DESTROYED(Window);

	if (m_hWindow)
		DestroyWindow(m_hWindow);

	SystemParametersInfoA(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
	SystemParametersInfoA(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
	SystemParametersInfoA(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);
}

BOOL Window::_SetTitle(LPCSTR lpString)
{
	m_sWindowTitle = String(lpString);

	return SetWindowText(m_hWindow, lpString);
}

void Window::SetDesktopDirectory(const char* pDesktopPath)
{
	m_sUserDesktopPath = String(pDesktopPath);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Performance::Init();
	Sleep(10);
	Performance::Calculate();

	Window::WindowInstanceHandle = hInstance;

	//	NOTE: all bullshit related to finding already running game is removed!

	FindGameDir();

	//	NOTE: InitialiseGame has been inlined! Originally at 93F680.
	GameConfig::g_Config = new GameConfig::Config();

	GameConfig::g_Config->Process(lpCmdLine, 0, "", 0);

	//	This is main game loop proc. Process function has standard while loop.
	g_Window->Process(Scene::GameUpdate);

	//	When deleting config class, rest of game objects are deleted as well.
	if (GameConfig::g_Config)
		delete GameConfig::g_Config;

	return 0;
}

void GetUserDocumentsDir(String& outString)
{
	char pszPath[MAX_PATH];

	if (SHGetFolderPath(0, 0x8005, 0, 0, pszPath) != S_FALSE)
		outString.Set(pszPath);
}

void FindGameDir()
{
	char currdir[1024];
	memset(currdir, NULL, sizeof(currdir));

	GetCurrentDirectory(sizeof(currdir), currdir);
	SetWorkingDir(currdir);
	SetGameWorkingDir(currdir);
	Window::GameDiscFound = false;
	SetErrorMode(SEM_FAILCRITICALERRORS);
	char driveLetter = NULL;
	char RootPathName[] = "\\:";

	for (int drive = GetLogicalDrives(); drive; ++driveLetter)
	{
		if (drive & 1)
		{
			*(unsigned char*)&RootPathName = 'A' + driveLetter;
#ifdef INCLUDE_FIXES
			if (GetDriveType(RootPathName) == DRIVE_CDROM || GetDriveType(RootPathName) == DRIVE_FIXED)
#else
			if (GetDriveType(RootPathName) == DRIVE_CDROM)
#endif
			{
				char buffer[64];
				strcpy(buffer, RootPathName);
				strcat(buffer, "\\Program Files\\Eidos\\Total Overdose\\TotalOverdose.exe");

				LogDump::LogA("Found cd/dvd drive %s - looking for ID file '%s'\n", RootPathName, buffer);

				if (GetFileAttributes(buffer) != -1)
				{
					memset(buffer, NULL, sizeof(buffer));
					strcpy(buffer, RootPathName);
					strcat(buffer, "/Program Files/Eidos/Total Overdose/");

					SetGameWorkingDir(buffer);
					Window::GameDiscFound = true;

					LogDump::LogA("Found game disc.\n");

					break;
				}
			}
		}
		drive = drive >> 1;
	}

	SetErrorMode(NULL);
}

void SetWorkingDir(const char* str)
{
	WorkingDirectory = str;
	WorkingDirectory.ConvertBackslashes();

	if (WorkingDirectory.m_szString[WorkingDirectory.m_nLength - 1] != '/')
		WorkingDirectory.Append("/");
}

void SetGameWorkingDir(const char* str)
{
	GameWorkingDirectory = String(str);
	GameWorkingDirectory.ConvertBackslashes();

	if (GameWorkingDirectory.m_szString[GameWorkingDirectory.m_nLength - 1] != '/')
		GameWorkingDirectory.Append("/");
}

void GetWorkingDirRelativePath(String* str)
{
	if (!str->m_nLength)
	{
		*str = WorkingDirectory;

		return;
	}

	char buffer[1024];
	memset(buffer, NULL, sizeof(buffer));

	strcpy(buffer, WorkingDirectory.m_szString);
	if (buffer[WorkingDirectory.m_nLength - 1] == '/' && str->m_szString[0] == '/')
		strcat(buffer, (const char*)((int)str->m_szString + 1));
	else
		strcat(buffer, str->m_szString);

	str->Set(buffer);
}

void GetGameWorkingDirRelativePath(String* str)
{
	if (!str->m_nLength)
	{
		*str = GameWorkingDirectory;

		return;
	}

	char buffer[1024];
	memset(buffer, NULL, sizeof(buffer));

	strcpy(buffer, GameWorkingDirectory.m_szString);
	if (buffer[GameWorkingDirectory.m_nLength - 1] == '/' && str->m_szString[0] == '/')
		strcat(buffer, (const char*)((int)str->m_szString + 1));
	else
		strcat(buffer, str->m_szString);

	str->Set(buffer);
}

void SetWarningString(const char* (*ptr)())
{
	WarningStringProc = ptr;
}

const char* GetWarningString()
{
	if (WarningStringProc)
		return WarningStringProc();
	else
		return NULL;
}

//	Apply patches specific to this class.
inline void PATCH_WINDOW()
{
	void * dwFunc;

	hook(0x4855D6, &GetSystemLanguageCode, PATCH_CALL);

	_asm	mov		eax, offset Window::GetMessageBoxResultButton
	_asm	mov		dwFunc, eax
	//	Override GetMessageBoxResultButton function.
	hook(0x470AF5, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetWindowPosNoCopyBits
	_asm	mov		dwFunc, eax
	//	Override SetWindowPosNoCopyBits function.
	//	TODO: find references to this function!
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::_SetWindowPos
	_asm	mov		dwFunc, eax
	//	Override _SetWindowPos function.
	//	TODO: find references to this function!
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetClientCenterRelative
	_asm	mov		dwFunc, eax
	//	Override GetClientCenterRelative function.
	//	TODO: find references to this function!
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetWindowCenterRelative
	_asm	mov		dwFunc, eax
	//	Override GetWindowCenterRelative function.
	//	TODO: find references to this function!
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetTopCorner
	_asm	mov		dwFunc, eax
	//	Override GetTopCorner function.
	//	TODO: find references to this function!
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetWindowResolutionDontMove
	_asm	mov		dwFunc, eax
	//	Override SetWindowResolutionDontMove function.
	hook(0x45C02C, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetWindowResolutionRaw
	_asm	mov		dwFunc, eax
	//	Override SetWindowResolutionRaw function.
	hook(0x45BE9A, dwFunc, PATCH_NOTHING);
	hook(0x45C000, dwFunc, PATCH_NOTHING);
	hook(0x45C154, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetMenuClickCallback
	_asm	mov		dwFunc, eax
	//	Override SetMenuClickCallback function.
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::ProcessMessages
	_asm	mov		dwFunc, eax
	//	Override ShouldProcessMessages function.
	//hook(0x43C4DD, dwFunc, PATCH_NOTHING); -- removed, since it's referenced in ProcessInputDevices() that is already hooked.
	hook(0x44D150, dwFunc, PATCH_NOTHING);
	hook(0x46F7FB, dwFunc, PATCH_NOTHING);
	hook(0x46FACF, dwFunc, PATCH_NOTHING);
	hook(0x470A55, dwFunc, PATCH_NOTHING);
	hook(0x470B59, dwFunc, PATCH_NOTHING);
	hook(0x470B9E, dwFunc, PATCH_NOTHING);
	hook(0x473C1E, dwFunc, PATCH_NOTHING);
	hook(0x473F88, dwFunc, PATCH_NOTHING);
	hook(0x475A90, dwFunc, PATCH_NOTHING);
	hook(0x475A90, dwFunc, PATCH_NOTHING);
	hook(0x475D4B, dwFunc, PATCH_NOTHING);
	hook(0x475F4B, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::QuitGame
	_asm	mov		dwFunc, eax
	//	Override QuitGame function.
	hook(0x485666, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetCoverdemoPlayMode
	_asm	mov		dwFunc, eax
	//	Override GetCoverdemoPlayMode function.
	hook(0x485676, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetCoverdemoInactiveTimeoutSec
	_asm	mov		dwFunc, eax
	//	Override GetCoverdemoInactiveTimeoutSec function.
	hook(0x485696, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetCoverdemoGameplayTimeoutSec
	_asm	mov		dwFunc, eax
	//	Override GetCoverdemoGameplayTimeoutSec function.
	hook(0x4856B6, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::_SetTitle
	_asm	mov		dwFunc, eax
	//	Override _SetTitle function.
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetDesktopDirectory
	_asm	mov		dwFunc, eax
	//	Override SetDesktopDirectory function.
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	//	Override WinMain function.
	hook(0x95383F, &WinMain, PATCH_CALL);
}