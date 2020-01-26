#pragma once
#include "stdafx.h"

#include "KapowStringsPool.h"
#include "Performance.h"

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
 *------- Probably should be called KapowApplication ---------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class KapowWindow
{
	//	Size is 0x44 (68) bytes.
	//	NOTE: some methods ARE static.
	//	NOTE: is it CBaseWindow, from DirectShow?
private:
	int					m_nClassNameLength;
	const char*			m_szClassName;
	int					m_nWindowStyle;
	char				m_unkChar;
	KapowString			m_szUserDesktopPath;
	void				*m_pMenuItemClickedCallback;
	HWND				m_hWindow;
	int					m_unkInt7;						//	Some unknown flags. TODO: Needs to be union {}.
	bool				m_bVisible;
	bool				m_bDestroyed;
	BYTE				m_bQuitRequested;
	HCURSOR				m_hCursor;
	int					m_unkInt9;
	int					m_unkInt10;
	LONG				m_nWindowLeft;
	LONG				m_nWindowTop;

public:
	//	>> 43B920
	KapowWindow*				GetInstance() { return g_kapowWindow; }
	//	>> 43B930
	void						QuitGame() { m_bQuitRequested = true; }
	//	>> 43B950
	bool						ProcessMessages();
	//	>> 43B9C0
	void						SetMenuClickCallback(void* pCallback);
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
	//	>> 43BC30. TODO: give better name.
	void						GlobalPause();
	//	>> 43BCA0
	void						ClipCursorInsideWindow(bool bDontClip);
	//	>> 43BD50
	int							GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type);
	//	>> 43BDC0
	static signed int			GetSystemLanguageCode();
	//	>> 43BF70
	int							GetCoverdemoPlayMode() { return 0; };
	//	>> 43BF80
	int							GetCoverdemoInactiveTimeoutSec() { return MAXINT; }
	//	>> 43BF90
	long						GetCoverdemoGameplayTimeoutSec() { return MAXLONG; }
	//	>> 43BFB0
	static void					FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize);
	//	>> 43C040
	void						IncompatibleMachineParameterError(int messageID, char bWarningIcon);
	//	>> 43C140
	static void					SetAccessibilityFeatures(bool bCollect);
	//	>> 43C230
	void						_DestroyWindow();
	//	>> 43C2C0
	void						_CreateWindow(UINT16 nIconResourceId);
	//	>> 43C320
	static LRESULT CALLBACK		WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	//	>> 43C4A0
	void						ProcessInputDevices(int (*unkGameLoopProc)(void));
	//	>> 43C570
	ATOM						RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId);
	//	>> 43C630
	void						InitEnvironment(const char* wndClassName, int unkParam1, UINT16 nMenuResourceId, int unkParam2, UINT16 nIconResourceId);
	//	>> 43C850
	BOOL						_SetTitle(LPCSTR lpString);
	//	>> 43C8B0
	void						SetDesktopDirectory(const char* pDesktopPath);
	//	>> 43C900
	void						ProcessScreenshotsDirs(KapowString& outString);
	//	>> 43C990
	static	bool				IsProcessAGameProcess(DWORD dwProcessId, int unk1, const char* unkProcName, int unk2, char unk3);
	//	>> 43CAE0.
	static	void				GetUserDocumentsDir(DWORD& outString);
	//	>> 43CB40
	static	int	CALLBACK		WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
};

