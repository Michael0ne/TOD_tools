#include "Window.h"
#include "GfxInternal_Dx9.h"

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

inline void Window::SetMenuClickCallback(void* pCallback)
{
	m_pMenuItemClickedCallback = pCallback;
}

inline void Window::SetWindowResolutionRaw(const D3DDISPLAYMODE& resolution)
{
	SetWindowPos(m_hWindow, 0, 0, 0, resolution.Width, resolution.Height, SWP_NOMOVE);
}

void Window::SetWindowResolutionDontMove(const D3DDISPLAYMODE& resolution)
{
	tagRECT		Rect = { 0, 0, (LONG)resolution.Width, (LONG)resolution.Height };
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

inline void Window::_SetWindowPos(Vector2<int>& pos)
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

//	TODO: fix. This crashes the game now!
void Window::GlobalPause()
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
			SoundManager__SetGlobalPause(g_soundManager, 0);
		}else{
			SoundManager__SetGlobalPause(g_soundManager, 1);
			SoundManager__MeasureWaitForSoftPause(g_soundManager);
		}
	}
}

void Window::ClipCursorInsideWindow(bool bDontClip)
{
	m_bDestroyed = bDontClip;

	ShowCursor(bDontClip);

	if (bDontClip) {
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
	switch (MessageBox(m_hWindow, lpText, lpCaption, uType[type])) {
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

//	TODO: fix. This crashes the game now!
int Window::GetSystemLanguageCode()
{
	CHAR	LocaleData;
	int		nLanguageCode = 0;
	bool	bUnk;

	GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, &LocaleData, 5);
	sscanf(&LocaleData, "%x", &nLanguageCode);

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

void Window::FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize)
{
	int	unk1 = nBaseStringResourcesAddr & 0xF;
	*outString = 0;

	HRSRC strResHandle = FindResourceExA(NULL, RT_STRING, (LPCSTR)((nBaseStringResourcesAddr >> 4) + 1), 0);
	HGLOBAL stringResourceData = LoadResource(NULL, strResHandle);
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

void Window::IncompatibleMachineParameterError(int messageID, char bWarningIcon)
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
	FindStringResource(*stringResourceBaseAddr + 220, &Caption, 256);
	FindStringResource(*stringResourceBaseAddr + nMessageId, &Text, 512);

	MessageBoxW(m_hWindow, &Text, &Caption, bWarningIcon ? MB_ICONWARNING : MB_ICONERROR);

	//	TODO: this calls exit_0(1) which calls doexit(...) which does something other than just exit().
	exit(1);
}

void Window::SetAccessibilityFeatures(bool bCollect)
{
	if (bCollect) {
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sSTICKYKEYS, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &sTOGGLEKEYS, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &sFILTERKEYS, 0);

		return;
	}

	STICKYKEYS sNewStickyKeys = *sSTICKYKEYS;
	TOGGLEKEYS sNewToggleKeys = *sTOGGLEKEYS;
	FILTERKEYS sNewFilterKeys = *sFILTERKEYS;

	if (!(sSTICKYKEYS->dwFlags & 1)) {
		sNewStickyKeys.dwFlags = sSTICKYKEYS->dwFlags & 0xFFFFFFF3;
		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sNewStickyKeys, 0);
	}

	if (!(sTOGGLEKEYS->dwFlags & 1)) {
		sNewToggleKeys.dwFlags = sTOGGLEKEYS->dwFlags & 0xFFFFFFF3;
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &sNewToggleKeys, 0);
	}

	if (!(sFILTERKEYS->dwFlags & 1)) {
		sNewFilterKeys.dwFlags = sFILTERKEYS->dwFlags & 0xFFFFFFF3;
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &sNewFilterKeys, 0);
	}
}

void Window::Release()
{
	if (m_hWindow)
		DestroyWindow(m_hWindow);

	g_kapowWindow = NULL;

	SystemParametersInfoA(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sSTICKYKEYS, 0);
	SystemParametersInfoA(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &sTOGGLEKEYS, 0);
	SystemParametersInfoA(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &sFILTERKEYS, 0);

	//	TODO: multithreaded code should go here.
}

void Window::_CreateWindow(UINT16 nIconResourceId)
{
	DWORD	windowStyle = (m_unkFlags & 1) != 0 ? 0xC70000 : 0xC20000;
	if (nIconResourceId)
		windowStyle |= 0x80000;

	m_hWindow = CreateWindowExA(0x40000, m_sWindowTitle.m_szString, m_sWindowTitle.m_szString, windowStyle, 0, 0, 0x80000000, 0x80000000, 0, 0, *g_hInstance, 0);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Vector2<LONG>	wndRect;

	if (Msg <= WM_SETCURSOR) {
		if (Msg != WM_SETCURSOR) {
			if (Msg == WM_DESTROY) {
				g_kapowWindow->_GetWindowRect(wndRect);
				g_kapowWindow->m_bDestroyed = true;
				ShowCursor(1);
				ClipCursor(0);
				PostQuitMessage(0);
				g_kapowWindow->m_hWindow = NULL;

				return 0;
			}
			if (Msg != WM_ERASEBKGND) {
				if (Msg == WM_ACTIVATEAPP && !wParam && g_pRenderer)
					if (g_pRenderer->m_bResolutionDetected)
						ShowWindow(hWnd, SW_MINIMIZE);
				return DefWindowProc(hWnd, Msg, wParam, lParam);
			}
			return 1;
		}
		if (!g_kapowWindow->m_bDestroyed) {
			SetCursor(0);
			return 0;
		}
		SetCursor(g_kapowWindow->m_hCursor);

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (Msg != WM_NCHITTEST) {
		if (Msg == WM_COMMAND) {
			if (g_kapowWindow->m_pMenuItemClickedCallback)
				((void(__cdecl *)(DWORD))g_kapowWindow->m_pMenuItemClickedCallback)(wParam);
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
		if (Msg == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) && g_pRenderer->m_bResolutionDetected)
			return 1;
		
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	if (!g_pRenderer->m_bResolutionDetected)
		return DefWindowProc(hWnd, Msg, wParam, lParam);

	return 1;
}

//	TODO: fix. This crashes the game now!
//	TODO: basic implementation for KapowInput and StreamedSoundBuffers.
void Window::ProcessInputDevices(bool(*gameLoop)(void))
{
	ShowWindow(m_hWindow, SW_SHOW);

	while (true) {
		if (m_bQuitRequested)
			SendMessage(m_hWindow, WM_CLOSE, 0, 0);

		if (!ProcessMessages())
			break;

		GlobalPause();

		if (m_bVisible) {
			if (g_pInputMouse)
				KapowInput__ProcessMouse(g_pInputMouse);

			if (g_pInputKeyboard)
				KapowInput__ProcessKeyboard(g_pInputKeyboard);

			KapowInput__ProcessGameControllers();

			if (!gameLoop())
				return;

			if (g_pInputMouse)
				KapowInput__ResetMouse(g_pInputMouse);

			if (g_pInputKeyboard)
				KapowInput__ResetKeyboard(g_pInputKeyboard);
		}
		else
			WaitMessage();
	}
}

ATOM Window::RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId)
{
	//_RegisterWindowClass(_this, nMenuResourceId, nIconResourceId);
	WNDCLASSA WndClass;

	debug("Creating menu with resource ID: %d\n", nMenuResourceId);

	WndClass.hCursor = 0;
	WndClass.hIcon = LoadIconA(*g_hInstance, MAKEINTRESOURCE(nIconResourceId));
	WndClass.lpszMenuName = MAKEINTRESOURCE(nMenuResourceId);
	WndClass.lpszClassName = m_sWindowTitle.m_szString;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hInstance = *g_hInstance;
	WndClass.style = 3;
	WndClass.lpfnWndProc = WindowProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;

	debug("Registering window class...\n");

	return RegisterClassA(&WndClass);
}

//	FIXME: this crashes the game now!
void Window::InitEnvironment(const char* wndClassName, int unkParam1, UINT16 nMenuResourceId, int unkParam2, UINT16 nIconResourceId)
{
	HKEY				phkResult;
	BYTE				szDesktopPath;
	MEMORYSTATUSEX		memoryStatus;

	m_unkFlags = unkParam1;
	m_bVisible = true;
	m_pMenuItemClickedCallback = NULL;
	m_bDestroyed = true;
	m_bQuitRequested = false;

	g_kapowWindow = this;

	//	TODO: this bullshit should be handled by string class constructor!
	m_sWindowTitle = String(strlen(wndClassName));
	KapowStringsPool__unkAllocation(&m_sWindowTitle);
	strcpy(m_sWindowTitle.m_szString, wndClassName);

	if (!RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\", 0, KEY_QUERY_VALUE, &phkResult)) {
		DWORD	nMaxValueSize = MAX_PATH;
		RegQueryValueEx(phkResult, "Desktop", NULL, NULL, &szDesktopPath, &nMaxValueSize);
		RegCloseKey(phkResult);
	}

	m_sUserDesktopPath = String(strlen((const char*)&szDesktopPath));
	KapowStringsPool__unkAllocation(&m_sUserDesktopPath);
	strcpy(m_sUserDesktopPath.m_szString, (const char*)&szDesktopPath);

	RegisterWindowClass(nMenuResourceId, nIconResourceId);
	_CreateWindow(nMenuResourceId);

	m_hCursor = LoadCursor(NULL, IDC_ARROW);

	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &sSTICKYKEYS, 0);
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &sTOGGLEKEYS, 0);
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &sFILTERKEYS, 0);

	SetAccessibilityFeatures(false);

	memoryStatus.dwLength = 64;
	GlobalMemoryStatusEx(&memoryStatus);

	if (memoryStatus.ullTotalPhys < 0xFB00000) {
		WCHAR	Caption, Text;

		FindStringResource(*stringResourceBaseAddr + 220, &Caption, 256);
		FindStringResource(*stringResourceBaseAddr + 290, &Text, 512);

		MessageBoxW(m_hWindow, &Text, &Caption, MB_ICONEXCLAMATION);
	}
}

BOOL Window::_SetTitle(LPCSTR lpString)
{
	m_sWindowTitle = String(strlen(lpString));
	KapowStringsPool__AllocateSpace(&m_sWindowTitle);
	strcpy(m_sWindowTitle.m_szString, lpString);

	return SetWindowText(m_hWindow, lpString);
}

void Window::SetDesktopDirectory(const char* pDesktopPath)
{
	m_sUserDesktopPath = String(strlen(pDesktopPath));
	KapowStringsPool__AllocateSpace(&m_sUserDesktopPath);
	strcpy(m_sUserDesktopPath.m_szString, pDesktopPath);
}

void Window::ProcessScreenshotsDirs(String& outString)
{
	char	PathName[MAX_PATH];
	int		screenshotNo = 1;
	char(__cdecl *sub_4182A0)(char* pUnkStr) = (char (__cdecl*)(char*))0x4182A0;

	sprintf(PathName, "%s/Kapow Screenshots", m_sUserDesktopPath.m_szString);
	CreateDirectory(PathName, NULL);

	do {
		sprintf(PathName, "%s/Kapow Screenshots/screenshot %03d.bmp", m_sUserDesktopPath.m_szString, screenshotNo++);
	} while (sub_4182A0(PathName));

	//	Originally - KapowStringBuffer::MakeString. Since I assume that it's actually a strings pool
	//	and there's no reference to this function in exe left, best would be to just pass a KapowStringBuffer
	//	object created on stack and utility class KapowStringsPool will allocate it on stack and return pointer
	//	to it, to be set to actual value later.
	//KapowStringsPool::Insert(outString)->Set(PathName);
}

//	TODO: fix. This crashes the game!
bool Window::IsProcessAGameProcess(DWORD dwProcessId, int unk1, const char* unkProcName, int unk2, char unk3)
{
	HMODULE	hModule;
	DWORD	unk;
	char	baseName[260];
	HANDLE	procHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE, false, dwProcessId);
	if (procHandle && EnumProcessModules(procHandle, &hModule, sizeof(hModule), &unk)) {
		GetModuleBaseName(procHandle, hModule, baseName, 260);
		
		if (!strcmp(baseName, unkProcName))
			return true;
	}
	CloseHandle(procHandle);

	return false;
}

void Window::GetUserDocumentsDir(String& outString)
{
	char	pszPath[MAX_PATH];

	if (SHGetFolderPath(NULL, CSIDL_FLAG_CREATE | CSIDL_PERSONAL, NULL, NULL, pszPath) < 0) {
		outString = String();
	}
	else {
		outString = String();
		outString.m_nLength = strlen(pszPath);
		KapowStringsPool__AllocateSpace(&outString);
		strcpy(outString.m_szString, pszPath);
	}
}

int CALLBACK Window::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HMODULE	moduleHandle;
	char	FileName[1024];
	DWORD	idProcess, cbNeeded;

	//Performance::Init();
	Performance__Initialise();
	Sleep(10);
	//Performance::Calculate();
	Performance__QueryCounter();

	*g_hInstance = hInstance;
	*g_CmdLine = lpCmdLine;

	moduleHandle = GetModuleHandle(NULL);
	GetModuleFileName(moduleHandle, FileName, 1024);

	//unkGetExeName(&FileName, &v32, &szString, &v31);
	//KapowStringsPool__MakeString(a2, &szString);
	//KapowStringsPool__MakeString(baseStr, &v31);

	if (EnumProcesses(&idProcess, 4096, &cbNeeded)) {
		int processesFound = cbNeeded >> 2;
		int gameCopiesRunning = 0;
		int procIndex = 0;

		if (cbNeeded >> 2 > 0) {
			do {
				if (IsProcessAGameProcess(*(&idProcess + procIndex), 0, "TOD.exe", 0, 0))
					++gameCopiesRunning;
				++procIndex;
			} while (procIndex < processesFound);

			if (gameCopiesRunning > 1)
				exit(1);
		}

		FindIdFile();
		KapowEngine__InitialiseEngine(lpCmdLine);
	}else{
		return 0;
	}
	return 0;
}

//	Apply patches specific to this class.
inline void PATCH_WINDOW()
{
	void * dwFunc;

	_asm	mov		eax, offset Window::RegisterWindowClass
	_asm	mov		dwFunc, eax
	//	Override RegisterWindowClass function.
	hook(0x43C767, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::_CreateWindow
	_asm	mov		dwFunc, eax
	//	Override CreateWindow function.
	hook(0x43C76F, dwFunc, PATCH_NOTHING);

	//	Override FindStringResource function.
	hook(0x43C807, &Window::FindStringResource, PATCH_CALL);
	hook(0x43C81A, &Window::FindStringResource, PATCH_CALL);
	hook(0x43C0AC, &Window::FindStringResource, PATCH_CALL);
	hook(0x43C0BF, &Window::FindStringResource, PATCH_CALL);

	_asm	mov		eax, offset Window::Release
	_asm	mov		dwFunc, eax
	//	Override Release function.
	hook(0x93CCBF, dwFunc, PATCH_NOTHING);

	//	Override SetAccessibilityFeatures function.
	hook(0x43C7B3, &Window::SetAccessibilityFeatures, PATCH_CALL);

	_asm	mov		eax, offset Window::IncompatibleMachineParameterError
	_asm	mov		dwFunc, eax
	//	Override IncompatibleMachineParameterError function.
	hook(0x43ADF0, dwFunc, PATCH_NOTHING);
	hook(0x43AE11, dwFunc, PATCH_NOTHING);
	hook(0x43AE31, dwFunc, PATCH_NOTHING);
	hook(0x43AE57, dwFunc, PATCH_NOTHING);
	hook(0x43AECC, dwFunc, PATCH_NOTHING);
	hook(0x43B53B, dwFunc, PATCH_NOTHING);
	hook(0x43B55E, dwFunc, PATCH_NOTHING);
	hook(0x43B57D, dwFunc, PATCH_NOTHING);
	hook(0x43B5A2, dwFunc, PATCH_NOTHING);
	hook(0x43B616, dwFunc, PATCH_NOTHING);
	hook(0x43D39E, dwFunc, PATCH_NOTHING);
	hook(0x43D3E0, dwFunc, PATCH_NOTHING);
	hook(0x43D4E9, dwFunc, PATCH_NOTHING);
	hook(0x43D532, dwFunc, PATCH_NOTHING);
	hook(0x43E309, dwFunc, PATCH_NOTHING);
	hook(0x43E329, dwFunc, PATCH_NOTHING);
	hook(0x45E7A2, dwFunc, PATCH_NOTHING);
	hook(0x45E7FE, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetSystemLanguageCode
	_asm	mov		dwFunc, eax
	//	Override GetSystemLanguageCode function.
	//hook(0x4855D6, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GetMessageBoxResultButton
	_asm	mov		dwFunc, eax
	//	Override GetMessageBoxResultButton function.
	hook(0x470AF5, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::ClipCursorInsideWindow
	_asm	mov		dwFunc, eax
	//	Override ClipCursorInsideWindow function.
	hook(0x93CBE1, dwFunc, PATCH_NOTHING);
	hook(0x93F2F0, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::GlobalPause
	_asm	mov		dwFunc, eax
	//	Override GlobalPause function.
	//hook(0x43C4E8, dwFunc, PATCH_NOTHING);

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

	_asm	mov		eax, offset Window::_GetWindowRect
	_asm	mov		dwFunc, eax
	//	Override _GetWindowRect function.
	hook(0x43C398, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetWindowResolutionDontMove
	_asm	mov		dwFunc, eax
	//	Override SetWindowResolutionDontMove function.
	hook(0x45BF08, dwFunc, PATCH_NOTHING);
	hook(0x45C02C, dwFunc, PATCH_NOTHING);
	hook(0x45C17E, dwFunc, PATCH_NOTHING);

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

	_asm	mov		eax, offset Window::ProcessInputDevices
	_asm	mov		dwFunc, eax
	//	Override ProcessInputDevices function.
	//hook(0x93D0BB, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::InitEnvironment
	_asm	mov		dwFunc, eax
	//	Override InitEnvironment function.
	//hook(0x93E008, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::_SetTitle
	_asm	mov		dwFunc, eax
	//	Override _SetTitle function.
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	_asm	mov		eax, offset Window::SetDesktopDirectory
	_asm	mov		dwFunc, eax
	//	Override SetDesktopDirectory function.
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	//	Override IsProcessAGameProcess function.
	//hook(0x43CCD4, &KapowWindow::IsProcessAGameProcess, PATCH_CALL);

	_asm	mov		eax, offset Window::ProcessScreenshotsDirs
	_asm	mov		dwFunc, eax
	//	Override ProcessScreenshotsDirs function.
	//hook(0x010000, dwFunc, PATCH_NOTHING);

	//	Override GetUserDocumentsDir function.
	hook(0x93E2ED, &Window::GetUserDocumentsDir, PATCH_CALL);

	//	Override WinMain function.
	hook(0x95383F, &Window::WinMain, PATCH_CALL);
}
