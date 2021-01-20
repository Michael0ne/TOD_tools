#include "Window.h"
#include "Config.h"
#include "StreamedSoundBuffers.h"
#include "ScriptDatabase.h"
#include "GfxInternal_Dx9.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "InputGameController.h"
#include "Performance.h"
#include "LogDump.h"

Window* g_Window = nullptr;

unsigned int Window::MessageBoxType[] = {
	0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0
};
const char Window::RegistryKey[] = "Software\\Eidos\\Total Overdose";
HINSTANCE Window::WindowInstanceHandle = nullptr;
LPSTR Window::CmdLine = nullptr;
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

	return false;
}

void Window::SetWindowResolutionRaw(const D3DDISPLAYMODE& resolution)
{
	SetWindowPos(m_WindowHandle, 0, 0, 0, resolution.Width, resolution.Height, SWP_NOMOVE);
}

void Window::SetWindowResolutionDontMove(const Vector2<float>& resolution)
{
	tagRECT		Rect = { 0, 0, (LONG)resolution.x, (LONG)resolution.y };
	DWORD		dwMenuName = GetClassLongA(m_WindowHandle, GCL_MENUNAME);
	DWORD		dwStyle = GetWindowLongA(m_WindowHandle, GWL_STYLE);

	AdjustWindowRect(&Rect, dwStyle, dwMenuName);
	SetWindowPos(m_WindowHandle, 0, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE);
}

void Window::_GetWindowRect(Vector2<LONG>& outRect)
{
	tagRECT		WindowRect;

	if (m_WindowHandle) {
		GetWindowRect(m_WindowHandle, &WindowRect);

		m_WindowLeft	= WindowRect.left;
		m_WindowTop	= WindowRect.top;
	}

	if (m_WindowLeft < 0 || m_WindowTop < 0) {
		m_WindowLeft = 0;
		m_WindowTop	= 0;
	}

	outRect.x	= m_WindowLeft;
	outRect.y	= m_WindowTop;
}

void Window::GetTopCorner(Vector2<LONG>& outRect) const
{
	tagRECT		Rect;

	GetClientRect(m_WindowHandle, &Rect);

	outRect.x	= Rect.left;
	outRect.y	= Rect.top;
}

void Window::GetWindowCenterRelative(Vector2<LONG>& outRect) const
{
	tagRECT		Rect;

	GetWindowRect(m_WindowHandle, &Rect);

	outRect.x = Rect.right - Rect.left;
	outRect.y = Rect.bottom - Rect.top;
}

void Window::GetClientCenterRelative(Vector2<LONG>& outRect) const
{
	tagRECT		Rect;

	GetClientRect(m_WindowHandle, &Rect);

	outRect.x	= Rect.right - Rect.left;
	outRect.y	= Rect.bottom - Rect.top;
}

void Window::_SetWindowPos(Vector2<int>& pos)
{
	SetWindowPos(m_WindowHandle, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

void Window::SetWindowPosNoCopyBits(tagPOINT* newPos)
{
	tagRECT		WindowRect;
	tagRECT		NewRect;

	DWORD	dwMenuName = GetClassLongA(m_WindowHandle, GCL_MENUNAME);
	DWORD	dwStyle = GetWindowLongA(m_WindowHandle, GWL_STYLE);

	GetWindowRect(m_WindowHandle, &WindowRect);

	NewRect.left = newPos->x;
	NewRect.top = newPos->y;
	NewRect.right = WindowRect.right - WindowRect.left + newPos->x;
	NewRect.bottom = WindowRect.bottom - WindowRect.top + newPos->y;

	AdjustWindowRect(&NewRect, dwStyle, dwMenuName);
	SetWindowPos(m_WindowHandle, 0, NewRect.left, NewRect.top, 0, 0, SWP_NOCOPYBITS);
}

void Window::UpdateVisibility()
{
	bool	WindowVisible = true;

	WINDOWPLACEMENT	windowPlacement;
	windowPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_WindowHandle, &windowPlacement);

	if ((windowPlacement.showCmd == SW_MINIMIZE || windowPlacement.showCmd == SW_SHOWMINIMIZED) && !(m_Flags & 4))
		WindowVisible = false;

	if (m_Visible != WindowVisible) {
		m_Visible = WindowVisible;

		if (WindowVisible) {
			g_StreamedSoundBuffers->SetGlobalPause(false);
		}else{
			g_StreamedSoundBuffers->SetGlobalPause(true);
			g_StreamedSoundBuffers->MeasureWaitForSoftPause();
		}
	}
}

void Window::SetCursorReleased(bool bReleased)
{
	m_CursorReleased = bReleased;

	ShowCursor(bReleased);

	if (bReleased) {
		ClipCursor(0);

		return;
	}

	tagRECT		WindowRect;
	tagPOINT	Point = { 0, 0 };
	RECT		ClipRect;

	SetActiveWindow(m_WindowHandle);
	GetClientRect(m_WindowHandle, &WindowRect);
	ClientToScreen(m_WindowHandle, &Point);

	ClipRect.left	= Point.x;
	ClipRect.top	= Point.y;
	ClipRect.bottom	= WindowRect.bottom;
	ClipRect.right	= WindowRect.right;

	ClipCursor(&ClipRect);
}

int	Window::GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type)
{
	switch (MessageBox(m_WindowHandle, lpText, lpCaption, MessageBoxType[type])) {
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

LanguageCode CALLBACK GetSystemLanguageCode()
{
	CHAR	LocaleData[8];
	int		langcode = 0;
	bool	bUnk;

	GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, LocaleData, 5);
	sscanf(LocaleData, "%x", &langcode);

	if (langcode > 5132)
	{
		if (langcode > 13322)
		{
			if (langcode > 17418)
			{
				if (langcode == 18442 || langcode == 19466)
					return LANGCODE_SPANISH;
				bUnk = langcode == 20490;
			}
			else
			{
				if (langcode == 17418 || langcode == 14346 || langcode == 15370)
					return LANGCODE_SPANISH;
				bUnk = langcode == 16394;
			}
		}
		else
		{
			if (langcode == 13322)
				return LANGCODE_SPANISH;
			if (langcode > 9226)
			{
				if (langcode == 10250 || langcode == 11274)
					return LANGCODE_SPANISH;
				langcode = langcode == 12298;
			}
			else
			{
				if (langcode == 9226)
					return LANGCODE_SPANISH;
				if (langcode <= 7178)
				{
					if (langcode == 7178 || langcode == 6154)
						return LANGCODE_SPANISH;
					if (langcode == 6156)
						return LANGCODE_FRENCH;
					return LANGCODE_ENGLISH;
				}
				bUnk = langcode == 8202;
			}
		}

		if (bUnk)
			return LANGCODE_SPANISH;
		return LANGCODE_ENGLISH;
	}
	if (langcode == 5132)
		return LANGCODE_FRENCH;
	if (langcode > 3079)
	{
		if (langcode <= 4106)
		{
			switch (langcode)
			{
			case 4106:
			case 3082:
				return LANGCODE_SPANISH;
			case 3084:
				return LANGCODE_FRENCH;
			case 4103:
				return LANGCODE_GERMAN;
			}
			return LANGCODE_ENGLISH;
		}
		if (langcode == 4108)
			return LANGCODE_FRENCH;
		if (langcode == 5127)
			return LANGCODE_GERMAN;
		bUnk = langcode == 5130;

		if (bUnk)
			return LANGCODE_SPANISH;
		return LANGCODE_ENGLISH;
	}
	if (langcode == 3079)
		return LANGCODE_GERMAN;
	if (langcode <= 2055)
	{
		if (langcode != 2055)
		{
			switch (langcode)
			{
			case 1031:
				return LANGCODE_GERMAN;
			case 1034:
				return LANGCODE_SPANISH;
			case 1036:
				return LANGCODE_FRENCH;
			case 1040:
				return LANGCODE_ITALIAN;
			default:
				return LANGCODE_ENGLISH;
			}
			return LANGCODE_ENGLISH;
		}
		return LANGCODE_GERMAN;
	}
	if (langcode != 2058)
	{
		if (langcode != 2060)
		{
			if (langcode == 2064)
				return LANGCODE_ITALIAN;
			return LANGCODE_ENGLISH;
		}
		return LANGCODE_FRENCH;
	}
	return LANGCODE_SPANISH;
}

void FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize)
{
	*outString = 0;
	LPVOID pStringResource = LockResource(LoadResource(NULL, FindResourceExA(NULL, RT_STRING, (LPCSTR)((nBaseStringResourcesAddr >> 4) + 1), 0)));

	if (pStringResource) {
		if (nBaseStringResourcesAddr & 15) {
			int	unk1 = nBaseStringResourcesAddr & 15;
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

void IncompatibleMachineParameterError(ErrorMessageId messageID, char bWarningIcon)
{
	signed int nMessageId = NULL;

	switch (messageID) {
	case ERRMSG_INCOMPATIBLE_MOUSE:
		nMessageId = 230;
		break;
	case ERRMSG_INCOMPATIBLE_KEYBOARD:
		nMessageId = 240;
		break;
	case ERRMSG_INCOMPATIBLE_SOUNDCARD:
		nMessageId = 250;
		break;
	case ERRMSG_INCOMPATIBLE_VIDEOCARD:
		nMessageId = 280;
		break;
	case ERRMSG_DIRECTX9_NOT_FOUND:
		nMessageId = 260;
		break;
	case ERRMSG_DISC_NOT_FOUND:
		nMessageId = 270;
		break;
	case ERRMSG_INSUFFICIENT_RAM:
		nMessageId = 290;
		break;
	}

	WCHAR	Caption, Text;
	FindStringResource(Script::LanguageStringsOffset + 220, &Caption, 256);
	FindStringResource(Script::LanguageStringsOffset + nMessageId, &Text, 512);

	if (!bWarningIcon)
	{
		MessageBoxW(g_Window->m_WindowHandle, &Text, &Caption, MB_ICONERROR);
		exit(1);
	}else
		MessageBoxW(g_Window->m_WindowHandle, &Text, &Caption, MB_ICONWARNING);
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
		Window::StickyKeysFeature.dwFlags &= 0xFFFFFFF3;	//	SKF_STICKYKEYSON | SKF_AVAILABLE
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
	}

	if (!(Window::ToggleKeysFeature.dwFlags & TKF_TOGGLEKEYSON)) {
		Window::ToggleKeysFeature.dwFlags &= 0xFFFFFFF3;	//	TKF_TOGGLEKEYSON | TKF_AVAILABLE
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
	}

	if (!(Window::FilterKeysFeature.dwFlags & FKF_FILTERKEYSON)) {
		Window::FilterKeysFeature.dwFlags &= 0xFFFFFFF3;	//	FKF_FILTERYSON | FKF_AVAILABLE
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);
	}
}

void Window::_CreateWindow(UINT16 nIconResourceId)
{
	DWORD windowStyle = (m_Flags & 1) != 0 ? WS_CAPTION | WS_MAXIMIZE : WS_CAPTION | WS_MINIMIZE;
	if (nIconResourceId)
		windowStyle |= WS_SYSMENU;

	m_WindowHandle = CreateWindowExA(WS_EX_APPWINDOW, m_WindowTitle.m_szString, m_WindowTitle.m_szString, windowStyle, NULL, NULL, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, WindowInstanceHandle, nullptr);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Vector2<LONG>	wndRect;

	if (Msg <= WM_SETCURSOR)
	{
		if (Msg != WM_SETCURSOR)
		{
			if (Msg == WM_DESTROY)
			{
				g_Window->_GetWindowRect(wndRect);
				g_Window->m_CursorReleased = true;
				ShowCursor(1);
				ClipCursor(0);
				PostQuitMessage(0);
				g_Window->m_WindowHandle = NULL;

				return 0;
			}

			if (Msg != WM_ERASEBKGND)
			{
				if (Msg == WM_ACTIVATEAPP && !wParam && g_RendererDx)
					if (g_RendererDx->m_Windowed)
						ShowWindow(hWnd, SW_MINIMIZE);

				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}
			return 1;
		}
		if (!g_Window->m_CursorReleased)
		{
			SetCursor(0);
			return 0;
		}
		SetCursor(g_Window->m_Cursor);

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (Msg != WM_NCHITTEST)
	{
		if (Msg == WM_COMMAND)
		{
			if (g_Window->m_MenuItemClickedCallback)
				g_Window->m_MenuItemClickedCallback(wParam);

			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}

		if (Msg == WM_SYSCOMMAND &&
			(wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) &&
			g_RendererDx->m_Windowed)
			return 1;
		
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (!g_RendererDx->m_Windowed)
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	return 1;
}

void Window::Process(bool (*GameLoop)(void))
{
	ShowWindow(m_WindowHandle, SW_SHOW);

	while (true)
	{
		if (m_QuitRequested)
			SendMessage(m_WindowHandle, WM_CLOSE, 0, 0);

		if (!ProcessMessages())
			break;

		UpdateVisibility();

		if (m_Visible)
		{
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

	LogDump::LogA("Creating menu with resource ID: %d\n", nMenuResourceId);

	WndClass.hCursor = 0;
	WndClass.hIcon = LoadIcon(WindowInstanceHandle, MAKEINTRESOURCE(nIconResourceId));
	WndClass.lpszMenuName = MAKEINTRESOURCE(nMenuResourceId);
	WndClass.lpszClassName = m_WindowTitle.m_szString;
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
		g_Window->m_QuitRequested = true;

		return 0;
	}

	char szBuffer[8];
	memset(szBuffer, 0, sizeof(szBuffer));

	int written = sprintf(szBuffer, "%i", wParam);

	szBuffer[written] = NULL;

	MessageBoxA(g_Window->m_WindowHandle, szBuffer, g_Window->m_WindowTitle.m_szString, MB_ICONEXCLAMATION | MB_OK);

	return 1;
}
#endif

//	NOTE: szFileSystem is passed in, but never used.
Window::Window(const char* wndClassName, int flags, UINT16 nMenuResourceId, char* szFileSystem, UINT16 nIconResourceId)
{
	MESSAGE_CLASS_CREATED(Window);

	HKEY				phkResult;
	char				szDesktopPath[MAX_PATH];
	MEMORYSTATUSEX		memoryStatus;

	patch(0xa35eb8, this, 4);

	m_Flags = flags;
	m_Visible = true;
#ifdef INCLUDE_FIXES
	m_MenuItemClickedCallback = MenuClickCallback;
#else
	m_MenuItemClickedCallback = nullptr;
#endif
	m_CursorReleased = true;
	m_QuitRequested = false;

	m_WindowTitle = wndClassName;

	if (!RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\", 0, KEY_QUERY_VALUE, &phkResult)) {
		DWORD	nMaxValueSize = MAX_PATH;
		RegQueryValueEx(phkResult, "Desktop", NULL, NULL, (byte*)szDesktopPath, &nMaxValueSize);
		RegCloseKey(phkResult);
	}

	m_UserDesktopPath = szDesktopPath;

	RegisterWindowClass(nMenuResourceId, nIconResourceId);
	_CreateWindow(nMenuResourceId);

	m_Cursor = LoadCursor(NULL, IDC_ARROW);

	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);

	SetAccessibilityFeatures(false);

	memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memoryStatus);

	if (memoryStatus.ullTotalPhys < 0xFB00000) {
		WCHAR	Caption, Text;

		FindStringResource(Script::LanguageStringsOffset + 220, &Caption, 256);
		FindStringResource(Script::LanguageStringsOffset + 290, &Text, 512);

		MessageBoxW(m_WindowHandle, &Text, &Caption, MB_ICONEXCLAMATION);
	}
}

Window::~Window()
{
	MESSAGE_CLASS_DESTROYED(Window);

	if (m_WindowHandle)
		DestroyWindow(m_WindowHandle);

	SystemParametersInfoA(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Window::StickyKeysFeature, 0);
	SystemParametersInfoA(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Window::ToggleKeysFeature, 0);
	SystemParametersInfoA(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Window::FilterKeysFeature, 0);
}

BOOL Window::_SetTitle(LPCSTR lpString)
{
	m_WindowTitle = lpString;

	return SetWindowText(m_WindowHandle, lpString);
}

void Window::SetDesktopDirectory(const char* pDesktopPath)
{
	m_UserDesktopPath = pDesktopPath;
}

void SetGlobalInstanceHandle(HINSTANCE inst)
{
	Window::WindowInstanceHandle = inst;
}

void SetGlobalCmdLinePtr(LPSTR cmdl)
{
	Window::CmdLine = cmdl;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Performance::Init();
	Sleep(10);
	Performance::Calculate();

	Window::WindowInstanceHandle = hInstance;
	Window::CmdLine = lpCmdLine;

	char exefname[1024];
	char exedir[1024];
	char exeext[8];
	char fname[16];
	DWORD procIdArr[1024];
	DWORD procIdRetnSize = NULL;

	GetModuleFileName(GetModuleHandle(nullptr), exefname, sizeof(exefname));
	File::ExtractFilePath(exefname, exedir, fname, exeext);

	char exefullname[32] = {};
	strcpy(exefullname, fname);
	strcat(exefullname, ".");
	strcat(exefullname, exeext);

	if (EnumProcesses(procIdArr, sizeof(procIdArr), &procIdRetnSize))
	{
		unsigned int copiesRunning = NULL;

		if (procIdRetnSize >> 2)
		{
			for (unsigned int i = NULL; i < (procIdRetnSize >> 2); i++)
				if (IsProcessAGameProcess(procIdArr[i], exefullname))
					copiesRunning++;

#ifdef INCLUDE_FIXES
			LogDump::LogA("Found %d already running game instances!\n", copiesRunning);
#else
			if (copiesRunning > 1)
				exit(1);
#endif
		}

		FindGameDir();
		GameConfig::InitialiseGame(lpCmdLine);
	}
	else
		return NULL;

	return NULL;
}

bool IsProcessAGameProcess(DWORD procId, LPCSTR exename)
{
	HMODULE procModHnd;
	DWORD procModRetn;
	CHAR procModBaseName[MAX_PATH];
	HANDLE procHnd = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, procId);

	if (procHnd && EnumProcessModules(procHnd, &procModHnd, sizeof(procModHnd), &procModRetn))
	{
		GetModuleBaseName(procHnd, procModHnd, procModBaseName, sizeof(procModBaseName));

		if (!strcmp(procModBaseName, exename))
			return true;
	}

	CloseHandle(procHnd);

	return false;
}

void GetUserDocumentsDir(String& outString)
{
	char pszPath[MAX_PATH];

	if (SHGetFolderPath(0, CSIDL_MYDOCUMENTS | CSIDL_FLAG_CREATE, NULL, NULL, pszPath) != S_FALSE)
		outString = pszPath;
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

	*str = buffer;
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

	*str = buffer;
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

inline void PATCH_WINDOW()
{
	void * dwFunc;

	hook(0x4855D6, &GetSystemLanguageCode, PATCH_CALL);

	_asm	mov		eax, offset Window::GetMessageBoxResultButton
	_asm	mov		dwFunc, eax
	//	Override GetMessageBoxResultButton function.
	hook(0x470AF5, dwFunc, PATCH_NOTHING);

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

	_asm	mov		eax, offset Window::ProcessMessages
	_asm	mov		dwFunc, eax
	//	Override ShouldProcessMessages function.
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

	//	Override WinMain function.
	hook(0x95383F, &WinMain, PATCH_CALL);
}