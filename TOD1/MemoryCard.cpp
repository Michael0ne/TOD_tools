#include "MemoryCard.h"
#include "File.h"
#include "ScriptDatabase.h"
#include "LogDump.h"

MemoryCard* MemoryCardInfo[SAVE_SLOT_LAST];

MemoryCard::MemoryCard(MemoryCardIndex index)
{
    MESSAGE_CLASS_CREATED(MemoryCard);

    m_SaveFolderPath = String();
    m_MemoryCardIndex = index;
    m_Formatted = true;
    field_16 = field_17 = NULL;
}

MemoryCard::~MemoryCard()
{
    MESSAGE_CLASS_DESTROYED(MemoryCard);
}

bool MemoryCard::IsFormatted() const
{
    if (!Script::SavePlatformPS2)
        return true;

    if (m_SaveFolderPath.m_Length)
    {
        if (m_Formatted)
        {
            if (FileBuffer::IsDirectoryValid(m_SaveFolderPath.m_Str))
            {
#ifdef INCLUDE_FIXES
                char temp[1024] = {};
                strcpy(temp, m_SaveFolderPath.m_Str);
                strcat(temp, "/Formatted.txt");

                if (FileBuffer::FindFileEverywhere(temp, 0))
#else
                String temp = m_SaveFolderPath;
                temp.Append("/Formatted.txt");

                if (FileBuffer::FindFileEverywhere(temp.m_Str))
#endif

                    return true;
            }
        }
    }
    else
        LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.");

    return false;
}

bool MemoryCard::FormatCard()
{
    if (!Script::SavePlatformPS2)
        return true;

    if (!m_SaveFolderPath.m_Length)
    {
        LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.");
        LogDump::LogA("Warning: Memory Card not found.");

        return false;
    }

    if (!m_Formatted || !FileBuffer::IsDirectoryValid(m_SaveFolderPath.m_Str))
    {
        LogDump::LogA("Warning: Memory Card not found.");

        return false;
    }

    Utils::DeleteAllFilesInFolder(m_SaveFolderPath.m_Str);
    FileBuffer::CreateNewDirectory(m_SaveFolderPath.m_Str); // NOTE: these above are not used outside this class, maybe just private static methods?

#ifdef INCLUDE_FIXES
    char temp[1024] = {};
    strcpy(temp, m_SaveFolderPath.m_Str);
    strcat(temp, "Formatted.txt");

    FileBuffer formattedFile(temp, 2, true);
#else
    String temp = m_SaveFolderPath;
    temp.Append("Formatted.txt");

    FileBuffer formattedFile(temp.m_Str, 2, true);
#endif

    return true;
}

bool MemoryCard::UnformatCard()
{
    if (Script::SavePlatformPS2)
    {
        if (!m_SaveFolderPath.m_Length)
        {
            LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
            return false;
        }

        if (!m_Formatted || !FileBuffer::IsDirectoryValid(m_SaveFolderPath.m_Str))
            return false;

        Utils::DeleteAllFilesInFolder(m_SaveFolderPath.m_Str);
        FileBuffer::CreateNewDirectory(m_SaveFolderPath.m_Str);
    }

    return true;
}

bool MemoryCard::IsSaveDirPresent(const char* const directory) const
{
    if (IsFormatted())
    {
#ifdef INCLUDE_FIXES
        char savePath[1024] = {};
        strcpy(savePath, m_SaveFolderPath.m_Str);
        strcat(savePath, "/");
        strcat(savePath, directory);

        return FileBuffer::IsDirectoryValid(savePath);
#else
        String savePath = m_SaveFolderPath;
        savePath.Append("/");
        savePath.Append(directory);

        return FileBuffer::IsDirectoryValid(savePath.m_Str);
#endif
    }

    LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
    return false;
}

bool MemoryCard::IsSaveFilePresent(const char* const directory, const char* const slot) const
{
    if (IsFormatted())
    {
        String tempStr;
        GetFullSaveFolderPath(tempStr, directory, slot);

        return FileBuffer::FindFileEverywhere(tempStr.m_Str, 0);
    }

    LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
    return false;
}

unsigned int MemoryCard::GetSavePointFileSize(const char* const directory, const char* const slotfilename) const
{
    if (IsFormatted())
    {
        String tempStr;
        FileBuffer savefile(GetFullSaveFolderPath(tempStr, directory, slotfilename).m_Str, 0x21, true);

        return savefile.GetSize();
    }

    LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
    return NULL;
}

String& MemoryCard::GetFullSaveFolderPath(String& outStr, const char* const directory, const char* const slot) const
{
    outStr = m_SaveFolderPath;
    outStr.Append(directory);
    outStr.Append("/");
    outStr.Append(slot);

    return outStr;
}

bool MemoryCard::DeleteSavePointFile(const char* const savedir, const char* const slotindstr)
{
    if (!IsFormatted())
    {
        LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
        return false;
    }

    String tempStr;
    if (FileBuffer::FindFileEverywhere(GetFullSaveFolderPath(tempStr, savedir, slotindstr).m_Str, 0))
        FileBuffer::FindDirectoryMappedFileAndDelete(tempStr.m_Str);
#ifdef INCLUDE_FIXES
    else
        return false;
#endif

    return true; // TODO: even if file was not found - return true. Bug or intent?
}

bool MemoryCard::IsSavePointFileExists(const char* const savedir, const char* const slotindstr) const
{
    if (!IsFormatted())
    {
        LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
        return false;
    }

    String tempStr;
    return FileBuffer::FindFileEverywhere(GetFullSaveFolderPath(tempStr, savedir, slotindstr).m_Str, 0);
}

bool MemoryCard::CreateSaveDirectory(const char* const savedir)
{
    if (!IsFormatted())
    {
        LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
        return false;
    }

    String savedirStr = m_SaveFolderPath;
    savedirStr.Append("/");
    savedirStr.Append(savedir);

    if (!FileBuffer::IsDirectoryValid(savedirStr.m_Str))
        FileBuffer::CreateNewDirectory(savedirStr.m_Str);

    return true;
}

const bool MemoryCard::IsSaveFolderAvailable() const
{
    if (!m_SaveFolderPath.Empty())
        return m_Formatted && FileBuffer::IsDirectoryValid(m_SaveFolderPath.m_Str);

    LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
    return false;
}