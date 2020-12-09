#include "Blocks.h"
#include "Globals.h"

Blocks* g_Blocks = NULL;

const char* Blocks::g_szBlockTypes[7] = //	@A11B64
{
	(const char*)0xC,
	"map",
	"submap",
	"mission",
	"cutscene",
	"playerdata",
	"",
};

#pragma message(TODO_IMPLEMENTATION)
Blocks::Blocks(bool loadBlocks)
{
	MESSAGE_CLASS_CREATED(Blocks);

	patch(0xA3D7C4, this, 4);
}

void Blocks::SetSceneName(const char* szSceneName)
{
	String sceneDir;
	Utils::ExtractFileDir(&sceneDir, szSceneName);

	if (sceneDir.m_nLength > 0 && sceneDir.m_szString[sceneDir.m_nLength - 1] != '/')
		sceneDir.Append("/");

	m_SceneNames.Add(&sceneDir);
}

int Blocks::GetFreeResourceTypeListItem(int index)
{
	if (index + 1 >= m_ResourceTypesList.m_nCurrIndex)
		return 0;

	TypeInfo** restype = &m_ResourceTypesList.m_pElements[index + 1];
	int freeind = index + 1;

	while (!*restype) {
		restype++;
		freeind++;

		if (freeind >= m_ResourceTypesList.m_nCurrIndex)
			return 0;
	}

	return freeind;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned int Blocks::AddEntity(Entity* ent)
{
	return (*(unsigned int(__thiscall*)(Blocks*, Entity*))0x875FA0)(this, ent);
}