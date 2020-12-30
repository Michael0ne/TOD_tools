#include "Folder.h"
#include "File.h"

void Folder_::GetResourcePathRelative(String& outPath, String resourceName, BlockTypeNumber blockType, const char* languageCode)
{
	char fileExt[8], fileDir[1024], fileName[128];
	memset(&fileExt, NULL, sizeof(fileExt));
	memset(&fileDir, NULL, sizeof(fileDir));
	memset(&fileName, NULL, sizeof(fileName));

	File::ExtractFilePath(resourceName.m_szString, fileDir, fileName, fileExt);

	if (languageCode)
	{
		strcat(fileDir, "/");
		strcat(fileDir, languageCode);
	}else
		strcat(fileDir, "shared/");

	strcat(fileDir, fileName);

	if (languageCode)
	{
		strcat(fileDir, "_");
		strcat(fileDir, languageCode);
	}

	strcat(fileDir, ".");
	strcat(fileDir, blockType ? Blocks::BlockTypeExtension[blockType] : Blocks::BlockTypeExtension[MAIN]);

	outPath.Set(fileDir);
}