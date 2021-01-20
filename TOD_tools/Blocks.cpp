#include "Blocks.h"
#include "Globals.h"
#include "File.h"

Blocks* g_Blocks = nullptr;
const char* Blocks::BlockTypeExtension[] = {
	".", "map", "submap", "mission", "cutscene", "playerdata", "main"
};

void Blocks::GetResourcePath(String& outStr, const char* path) const
{
	if (strstr(path, "/data") == nullptr)
	{
		if (strstr(path, "data/") == nullptr)
		{
			if (*path == '/')
			{
				outStr = "/data";
				outStr.Append(path);

				return;
			}
			else
			{
				outStr = GetCurrentSceneName();
				outStr.Append(path);

				return;
			}
		}
		else
		{
			outStr = "data/";
			outStr.Append(path);

			return;
		}
	}
}

void Blocks::IncreaseResourceReferenceCount(ResType::Resource* _res)
{
	++_res->field_18;
}

void Blocks::DecreaseResourceReferenceCount(ResType::Resource* _res)
{
	--_res->field_18;
}

const char* Blocks::GetCurrentSceneName() const
{
	return m_SceneNames.m_CurrIndex ? m_SceneNames.m_Elements[m_SceneNames.m_CurrIndex]->m_szString : nullptr;
}

AllocatorIndex Blocks::GetAllocatorType() const
{
	if (!m_LoadBlocks)
		return DEFAULT;

	if (m_BlockType >= NONE)
		return (AllocatorIndex)GetResourceBlockTypeNumber(m_BlockType);

	return DEFAULT;
}

int Blocks::InsertTypeListItem(void* res)
{
	field_0 = 1;

	if (m_ResourceTypesList.m_CurrIndex <= 1)
	{
		unsigned int _ind = m_ResourceTypesList.m_CurrIndex;
		AddTypesListItemAtPos((ResType::Resource*)res, _ind);

		return _ind;
	}

	unsigned int ind = 0;
	for (unsigned int i = 1; i < m_ResourceTypesList.m_CurrIndex; i++)
		if (!m_ResourceTypesList.m_Elements[i])
			ind = i;

	if (!ind)
		ind = m_ResourceTypesList.m_CurrIndex;

	AddTypesListItemAtPos((ResType::Resource*)res, ind);

	return ind;
}

#pragma message(TODO_IMPLEMENTATION)
void Blocks::GetInternalFileName(String& outName, const char* str)
{
	char* _scenename = NULL;

	if (m_SceneNames.m_CurrIndex)
		_scenename = m_SceneNames.m_Elements[m_SceneNames.m_CurrIndex - 1]->m_szString;

	if (String::EqualIgnoreCase(str, _scenename, strlen(_scenename)))
	{
		String _str(str);
		_str.Substring(&outName, strlen(_scenename), _str.m_nLength - strlen(_scenename));

		return;
	}

	const char* dataPath = "/data/";
	unsigned char readBytes = NULL;
	while (true)
	{
		if (!dataPath[str - "/data/"])
			break;
		if (!*dataPath ||
			((dataPath[str - "/data/"] ^ *dataPath) & 223) != NULL)
		{
			outName = str;

			return;
		}
		if (readBytes++ == 6)
		{
			String _str(str);
			_str.Substring(&outName, 5, 0x7FFFFFFE);

			return;
		}
		dataPath++;
	}
	if (!*dataPath)
	{
		String _str(str);
		_str.Substring(&outName, 5, 0x7FFFFFFE);

		return;
	}
	outName = str;

	return;
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

void Blocks::AddTypesListItemAtPos(ResType::Resource* element, unsigned int index)
{
	if (m_ResourceTypesList.m_CurrIndex < index + 1)
	{
		for (int ind = m_ResourceTypesList.m_CurrIndex; m_ResourceTypesList.m_CurrIndex < index + 1; ind++)
			m_ResourceTypesList.AddElement(nullptr);
	}

	m_ResourceTypesList.m_Elements[index] = element;
	field_0 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
Blocks::Blocks(bool loadBlocks)
{
	MESSAGE_CLASS_CREATED(Blocks);

	field_10C = (*(Allocator * (*)(AllocatorIndex))0x4777F0)(DEFRAGMENTING);

	m_UnkList_1 = List<int>(0x18B00, 9);

	const unsigned int listCapacity[] =
	{
		NULL, 11200, 4100, 6000, 2800, NULL, 100
	};

	for (unsigned int i = 1; i < 6; i++)
	{
		m_UnkList_2[i] = List<int>(0x27B00);
		m_UnkList_2[i].m_Flags |= 0x1A000;
		
		if (listCapacity[i])
			m_UnkList_2[i].SetCapacityAndErase(listCapacity[i]);
	}

	m_ResourceTypesList = List<ResType::Resource>(0x18B00);
	m_SceneNames = List<String>(0x18B00);
	m_UnkList_9 = List<int>(0x18B00);

	m_LoadBlocks = loadBlocks;

	g_Blocks = this;

	field_0 = NULL;
	m_RegionId = -1;

	field_1B0[0] = field_1B0[1] = field_1B0[2] = field_1B0[3] = field_1B0[4] = field_1B0[5] = 1;

	m_ResourceTypesList.AddElement(nullptr);

	field_1C8 = field_1CC = field_1D0 = field_1DC = m_EngineVersionTimestamp = NULL;
	m_BlockType = UNKNOWN;
	field_108 = 2;
}

#pragma message(TODO_IMPLEMENTATION)
Blocks::~Blocks()
{
	MESSAGE_CLASS_DESTROYED(Blocks);
}

void Blocks::SetSceneName(const char* szSceneName)
{
	String sceneDir;
	File::ExtractFileDir(sceneDir, szSceneName);

	if (sceneDir.m_nLength > 0 && sceneDir.m_szString[sceneDir.m_nLength - 1] != '/')
		sceneDir.Append("/");

	m_SceneNames.AddElement(&sceneDir);
}

int Blocks::GetFreeResourceTypeListItem(unsigned int index)
{
	if (index + 1 >= m_ResourceTypesList.m_CurrIndex)
		return 0;

	ResType::Resource** restype = &m_ResourceTypesList.m_Elements[index + 1];
	unsigned int freeind = index + 1;

	while (!*restype) {
		restype++;
		freeind++;

		if (freeind >= m_ResourceTypesList.m_CurrIndex)
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
	m_RegionId = id;
}