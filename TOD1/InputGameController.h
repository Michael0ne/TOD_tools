#pragma once
#include "Window.h"

namespace Input {

	static const GUID& GUID_RUMBLE_EFFECT = *(const GUID*)0x9D8694;
	static const GUID& GUID_CONTROLLER_DATA_FORMAT = *(const GUID*)0x9D822C;

#define INPUT_GAMEPAD_MAX_GAMEPADS 4
#define INPUT_GAMEPAD_MAX_BUTTONS 16

	class Gamepad
	{
	private:
		IDirectInputDevice8*	m_pDirectInputDevice;
		int						m_ButtonsStates[32];
		String					m_sModelName;
		Vector4f				m_vAxisPosition;
		Orientation				m_vAxisRotation;
		int						field_B4;
		int						m_SmartJoyGamepad;
		GUID					m_nGUID;
		bool					m_bForceFeedbackAvailable;
		char					field_CD;
		char					field_CE[2];
		IDirectInputEffect*		m_pDirectInputEffect;
		int						field_D4;
		float					m_fVibrationForceDefault;
		float					m_fVibrationForce;
		bool					m_bIsVibrating;
		char					field_E1;
		char					field_E2;
		char					field_E3;

	public:
		Gamepad(int controllerIndex, int unk1);	//	@43A2F0
		~Gamepad();	//	@439D60

		void* operator new(size_t size)
		{
			return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
		}
		void operator delete(void* ptr)
		{
			if (ptr)
				Allocators::ReleaseMemory(ptr, 0);
		}

		static Gamepad*			GetGameControllerByIndex(signed int controllerIndex);	//	@439660
		static BOOL				DIEnumDevicesCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);	//	@439680
		static signed int		_439690(int* unk1, int* unk2);	//	@439690
		static signed int		_4396D0(int* unk1, int unk2);	//	@4396D0
		static const char*		GamepadButtonToString(int buttonNumber);	//	@439A50
		static void				EnumGameControllers();	//	@439C90
		static int				NumberDirectInputDevices();	//	@439D00	//	NOTE: referenced from unused function.
		static void				Process();	//	@43A190
		static BOOL CALLBACK	EnumControllersCallback(LPCDIDEVICEINSTANCE devInst, Gamepad** pvRef);	//	@43A1D0
		static String*			AllocateGamepadNameStringBuffer(String* outString, char* inGamepadName, signed int length);	//	@43A670
		static Window*			GetWindow();	//	@43A730	//	NOTE: unused.
		
		void					_Acquire();	//	@4396F0
		void					_Unacquire();	//	@439700
		Vector4f*				GetAxisPosition(Vector4f* outPos, int axisNumber);	//	@439710
		Orientation*			GetAxisRotation(Orientation* outRot, int unk2);	//	@4397E0
		void					_439840();	//	@439840
		int						_439860(int unk1);	//	@439860
		bool					_4398A0(int unk1);	//	@4398A0
		bool					_439910(int unk1);	//	@439910
		char					_439970(int unk1);	//	@439970
		double					_4399D0(int unk1);	//	@4399D0
		bool					IsDInputDeviceFound();	//	@439A00	//	NOTE: unused.
		bool					IsControllerPresent(signed int controllerIndex);	//	@439A10
		void					SetControllerVibration(signed int controllerIndex, float force);	//	@439B10
		double					GetControllerVibration(int unk1);	//	@439C30
		void					StartRumbleEffect();	//	@439C60	//	NOTE: unused.
		const char*				GetModelNameString();	//	@439DF0	//	NOTE: unused.
		void					ProcessController();	//	@439E00

		static unsigned int		ControllersCreated;	//	@A35E74
		static LPDIENUMDEVICESCALLBACK EnumCallback;	//	@43A1D0
		static int				DirectInputGamepadsFound;	//	@A08FD0
		static Gamepad**		GamepadsArray;	//	@A35E6C
		static LPDIRECTINPUT8	g_pDirectInput8Interface;	//	@A35E78
		static unsigned int		_A08FD8[];	//	@A08FD8
		static unsigned int		_A09018[];	//	@A09018
	};
}

extern Input::Gamepad*			g_InputGamepad[INPUT_GAMEPAD_MAX_GAMEPADS];	//	@A35E7C
extern const char*				g_GamepadButtons[INPUT_GAMEPAD_MAX_BUTTONS];

ASSERT_CLASS_SIZE(Input::Gamepad, 228);