#pragma once

#include "stdafx.h"

namespace Input {

#define INPUT_GAMEPAD_CLASS_SIZE 228

	class Gamepad
	{
	private:
		BYTE	_pad[228];

	public:
		Gamepad()
		{
			debug("Input::Gamepad created at %X\n", this);
		}

		~Gamepad()
		{
			debug("Input::Gamepad destroyed!\n");
		}

		void	Init();
		void	Process();
		void	Reset();
	};
}

extern Input::Gamepad * g_InputGamepad;

static_assert(sizeof(Input::Gamepad) == INPUT_GAMEPAD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("InputGamepad"));