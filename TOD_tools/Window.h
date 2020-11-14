#pragma once
#include "stdafx.h"
#include "Types.h"
#include "Globals.h"

#include "StringsPool.h"
#include "MemoryAllocators.h"

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
	int					(__stdcall *m_pMenuItemClickedCallback)(WPARAM);
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
	void						QuitGame() { m_bQuitRequested = true; }	//	@43B930
	bool						ProcessMessages();	//	@43B950
	void						SetMenuClickCallback(int (__stdcall* pCallback)(WPARAM)) {m_pMenuItemClickedCallback = pCallback;};	//	@43B9C0
	void						SetWindowResolutionRaw(const D3DDISPLAYMODE& resolution);	//	@43B9D0
	void						SetWindowResolutionDontMove(const D3DDISPLAYMODE& resolution);	//	@43B9F0
	void						_GetWindowRect(Vector2<LONG>& outRect);	//	@43BA70
	void						GetTopCorner(Vector2<LONG>& outRect);	//	@43BAD0
	void						GetWindowCenterRelative(Vector2<LONG>& outRect);	//	@43BB00
	void						GetClientCenterRelative(Vector2<LONG>& outRect);	//	@43BB40
	void						_SetWindowPos(Vector2<int>& pos);	//	@43BB80
	void						SetWindowPosNoCopyBits(tagPOINT *newPos);	//	@43BBA0
	void						UpdateVisibility();	//	@43BC30
	void						SetCursorReleased(bool bReleased);	//	@43BCA0
	int							GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type);	//	@43BD50
	int							GetCoverdemoPlayMode() { return 0; };	//	@43BF70
	int							GetCoverdemoInactiveTimeoutSec() { return MAXINT; }	//	@43BF80
	long						GetCoverdemoGameplayTimeoutSec() { return MAXLONG; }	//	@43BF90
	void						Release();	//	@43C230
	void						_CreateWindow(UINT16 nIconResourceId);	//	@43C2C0
	void						Process(bool (*GameLoop)(void));	//	@43C4A0
	ATOM						RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId);	//	@43C570
	void						Init(const char* wndClassName, int unkFlags, UINT16 nMenuResourceId, char* szFileSystem, UINT16 nIconResourceId);	//	@43C630
	BOOL						_SetTitle(LPCSTR lpString);	//	@43C850
	void						SetDesktopDirectory(const char* szDesktopPath);	//	@43C8B0

public:
	Window() {
		MESSAGE_CLASS_CREATED(Window);

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
	}
	~Window()
	{
		MESSAGE_CLASS_DESTROYED(Window);
	}

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	static Window*	ms_Instance;
};

extern Window* g_Window;

int	CALLBACK		GetSystemLanguageCode();	//	@43BDC0
void				FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize);	//	@43BFB0
void				IncompatibleMachineParameterError(int messageID, char bWarningIcon);	//	@43C040
void				SetAccessibilityFeatures(bool bCollect);	//	@43C140
LRESULT CALLBACK	WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	//	@43C320
int	CALLBACK		WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);	//	@43CB40
void				GetUserDocumentsDir(String& outString);	//	@43CAE0
void				FindIdFile();	//	@439230

#ifdef INCLUDE_FIXES
int CALLBACK		MenuClickCallback(WPARAM wParam);
#endif

static_assert(sizeof(Window) == WINDOW_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Window));