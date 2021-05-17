#include "AssetManager.h"
#include "Globals.h"
#include "File.h"
#include "LogDump.h"
#include "Performance.h"
#include "Progress.h"
#include "ScriptDatabase.h"
#include "GfxInternal.h"
#include "Scene.h"

AssetManager* g_AssetManager;
bool AssetManager::ChecksumChecked;
bool AssetManager::_A3D7C0;

void AssetManager::CorrectTextureResourcePath(String& outPath, const char* respath, RegionCode region, Asset::PlatformId platform)
{
	const char* const pcplatformdir = strstr(respath, "pc_lores");
	if (!platform)
	{
		if (!pcplatformdir)
		{
			outPath = respath;
			return;
		}

		char pathfinal[128] = {};
;
		strncpy(pathfinal, respath, pcplatformdir - respath);
		strcat(pathfinal, "pc_lores");
		strcat(pathfinal, &respath[pcplatformdir - respath + 8]);

		outPath = pathfinal;
		return;
	}

	if (platform != Asset::PlatformId::PS2)
	{
		if (platform != Asset::PlatformId::XBOX)
		{
			outPath = respath;
			return;
		}

		if (!pcplatformdir)
		{
			outPath = respath;
			return;
		}

		char pathfinal[128] = {};

		strncpy(pathfinal, respath, pcplatformdir - respath);
		
		if (!region)
		{
			strcat(pathfinal, "xbox_ntsc");
			strcat(pathfinal, &respath[pcplatformdir - respath + 8]);

			outPath = pathfinal;
			return;
		}

		if (region != REGION_USA)
		{
			outPath = pathfinal;
			return;
		}

		strcat(pathfinal, "xbox_pal");
		strcat(pathfinal, &respath[pcplatformdir - respath + 8]);

		outPath = pathfinal;
		return;
	}

	if (!pcplatformdir)
	{
		outPath = respath;
		return;
	}

	const char* const pathsuffix = &respath[pcplatformdir - respath + 8];
	char pathprefix[128] = {};

	if (region)
	{
		if (region != REGION_USA)
		{
			//	TODO: check if this is correct.
			outPath = respath;
			outPath.Append(pathsuffix);
			return;
		}

		strncpy(pathprefix, respath, pcplatformdir - respath);
		outPath = pathprefix;
		outPath.Append("ps2_pal");
		outPath.Append(pathsuffix);
		return;
	}

	strncpy(pathprefix, respath, pcplatformdir - respath);
	outPath = pathprefix;
	outPath.Append("ps2_ntsc");
	outPath.Append(pathsuffix);
}

AssetManager::RegionCode AssetManager::RegionIdByName(const String& region)
{
	if (region.Equal("europe"))
		return REGION_EUROPE;
	else
		if (region.Equal("usa"))
			return REGION_USA;
		else
			if (region.Equal("asia"))
				return REGION_ASIA;
			else
				return REGION_NOT_SET;
}

#pragma message(TODO_IMPLEMENTATION)
void AssetManager::GetResourcePath(String& outStr, const char* path) const
{
	if (!path || !*path)
		return;

	String path_;
	if (m_SceneNames.size())
		path_ = m_SceneNames[m_SceneNames.size() - 1];

	//	TODO: what this does exactly?
	outStr = path;
}

void AssetManager::IncreaseResourceReferenceCount(Asset* _res)
{
	++_res->m_Flags.m_ReferenceCount;
}

void AssetManager::DecreaseResourceReferenceCount(Asset* _res)
{
	--_res->m_Flags.m_ReferenceCount;
}

const char* AssetManager::GetCurrentSceneName() const
{
	return m_SceneNames.size() ? m_SceneNames.end()->m_szString : nullptr;
}

AllocatorIndex AssetManager::GetAllocatorType() const
{
	if (m_LoadBlocks && m_ActiveBlockId >= NULL)
		return Asset::AllocatorIndexByBlockType(g_AssetManager->m_ActiveBlockId);
	else
		return DEFAULT;
}

int AssetManager::InsertTypeListItem(void* res)
{
	field_0 = 1;

	if (m_ResourcesInstancesList.size() <= 1)
	{
		unsigned int _ind = m_ResourcesInstancesList.size();
		AddTypesListItemAtPos((Asset*)res, _ind);

		return _ind;
	}

	unsigned int ind = 0;
	for (unsigned int i = 1; i < m_ResourcesInstancesList.size(); i++)
		if (!m_ResourcesInstancesList[i])
			ind = i;

	if (!ind)
		ind = m_ResourcesInstancesList.size();

	AddTypesListItemAtPos((Asset*)res, ind);

	return ind;
}

#ifdef INCLUDE_FIXES
void AssetManager::GetPlatformSpecificPath(char* outStr, const char* respath, const char* resext, Asset::PlatformId platform)
#else
void AssetManager::GetPlatformSpecificPath(String& outStr, const char* respath, const char* resext, Asset::PlatformId platform)
#endif
{
	char buff[1024] = {};

	switch (platform)
	{
	case Asset::PlatformId::PC:
		strcpy(buff, "/data_pc");
		break;
	case Asset::PlatformId::PS2:
		strcpy(buff, "/data_ps2");
		break;
	case Asset::PlatformId::XBOX:
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

#ifdef INCLUDE_FIXES
	strcpy(outStr, buff);
#else
	outStr = buff;
#endif
}

const char* AssetManager::GetResourcePathSceneRelative(const char* const path)
{
	const String& scenename = m_SceneNames.back();
	if (strncmp(path, scenename.m_szString, scenename.m_nLength))
		if (strstr(scenename.m_szString, "/data/") &&
			strstr(path, "/data/"))
			return path + 5;
		else
			return path;
	else
		return &path[scenename.m_nLength];
}

AssetHeaderStruct_t::Header_t::Header_t()
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

void AssetHeaderStruct_t::Header_t::_401450(char* key, char* keydata)
{
	_4010C0(key);

	for (unsigned int i = 0; i < strlen(keydata); i++)
	{
		*key = keydata[i];
		_4011A0(key);
		keydata[i] = *key;
	}
}

void AssetHeaderStruct_t::Header_t::_4010C0(const char* key)
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

void AssetHeaderStruct_t::Header_t::_4011A0(char* key)
{
	//	NOTE: this routine reverses bytes
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
Asset* AssetManager::LoadResourceFile(const char* const respath)
{
	size_t respathlen = strlen(respath);
	if (!respath || !*respath || !respathlen)
		return nullptr;

	AssetInstance* assinst = AssetInstance::GetAssetInstanceByName(respath);
	if (_A3D7C0 && assinst == TextureAsset::Instance)
	{
		TextureAsset* texass = (TextureAsset*)TextureAsset::Instance->m_Creator();
		if (texass->SetResourcePlaceholder())
		{
			texass->m_ResourceTimestamp = NULL;
			texass->m_Flags.m_ReferenceCount |= 0x20000;
		}

		texass->SetResourcePath(respath);
		return texass;
	}

	//	NOTE: actually load asset from a specified path.
}

#pragma message(TODO_IMPLEMENTATION)
void* AssetManager::LoadResourceBlock(File* file, int* resbufferptr, unsigned int* resdatasize, unsigned int resblockid)
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

		assetHeaderStruct.field_38._401450("1E564E3B-D243-4ec5-AFB7", keydatabuf);
		strcpy(m_FingerprintKey, keydatabuf);

		int buf_ = NULL;
		file->Read(&buf_, sizeof(buf_));
		file->Read(&field_108, sizeof(field_108));
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

		if (GetGlobalPropertyListChecksum() != checksum)
#ifndef INCLUDE_FIXES
			return nullptr;
#else
			LogDump::LogA("Properties checksum mismatch! Asset has %x, engine has %x\n", checksum, GlobalPropertyListChecksum);
#endif

		file->Read(&checksum, sizeof(checksum));
		if (GetGlobalCommandListChecksum() != checksum)
#ifndef INCLUDE_FIXES	
			return nullptr;
#else
			LogDump::LogA("Commands checksum mismatch! Asset has %x, engine has %x\n", checksum, GlobalCommandListChecksum);
#endif

		ChecksumChecked = true;
	}

	unsigned int	totalResources = NULL;
	unsigned int	resourcesInfoSize = NULL;
	unsigned int	resourceBufferSize = NULL;
	int*			resourcesInfoBuffer = nullptr;
	int*			resourceDataBuffer = nullptr;

	file->Read(&totalResources, sizeof(totalResources));
	file->Read(&resourcesInfoSize, sizeof(resourcesInfoSize));
	file->Read(&resourceBufferSize, sizeof(resourceBufferSize));

	*resdatasize = resourcesInfoSize;
	//Asset::AllocateResourceBlockBufferAligned(resourcesInfoSize, &resourcesInfoBuffer, &resbufferptr, resblockid);
	resourceDataBuffer = (int*)MemoryManager::AllocateByType(RENDERLIST, resourceBufferSize);
	g_Progress->UpdateProgressTime(NULL, __rdtsc());

	file->SetPosAligned(0);
	file->Read(resourcesInfoBuffer, resourcesInfoSize);

	std::vector<int> ResList;

	if (totalResources > 0)
		ResList.resize(totalResources);

	time_t	fileTimestamp = File::GetFileTimestamp(file->GetFileName());
	char* resourcesDataSize = new char[16 * totalResources];	//	NOTE: intentionally make space for 4 ints so additional data can be written later.

	file->SetPosAligned(0);
	file->Read(resourcesDataSize, 4 * totalResources);

	if (totalResources > 0)
	{
		std::vector<int>::iterator it = ResList.begin();
		unsigned int assetSize = (int)resourcesDataSize - *it;

		for (unsigned int i = totalResources; i; i--)
		{
			Performance::GetMilliseconds();
			g_Progress->UpdateProgressTime(NULL, __rdtsc());

			assetSize = *(int*)(*it + assetSize);
			if (assetSize > 0)
			{
				file->SetPosAligned(0);
				file->Read(resourceDataBuffer, assetSize);
			}

			//	TODO: apply read resource data into an actual resource instance and put it into resources list.

			*it = (int)resourcesInfoBuffer;

			it++;
		}
	}

	delete[] resourcesDataSize;

	if (totalResources > 0)
		for (unsigned int i = 0; i < totalResources; i++)
			m_AssetsList.push_back(ResList[i]);

	LogDump::LogA("Done. Loading %d resource took %.2f secs.\n", totalResources, (Performance::GetMilliseconds() - timeStart) * 0.001f);
	delete resourceDataBuffer;

	return resourcesInfoBuffer;
}

Entity* AssetManager::_8755E0()
{
	unsigned int nodeid = FindNodeById(0x100000);
	if (nodeid)
		return (m_NodesList[(nodeid >> 20) & 7][nodeid & 0xFF8FFFFF]);
	else
		return nullptr;
}

Entity* AssetManager::_875610(Entity* node)
{
	unsigned int nodeid = FindNodeById(node->m_Id >> 8);
	if (nodeid)
		return (m_NodesList[(nodeid >> 20) & 7][nodeid & 0xFF8FFFFF]);
	else
		return nullptr;
}

void AssetManager::ResetSceneChildrenNodes(const int)
{
	g_GfxInternal->Render(nullptr, false, -1, -1);
	MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->stub19();
	MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->GetTotalAllocations();

	if (Scene::SceneInstance)
	{
		for (Node* child = Scene::SceneInstance->m_FirstChild; child; child = child->m_NextSibling)
		{
			child->nullsub_5();
			child->DestroyChildren();
		}
	}

	int alloc = MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->stub19();
	MemoryManager::AllocatorsList[AllocatorIndex::RENDERLIST]->GetTotalAllocations();

	if (alloc > 4 && Scene::SceneInstance)
		Scene::SceneInstance->ProfileMemory(0);

	g_GfxInternal->SetRenderBufferIsEmpty(false);
}

AssetManager::RegionCode AssetManager::GetRegion() const
{
	return m_RegionId;
}

String& AssetManager::GetDataPath(String& outstr) const
{
	outstr = "/data/";
	return outstr;
}

Asset* AssetManager::FindFirstFreeResource() const
{
	if (m_ResourcesInstancesList.size() <= 1)
		return nullptr;

	for (unsigned int i = 0; i < m_ResourcesInstancesList.size(); ++i)
		if (m_ResourcesInstancesList[i])
			return m_ResourcesInstancesList[i];

	return nullptr;
}

void AssetManager::DestroyTextureAsset(TextureAsset& ass)
{
	ass.DestroyResource();
	field_0 = 1;
}

void AssetManager::AddTypesListItemAtPos(Asset* element, unsigned int index)
{
	m_ResourcesInstancesList.insert(m_ResourcesInstancesList.begin(), index, {});
	m_ResourcesInstancesList.push_back(element);
	field_0 = 1;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned int AssetManager::FindNodeById(unsigned int id)
{
	unsigned int block_id;

	while (true)
	{
		block_id = ((id >> 20) & 7) - 1;

		if (((id & 0xFF8FFFFF) + 1) < m_NodesList[block_id].size())
			break;

		if (block_id >= 6)
			return NULL;

		id = (block_id + 1) << 20;
	};

	unsigned int i = (id & 0xFF8FFFFF) + 1;
	for (i; i < m_NodesList[block_id].size(); i++);

	return i | ((block_id + 1) << 20);
}

AssetManager::AssetManager(bool loadBlocks)
{
	MESSAGE_CLASS_CREATED(AssetManager);

	m_Defragmentator = MemoryManager::_4777F0(DEFRAGMENTING);
	m_LoadBlocks = loadBlocks;

	g_AssetManager = this;

	m_NodesList[0].reserve(11200);
	m_NodesList[1].reserve(4100);
	m_NodesList[2].reserve(6000);
	m_NodesList[3].reserve(2800);
	m_NodesList[5].reserve(100);
	field_0 = NULL;
	m_RegionId = REGION_NOT_SET;
	m_ResourcesInstancesList.reserve(1);
	field_1C8 = NULL;
	m_BlocksUnloaded = NULL;
	field_1D0 = nullptr;
	m_CheckTimestamp = false;
	m_EngineVersionTimestamp = NULL;
	m_ActiveBlockId = -1;
	field_108 = 2;
}

#pragma message(TODO_IMPLEMENTATION)
AssetManager::~AssetManager()
{
	MESSAGE_CLASS_DESTROYED(AssetManager);

	Asset* res = FindFirstFreeResource();
	if (res)
	{
		//	TODO: finish this!
	}

	g_AssetManager = nullptr;

	m_AssetsList.clear();
	m_SceneNames.clear();
	m_ResourcesInstancesList.clear();

	for (unsigned int i = 0; i < 6; i++)
		m_NodesList[i].clear();

	m_FastFindNodeVector.clear();
}

void AssetManager::SetSceneName(const char* scenename)
{
	String sceneDir;
	File::ExtractFileDir(sceneDir, scenename);

	if (sceneDir.m_nLength > 0 && sceneDir.m_szString[sceneDir.m_nLength - 1] != '/')
		sceneDir.Append("/");

	m_SceneNames.push_back(sceneDir);
}

void AssetManager::RemoveLastSceneName()
{
	if (m_SceneNames.size())
		m_SceneNames.pop_back();
}

unsigned int AssetManager::GetFreeResourceTypeListItem(unsigned int index)
{	
	unsigned int freeind = index + 1;
	if (freeind >= m_ResourcesInstancesList.size())
		return 0;

	for (const Asset* res = m_ResourcesInstancesList[freeind]; !res; res++)
		if (++freeind >= m_ResourcesInstancesList.size())
			return 0;

	return freeind;
}

unsigned int AssetManager::AddEntity(Entity* ent)
{
	unsigned int listind = m_ActiveBlockId == -1 ? 0 : m_ActiveBlockId;
	unsigned int listcap;

	if (field_1C8)
	{
		listcap = field_1C8;
		field_1C8 = NULL;
	}
	else
		listcap = m_NodesInNodeList[listind];
	listcap = listcap & 0xFF8FFFFF;

	if (listcap >= m_NodesList[listind].size())
		LogDump::LogA("Warning: vEntity[%d] is grown to size %d. Please adjust reserve() calls.\n", listind, listcap);

	m_NodesList[listind].push_back(ent);
	m_NodesInNodeList[listind]++;

	return listcap | ((listind + 1) << 20);
}

void AssetManager::SetRegion(RegionCode id)
{
	m_RegionId = id;
}