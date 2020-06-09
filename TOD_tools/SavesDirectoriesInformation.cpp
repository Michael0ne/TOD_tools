#include "SavesDirectoriesInformation.h"
#include "Config.h"

SavesDirectoriesInformation* g_SavesDirsInfo[3] = {
	nullptr, nullptr, nullptr
};

void SavesDirectoriesInformation::Init(unsigned int index)
{
	if (index == 8)
		//	PC.
		patch(0xA35EA4, this, 4);
	else
		//	PS2 emulation.
		patch(0xA35E84 + index, this, 4);

	lpVtbl = (SavesDirectoriesInformation::__vtable*)0x9D79D4;
	field_4 = index;
	m_bFormatted = true;
}

bool SavesDirectoriesInformation::IsFormatted()
{
	if (!Script::SavePlatformPS2)
		return true;

	if (m_sSaveFolderPath.m_nLength) {
		if (m_bFormatted) {
			if (Utils::IsDirectoryValid(m_sSaveFolderPath.m_szString)) {
				//	NOTE: original function uses String for this, but it requires heap allocation, don't really need that for string appending.
				char temp[MAX_PATH];
				strcpy(temp, m_sSaveFolderPath.m_szString);
				strcat(temp, "/Formatted.txt");

				if (Utils::FindFileEverywhere(temp))
					return true;
			}
		}
	}else
		debug("Warning: Emulation dir not set. All operations will be ignored.");

	return false;
}

bool SavesDirectoriesInformation::FormatCard()
{
	if (!Script::SavePlatformPS2)
		return true;

	if (!m_sSaveFolderPath.m_nLength) {
		debug("Warning: Emulation dir not set. All operations will be ignored.");
		debug("Warning: Memory Card not found.");

		return false;
	}

	if (!m_bFormatted || !Utils::IsDirectoryValid(m_sSaveFolderPath.m_szString)) {
		debug("Warning: Memory Card not found.");

		return false;
	}

	Utils::DeleteAllFilesInFolder(m_sSaveFolderPath.m_szString);
	Utils::CreateDirectoryIfNotFound(m_sSaveFolderPath.m_szString);	//	NOTE: these above are not used outside this class, maybe just private static methods?

	char temp[MAX_PATH];
	strcpy(temp, m_sSaveFolderPath.m_szString);
	strcat(temp, "Formatted.txt");

	FileInternal formattedFile;
	formattedFile.Open(temp, 2, true);
	formattedFile.Close();

	return true;
}
