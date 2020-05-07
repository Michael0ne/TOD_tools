#include "InputGameController.h"

Input::Gamepad* g_InputGamepad[INPUT_GAMEPAD_MAX_GAMEPADS] = {
	nullptr,
	nullptr,
	nullptr,
	nullptr
};

namespace Input
{
	int& Gamepad::ControllersCreated = *(int*)0xA35E74;
	LPDIENUMDEVICESCALLBACK Gamepad::EnumCallback = (LPDIENUMDEVICESCALLBACK)0x43A1D0;
	int& Gamepad::DirectInputGamepadsFound = *(int*)0xA08FD0;

	void Gamepad::Init()
	{
		patch(0xA35E7C, this, 4);

		CreateDevice();
	}

	void Gamepad::CreateDevice()
	{
		//	TODO: implementation!
	}

	void Gamepad::Process()
	{
		Gamepad** gamepadarr = g_InputGamepad;
		int controllerIndex = 0;

		do {
			if (controllerIndex >= 0 && controllerIndex < INPUT_GAMEPAD_MAX_GAMEPADS && controllerIndex < DirectInputGamepadsFound) {
				if (gamepadarr) {
					Gamepad* gamepad = gamepadarr[controllerIndex];

					if (gamepad) {
						if (gamepad->m_pDirectInputDevice) {
							gamepad->ProcessController();
							gamepadarr = g_InputGamepad;
						}
					}
				}
			}

			controllerIndex++;
		} while (controllerIndex < INPUT_GAMEPAD_MAX_GAMEPADS);
	}

	void Gamepad::ProcessController()
	{
		//	TODO: implementation!
	}

	void Gamepad::Reset()
	{

	}
}

void inline PATCH_INPUT_GAMEPAD()
{}