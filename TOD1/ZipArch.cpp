#include "ZipArch.h"
#include "Globals.h"

String ZipArch::ZipNames[ZIP_MAX_SLOTS] = {};
std::map<unsigned int, ZipArch::FileInfo> ZipArch::SlotInfo[ZIP_MAX_SLOTS];
unsigned int ZipArch::SlotId = NULL;

bool ZipArch::FindFile(const char* inPathStr, FileInfo& outFileInfo, int* outZipSlotIndex)
{
    String::ToLowerCase((char*)(inPathStr + 1));
    const unsigned int checksum = Utils::CalcCRC32(inPathStr + 1, strlen(inPathStr) - 1);

    if (SlotId <= NULL)
        return false;

    for (unsigned int i = 0; i < ZIP_MAX_SLOTS; i++)
    {
        const std::map<unsigned int, FileInfo>::iterator& finf = SlotInfo[i].find(checksum);

        if (finf != SlotInfo[i].end() && finf->second.m_FileSize != 0 && finf->second.m_OffsetInArch != 0)
        {
            outFileInfo = finf->second;
            *outZipSlotIndex = i;

            return true;
        }
    }

    return true;
}