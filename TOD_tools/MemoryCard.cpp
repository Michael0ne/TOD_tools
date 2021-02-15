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

	if (m_SaveFolderPath.m_nLength) {
		if (m_Formatted) {
			if (Utils::IsDirectoryValid(m_SaveFolderPath.m_szString)) {
				//	NOTE: original function uses String for this, but it requires heap allocation, don't really need that for string appending.
				char temp[MAX_PATH];
				strcpy(temp, m_SaveFolderPath.m_szString);
				strcat(temp, "/Formatted.txt");

				if (File::FindFileEverywhere(temp))
					return true;
			}
		}
	}else
		debug("Warning: Emulation dir not set. All operations will be ignored.");

	return false;
}

bool MemoryCard::FormatCard()
{
	if (!Script::SavePlatformPS2)
		return true;

	if (!m_SaveFolderPath.m_nLength) {
		debug("Warning: Emulation dir not set. All operations will be ignored.");
		debug("Warning: Memory Card not found.");

		return false;
	}

	if (!m_Formatted || !Utils::IsDirectoryValid(m_SaveFolderPath.m_szString)) {
		debug("Warning: Memory Card not found.");

		return false;
	}

	Utils::DeleteAllFilesInFolder(m_SaveFolderPath.m_szString);
	Utils::CreateDirectoryIfNotFound(m_SaveFolderPath.m_szString);	//	NOTE: these above are not used outside this class, maybe just private static methods?

	char temp[MAX_PATH];
	strcpy(temp, m_SaveFolderPath.m_szString);
	strcat(temp, "Formatted.txt");

	File formattedFile(temp, 2, true);

	return true;
}

bool MemoryCard::IsSaveDirPresent(const char* const directory) const
{
	if (IsFormatted())
	{
		String savePath = m_SaveFolderPath;
		savePath.Append("/");
		savePath.Append(directory);

		return Utils::IsDirectoryValid(savePath.m_szString);
	}
	else
	{
		LogDump::LogA("Warning: Memory Card not found or not formatted.\n");

		return false;
	}
}

bool MemoryCard::IsSaveFilePresent(const char* const directory, const char* const slot) const
{
	if (IsFormatted())
	{
		String tempStr;
		return File::FindFileEverywhere(GetFullSaveFolderPath(tempStr, directory, slot).m_szString);
	}
	else
	{
		LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
		
		return false;
	}
}

unsigned int MemoryCard::GetSavePointFileSize(const char* const directory, const char* const slotfilename) const
{
	if (IsFormatted())
	{
		String tempStr;
		File savefile(GetFullSaveFolderPath(tempStr, directory, slotfilename).m_szString, 0x21, true);
		
		return savefile.GetSize();
	}
	else
	{
		LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
		return NULL;
	}
}

String& MemoryCard::GetFullSaveFolderPath(String& outStr, const char* const directory, const char* const slot) const
{
	outStr = m_SaveFolderPath;
	outStr.Append(directory);
	outStr.Append("/");
	outStr.Append(slot);

	return outStr;
}