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

	if (m_SaveFolderPath.m_nLength)
	{
		if (m_Formatted)
		{
			if (File::IsDirectoryValid(m_SaveFolderPath.m_szString))
			{
#ifdef INCLUDE_FIXES
				char temp[1024] = {};
				strcpy(temp, m_SaveFolderPath.m_szString);
				strcat(temp, "/Formatted.txt");

				if (File::FindFileEverywhere(temp))
#else
				String temp = m_SaveFolderPath;
				temp.Append("/Formatted.txt");
				
				if (File::FindFileEverywhere(temp.m_szString))
#endif

					return true;
			}
		}
	}else
		LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.");

	return false;
}

bool MemoryCard::FormatCard()
{
	if (!Script::SavePlatformPS2)
		return true;

	if (!m_SaveFolderPath.m_nLength)
	{
		LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.");
		LogDump::LogA("Warning: Memory Card not found.");

		return false;
	}

	if (!m_Formatted || !File::IsDirectoryValid(m_SaveFolderPath.m_szString))
	{
		LogDump::LogA("Warning: Memory Card not found.");

		return false;
	}

	Utils::DeleteAllFilesInFolder(m_SaveFolderPath.m_szString);
	File::CreateNewDirectory(m_SaveFolderPath.m_szString);	//	NOTE: these above are not used outside this class, maybe just private static methods?

#ifdef INCLUDE_FIXES
	char temp[1024] = {};
	strcpy(temp, m_SaveFolderPath.m_szString);
	strcat(temp, "Formatted.txt");

	File formattedFile(temp, 2, true);
#else
	String temp = m_SaveFolderPath;
	temp.Append("Formatted.txt");
	
	File formattedFile(temp.m_szString, 2, true);
#endif

	return true;
}

bool MemoryCard::UnformatCard()
{
	if (Script::SavePlatformPS2)
	{
		if (!m_SaveFolderPath.m_nLength)
		{
			LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
			return false;
		}

		if (!m_Formatted || !File::IsDirectoryValid(m_SaveFolderPath.m_szString))
			return false;

		Utils::DeleteAllFilesInFolder(m_SaveFolderPath.m_szString);
		File::CreateNewDirectory(m_SaveFolderPath.m_szString);
	}

	return true;
}

bool MemoryCard::IsSaveDirPresent(const char* const directory) const
{
	if (IsFormatted())
	{
#ifdef INCLUDE_FIXES
		char savePath[1024] = {};
		strcpy(savePath, m_SaveFolderPath.m_szString);
		strcat(savePath, "/");
		strcat(savePath, directory);

		return File::IsDirectoryValid(savePath);
#else
		String savePath = m_SaveFolderPath;
		savePath.Append("/");
		savePath.Append(directory);
		
		return File::IsDirectoryValid(savePath.m_szString);
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

		return File::FindFileEverywhere(tempStr.m_szString);
	}
	
	LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
	return false;
}

unsigned int MemoryCard::GetSavePointFileSize(const char* const directory, const char* const slotfilename) const
{
	if (IsFormatted())
	{
		String tempStr;
		File savefile(GetFullSaveFolderPath(tempStr, directory, slotfilename).m_szString, 0x21, true);
		
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
	if (File::FindFileEverywhere(GetFullSaveFolderPath(tempStr, savedir, slotindstr).m_szString))
		File::FindDirectoryMappedFileAndDelete(tempStr.m_szString);
#ifdef INCLUDE_FIXES
	else
		return false;
#endif

	return true;	//	TODO: even if file was not found - return true. Bug or intent?
}

bool MemoryCard::IsSavePointFileExists(const char* const savedir, const char* const slotindstr) const
{
	if (!IsFormatted())
	{
		LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
		return false;
	}

	String tempStr;
	return File::FindFileEverywhere(GetFullSaveFolderPath(tempStr, savedir, slotindstr).m_szString);
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

	if (!File::IsDirectoryValid(savedirStr.m_szString))
		File::CreateNewDirectory(savedirStr.m_szString);

	return true;
}