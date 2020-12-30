#pragma once
#include "Types.h"
#include "Globals.h"

#define WINDOW_CLASS_SIZE 68

//	Game window wrapper.
class Window
{
public:
	String			m_sWindowTitle;
	String			m_sUserDesktopPath;
	int				(__stdcall *m_pMenuItemClickedCallback)(WPARAM);
	HWND			m_hWindow;
	int				m_unkFlags;						//	Some unknown flags. TODO: Needs to be union {}.
	bool			m_bVisible;
	bool			m_bCursorReleased;
	bool			m_bQuitRequested;
	HCURSOR			m_hCursor;
	int				m_unkInt9;
	int				m_unkInt10;
	LONG			m_nWindowLeft;
	LONG			m_nWindowTop;

private:
	static unsigned int		MessageBoxType[];	//	@A091A8

public:
	void			QuitGame() { m_bQuitRequested = true; }	//	@43B930
	bool			ProcessMessages();	//	@43B950
	void			SetMenuClickCallback(int (__stdcall* pCallback)(WPARAM)) {m_pMenuItemClickedCallback = pCallback;};	//	@43B9C0
	void			SetWindowResolutionRaw(const D3DDISPLAYMODE& resolution);	//	@43B9D0
	void			SetWindowResolutionDontMove(const D3DDISPLAYMODE& resolution);	//	@43B9F0
	void			_GetWindowRect(Vector2<LONG>& outRect);	//	@43BA70
	void			GetTopCorner(Vector2<LONG>& outRect);	//	@43BAD0
	void			GetWindowCenterRelative(Vector2<LONG>& outRect);	//	@43BB00
	void			GetClientCenterRelative(Vector2<LONG>& outRect);	//	@43BB40
	void			_SetWindowPos(Vector2<int>& pos);	//	@43BB80
	void			SetWindowPosNoCopyBits(tagPOINT *newPos);	//	@43BBA0
	void			UpdateVisibility();	//	@43BC30
	void			SetCursorReleased(bool bReleased);	//	@43BCA0
	int				GetMessageBoxResultButton(LPCSTR lpText, LPCSTR lpCaption, int type);	//	@43BD50
	int				GetCoverdemoPlayMode() { return 0; };	//	@43BF70
	int				GetCoverdemoInactiveTimeoutSec() { return MAXINT; }	//	@43BF80
	long			GetCoverdemoGameplayTimeoutSec() { return MAXLONG; }	//	@43BF90
	void			_CreateWindow(UINT16 nIconResourceId);	//	@43C2C0
	void			Process(bool (*GameLoop)(void));	//	@43C4A0
	ATOM			RegisterWindowClass(UINT16 nMenuResourceId, UINT16 nIconResourceId);	//	@43C570
	BOOL			_SetTitle(LPCSTR lpString);	//	@43C850
	void			SetDesktopDirectory(const char* szDesktopPath);	//	@43C8B0

public:
	Window(const char* wndClassName, int unkFlags, UINT16 nMenuResourceId, char* szFileSystem, UINT16 nIconResourceId);	//	@43C630
	~Window();	//	@43C230

	void* operator new(size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	static const char	RegistryKey[];	//	@A09178
	static HINSTANCE	WindowInstanceHandle;	//	@A35EB0
	static STICKYKEYS	StickyKeysFeature;	//	@A0917C
	static TOGGLEKEYS	ToggleKeysFeature;	//	@A09184
	static FILTERKEYS	FilterKeysFeature;	//	@A0918C
	static bool			GameDiscFound;	//	@A35E68
};

extern Window* g_Window;

int	CALLBACK		GetSystemLanguageCode();	//	@43BDC0
void				FindStringResource(int nBaseStringResourcesAddr, wchar_t* outString, int nMaxsize);	//	@43BFB0
void				IncompatibleMachineParameterError(int messageID, char bWarningIcon);	//	@43C040
void				SetAccessibilityFeatures(bool bCollect);	//	@43C140
LRESULT CALLBACK	WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);	//	@43C320
int	CALLBACK		WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd);	//	@43CB40
void				GetUserDocumentsDir(String& outString);	//	@43CAE0
void				FindGameDir();	//	@439230
void				SetWorkingDir(const char* str);	//	@438460
void				SetGameWorkingDir(const char* str);	//	@438560
void				GetWorkingDirRelativePath(String* str);	//	@437A70
void				GetGameWorkingDirRelativePath(String* str);	//	@437B80
void				SetWarningString(const char* (*ptr)());	//	@406E00
const char*			GetWarningString();	//	@406E10

static const char*	(*WarningStringProc)() = nullptr;	//	@A35B80

extern String		WorkingDirectory;	//	@A08FA0
extern String		GameWorkingDirectory;	//	@A08FB0

#ifdef INCLUDE_FIXES
int CALLBACK		MenuClickCallback(WPARAM wParam);
#endif

static_assert(sizeof(Window) == WINDOW_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Window));