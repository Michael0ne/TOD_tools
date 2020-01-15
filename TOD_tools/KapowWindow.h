#pragma once
#include "stdafx.h"

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

//	TODO: move to something like KapowBuiltinTypes header.
template <typename T>
struct Vector2 {
	T	firstVal;
	T	secondVal;
};

/*------------------------------------------------------------
 *------------------------------------------------------------
 *------------------ Main Window Object ----------------------
 *------------------------------------------------------------
 *------- Probably should be called KapowApplication ---------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class KapowWindow
{
	//	Possible size - 0x44 (68) bytes.
	//	Another possible size - 0x58 (0x3A) bytes.
	//	NOTE: some methods ARE static.
private:
	int			m_nClassNameLength;
	const char*	m_szClassName;
	int			m_nWindowStyle;
	char		m_unkChar;
	int			m_unkInt;
	int			m_unkInt2;
	int			m_unkInt3;
	int			m_unkInt4;
	void		*m_pMenuItemClickedCallback;
	HWND		m_hWindow;
	int			m_unkInt7;
	BYTE		m_bVisible;
	BYTE		m_bDestroyed;
	BYTE		m_unkByte2;
	HCURSOR		m_hCursor;
	int			m_unkInt9;
	int			m_unkInt10;
	LONG		m_nWindowLeft;
	LONG		m_nWindowTop;
	BYTE		m_pad[13];
	int			m_unkInt11_proc;

public:
	void	unkSetByteTrue() { m_unkByte2 = true; }		//	>> 43B930
	bool	ShouldProcessMessages();	//	>> 43B950
	BOOL	SetWindowPos(D3DDISPLAYMODE* pos);	//	>> 43B9D0
	BOOL	SetWindowRes(D3DDISPLAYMODE* pos);	//	>> 43B9F0
	void	GetWindowRect(Vector2<LONG>& outRect);	//	>> 43BA70
	void	GlobalPause();		//	>> 43BC30. TODO: give better name.
	void	unkClipCursor(bool bUnk);		//	>> 43BCA0
	int		GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type);	//	>> 43BD50	//	NOTE: this could be inlined, also return type should be enume'd.
	signed int GetSystemLanguageCode();	//	>> 43BDC0	//	TODO: inline it!
	int		sub_43BF70() { return 0; }	//	>> 43BF70	//	NOTE: this should be inlined.
	long	maximum_signed_value2() { return 0x7FFFFFFF; }	//	>> 43BF80	//	NOTE: this should be inlined.
	long	maximum_signed_value() { return 0x7FFFFFFF;	}	//	>> 43BF90	//	NOTE: this should be inlined.
	void*	FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize);	//	>> 43BFB0
	void	IncompatibleMachineParameterError(int messageID, char bWarningIcon);	//	>> 43C040
	void	SetAccessibilityFeatures(bool bCollect);	//	>> 43C140
	void	_DestroyWindow();	//	>> 43C230
	void	_CreateWindow(UINT16 nIconResourceId);	//	>> 43C2C0
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	//	>> 43C320
	void	ProcessInputDevices(int (*unkProcPtr)(void));	//	>> 43C4A0
	ATOM	RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId);	//	>> 43C570
	void	InitEnvironment(const char* wndClassName, int unkParam1, UINT16 nMenuResourceId, int unkParam2, UINT16 nIconResourceId);	//	>> 43C630
	BOOL	SetTitle(LPCSTR lpString);		//	>> 43C850
	bool	IsProcessAGameProcess(DWORD dwProcessId, int unk1, const char* unkProcName, int unk2, char unk3);	//	>> 43C990
	void	GetUserDocumentsDir(DWORD unk1);	//	>> 43CAE0. unk1 is probably a reference to buffer.
	int		CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);	//	>> 43CB40. Not sure if this should be here, since method's calling conv is __stdcall.
	void	unkSetter4(int unk);	//	>> 43CDA0
	void	unkSetter3(int unk);	//	>> 43CDB0
	void	unkSetter2(int unk);	//	>> 43CDC0
	void	unkSetter1(int unk);	//	>> 43CDD0
};

