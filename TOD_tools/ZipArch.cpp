#include "ZipArch.h"

String* ZipArch::ZipNames[8] = {
	(String*)0xA085A8,
	(String*)0xA085B8,
	(String*)0xA085C8,
	(String*)0xA085D8,
	(String*)0xA085E8,
	(String*)0xA085F8,
	(String*)0xA08608,
	(String*)0xA08618
};

ZipSlotInfo* ZipArch::_A08628[8] = {
	(ZipSlotInfo*)0xA08628,
	(ZipSlotInfo*)0xA08638,
	(ZipSlotInfo*)0xA08648,
	(ZipSlotInfo*)0xA08658,
	(ZipSlotInfo*)0xA08668,
	(ZipSlotInfo*)0xA08678,
	(ZipSlotInfo*)0xA08688,
	(ZipSlotInfo*)0xA08698
};

int& ZipArch::SlotId = *(int*)0xA35DDC;

/*
 *	@outPath - Pass NULL pointer if this info is not needed.
 *	@result - if found anything, function returns true.
*/
bool ZipArch::FindFile(const char* inPathStr, int* outPath, int* outZipSlot)
{
	//	TODO: implementation!
	return (*(bool (*)(const char*, int*, int*))0x41A500)(inPathStr, outPath, outZipSlot);
}

String* ZipSlotInfo::_4198F0(const char* hashStr)
{
	return (*(String * (__thiscall*)(ZipSlotInfo*, const char*))0x4198F0)(this, hashStr);
}