#include "Control.h"
#include "IntegerType.h"
#include "StringType.h"
#include "InputKeyboard.h"
#include "InputMouse.h"
#include "InputGameController.h"
#ifdef DIRECTX
#include "GfxInternal_Dx9.h"
#endif
#ifdef OPENGL
#include "GfxInternal_OGL.h"
#endif

EntityType* tControl;

void Control::Register()
{
	tControl = new EntityType("Control");
	tControl->InheritFrom(tNode);
	tControl->SetCreator((EntityType::CREATOR)Create);

	tControl->RegisterProperty(tINTEGER, "key", &GetKey, NULL, NULL, NULL, &SetKey, NULL, NULL, NULL, "control=string", NULL, NULL, -1);
	tControl->RegisterProperty(tSTRING, "keystr", &GetKeyStr, NULL, NULL, NULL, &SetKeyStr, NULL, NULL, NULL, "control=string", NULL, NULL, -1);

	tControl->_86E9B0();
}

const int Control::MousePositionEngineToString() const
{
	if (m_ControlType != MOUSE)
		return NULL;

	switch (m_Key)
	{
	case 0:
		return g_InputMouse->m_Position_X;
	case 1:
		return g_InputMouse->m_Position_Y;
	default:
		return NULL;
	}
}

const int Control::MousePositionRawToString() const
{
	if (m_ControlType != MOUSE)
		return NULL;

	switch (m_Key)
	{
	case 0:
		return
#ifdef DIRECTX
			g_GfxInternal_Dx9->m_Windowed
#endif
#ifdef OPENGL
			g_GfxInternal_OGL->m_Windowed
#endif
			? g_InputMouse->m_WindowedMousePosition.x : g_InputMouse->m_FullscreenMousePosition.x;
	case 1:
		return
#ifdef DIRECTX
			g_GfxInternal_Dx9->m_Windowed
#endif
#ifdef OPENGL
			g_GfxInternal_OGL->m_Windowed
#endif
			? g_InputMouse->m_WindowedMousePosition.y : g_InputMouse->m_FullscreenMousePosition.y;
	default:
		return NULL;
	}
}

const int Control::GetKey() const
{
	return m_Key;
}

void Control::SetKey(const int keyind)
{
	if (keyind >= 0)
		m_Key = keyind;
}

const char* Control::GetKeyStr() const
{
	switch (m_ControlType)
	{
	case NONE:
		return nullptr;
	case KEYBOARD:
		return Input::Keyboard::KeyboardButtonToStr(m_Key);
	case MOUSE:
		return Input::Mouse::MousePositionTostr(m_Key);
	case MOUSE_BUTTON:
		return Input::Mouse::MouseButtonToStr(m_Key);
	case GAMEPAD:
		return Input::Gamepad::GamepadButtonToString(m_Key);
	case DPAD:
		return DPadKeyToStr(m_Key);
	default:
		return nullptr;
	}
}

void Control::SetKeyStr(const char* args)
{
	m_Key = -1;
	m_ControlType = NONE;

	for (unsigned int i = 0; i < 255; ++i)
	{
		if (strcmp(Input::Keyboard::KeyboardButtonToStr(i), args) == NULL)
		{
			m_Key = i;
			m_ControlType = KEYBOARD;

			return;
		}
	}

	for (unsigned int i = 0; i < 2; ++i)
	{
		if (strcmp(Input::Mouse::MousePositionTostr(i), args) == NULL)
		{
			m_Key = i;
			m_ControlType = MOUSE;

			return;
		}
	}

	for (unsigned int i = 0; i < 10; ++i)
	{
		if (strcmp(Input::Mouse::MouseButtonToStr(i), args) == NULL)
		{
			m_Key = i;
			m_ControlType = MOUSE_BUTTON;

			return;
		}
	}

	for (unsigned int i = 0; i < 255; ++i)
	{
		if (strcmp(Input::Gamepad::GamepadButtonToString(i), args) == NULL)
		{
			m_Key = i;
			m_ControlType = GAMEPAD;

			return;
		}
	}

	for (unsigned int i = 0; i < 255; ++i)
	{
		if (strcmp(DPadKeyToStr(i), args) == NULL)
		{
			m_Key = i;
			m_ControlType = DPAD;

			return;
		}
	}

	LogDump::LogA("WARNING: keystr '%s' didn't map to any key\n", args);
}

const char* Control::DPadKeyToStr(const unsigned int keyid) const
{
	switch (keyid)
	{
	case 0:
		return "A_LEFT";
	case 1:
		return "A_RIGHT";
	case 2:
		return "A_UP";
	case 3:
		return "A_DOWN";
	case 4:
		return "C_LEFT";
	case 5:
		return "C_RIGHT";
	case 6:
		return "C_UP";
	case 7:
		return "C_DOWN";
	default:
		return nullptr;
	}
}

Control* Control::Create(AllocatorIndex)
{
	return new Control;
}