#pragma once

#include "Types.h"
#include "StringsPool.h"

#define ZIPSLOT_STRUCT_SIZE 16

struct ZipSlotInfo_fldC
{
	int		field_0;
	int		field_4;
};

struct ZipSlotInfo
{
public:
	int					field_0;
	union {
		unsigned char flag0 : 1;
		unsigned char flag1 : 1;
		unsigned char flag2 : 1;
		unsigned char flag3 : 1;
	} field_4;
	int*				field_8;
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

static_assert(sizeof(ZipSlotInfo) == ZIPSLOT_STRUCT_SIZE, MESSAGE_WRONG_CLASS_SIZE(ZipSlotInfo));