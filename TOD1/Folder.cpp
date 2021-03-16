#include "Folder.h"
#include "File.h"

ScriptType_Entity*	tFolder;

void Folder_::GetResourcePathRelative(String& outPath, String resourceName, ResType::BlockTypeNumber blockType, const char* languageCode)
{
	char	fileExt[16] = {};
	char	fileDir[1024] = {};
	char	fileName[128] = {};

	File::ExtractFilePath(resourceName.m_szString, fileDir, fileName, fileExt);

	if (languageCode)
	{
		strcat(fileDir, languageCode);
		strcat(fileDir, "/");
	}else
		strcat(fileDir, "shared/");

	strcat(fileDir, fileName);

	if (languageCode)
	{
		strcat(fileDir, "_");
		strcat(fileDir, languageCode);
	}

	strcat(fileDir, ".");
	strcat(fileDir, blockType ? ResType::BlockTypeExtension[blockType] : ResType::BlockTypeExtension[ResType::BlockTypeNumber::MAIN]);

	outPath = fileDir;
}

Folder_::Folder_() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(Folder_);

	m_BlockId = 0;
	m_AssetBlockInfo = nullptr;
}

void Folder_::Register()
{
	tFolder = new ScriptType_Entity("Folder");
	tFolder->InheritFrom(tNode);
	tFolder->SetCreator((ScriptType_Entity::CREATOR)Create);

	//tFolder->RegisterProperty(tINTEGER, "block_id", )

	tFolder->_86E9B0();
}

Folder_* Folder_::Create()
{
	return new Folder_();
}