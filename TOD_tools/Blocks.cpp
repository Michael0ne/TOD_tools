#include "Blocks.h"
#include "Globals.h"

Blocks* g_Blocks = nullptr;
const char* Blocks::BlockTypeExtension[] = {
	".", "map", "submap", "mission", "cutscene", "playerdata", "main"
};

signed int Blocks::GetAllocatorType()
{
	if (!m_bLoad)
		return 0;

	if (m_nBlockType >= 0)
		return GetResourceBlockTypeNumber(m_nBlockType);

	return 0;
}

#pragma message(TODO_IMPLEMENTATION)
int Blocks::InsertTypeListItem(void* res)
{
	return (*(int(__thiscall*)(Blocks*, void*))0x877A90)(this, res);
}

#pragma message(TODO_IMPLEMENTATION)
void Blocks::GetInternalFileName(String& outName, const char* str)
{
	(*(void* (__thiscall*)(Blocks*, String*, const char*))0x8773A0)(this, &outName, str);
}

ResourceBlockTypeNumber Blocks::GetResourceBlockTypeNumber(BlockTypeNumber resourceBlockId)
{
	if (!resourceBlockId ||
		memcmp(BlockTypeExtension[resourceBlockId], "map", 4) ||
		memcmp(BlockTypeExtension[resourceBlockId], "submap", 7))
		return (ResourceBlockTypeNumber)(MAP | SUBMAP);

	if (!memcmp(BlockTypeExtension[resourceBlockId], "mission", 8))
		return RESTYPE_MISSION;

	if (!memcmp(BlockTypeExtension[resourceBlockId], "cutscene", 9))
		return RESTYPE_CUTSCENE;

	if (!memcmp(BlockTypeExtension[resourceBlockId], "playerdata", 11))
		return RESTYPE_PLAYERDATA;

	return RESTYPE_NONE;
}

#pragma message(TODO_IMPLEMENTATION)
Blocks::Blocks(bool loadBlocks)
{
	MESSAGE_CLASS_CREATED(Blocks);

	patch(0xA3D7C4, this, 4);
}

#pragma message(TODO_IMPLEMENTATION)
Blocks::~Blocks()
{
	MESSAGE_CLASS_DESTROYED(Blocks);
}

void Blocks::SetSceneName(const char* szSceneName)
{
	String sceneDir;
	Utils::ExtractFileDir(&sceneDir, szSceneName);

	if (sceneDir.m_nLength > 0 && sceneDir.m_szString[sceneDir.m_nLength - 1] != '/')
		sceneDir.Append("/");

	m_SceneNames.Add(&sceneDir);
}

int Blocks::GetFreeResourceTypeListItem(unsigned int index)
{
	if (index + 1 >= m_ResourceTypesList.m_nCurrIndex)
		return 0;

	Resource** restype = &m_ResourceTypesList.m_pElements[index + 1];
	unsigned int freeind = index + 1;

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

void Blocks::SetRegionId(signed int id)
{
	m_nRegionId = id;
}