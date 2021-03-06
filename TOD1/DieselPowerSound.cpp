#include "DieselPowerSound.h"

namespace Audio
{
	DieselPower*			g_DieselPower = nullptr;	//	@A5E848

	const char*				DieselPower::RequiredVersion = AUDIO_DIESELPO_DLL_SUPPORTED_VERSION;	//	@A1BA50	//	NOTE: this becomes unused, since 'CheckDllVersion' doesn't use this for now.
	char					DieselPower::DetectedVersion[64];	//	@A5E800
	HMODULE					DieselPower::LibraryHandle;	//	@A5E840
	int						DieselPower::ErrorCode;	//	@A5E844
	DIESELPOWERCREATEPROC	DieselPower::FactoryFunction;	//	@A5E868;
	char*					DieselPower::ErrorMessageBuffer;	//	@A5E700
	bool					DieselPower::InstanceAcquired;	//	@A5E84C
	unsigned int			DieselPower::VersionNumberMajor;	//	@A5E850
	unsigned int			DieselPower::VersionNumberMinor;	//	@A5E854
	unsigned int			DieselPower::VersionNumberBuild;	//	@A5E858
	unsigned int			DieselPower::VersionNumberRequiredMajor;	//	@A5E85C
	unsigned int			DieselPower::VersionNumberRequiredMinor;	//	@A5E860
	unsigned int			DieselPower::VersionNumberRequiredBuild;	//	@A5E864

	bool DieselPower::CheckDllVersion()
	{
		//	NOTE: below is a recreation of what was a pretty good compiler optimization work.
		sscanf_s(DetectedVersion, "v.%d.%d.%d.", &VersionNumberMajor, &VersionNumberMinor, &VersionNumberBuild);
		sscanf_s(AUDIO_DIESELPO_DLL_SUPPORTED_VERSION, "v.%d.%d.%d.", &VersionNumberRequiredMajor, &VersionNumberRequiredMinor, &VersionNumberRequiredBuild);

		return VersionNumberMajor == VersionNumberRequiredMajor ? VersionNumberMinor == VersionNumberRequiredMinor : false;
	}

#pragma message(TODO_IMPLEMENTATION)
	DieselPower::DieselPower()
	{
		MESSAGE_CLASS_CREATED(DieselPower);
	}

	DieselPower::~DieselPower()
	{
		if (!LibraryHandle)
		{
			ErrorCode = ERROR_DLL_NOT_LOADED;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[11];

			return;
		}

		if (!this || g_DieselPower != this)
		{
			ErrorCode = ERROR_RELEASE_POINTERS_MISMATCH;

			if (this)
				ErrorMessageBuffer = (char*)DieselPowerErrorStrings[7];
			else
			{
				if (!FactoryFunction || g_DieselPower)
				{
					ErrorMessageBuffer = (char*)DieselPowerErrorStrings[8];
					return;
				}

				FactoryFunction = nullptr;
				InstanceAcquired = false;
				if (!FreeLibrary(LibraryHandle))
				{
					ErrorCode = ERROR_PROBLEM_RELEASING_DLL;
					ErrorMessageBuffer = (char*)DieselPowerErrorStrings[10];
				}
			}

			return;
		}

		Sleep(1000);

		delete g_DieselPower;
		g_DieselPower = nullptr;

		if (!LibraryHandle || !FreeLibrary(LibraryHandle))
		{
			LibraryHandle = NULL;
			InstanceAcquired = false;
			ErrorCode = ERROR_SUCCESSFULLY_ACQUIRED;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[9];
		}else{
			ErrorCode = ERROR_PROBLEM_RELEASING_INSTANCE;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[10];
		}

		MESSAGE_CLASS_DESTROYED(DieselPower);
	}

	DieselPower* DieselPower::CallFactory(unsigned int versionMajor, unsigned int versionMinor, unsigned int versionBuild, float unk, HWND windowHandle, int unk1, int unk2, int unk3)
	{
		if (LibraryHandle)
		{
			if (!FactoryFunction)
				FactoryFunction = (DIESELPOWERCREATEPROC)GetProcAddress(LibraryHandle, MAKEINTRESOURCE(1));	//	NOTE: DieselPower dll doesn't actually export function name, just ordinal number.
			
			if (!FactoryFunction)
			{
				ErrorCode = ERROR_CANT_ACQUIRE_FACTORY;
				ErrorMessageBuffer = (char*)DieselPowerErrorStrings[6];

				FreeLibrary(LibraryHandle);
				LibraryHandle = NULL;

				return NULL;
			}

			if (g_DieselPower)
				return g_DieselPower;
		}else{
			LibraryHandle = LoadLibrary(AUDIO_DIESELPO_DLL_NAME);
			if (!LibraryHandle)
			{
				memcpy((void*)DetectedVersion, "v.0.0.0.", 7);
				CheckDllVersion();
				ErrorCode = ERROR_DLL_CANT_BE_FOUND;
				ErrorMessageBuffer = (char*)DieselPowerErrorStrings[0];

				LibraryHandle = NULL;

				return NULL;
			}
		}

		if (!FactoryFunction)
		{
			FactoryFunction = (DIESELPOWERCREATEPROC)GetProcAddress(LibraryHandle, MAKEINTRESOURCE(1));
			if (!FactoryFunction)
			{
				ErrorCode = ERROR_CANT_ACQUIRE_FACTORY;
				ErrorMessageBuffer = (char*)DieselPowerErrorStrings[6];

				FreeLibrary(LibraryHandle);
				LibraryHandle = NULL;

				return NULL;
			}
		}

		if (!versionMajor && !versionMinor && !versionBuild && !unk && !windowHandle && !unk1 && !unk2 && !unk3 ||
			!InstanceAcquired)
		{
			g_DieselPower = (DieselPower*)FactoryFunction(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, DetectedVersion);
			InstanceAcquired = true;

			if (!versionMajor && !versionMinor && !versionBuild && unk == 0.f && !windowHandle && !unk1 && !unk2 && !unk3)
			{
				ErrorCode = ERROR_SUCCESSFULLY_ACQUIRED;
				ErrorMessageBuffer = (char*)DieselPowerErrorStrings[5];

				return NULL;
			}
		}

		if (CheckDllVersion())
		{
			g_DieselPower = (DieselPower*)FactoryFunction(versionMajor, versionMinor, versionBuild, unk, windowHandle, unk1, unk2, NULL, NULL);
			
			ErrorCode = ERROR_SUCCESSFULLY_ACQUIRED;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[1];

			return g_DieselPower;
		}

		if (VersionNumberMajor != VersionNumberRequiredMajor)
		{
			ErrorCode = ERROR_VERSIONS_MISMATCH;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[4];

			return NULL;
		}

		if (VersionNumberMinor == VersionNumberRequiredMinor)
		{
			ErrorCode = ERROR_VERSIONS_MISMATCH;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[4];

			return NULL;
		}

		if (VersionNumberMinor > VersionNumberRequiredMinor)
		{
			ErrorCode = ERROR_VERSIONS_HIGHER;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[3];

			return NULL;
		}

		if (VersionNumberMinor >= VersionNumberRequiredMinor)
		{
			ErrorCode = ERROR_VERSIONS_MISMATCH;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[4];

			return NULL;
		}else{
			ErrorCode = ERROR_VERSION_LOWER;
			ErrorMessageBuffer = (char*)DieselPowerErrorStrings[2];

			return NULL;
		}
	}
}