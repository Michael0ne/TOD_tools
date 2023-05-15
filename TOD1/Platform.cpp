#include "Platform.h"
#include "KapowEngineClass.h"
#include "StreamedSoundBuffers.h"
#include "ScriptDatabase.h"
#include "GfxInternal_Dx9.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "InputGameController.h"
#include "Timer.h"
#include "LogDump.h"

Platform* g_Platform = nullptr;

unsigned int Platform::MessageBoxType[] = {
 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0
};
const char Platform::RegistryKey[] = "Software\\Eidos\\Total Overdose";
HINSTANCE Platform::WindowInstanceHandle = nullptr;
LPSTR Platform::CmdLine = nullptr;
STICKYKEYS Platform::StickyKeysFeature = { 8, 0 };
TOGGLEKEYS Platform::ToggleKeysFeature = { 8, 0 };
FILTERKEYS Platform::FilterKeysFeature = { 24, 0, 0, 0, 0, 0 };

void Platform::QuitGame()
{
    m_QuitRequested = true;
}

bool Platform::ProcessMessages()
{
    tagMSG Msg;

    if (!PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
        return true;

    while (Msg.message != WM_QUIT)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);

        if (!PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
            return true;
    }

    return false;
}

void Platform::SetMenuClickCallback(MenuItemClickCallback callback)
{
    m_MenuItemClickCallback = callback;
}

void Platform::SetWindowResolutionRaw(const Vector2<unsigned int>& resolution)
{
    SetWindowPos(m_WindowHandle, 0, 0, 0, resolution.x, resolution.y, SWP_NOMOVE);
}

void Platform::SetWindowResolutionDontMove(const Vector2<unsigned int>& resolution)
{
    tagRECT  Rect = { 0, 0, (LONG)resolution.x, (LONG)resolution.y };
    DWORD  dwMenuName = GetClassLongA(m_WindowHandle, GCL_MENUNAME);
    DWORD  dwStyle = GetWindowLongA(m_WindowHandle, GWL_STYLE);

    AdjustWindowRect(&Rect, dwStyle, dwMenuName);
    SetWindowPos(m_WindowHandle, 0, 0, 0, Rect.right - Rect.left, Rect.bottom - Rect.top, SWP_NOMOVE);
}

void Platform::GetWindowTopPosition(Vector2<LONG>& outRect)
{
    tagRECT  WindowRect;

    if (m_WindowHandle)
    {
        GetWindowRect(m_WindowHandle, &WindowRect);

        m_WindowLeft = WindowRect.left;
        m_WindowTop = WindowRect.top;
    }

    if (m_WindowLeft < 0 || m_WindowTop < 0)
    {
        m_WindowLeft = 0;
        m_WindowTop = 0;
    }

    outRect.x = m_WindowLeft;
    outRect.y = m_WindowTop;
}

void Platform::GetTopCorner(Vector2<LONG>& outRect) const
{
    tagRECT  Rect;

    GetClientRect(m_WindowHandle, &Rect);

    outRect.x = Rect.left;
    outRect.y = Rect.top;
}

void Platform::GetWindowCenterRelative(Vector2<LONG>& outRect) const
{
    tagRECT  Rect;

    GetWindowRect(m_WindowHandle, &Rect);

    outRect.x = Rect.right - Rect.left;
    outRect.y = Rect.bottom - Rect.top;
}

void Platform::GetClientCenterRelative(Vector2<LONG>& outRect) const
{
    tagRECT  Rect;

    GetClientRect(m_WindowHandle, &Rect);

    outRect.x = Rect.right - Rect.left;
    outRect.y = Rect.bottom - Rect.top;
}

void Platform::SetWindowPositionNative(const Vector2<int>& pos)
{
    SetWindowPos(m_WindowHandle, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

void Platform::SetWindowPosNoCopyBits(const tagPOINT& newPos)
{
    tagRECT  WindowRect;
    tagRECT  NewRect;

    DWORD dwMenuName = GetClassLongA(m_WindowHandle, GCL_MENUNAME);
    DWORD dwStyle = GetWindowLongA(m_WindowHandle, GWL_STYLE);

    GetWindowRect(m_WindowHandle, &WindowRect);

    NewRect.left = newPos.x;
    NewRect.top = newPos.y;
    NewRect.right = WindowRect.right - WindowRect.left + newPos.x;
    NewRect.bottom = WindowRect.bottom - WindowRect.top + newPos.y;

    AdjustWindowRect(&NewRect, dwStyle, dwMenuName);
    SetWindowPos(m_WindowHandle, 0, NewRect.left, NewRect.top, 0, 0, SWP_NOCOPYBITS);
}

void Platform::UpdateVisibility()
{
    bool WindowVisible = true;

    WINDOWPLACEMENT windowPlacement;
    windowPlacement.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(m_WindowHandle, &windowPlacement);

    if ((windowPlacement.showCmd == SW_MINIMIZE || windowPlacement.showCmd == SW_SHOWMINIMIZED) && !(m_Flags & 4))
        WindowVisible = false;

    if (m_Visible != WindowVisible) {
        m_Visible = WindowVisible;

        if (WindowVisible) {
            g_StreamedSoundBuffers->SetGlobalPause(false);
        }
        else
        {
            g_StreamedSoundBuffers->SetGlobalPause(true);
            g_StreamedSoundBuffers->WaitForSoftPause();
        }
    }
}

void Platform::SetCursorReleased(bool released)
{
    m_CursorReleased = released;

    ShowCursor(released);

    if (released)
    {
        ClipCursor(0);
        return;
    }

    tagRECT  WindowRect;
    tagPOINT Point = { 0, 0 };
    RECT  ClipRect;

    SetActiveWindow(m_WindowHandle);
    GetClientRect(m_WindowHandle, &WindowRect);
    ClientToScreen(m_WindowHandle, &Point);

    ClipRect.left = Point.x;
    ClipRect.top = Point.y;
    ClipRect.bottom = WindowRect.bottom;
    ClipRect.right = WindowRect.right;

    ClipCursor(&ClipRect);
}

int Platform::GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type)
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

int Platform::GetCoverdemoPlayMode()
{
    return 0;
}

int Platform::GetCoverdemoInactiveTimeoutSec() const
{
    return -1;
}

int Platform::GetCoverdemoGameplayTimeoutSec() const
{
    return -1;
}

LanguageCode CALLBACK GetSystemLanguageCode()
{
    CHAR LocaleData[8];
    int  langcode = 0;
    bool bUnk;

    GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, LocaleData, 5);
    sscanf_s(LocaleData, "%x", &langcode);

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

    char caption[16] = {};
    char text[512] = {};
    LoadString(Platform::WindowInstanceHandle, Script::LanguageStringsOffset + 220, caption, sizeof(caption) / sizeof(char));
    LoadString(Platform::WindowInstanceHandle, Script::LanguageStringsOffset + nMessageId, text, sizeof(text) / sizeof(char));

    if (!bWarningIcon)
    {
        MessageBox(g_Platform->m_WindowHandle, text, caption, MB_ICONERROR);
        exit(1);
    }
    else
        MessageBox(g_Platform->m_WindowHandle, text, caption, MB_ICONWARNING);
}

void SetAccessibilityFeatures(bool bCollect)
{
    if (bCollect)
    {
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Platform::StickyKeysFeature, 0);
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Platform::ToggleKeysFeature, 0);
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Platform::FilterKeysFeature, 0);

        return;
    }

    if (!(Platform::StickyKeysFeature.dwFlags & SKF_STICKYKEYSON))
    {
        Platform::StickyKeysFeature.dwFlags &= 0xFFFFFFF3; // SKF_STICKYKEYSON | SKF_AVAILABLE
        SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Platform::StickyKeysFeature, 0);
    }

    if (!(Platform::ToggleKeysFeature.dwFlags & TKF_TOGGLEKEYSON))
    {
        Platform::ToggleKeysFeature.dwFlags &= 0xFFFFFFF3; // TKF_TOGGLEKEYSON | TKF_AVAILABLE
        SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Platform::ToggleKeysFeature, 0);
    }

    if (!(Platform::FilterKeysFeature.dwFlags & FKF_FILTERKEYSON))
    {
        Platform::FilterKeysFeature.dwFlags &= 0xFFFFFFF3; // FKF_FILTERYSON | FKF_AVAILABLE
        SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Platform::FilterKeysFeature, 0);
    }
}

void Platform::CreateWindowInstance(UINT16 nIconResourceId)
{
    DWORD windowStyle = (m_Flags & 1) != 0 ? WS_CAPTION | WS_MAXIMIZE : WS_CAPTION | WS_MINIMIZE;
    if (nIconResourceId)
        windowStyle |= WS_SYSMENU;

    m_WindowHandle = CreateWindowExA(WS_EX_APPWINDOW, m_WindowTitle.m_Str, m_WindowTitle.m_Str, windowStyle, NULL, NULL, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, WindowInstanceHandle, nullptr);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    Vector2<LONG> wndRect;

    if (Msg <= WM_SETCURSOR)
    {
        if (Msg != WM_SETCURSOR)
        {
            if (Msg == WM_DESTROY
#ifdef INCLUDE_FIXES
                || Msg == WM_CLOSE
#endif
                )
            {
                g_Platform->GetWindowTopPosition(wndRect);
                g_Platform->m_CursorReleased = true;
                ShowCursor(1);
                ClipCursor(0);
                PostQuitMessage(0);
                g_Platform->m_WindowHandle = NULL;

                return 0;
            }

            if (Msg != WM_ERASEBKGND)
            {
                if (Msg == WM_ACTIVATEAPP && !wParam && g_GfxInternal_Dx9)
                    if (g_GfxInternal_Dx9->m_Windowed)
                        ShowWindow(hWnd, SW_MINIMIZE);

                return DefWindowProc(hWnd, Msg, wParam, lParam);
            }
            return 1;
        }
        if (!g_Platform->m_CursorReleased)
        {
            SetCursor(0);
            return 0;
        }
        SetCursor(g_Platform->m_Cursor);

        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    if (Msg != WM_NCHITTEST)
    {
        if (Msg == WM_COMMAND)
        {
            if (g_Platform->m_MenuItemClickCallback)
                g_Platform->m_MenuItemClickCallback(wParam);

            return DefWindowProc(hWnd, Msg, wParam, lParam);
        }

        if (Msg == WM_SYSCOMMAND &&
            (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) &&
            g_GfxInternal_Dx9->m_Windowed)
            return 1;

        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    if (!g_GfxInternal_Dx9->m_Windowed)
        return DefWindowProc(hWnd, Msg, wParam, lParam);

    return 1;
}

void Platform::Process(GameLoopCallback callback)
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

            if (!callback())
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

ATOM Platform::RegisterWindowClass(UINT16 menuResId, UINT16 iconResId)
{
    WNDCLASSA WndClass;

    LogDump::LogA("Creating menu with resource ID: %d\n", menuResId);

    WndClass.hCursor = 0;
    WndClass.hIcon = LoadIcon(WindowInstanceHandle, MAKEINTRESOURCE(iconResId));
    WndClass.lpszMenuName = MAKEINTRESOURCE(menuResId);
    WndClass.lpszClassName = m_WindowTitle.m_Str;
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
        // ID_EXIT
        g_Platform->m_QuitRequested = true;

        return NULL;
    }

    ProcessDebugMenuOption(g_Platform->m_WindowHandle, Platform::WindowInstanceHandle, wParam);

    return 1;
}
#endif

Platform::Platform(const char* wndClassName, int flags, UINT16 menuResId, char* fileSystem, UINT16 iconResId)
{
    MESSAGE_CLASS_CREATED(Platform);

    HKEY    phkResult;
    char    szDesktopPath[MAX_PATH];
    MEMORYSTATUSEX  memoryStatus;

    m_Flags = flags;
    m_Visible = true;
#ifdef INCLUDE_FIXES
    m_MenuItemClickCallback = MenuClickCallback;
#else
    m_MenuItemClickedCallback = nullptr;
#endif
    m_CursorReleased = true;
    m_QuitRequested = false;

    m_WindowTitle = wndClassName;

    if (!RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders\\", 0, KEY_QUERY_VALUE, &phkResult)) {
        DWORD nMaxValueSize = MAX_PATH;
        RegQueryValueEx(phkResult, "Desktop", NULL, NULL, (byte*)szDesktopPath, &nMaxValueSize);
        RegCloseKey(phkResult);
    }

    m_UserDesktopPath = szDesktopPath;

    RegisterWindowClass(menuResId, iconResId);
    CreateWindowInstance(menuResId);

    m_Cursor = LoadCursor(NULL, IDC_ARROW);

    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &Platform::StickyKeysFeature, 0);
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Platform::ToggleKeysFeature, 0);
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &Platform::FilterKeysFeature, 0);

    SetAccessibilityFeatures(false);

    memoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memoryStatus);

    if (memoryStatus.ullTotalPhys < (251 * 1024 * 1024))
        IncompatibleMachineParameterError(ERRMSG_INSUFFICIENT_RAM, true);
}

Platform::~Platform()
{
    MESSAGE_CLASS_DESTROYED(Platform);

    if (m_WindowHandle)
        DestroyWindow(m_WindowHandle);

    SystemParametersInfoA(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &Platform::StickyKeysFeature, 0);
    SystemParametersInfoA(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &Platform::ToggleKeysFeature, 0);
    SystemParametersInfoA(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &Platform::FilterKeysFeature, 0);
}

BOOL Platform::SetWindowTitle(LPCSTR lpString)
{
    m_WindowTitle = lpString;

    return SetWindowText(m_WindowHandle, lpString);
}

void Platform::SetDesktopDirectory(LPCSTR pDesktopPath)
{
    m_UserDesktopPath = pDesktopPath;
}

void SetGlobalInstanceHandle(HINSTANCE inst)
{
    Platform::WindowInstanceHandle = inst;
}

void SetGlobalCmdLinePtr(LPSTR cmdl)
{
    Platform::CmdLine = cmdl;
}

#ifdef _EXE
static FILE* logfile;
void debug(char* message, ...)
{
    if (!logfile)
        return;
    SYSTEMTIME t;
    GetLocalTime(&t);
    fprintf(logfile, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    va_list arg;
    va_start(arg, message);
    vfprintf(logfile, message, arg);
    fflush(logfile);
    va_end(arg);
}
#endif

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
#ifdef _EXE
    // Begin tracing.
    fopen_s(&logfile, "tod_trace.log", "ab");
    if (!logfile)
        return false;
    debug("Log Started!\n");
#endif

    Timer::Init();
    Sleep(10);
    Timer::Calculate();

    Platform::WindowInstanceHandle = hInstance;
    Platform::CmdLine = lpCmdLine;

    char exefname[1024] = {};
    char exedir[1024] = {};
    char exeext[8] = {};
    char fname[16] = {};
    DWORD procIdArr[1024] = {};
    DWORD procIdRetnSize = NULL;

    GetModuleFileName(GetModuleHandle(nullptr), exefname, sizeof(exefname));
    String::ConvertBackslashes(exefname);
    FileBuffer::ExtractFilePath(exefname, exedir, fname, exeext);

    char exefullname[32] = {};
    strcpy_s(exefullname, fname);
    strcat_s(exefullname, ".");
    strcat_s(exefullname, exeext);

    if (EnumProcesses(procIdArr, sizeof(procIdArr), &procIdRetnSize))
    {
        unsigned int copiesRunning = 0;

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

        FileBufferImpl::FindGameDir();
        InitialiseGame(lpCmdLine);
    }

#ifdef _EXE
    // Stop tracing.
    if (logfile)
    {
        debug("Log Closed!\n\n");
        fclose(logfile);
        logfile = NULL;
    }
#endif

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

#ifndef _EXE
inline void PATCH_WINDOW()
{
    void* dwFunc;

    _asm mov  eax, offset Platform::ProcessMessages
    _asm mov  dwFunc, eax
    // Override ShouldProcessMessages function.
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

    _asm mov  eax, offset Platform::QuitGame
    _asm mov  dwFunc, eax
    // Override QuitGame function.
    hook(0x485666, dwFunc, PATCH_NOTHING);

    // Override WinMain function.
    hook(0x95383F, &WinMain, PATCH_CALL);
}
#endif