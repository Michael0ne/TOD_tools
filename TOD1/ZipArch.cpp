#include "ZipArch.h"
#include "Globals.h"

#include <string>

String ZipArch::ZipNames[ZIP_MAX_SLOTS] = {};
std::map<unsigned int, ZipArch::FileInfo> ZipArch::SlotInfo[ZIP_MAX_SLOTS];
unsigned int ZipArch::SlotId = 0;

bool ZipArch::FindFile(const char* inPathStr, FileInfo& outFileInfo, int* outZipSlotIndex)
{
    const std::string lowerCasePath(inPathStr);

    const auto checksum = Utils::CalcCRC32(lowerCasePath.c_str(), lowerCasePath.length());
    //String::ToLowerCase((char*)(inPathStr + 1));
    //const unsigned int checksum = Utils::CalcCRC32(inPathStr + 1, strlen(inPathStr) - 1);

    if (SlotId <= 0)
        return false;

    for (uint32_t i = 0; i < ZIP_MAX_SLOTS; i++)
    {
        const auto slotData = SlotInfo[i].find(checksum);

        if (slotData == SlotInfo[i].end())
            continue;

        if (slotData->second.m_FileSize != 0 &&
            slotData->second.m_OffsetInArch != 0)
        {
            outFileInfo = slotData->second;
            *outZipSlotIndex = i;

            return true;
        }
    }

    return true;
}