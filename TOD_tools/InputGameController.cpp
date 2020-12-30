#include "InputGameController.h"
#include "ScriptDatabase.h"

Input::Gamepad* g_InputGamepad[INPUT_GAMEPAD_MAX_GAMEPADS] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr
};

const char* g_GamepadButtons[INPUT_GAMEPAD_MAX_BUTTONS] = {
	"SELECT",
	"L3",
	"R3",
	"START",
	"D_UP",
	"D_RIGHT",
	"D_DOWN",
	"D_LEFT",
	"L2",
	"R2",
	"L1",
	"R1",
	"TRIANGLE",
	"CIRCLE",
	"CROSS",
	"SQUARE",
};

namespace Input
{
	Gamepad::Gamepad(int controllerIndex, int unk1)
	{
		MESSAGE_CLASS_CREATED(Gamepad);

		m_sModelName = String();
		field_B4 = unk1;
		m_bForceFeedbackAvailable = false;
		m_bIsVibrating = false;
		field_E1 = NULL;
		m_pDirectInputEffect = nullptr;
		field_D4 = NULL;
		m_fVibrationForceDefault = 0.0f;
		m_fVibrationForce = 0.0f;
		m_pDirectInputDevice = nullptr;

		if (!ControllersCreated)
		{
			DirectInput8Create_Hooked(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&g_pDirectInput8Interface, NULL);
			EnumGameControllers();
		}

		++ControllersCreated;
		g_InputGamepad[controllerIndex] = this;

		if (controllerIndex >= DirectInputGamepadsFound)
			return;

		GamepadsArray = nullptr;

		int _pvRef = controllerIndex;

		if (Script::ForceFeedback &&
			g_pDirectInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)&EnumControllersCallback, &_pvRef, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY) >= NULL &&
			GamepadsArray)
		{
			debug("CONTROLLER: Found a force-feedback device!");
			m_bForceFeedbackAvailable = true;

			if (!m_pDirectInputDevice)
			{
				debug("CONTROLLER: Error - could not create input device!");
				return;
			}
		}

		if (g_pDirectInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)&EnumControllersCallback, &_pvRef, DIEDFL_ALLDEVICES) >= NULL &&
			GamepadsArray)
		{
			debug("CONTROLLER: Using standard game device");

			if (!m_pDirectInputDevice)
			{
				debug("CONTROLLER: Error - could not create input device!");
				return;
			}

			m_pDirectInputDevice->Acquire();

			memset(&m_ButtonsStates, NULL, sizeof(m_ButtonsStates));

			debug("CONTROLLER: Model of game device = %s", m_sModelName.m_szString);

			String sModelName;
			AllocateGamepadNameStringBuffer(&sModelName, m_sModelName.m_szString, 8);

			if (strncmp(sModelName.m_szString, "SmartJoy", 8) == 0)
				m_SmartJoyGamepad = 1;
			else
				m_SmartJoyGamepad = 0;

			if (!m_bForceFeedbackAvailable)
				return;

			//	Disable auto-center.
			DIPROPDWORD DIPropAutoCenterDisable;

			DIPropAutoCenterDisable.diph.dwSize = sizeof(DIPropAutoCenterDisable);
			DIPropAutoCenterDisable.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			DIPropAutoCenterDisable.diph.dwObj = NULL;
			DIPropAutoCenterDisable.diph.dwHow = DIPH_DEVICE;
			DIPropAutoCenterDisable.dwData = DIPROPAUTOCENTER_OFF;

			if (FAILED(m_pDirectInputDevice->SetProperty(DIPROP_AUTOCENTER, &DIPropAutoCenterDisable.diph)))
				debug("CONTROLLER: Could not disable auto-center");

			//	Create rumble effect.
			//	TODO: these parameters are NOT what they are in exe.
			DWORD dwAxes[2] = { DIJOFS_X, DIJOFS_Y };
			long lDirection[2] = { NULL, NULL };

			DIPERIODIC	diPeriodic;
			DIENVELOPE	diEnvelope;
			DIEFFECT	diEffect;

			diPeriodic.dwMagnitude = DI_FFNOMINALMAX;
			diPeriodic.lOffset = 0;
			diPeriodic.dwPhase = 0;
			diPeriodic.dwPeriod = (DWORD)(0.05f * DI_SECONDS);

			diEnvelope.dwSize = sizeof(DIENVELOPE);
			diEnvelope.dwAttackLevel = 0;
			diEnvelope.dwAttackTime = (DWORD)(0.5f * DI_SECONDS);
			diEnvelope.dwFadeLevel = 0;
			diEnvelope.dwFadeTime = (DWORD)(1.0f * DI_SECONDS);

			diEffect.dwSize = sizeof(DIEFFECT);
			diEffect.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
			diEffect.dwDuration = (2 * DI_SECONDS);

			diEffect.dwSamplePeriod = 0;
			diEffect.dwGain = DI_FFNOMINALMAX;
			diEffect.dwTriggerButton = DIJOFS_BUTTON0;
			diEffect.dwTriggerRepeatInterval = 0;
			diEffect.cAxes = 2;
			diEffect.rgdwAxes = dwAxes;
			diEffect.rglDirection = &lDirection[0];
			diEffect.lpEnvelope = &diEnvelope;
			diEffect.cbTypeSpecificParams = sizeof(diPeriodic);
			diEffect.lpvTypeSpecificParams = &diPeriodic;

			LPDIRECTINPUTEFFECT lpdiRumbleEffect;

			if (FAILED(m_pDirectInputDevice->CreateEffect(GUID_RUMBLE_EFFECT, &diEffect, &lpdiRumbleEffect, NULL)))
				debug("CONTROLLER: Could not create rumble effect for some reason");

			return;
		}

		debug("CONTROLLER: WARNING - Could not create an input device");
	}

	Gamepad::~Gamepad()
	{
		MESSAGE_CLASS_DESTROYED(Gamepad);

		--ControllersCreated;

		if (m_pDirectInputDevice)
		{
			m_pDirectInputDevice->Unacquire();
			if (m_pDirectInputDevice)
			{
				m_pDirectInputDevice->Release();
				m_pDirectInputDevice = nullptr;
			}
		}

		if (!ControllersCreated)
		{
			if (g_pDirectInput8Interface)
			{
				g_pDirectInput8Interface->Release();
				g_pDirectInput8Interface = nullptr;
			}
		}
	}

	Gamepad* Gamepad::GetGameControllerByIndex(signed int controllerIndex)
	{
		if (controllerIndex >= 0 ||
			controllerIndex < INPUT_GAMEPAD_MAX_GAMEPADS)
			return g_InputGamepad[controllerIndex];
		else
			return nullptr;
	}

	BOOL Gamepad::DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
	{
		++DirectInputGamepadsFound;

		return TRUE;
	}

	signed int Gamepad::_439690(int* unk1, int* unk2)
	{
		debug("Found effect: %s\n", (const char*)(unk1 + 8));
		*unk2 = unk1[1];
		unk2[1] = unk1[2];
		unk2[2] = unk1[3];
		unk2[3] = unk1[4];
		return 1;
	}

	signed int Gamepad::_4396D0(int* unk1, int unk2)
	{
		debug("Found object: %s\n", (const char*)(unk1 + 8));
		return 1;
	}

	const char* Gamepad::GamepadButtonToString(int buttonNumber)
	{
		if (buttonNumber >= 0 &&
			buttonNumber < INPUT_GAMEPAD_MAX_BUTTONS)
			return g_GamepadButtons[buttonNumber];
		else
			return "<none>";
	}

	void Gamepad::EnumGameControllers()
	{
		DirectInputGamepadsFound = NULL;

		g_pDirectInput8Interface->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)DIEnumDevicesCallback, NULL, NULL);
		*(unsigned int*)g_InputGamepad = (unsigned int)NULL;
	}

	int Gamepad::NumberDirectInputDevices()
	{
		if (DirectInputGamepadsFound != -1)
			return DirectInputGamepadsFound;

		bool bInterfaceCreated = false;

		if (!g_pDirectInput8Interface)
		{
			DirectInput8Create(Window::WindowInstanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8A, (LPVOID*)&g_pDirectInput8Interface, NULL);
			bInterfaceCreated = true;
		}

		EnumGameControllers();

		if (bInterfaceCreated)
		{
			if (g_pDirectInput8Interface)
			{
				g_pDirectInput8Interface->Release();
				g_pDirectInput8Interface = nullptr;
			}
		}

		return DirectInputGamepadsFound;
	}

	void Gamepad::Process()
	{
		for (int ind = 0; ind < INPUT_GAMEPAD_MAX_GAMEPADS; ind++)
		{
			if (ind >= 0 && ind < INPUT_GAMEPAD_MAX_GAMEPADS && ind < DirectInputGamepadsFound)
			{
				if (g_InputGamepad[ind] &&
					g_InputGamepad[ind]->m_pDirectInputDevice)
				{
					g_InputGamepad[ind]->ProcessController();
				}
			}
		}
	}

	BOOL CALLBACK Gamepad::EnumControllersCallback(LPCDIDEVICEINSTANCE devInst, Gamepad** pvRef)
	{
		if (*GamepadsArray != *pvRef)
		{
			GamepadsArray++;
			return DIENUM_CONTINUE;
		}

		pvRef[1]->m_nGUID.Data1 = devInst->guidInstance.Data1;
		pvRef[1]->m_nGUID.Data2 = devInst->guidInstance.Data2;
		*(int*)(pvRef[1]->m_nGUID.Data4) = *(int*)(devInst->guidInstance.Data4);
		pvRef[1]->m_nGUID.Data4[4] = pvRef[1]->m_nGUID.Data4[4];

		strcpy(pvRef[1]->m_sModelName.m_szString, devInst->tszProductName);
		g_pDirectInput8Interface->CreateDevice(devInst->guidInstance, &pvRef[1]->m_pDirectInputDevice, NULL);

		if (FAILED(pvRef[1]->m_pDirectInputDevice->SetDataFormat((LPCDIDATAFORMAT)&GUID_CONTROLLER_DATA_FORMAT)))
		{
			MessageBox(g_Window->m_hWindow, "Unable to set game controller data format", "Error", MB_OK);

			if (!pvRef[1]->m_pDirectInputDevice)
				return DIENUM_STOP;
			pvRef[1]->m_pDirectInputDevice->Release();
			pvRef[1]->m_pDirectInputDevice = nullptr;

			return DIENUM_STOP;
		}

		if (Script::ForceFeedback)
			if (FAILED(pvRef[1]->m_pDirectInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_EXCLUSIVE | DISCL_BACKGROUND)))
			{
				MessageBox(g_Window->m_hWindow, "Unable to set game controller cooperative level", "Error", MB_OK);

				if (!pvRef[1]->m_pDirectInputDevice)
					return DIENUM_STOP;
				pvRef[1]->m_pDirectInputDevice->Release();
				pvRef[1]->m_pDirectInputDevice = nullptr;

				return DIENUM_STOP;
			}

		GamepadsArray++;

		return DIENUM_STOP;
	}

	String* Gamepad::AllocateGamepadNameStringBuffer(String* outString, char* inGamepadName, signed int length)
	{
		outString->Set(inGamepadName);

		return outString;
	}

	Window* Gamepad::GetWindow()
	{
		return g_Window;
	}

	void Gamepad::_Acquire()
	{
		if (m_pDirectInputDevice)
			m_pDirectInputDevice->Acquire();
	}

	void Gamepad::_Unacquire()
	{
		if (m_pDirectInputDevice)
			m_pDirectInputDevice->Unacquire();
	}

	Vector4f* Gamepad::GetAxisPosition(Vector4f* outPos, int axisNumber)
	{
		if (!m_pDirectInputDevice)
		{
			outPos->x = outPos->y = outPos->z = outPos->a = 0.0f;
			return outPos;
		}

		if (field_B4 != 1 &&
			field_B4 != 2)
		{
			if (!axisNumber)
			{
				*outPos = m_vAxisPosition;
				return outPos;
			}

			outPos->x = outPos->y = outPos->z = outPos->a = 0.0f;
			return outPos;
		}

		if (axisNumber)
		{
			if (axisNumber != 1)
			{
				outPos->x = outPos->y = outPos->z = outPos->a = 0.0f;
				return outPos;
			}

			outPos->y = m_vAxisRotation.z;
			outPos->x = m_vAxisPosition.z;
		}else{
			outPos->y = m_vAxisPosition.y;
			outPos->x = m_vAxisPosition.x;
		}

		outPos->z = 0.0f;
		outPos->a = 0.0f;
		return outPos;
	}

	Orientation* Gamepad::GetAxisRotation(Orientation* outRot, int unk2)
	{
		if (!m_pDirectInputDevice)
		{
			outRot->x = outRot->y = outRot->z = outRot->w = 0.0f;
			return outRot;
		}

		if (unk2)
		{
			outRot->x = outRot->y = outRot->z = outRot->w = 0.0f;
			return outRot;
		}

		*outRot = m_vAxisRotation;
		return outRot;
	}

	void Gamepad::_439840()
	{
		for (int ind = 32; ind != 0; ind--)
			m_ButtonsStates[ind] &= 2;
	}

	int Gamepad::_439860(int unk1)
	{
		if (field_B4 != 1)
			return unk1;

		if (!m_SmartJoyGamepad)
			return *(int*)(&ms_A08FD8 + unk1);

		if (m_SmartJoyGamepad == 1)
			return *(int*)(&ms_A09018 + unk1);

		return unk1;
	}

	bool Gamepad::_4398A0(int unk1)
	{
		if (!m_pDirectInputDevice)
			return false;

		if (field_B4 != 1)
			return (m_ButtonsStates[unk1] & 2) && (m_ButtonsStates[unk1] & 1);

		if (!m_SmartJoyGamepad)
			return (m_ButtonsStates[*(int*)(&ms_A08FD8 + unk1)] & 2) && (m_ButtonsStates[*(int*)(&ms_A08FD8 + unk1)] & 1);

		if (m_SmartJoyGamepad != 1)
			return (m_ButtonsStates[unk1] & 2) && (m_ButtonsStates[unk1] & 1);

		return (m_ButtonsStates[*(int*)(&ms_A09018 + unk1)] & 2) && (m_ButtonsStates[*(int*)(&ms_A09018 + unk1)] & 1);
	}

	bool Gamepad::_439910(int unk1)
	{
		if (!m_pDirectInputDevice)
			return false;

		if (field_B4 != 1)
			return (~m_ButtonsStates[unk1] & 2) && (m_ButtonsStates[unk1] & 1);

		if (!m_SmartJoyGamepad)
			return (~m_ButtonsStates[*(int*)(&ms_A08FD8 + unk1)] & 2) && (m_ButtonsStates[*(int*)(&ms_A08FD8 + unk1)] & 1);

		if (m_SmartJoyGamepad != 1)
			return (~m_ButtonsStates[unk1] & 2) && (m_ButtonsStates[unk1] & 1);

		return (~m_ButtonsStates[*(int*)(&ms_A09018 + unk1)] & 2) && (m_ButtonsStates[*(int*)(&ms_A09018 + unk1)] & 1);
	}

	char Gamepad::_439970(int unk1)
	{
		if (!m_pDirectInputDevice)
			return NULL;

		if (field_B4 != 1)
			return (m_ButtonsStates[unk1] >> 1) & 1;
		if (!m_SmartJoyGamepad)
			return (m_ButtonsStates[*(int*)(&ms_A08FD8 + unk1)] >> 1) & 1;
		if (m_SmartJoyGamepad != 1)
			return (m_ButtonsStates[unk1] >> 1) & 1;

		return (m_ButtonsStates[*(int*)(&ms_A09018 + unk1)] >> 1) & 1;
	}

	double Gamepad::_4399D0(int unk1)
	{
		if (!_439970(unk1))
			return 0.0;
		else
			return 1.0;
	}

	bool Gamepad::IsDInputDeviceFound()
	{
		return m_pDirectInputDevice != nullptr;
	}

	bool Gamepad::IsControllerPresent(signed int controllerIndex)
	{
		if (controllerIndex < 0 ||
			controllerIndex >= 4)
			return false;

		if (controllerIndex < DirectInputGamepadsFound &&
			g_InputGamepad &&
			g_InputGamepad[controllerIndex] &&
			GetGameControllerByIndex(controllerIndex)->m_pDirectInputDevice)
			return true;

		return false;
	}

	void Gamepad::SetControllerVibration(signed int controllerIndex, float force)
	{
		if (!m_bForceFeedbackAvailable)
		{
			if (controllerIndex)
			{
				if (controllerIndex == 1)
					m_fVibrationForce = force;
			}else
				m_fVibrationForceDefault = force;

			return;
		}

		DIEFFECT dEffect;

		memset(&dEffect, NULL, sizeof(DIEFFECT));
		int _force = (int)(force * 10000.0f);

		dEffect.dwSize = sizeof(DIEFFECT);
		dEffect.cbTypeSpecificParams = 4;
		dEffect.lpvTypeSpecificParams = &_force;

		//	NOTE: wtf?
		if (controllerIndex)
		{
			if (controllerIndex)
			{
				if (controllerIndex == 1)
					m_fVibrationForce = force;
			}else
				m_fVibrationForceDefault = force;

			return;
		}

		if (!m_pDirectInputEffect)
		{
			if (controllerIndex)
			{
				if (controllerIndex == 1)
					m_fVibrationForce = force;
			}else
				m_fVibrationForceDefault = force;

			return;
		}

		if (force > 0.0f && !m_bIsVibrating)
		{
			m_pDirectInputEffect->Start(1, DIES_NODOWNLOAD);
			m_bIsVibrating = true;
		}

		if (force == 0.0f)
		{
			if (!m_bIsVibrating)
			{
				if (controllerIndex)
				{
					if (controllerIndex == 1)
						m_fVibrationForce = force;
				}
				else
					m_fVibrationForceDefault = force;

				return;
			}

			m_pDirectInputEffect->Stop();
			m_bIsVibrating = false;
		}

		if (m_bIsVibrating)
			m_pDirectInputEffect->SetParameters(&dEffect, DIEP_TYPESPECIFICPARAMS);

		if (controllerIndex)
		{
			if (controllerIndex == 1)
				m_fVibrationForce = force;
		}
		else
			m_fVibrationForceDefault = force;
	}

	double Gamepad::GetControllerVibration(int unk1)
	{
		if (!unk1)
			return m_fVibrationForceDefault;
		if (unk1 == 1)
			return m_fVibrationForce;

		return 0.0;
	}

	void Gamepad::StartRumbleEffect()
	{
		debug("rumble, rumble, rumble...\n");

		if (m_pDirectInputEffect)
			m_pDirectInputEffect->Start(1, DIES_NODOWNLOAD);
	}

	const char* Gamepad::GetModelNameString()
	{
		return m_sModelName.m_szString;
	}

	void Gamepad::ProcessController()
	{
		if (m_pDirectInputDevice)
		{
			DIJOYSTATE joyState;
			memset(&joyState, NULL, sizeof(DIJOYSTATE));

			m_pDirectInputDevice->Poll();
			HRESULT devstate = m_pDirectInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState);

			if (devstate == DIERR_INPUTLOST || devstate == DIERR_NOTACQUIRED)
				return;
			if (m_pDirectInputDevice)
				m_pDirectInputDevice->Acquire();
			if (FAILED(m_pDirectInputDevice->GetDeviceState(sizeof(DIJOYSTATE), &joyState)))
				return;

			//	TODO: poll device buttons and axis rotations and fill in structures.
		}

		if (FAILED(g_pDirectInput8Interface->CreateDevice(m_nGUID, &m_pDirectInputDevice, NULL)))
		{
			m_pDirectInputDevice = nullptr;
			return;
		}

		if (FAILED(m_pDirectInputDevice->SetDataFormat((LPCDIDATAFORMAT)&GUID_CONTROLLER_DATA_FORMAT)))
		{
			MessageBox(g_Window->m_hWindow, "Unable to set game controller data format", "Error", MB_OK);
			if (m_pDirectInputDevice)
			{
				m_pDirectInputDevice->Release();
				m_pDirectInputDevice = nullptr;
			}
		}

		if (FAILED(m_pDirectInputDevice->SetCooperativeLevel(g_Window->m_hWindow, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			MessageBox(g_Window->m_hWindow, "Unable to set game controller cooperative level", "Error", MB_OK);
			if (m_pDirectInputDevice)
			{
				m_pDirectInputDevice->Release();
				m_pDirectInputDevice = nullptr;
			}
		}
	}

	int& Gamepad::ControllersCreated = *(int*)0xA35E74;
	LPDIENUMDEVICESCALLBACK Gamepad::EnumCallback = (LPDIENUMDEVICESCALLBACK)0x43A1D0;
	int& Gamepad::DirectInputGamepadsFound = *(int*)0xA08FD0;
	Gamepad** Gamepad::GamepadsArray = (Gamepad**)0xA35E6C;
	IDirectInput8* Gamepad::g_pDirectInput8Interface = (IDirectInput8*)0xA35E78;
	int& Gamepad::ms_A08FD8 = *(int*)0xA08FD8;
	int& Gamepad::ms_A09018 = *(int*)0xA09018;

}