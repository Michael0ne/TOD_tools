#include "ZipArch.h"
#include "Globals.h"

String ZipArch::ZipNames[] = {
	String(), String(), String(), String(), String(), String(), String(), String()
};
ZipSlotInfo ZipArch::SlotInfo[] = {
	ZipSlotInfo(), ZipSlotInfo(), ZipSlotInfo(), ZipSlotInfo(), ZipSlotInfo(), ZipSlotInfo(), ZipSlotInfo(), ZipSlotInfo()
};
unsigned int ZipArch::SlotId = 0;

ZipSlotInfo::ZipSlotInfo()
{
	MESSAGE_CLASS_CREATED(ZipSlotInfo);

	*(int*)&field_4 = 0x4F | 0x10000;
}

ZipSlotInfo::~ZipSlotInfo()
{
	MESSAGE_CLASS_DESTROYED(ZipSlotInfo);

	delete field_8;
	delete field_C;
}

bool ZipArch::FindFile(const char* inPathStr, int* outPath, int* outZipSlot)
{
	char* path = (char*)inPathStr;
	Utils::ToLowercase(path);
	const unsigned int hash = Utils::CalcCRC32(path, strlen(path));

	if (SlotId <= 0)
		return false;

	int slotInfo[2] = { NULL, NULL };
	unsigned int _slotindex = NULL;

	for (int ind = 0; ind < 8; ind++)
	{
		int* _slotinfo = SlotInfo[ind]._4198F0(&hash);
		if (_slotinfo)
		{
			slotInfo[0] = *_slotinfo;
			slotInfo[1] = *(slotInfo + 1);
			break;
		}
		if (++_slotindex >= SlotId)
			return false;
	}

	if (outPath)
	{
		*(outPath++) = slotInfo[0];
		*outPath = slotInfo[1];
	}

	*outZipSlot = +_slotindex;

	return false;
}

int* ZipSlotInfo::_4198F0(const unsigned int* hashStr)
{
	//	NOTE: this is reconstructed assembly code.
	int v2 = NULL, v3 = NULL, v4 = NULL;

	if (field_0)
	{
		do 
		{
			v4 = v3 + (v2 - v3) / 2;
			if (field_8[v4] < *hashStr)
				v3 = v4 + 1;
			else
				v2 = v3 + (v2 - v3) / 2;
		} while (v3 != v2);
	}

	if (v2 >= field_0 ||
		*hashStr < field_8[v2])
		return nullptr;
	else
		return (int*)&field_C[v2];
}

void ZipSlotInfo::_41A5F0(unsigned int* a1)
{
	//	NOTE: this is reconstructed assembly code.
	if (field_4._2 & 1)
	{
		delete field_8;
		delete field_C;
	}

	field_0 = a1[1] & 0x7FFFFF;
	field_8 = new unsigned int[4 * field_0];
	field_C = new unsigned int[8 * field_0];
	*(int*)&field_4 |= 0x10000;

	int* v1 = (int*)a1[2];

	if (v1)
		for (int* i = (int*)v1[3]; i; i = (int*)i[3])
			v1 = i;

	if (v1)
	{
		int v2 = 0, v3 = 0;

		do
		{
			if (&field_8[v3])
				field_8[v3] = *v1;
			if (&field_C[v2])
			{
				field_C[v2] = v1[1];
				field_C[v2 + 1] = v1[2];
			}

			++v3;
			v2 += 2;

			int* v4 = (int*)v1[4];
			int* v5 = v1;

			if (v4)
			{
				for (int* j = (int*)v4[3]; j; j = (int*)j[3])
					v4 = j;
				v1 = v4;
			}else{
				v1 = (int*)(v1[5] * 4);
				if (!v1)
					return;
				do 
				{
					if ((int*)v1[4] != v5)
						break;
					v5 = v1;
					v1 = (int*)(v1[5] * 4);
				} while (v1);
			}
		} while (v1);
	}
}