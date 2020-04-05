#pragma once
#include "stdafx.h"
#include "Types.h"
#include "Globals.h"

#include "StringsPool.h"

#define WINDOW_CLASS_SIZE 68

enum eWindowStyles {
	Overlapped = WS_OVERLAPPED,
	Popup = WS_POPUP,
	Child = WS_CHILD,
	Minimize = WS_MINIMIZE,
	Visible = WS_VISIBLE,
	Disabled = WS_DISABLED,
	ClipSiblings = WS_CLIPSIBLINGS,
	ClipChildren = WS_CLIPCHILDREN,
	Maximize = WS_MAXIMIZE,
	Caption = WS_CAPTION,
	Border = WS_BORDER,
	DlgFrame = WS_DLGFRAME,
	VScroll = WS_VSCROLL,
	HScroll = WS_HSCROLL,
	Sysmenu = WS_SYSMENU,
	ThickFrame = WS_THICKFRAME,
	Group = WS_GROUP,
	TabStop = WS_TABSTOP,
	MinimizeBox = WS_MINIMIZEBOX,
	MaximizeBox = WS_MAXIMIZEBOX,
	Tiled = WS_TILED,
	Iconic = WS_ICONIC,
	SizeBox = WS_SIZEBOX,
	TiledWindow = WS_TILEDWINDOW
};

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *------------------ Main Window Object ----------------------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class Window
{
public:
	String				m_sWindowTitle;
	String				m_sUserDesktopPath;
	void				(__cdecl *m_pMenuItemClickedCallback)(WPARAM);
	HWND				m_hWindow;
	int					m_unkFlags;						//	Some unknown flags. TODO: Needs to be union {}.
	bool				m_bVisible;
	bool				m_bCursorReleased;
	bool				m_bQuitRequested;
	HCURSOR				m_hCursor;
	int					m_unkInt9;
	int					m_unkInt10;
	LONG				m_nWindowLeft;
	LONG				m_nWindowTop;

public:
	//	>> 43B930
	void						QuitGame() { m_bQuitRequested = true; }
	//	>> 43B950
	bool						ProcessMessages();
	//	>> 43B9C0
	void						SetMenuClickCallback(void (__cdecl* pCallback)(WPARAM)) {m_pMenuItemClickedCallback = pCallback;};
	//	>> 43B9D0
	void						SetWindowResolutionRaw(const D3DDISPLAYMODE& resolution);
	//	>> 43B9F0
	void						SetWindowResolutionDontMove(const D3DDISPLAYMODE& resolution);
	//	>> 43BA70
	void						_GetWindowRect(Vector2<LONG>& outRect);
	//	>> 43BAD0
	void						GetTopCorner(Vector2<LONG>& outRect);
	//	>> 43BB00
	void						GetWindowCenterRelative(Vector2<LONG>& outRect);
	//	>> 43BB40
	void						GetClientCenterRelative(Vector2<LONG>& outRect);
	//	>> 43BB80
	void						_SetWindowPos(Vector2<int>& pos);
	//	>> 43BBA0
	void						SetWindowPosNoCopyBits(tagPOINT *newPos);
	//	>> 43BC30
	void						UpdateVisibility();
	//	>> 43BCA0
	void						SetCursorReleased(bool bReleased);
	//	>> 43BD50
	int							GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type);
	//	>> 43BF70
	int							GetCoverdemoPlayMode() { return 0; };
	//	>> 43BF80
	int							GetCoverdemoInactiveTimeoutSec() { return MAXINT; }
	//	>> 43BF90
	long						GetCoverdemoGameplayTimeoutSec() { return MAXLONG; }
	//	>> 43C230
	void						Release();
	//	>> 43C2C0
	void						_CreateWindow(UINT16 nIconResourceId);
	//	>> 43C4A0
	void						Process(bool (*GameLoop)(void));
	//	>> 43C570
	ATOM						RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId);
	//	>> 43C630
	void						Init(const char* wndClassName, int unkFlags, UINT16 nMenuResourceId, char* szFileSystem, UINT16 nIconResourceId);
	//	>> 43C850
	BOOL						_SetTitle(LPCSTR lpString);
	//	>> 43C8B0
	void						SetDesktopDirectory(const char* szDesktopPath);

public:
	Window() {
		m_sWindowTitle = String();
		m_sUserDesktopPath = String();
		m_pMenuItemClickedCallback = nullptr;
		m_hWindow = NULL;
		m_unkFlags = 0;						//	Some unknown flags. TODO: Needs to be union {}.
		m_bVisible = false;
		m_bCursorReleased = false;
		m_bQuitRequested = false;
		m_hCursor = NULL;
		m_unkInt9 = 0;
		m_unkInt10 = 0;
		m_nWindowLeft = 0;
		m_nWindowTop = 0;

		debug("Window created at %X\n", this);
	}

	~Window() {
		debug("Window destroyed!\n");
	}
};

extern Window* g_Window;

//	>> 43BDC0 - ?
int					GetSystemLanguageCode();
//	>> 43BFB0
void				FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize);
//	>> 43C040
void				IncompatibleMachineParameterError(int messageID, char bWarningIcon);
//	>> 43C140
void				SetAccessibilityFeatures(bool bCollect);
//	>> 43C320
LRESULT CALLBACK	WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//	>> 43CB40
int	CALLBACK		WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
//	>> 43CAE0
void				GetUserDocumentsDir(String& outString);
//	>> 439230
void				FindIdFile();

static_assert(sizeof(Window) == WINDOW_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Window"));