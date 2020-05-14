#include "DieselPowerSound.h"

const char* Audio::DieselPower::RequiredVersion = (const char*)0xA1BA50;
char* Audio::DieselPower::DetectedVersion = (char*)0xA5E800;
HMODULE& Audio::DieselPower::LibraryHandle = *(HMODULE*)0xA5E840;
int& Audio::DieselPower::ErrorCode = *(int*)0xA5E844;
Audio::DIESELPOWERCREATEPROC Audio::DieselPower::FactoryFunction = (Audio::DIESELPOWERCREATEPROC)0xA5E868;
char* Audio::DieselPower::ErrorMessageBuffer = (char*)0xA5E700;
bool& Audio::DieselPower::Created = *(bool*)0xA5E84C;