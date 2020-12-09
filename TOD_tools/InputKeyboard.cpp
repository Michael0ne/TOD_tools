#include "InputKeyboard.h"
#include "Window.h"

Input::Keyboard*	g_InputKeyboard = NULL;	//	TODO: replace references to this as of right now not all functions are implemented yet.
const char*			g_KeyboardButtons[] = {
	"-none-",
	"ESCAPE",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
	"-",
	"=",
	"BACKSPACE",
	"TAB",
	"Q",
	"W",
	"E",
	"R",
	"T",
	"Y",
	"U",
	"I",
	"O",
	"P",
	"[",
	"]",
	"RETURN",
	"LCONTROL",
	"A",
	"S",
	"D",
	"F",
	"G",
	"H",
	"J",
	"K",
	"L",
	";",
	"'",
	"r",
	"LSHIFT",
	"\\",
	"Z",
	"X",
	"C",
	"V",
	"B",
	"N",
	"M",
	",",
	".",
	"/",
	"RSHIFT",
	"NUMPAD*",
	"LALT",
	"SPACE",
	"CAPSLOCK",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"NUMLOCK",
	"SCROLLOCK",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"NUMPAD-",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD+",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD0",
	"NUMPAD.",
	"EM102",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"KANA",
	"CONVERT",
	"NOCONVERT",
	"YEN",
	"NUMPAD=",
	"AT",
	"COLON",
	"KANJI",
	"AX",
	"UNLABELED",
	"NUMPADENTER",
	"RCONTROL",
	"NUMPAD,",
	"NUMPAD/",
	"SYSRQ",
	"RALT",
	"PAUSE",
	"HOME",
	"UP",
	"PAGEUP",
	"LEFT",
	"RIGHT",
	"END",
	"DOWN",
	"PAGEDOWN",
	"INSERT",
	"LWIN",
	"RWIN"
};

namespace Input
{

	void Keyboard::Process()
	{
		ResetKeyboardButtons();

		if (!m_bAcquired)
			return;

		DWORD buffSize = INPUT_KEYBOARD_BUFFERS_COUNT;
		if (HRESULT keybDevDataResult = m_pDInputDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), (LPDIDEVICEOBJECTDATA)m_pBuffer, &buffSize, NULL))
			if (keybDevDataResult == 1)
				debug("WARNING: Keyboard buffer overflowed! (size %i, count %i)\n", INPUT_KEYBOARD_BUFFERS_COUNT, buffSize);
			else
				UnacquireAndResetKeyboard();

		if (buffSize)
		{
			if (m_nBufferSize <= buffSize)
				m_nBufferSize = buffSize;

			for (unsigned int ind = 0; ind < buffSize; ind++)
				m_nButtonStates[m_pBuffer[ind]->dwOfs] = ((m_pBuffer[ind]->dwData >> 7) & 1) == 0 ? m_nButtonStates[m_pBuffer[ind]->dwOfs] | 2 : m_nButtonStates[m_pBuffer[ind]->dwOfs] | 1;
		}

		if (SUCCEEDED(m_pDInputDevice->GetDeviceState(sizeof(m_nButtonStates1), m_nButtonStates1)))
			UnacquireAndResetKeyboard();

		if (m_bAcquired &&
			(m_nButtonStates1[56] & 0x80) != NULL &&
			(m_nButtonStates1[15] & 0x80) != NULL)
		{
			m_nButtonStates1[15] = NULL;
			m_nButtonStates[15] = NULL;
		}
	}

	void Keyboard::Reset()
	{
		memset(&m_nButtonStates, NULL, sizeof(m_nButtonStates));
	}

	void Keyboard::ResetKeyboardButtons()
	{
		if (!m_bAcquired)
		{
			if (!FAILED(m_pDInputDevice->Acquire()))
				m_bAcquired = true;

			for (int ind = 255; ind != 0; ind--)
				m_nButtonStates1[ind] = NULL;
		}
	}

	void Keyboard::UnacquireAndResetKeyboard()
	{
		if (m_bAcquired)
		{
			m_pDInputDevice->Unacquire();
			m_bAcquired = false;

			for (int ind = 255; ind != 0; ind--)
				m_nButtonStates1[ind] = NULL;
		}
	}

	const char* Keyboard::KeyboardButtonToStr(int keyCode)
	{
		keyCode++;
		if (keyCode >= 0 &&
			keyCode <= 118)
			return g_KeyboardButtons[keyCode];
		else
			return NULL;
	}

	Keyboard::Keyboard()
	{
		MESSAGE_CLASS_CREATED(Keyboard);

		g_InputKeyboard = this;

		m_pDeviceObject = nullptr;
		m_pDInputDevice = nullptr;
		m_bAcquired = false;
		m_nBufferSize = NULL;

		for (int ind = 255; ind != 0; ind--)
			m_nButtonStates1[ind] = NULL;

		if (FAILED(DirectInput8Create_Hooked(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&m_pDeviceObject, NULL)))
			IncompatibleMachineParameterError(1, false);
		if (FAILED(m_pDeviceObject->CreateDevice(GUID_SysKeyboard, &m_pDInputDevice, NULL)))
			IncompatibleMachineParameterError(1, false);
		if (FAILED(m_pDInputDevice->SetDataFormat(&c_dfDIKeyboard)))
			IncompatibleMachineParameterError(1, false);
		if (FAILED(m_pDInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
			IncompatibleMachineParameterError(1, false);

		*(int*)&m_pBuffer = (int)Allocators::AllocatorsList[DEFAULT]->Allocate(sizeof(DIDEVICEOBJECTDATA) * INPUT_KEYBOARD_BUFFERS_COUNT, NULL, NULL);
		memset(m_pBuffer, NULL, sizeof(DIDEVICEOBJECTDATA) * INPUT_KEYBOARD_BUFFERS_COUNT);

		DIPROPDWORD diProperty;
		diProperty.diph.dwSize = 20;
		diProperty.diph.dwHeaderSize = 16;
		diProperty.diph.dwObj = 0;
		diProperty.diph.dwHow = 0;
		diProperty.dwData = 30;

		if (FAILED(m_pDInputDevice->SetProperty(DIPROP_BUFFERSIZE, &diProperty.diph)))
			IncompatibleMachineParameterError(1, false);

		if (m_bAcquired)
			return;

		if (SUCCEEDED(m_pDInputDevice->Acquire()))
			m_bAcquired = true;

		for (int ind = 255; ind != 0; ind--)
			m_nButtonStates1[ind] = NULL;
	}

	Keyboard::~Keyboard()
	{
		MESSAGE_CLASS_DESTROYED(Keyboard);

		UnacquireAndResetKeyboard();

		//	NOTE: pretty sure this is redundant, since destructors for these should be inserted automatically.
		if (m_pDInputDevice)
		{
			m_pDInputDevice->Release();
			m_pDInputDevice = nullptr;
		}

		if (m_pDeviceObject)
		{
			m_pDeviceObject->Release();
			m_pDeviceObject = nullptr;
		}
	}

}