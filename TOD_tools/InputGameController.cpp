#include "InputGameController.h"

Input::Gamepad* g_InputGamepad = NULL;

namespace Input
{
	int& Gamepad::ControllersCreated = *(int*)0xA35E74;
	LPDIENUMDEVICESCALLBACK Gamepad::EnumCallback = (LPDIENUMDEVICESCALLBACK)0x43A1D0;

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

	}

	void Gamepad::Reset()
	{

	}
}

void inline PATCH_INPUT_GAMEPAD()
{}