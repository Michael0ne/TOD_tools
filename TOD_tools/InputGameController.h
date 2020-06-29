#pragma once

#include "stdafx.h"

#include "Types.h"
#include "StringsPool.h"
#include "MemoryAllocators.h"

namespace Input {

#define INPUT_GAMEPAD_CLASS_SIZE 228
#define INPUT_GAMEPAD_MAX_GAMEPADS 4

	class Gamepad
	{
	private:
		IDirectInputDevice8* m_pDirectInputDevice;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		int field_58;
		int field_5C;
		int field_60;
		int field_64;
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
		int field_80;
		String m_sModelName;
		Orientation m_vAxisRotation;
		int field_A4;
		int field_A8;
		float m_fAC;	//	NOTE: this is related to axis rotation.
		int field_B0;
		int field_B4;
		int field_B8;
		GUID m_nGUID;
		char field_CC[4];
		IDirectInputEffect* m_pDirectInputEffect;
		int field_D4;
		float m_fVibrationForceDefault;
		float m_fVibrationForce;
		char field_E0[4];

		void	CreateDevice();

	public:
		Gamepad()
		{
			MESSAGE_CLASS_CREATED(Gamepad);
		}

		~Gamepad()
		{
			MESSAGE_CLASS_DESTROYED(Gamepad);
		}

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
		}

		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
		}

		void	Init();	//	@43A2F0
		void	ProcessController();	//	@439E00
		void	Reset();
		Orientation*	GetControllerAxisRotation(Orientation* outOrientation, unsigned int axisNumber);	//	@439710
		char			_439970(int);	//	@439970 NOTE: could be 'GetKeyState'.

		static void	Process();	//	@43A190

		static int& ControllersCreated;	//	@A35E74
		static LPDIENUMDEVICESCALLBACK EnumCallback;	//	@43A1D0
		static int& DirectInputGamepadsFound;	//	@A08FD0
	};
}

extern Input::Gamepad * g_InputGamepad[INPUT_GAMEPAD_MAX_GAMEPADS];

static_assert(sizeof(Input::Gamepad) == INPUT_GAMEPAD_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Input::Gamepad));