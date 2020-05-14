#pragma once

#include "Types.h"
#include "StringsPool.h"

struct ZipSlotInfo_fldC
{
	int		field_0;	//	String pointer originally.
	int		field_4;
};

struct ZipSlotInfo
{
private:
	int		field_0;
	int		field_4;
	int*	field_8;
	ZipSlotInfo_fldC*	field_C;
public:
	String*				_4198F0(const char* hashStr);			//	@4198F0
};

class ZipArch
{
public:
	static bool			FindFile(const char* inPathStr, String* outPath, int* outZipSlot);		//	@41A500
	
	static String*		ZipNames[8];		//	@A085A8
	static ZipSlotInfo* _A08628[8];			//	@A08628
	static int&			SlotId;				//	@A35DDC
};