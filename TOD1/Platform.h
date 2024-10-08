#pragma once
#include "Types.h"
#include "Globals.h"
#include "StringsPool.h"
#include <windef.h>

enum LanguageCode
{
    LANGCODE_ENGLISH = 0,
    LANGCODE_FRENCH = 1,
    LANGCODE_ITALIAN = 2,
    LANGCODE_GERMAN = 3,
    LANGCODE_SPANISH = 4
};

enum ErrorMessageId
{
    ERRMSG_INCOMPATIBLE_MOUSE = 0,
    ERRMSG_INCOMPATIBLE_KEYBOARD = 1,
    ERRMSG_INCOMPATIBLE_SOUNDCARD = 2,
    ERRMSG_INCOMPATIBLE_VIDEOCARD = 3,
    ERRMSG_DIRECTX9_NOT_FOUND = 4,
    ERRMSG_DISC_NOT_FOUND = 5,
    ERRMSG_INSUFFICIENT_RAM = 6
};

class Platform
{
    typedef int (CALLBACK *MenuItemClickCallback)(WPARAM);
    typedef bool (*GameLoopCallback)(void);
public:
    String      m_WindowTitle;
    String      m_UserDesktopPath;
    MenuItemClickCallback m_MenuItemClickCallback;
    HWND        m_WindowHandle;
    int         m_Flags;    //  NOTE: that's probably a 'showCmd' value reference, see 'WinUser.h' 'SW_' defines.
    bool        m_Visible;
    bool        m_CursorReleased;
    bool        m_QuitRequested;
    HCURSOR     m_Cursor;
    int         field_34;
    int         field_38;
    LONG        m_WindowLeft;
    LONG        m_WindowTop;

private:
    static unsigned int MessageBoxType[]; // @A091A8

public:
    void        QuitGame(); // @43B930
    bool        ProcessMessages(); // @43B950
    void        SetMenuClickCallback(MenuItemClickCallback callback);    //  @43B9C0
    void        SetWindowResolutionRaw(const Vector2<unsigned int>& resolution); // @43B9D0
    void        SetWindowResolutionDontMove(const Vector2<unsigned int>& resolution); // @43B9F0
    void        GetWindowTopPosition(Vector2<LONG>& outRect); // @43BA70
    void        GetTopCorner(Vector2<LONG>& outRect) const; // @43BAD0
    void        GetWindowCenterRelative(Vector2<LONG>& outRect) const; // @43BB00
    void        GetClientCenterRelative(Vector2<LONG>& outRect) const; // @43BB40
    void        SetWindowPositionNative(const Vector2<int>& pos); // @43BB80
    void        SetWindowPosNoCopyBits(const tagPOINT& newPos); // @43BBA0
    void        UpdateVisibility(); // @43BC30
    void        SetCursorReleased(bool bReleased); // @43BCA0
    int         GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type); // @43BD50
    int         GetCoverdemoPlayMode(); // @43BF70
    int         GetCoverdemoInactiveTimeoutSec() const; // @43BF80
    int         GetCoverdemoGameplayTimeoutSec() const; // @43BF90
    void        CreateWindowInstance(UINT16 nIconResourceId); // @43C2C0
    void        Process(GameLoopCallback callback); // @43C4A0
    ATOM        RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId); // @43C570
    BOOL        SetWindowTitle(LPCSTR lpString); // @43C850
    void        SetDesktopDirectory(LPCSTR pDesktopPath); // @43C8B0

public:
    Platform(const char* wndClassName, int flags, UINT16 nMenuResourceId, char* szFileSystem, UINT16 nIconResourceId); // @43C630
    ~Platform(); // @43C230

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
    }

    static const char   RegistryKey[]; // @A09178
    static HINSTANCE    WindowInstanceHandle; // @A35EB0
    static LPSTR        CmdLine; // @A35EB4
    static STICKYKEYS   StickyKeysFeature; // @A0917C
    static TOGGLEKEYS   ToggleKeysFeature; // @A09184
    static FILTERKEYS   FilterKeysFeature; // @A0918C
};

extern Platform* g_Platform;

void            SetGlobalInstanceHandle(HINSTANCE); // @A35EB0
void            SetGlobalCmdLinePtr(LPSTR); // @43BF60
LanguageCode CALLBACK   GetSystemLanguageCode(); // @43BDC0
void            IncompatibleMachineParameterError(ErrorMessageId messageID, char bWarningIcon); // @43C040
void            SetAccessibilityFeatures(bool bCollect); // @43C140
LRESULT CALLBACK    WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); // @43C320
int CALLBACK    WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd); // @43CB40
bool            IsProcessAGameProcess(DWORD, LPCSTR); // @43C990
void            GetUserDocumentsDir(String& outString); // @43CAE0

// TODO: this stuff is not supposed to be here, but no appropriate namespace exists right now.
void            SetWarningString(const char* (*ptr)()); // @406E00
const char*     GetWarningString(); // @406E10

static const char* (*WarningStringProc)() = nullptr; // @A35B80

#ifdef INCLUDE_FIXES
int CALLBACK  MenuClickCallback(WPARAM wParam);
#endif

ASSERT_CLASS_SIZE(Platform, 68);