#include "Blocks.h"
#include "Globals.h"
#include "File.h"
#include "LogDump.h"
#include "Performance.h"
#include "Progress.h"

Blocks* g_Blocks = nullptr;

const char* Blocks::BlockTypeExtension[] = {
	".", "map", "submap", "mission", "cutscene", "playerdata", "main"
};
bool		Blocks::ChecksumChecked;

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

void Blocks::GetFullResourcePath(String& outStr, const char* respath, const char* resext, ResType::PlatformId platform)
{
	char buff[1024] = {};

	switch (platform)
	{
	case ResType::PLATFORM_PC:
		strcpy(buff, "/data_pc");
		break;
	case ResType::PLATFORM_PS2:
		strcpy(buff, "/data_ps2");
		break;
	case ResType::PLATFORM_XBOX:
		strcpy(buff, "/data_xbox");
		break;
	}

	String respath_str;
	GetResourcePath(respath_str, respath);
	char res_dir[1024] = {};
	char res_name[128] = {};
	char res_ext[16] = {};
	File::ExtractFilePath(respath_str.m_szString, res_dir, res_name, res_ext);

	strcat(buff, res_dir + 5);
	strcat(buff, res_name);
	
	if (resext && strcmp(resext, ""))
	{
		strcat(buff, ".");
		strcat(buff, resext);
	}

	outStr = buff;
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

AssetHeaderStruct_t::AssetHeaderStruct_1::AssetHeaderStruct_1()
{
	field_20 = 0x13579BDF;
	field_24 = 0x2468ACE0;
	field_28 = 0xFDB97531;
	field_2C = 0x80000062;
	field_30 = 0x40000020;
	field_34 = 0x10000002;
	field_38 = 0x7FFFFFFF;
	field_3C = 0x3FFFFFFF;
	field_40 = 0xFFFFFFF;
	field_44 = 0x80000000;
	field_48 = 0xC0000000;
	field_4C = 0xF0000000;

	memset(m_OriginalKey, NULL, sizeof(m_OriginalKey));
}

void AssetHeaderStruct_t::AssetHeaderStruct_1::_401450(char* key, char* keydata)
{
	_4010C0(key);

	for (unsigned int i = 0; i < strlen(keydata); i++)
	{
		*key = keydata[i];
		_4011A0(key);
		keydata[i] = *key;
	}
}

void AssetHeaderStruct_t::AssetHeaderStruct_1::_4010C0(const char* key)
{
	strcpy(m_OriginalKey, key);

	field_20 = key[3] | ((key[2] | ((key[1] | (0x0000 | (key[0] << 8))) << 8)) << 8);
	if (!field_20)
		field_20 = 0x13579BDF;

	field_24 = key[7] | ((key[6] | ((key[5] | (key[4] << 8)) << 8)) << 8);
	if (!field_24)
		field_24 = 0x2468ACE0;

	field_28 = key[11] | ((key[10] | ((key[9] | (key[8] << 8)) << 8)) << 8);
	if (!field_28)
		field_28 = 0xFDB97531;
}

void AssetHeaderStruct_t::AssetHeaderStruct_1::_4011A0(char* key)
{
	unsigned int rounds = 2;
	unsigned char v3 = field_24 & 1,
		v7 = 0,
		v9 = 0,
		v12 = 0,
		v17 = 0,
		v19 = 0,
		v20 = 0,
		v21 = 0,
		v22 = 0,
		v23 = field_24 & 1,
		v24 = field_28 & 1,
		fld20 = field_20;

	do 
	{
		if (fld20 & 1)
		{
			v7 = field_44 | ((fld20 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v3 = v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
			else
			{
				v3 = v23 = 0;
				field_24 = field_3C & (field_24 >> 1);
			}
		}
		else
		{
			v7 = field_38 & (fld20 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v20 = (2 * v19) | v24 ^ v3;
		if (v7 & 1)
		{
			v9 = field_44 | ((v7 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
			else
			{
				v23 = 0;
				field_24 = field_3C & (field_24 >> 1);
			}
		}
		else
		{
			v9 = field_38 & (v7 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}
		
		v21 = (2 * v20) | v24 ^ v23;
		if (v9 & 1)
		{
			v12 = field_44 | ((v9 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
		}
		else
		{
			v12 = field_38 & (v9 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v22 = (2 * v21) | v24 & v23;
		if (v12 & 1)
		{
			fld20 = field_44 | ((v12 ^ field_2C) >> 1);
			if (field_24 & 1)
			{
				v23 = 1;
				field_24 = field_48 | ((field_24 ^ field_30) >> 1);
			}
			else
			{
				v23 = 0;
				field_24 = field_3C & (field_24 >> 1);
			}
		}
		else
		{
			fld20 = field_38 & (v12 >> 1);
			if (field_28 & 1)
			{
				v24 = 1;
				field_28 = field_4C | ((field_28 ^ field_34) >> 1);
			}
			else
			{
				v24 = 0;
				field_28 = field_40 & (field_28 >> 1);
			}
		}

		v19 = v17 = (2 * v22) | v24 ^ v23;
		v3 = v23;

	} while (--rounds);

	field_20 = fld20;
	*key ^= v17;
	if (v17 == *key)
		*key = v17;
}

#pragma message(TODO_IMPLEMENTATION)
void* Blocks::LoadResourceBlock(class File* file, void* resbufferptr, unsigned int* resdatasize, BlockTypeNumber resblockid)
{
	AssetHeaderStruct_t assetHeaderStruct;

	LogDump::LogA("Loading resource block with ID=%i...\n", resblockid);
	unsigned int timeStart = Performance::GetMilliseconds();

	if (resblockid == 1)
	{
		char keydatabuf[256] = {};

		for (unsigned int i = 0; i < 36; i++)
			assetHeaderStruct.m_AssetId[i] = file->ReadBlock();
		for (unsigned int i = 0; i < 255; i++)
			keydatabuf[i] = file->ReadBlock();

		//	NOTE: roate bits?
		assetHeaderStruct.field_38._401450("1E564E3B-D243-4ec5-AFB7", keydatabuf);
		strcpy(field_8, keydatabuf);

		int buf_ = NULL;
		file->Read(&buf_, sizeof(buf_));
		file->Read(&m_ResourcesInBlock, sizeof(m_ResourcesInBlock));
	}

	unsigned int assettimestamp = NULL;
	file->Read(&assettimestamp, sizeof(assettimestamp));

	if (m_CheckTimestamp && assettimestamp != m_EngineVersionTimestamp)
	{
		LogDump::LogA("Time stamp of engine changed. Ignoring asset block.\n");
		return nullptr;
	}

	if (ChecksumChecked)
	{
		unsigned int dummy = NULL;
		file->Read(&dummy, sizeof(dummy));
		file->Read(&dummy, sizeof(dummy));
	}
	else
	{
		unsigned int checksum = NULL;
		file->Read(&checksum, sizeof(checksum));

		if (GetGlobalPropertiesListCRC() != checksum)
			return nullptr;

		file->Read(&checksum, sizeof(checksum));
		if (GetGlobalCommandsListCRC() != checksum)
			return nullptr;

		ChecksumChecked = true;
	}

	unsigned int	totalResources = NULL;
	unsigned int	resourceElementSize = NULL;
	unsigned int	resourceBufferSize = NULL;
	int*			resourceBuffer = nullptr;
	int*			resourceDataBuffer = nullptr;

	file->Read(&totalResources, sizeof(totalResources));
	file->Read(&resourceElementSize, sizeof(resourceElementSize));
	file->Read(&resourceBufferSize, sizeof(resourceBufferSize));

	*resdatasize = resourceElementSize;
	AllocateResourceBlockBufferAligned(resourceElementSize, &resourceBuffer, (int*)resbufferptr, resblockid);
	resourceDataBuffer = (int*)Allocators::AllocateByType(RENDERLIST, resourceBufferSize);
	g_Progress->UpdateProgressTime(NULL, __rdtsc());

	file->Seek(~(1 - 1)& (1 + file->GetPosition() - 1));	//	NOTE: 417DF0 inlined.
	file->Read(&resourceBuffer, resourceElementSize);

	if (totalResources > 0)
		;
	//	TODO: complete!

	LogDump::LogA("Done. Loading %d resource took %.2f secs.\n", totalResources, (Performance::GetMilliseconds() - timeStart) * 0.001f);
	delete resourceDataBuffer;

	return resourceBuffer;
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

void Blocks::AllocateResourceBlockBufferAligned(unsigned int pos, int** resBufStartPos, int* resBufSpace, BlockTypeNumber resBlockId)
{
	*resBufSpace = (int)Allocators::AllocateByType(GetResourceBlockTypeNumber(resBlockId), pos + ResType::ResourceAlignment[0]);
	*resBufStartPos = (int*)(~(ResType::ResourceAlignment[0] - 1) & (*resBufSpace + ResType::ResourceAlignment[0] - 1));
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

	field_1C8 = NULL;
	field_1CC = NULL;
	field_1D0 = nullptr;
	m_CheckTimestamp = NULL;
	m_EngineVersionTimestamp = NULL;
	m_BlockType = UNKNOWN;
	m_ResourcesInBlock = 2;
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