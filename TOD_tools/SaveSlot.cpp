#include "SaveSlot.h"
#include "File.h"

SaveSlot::SaveSlot(SaveSlotIndex index)
{
	MESSAGE_CLASS_CREATED(SaveSlot);

	m_SaveFolderPath = String();
	m_MemoryCardIndex = index;
	m_Formatted = true;
	field_16 = field_17 = NULL;
}

SaveSlot::~SaveSlot()
{
	MESSAGE_CLASS_DESTROYED(SaveSlot);
}

bool SaveSlot::IsFormatted()
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

				if (Utils::FindFileEverywhere(temp))
					return true;
			}
		}
	}else
		debug("Warning: Emulation dir not set. All operations will be ignored.");

	return false;
}

bool SaveSlot::FormatCard()
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