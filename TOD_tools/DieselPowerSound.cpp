#include "DieselPowerSound.h"

namespace Audio
{
	//	TODO: implementation!
	DieselPower::DieselPower()
	{
		MESSAGE_CLASS_CREATED(DieselPower);
	}

	//	TODO: implementation!
	DieselPower::~DieselPower()
	{
		MESSAGE_CLASS_DESTROYED(DieselPower);
	}

	//	TODO: implementation!
	DieselPower* DieselPower::CallFactory(unsigned int versionMajor, unsigned int versionMinor, unsigned int versionBuild, float unk, HWND windowHandle, int unk1, int unk2, int unk3)
	{
		return nullptr;
	}

	const char* DieselPower::RequiredVersion = (const char*)0xA1BA50;
	char*					DieselPower::DetectedVersion = (char*)0xA5E800;
	HMODULE&				DieselPower::LibraryHandle = *(HMODULE*)0xA5E840;
	int&					DieselPower::ErrorCode = *(int*)0xA5E844;
	DIESELPOWERCREATEPROC	DieselPower::FactoryFunction = (DIESELPOWERCREATEPROC)0xA5E868;
	char*					DieselPower::ErrorMessageBuffer = (char*)0xA5E700;
	bool&					DieselPower::Created = *(bool*)0xA5E84C;
}