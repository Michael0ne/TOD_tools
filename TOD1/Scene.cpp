#include "Scene.h"
#include "SceneSaveLoad.h"
#include "LogDump.h"
#include "Performance.h"
#include "Progress.h"
#include "ScriptDatabase.h"
#include "SavePoint.h"

ScriptType_Entity* tScene = nullptr;
Scene* Scene::SceneInstance = nullptr;

AuxQuadTree* Scene::SceneTree;
int Scene::RealTimeMs;
int Scene::GameTimeMs;
int Scene::NextUpdateTime;
int Scene::TotalFrames;
int Scene::NewFrameNumber;
bool Scene::IsRewindBufferInUse;
bool Scene::LoadingAssetBlock;
float Scene::FrameRate;

#pragma message(TODO_IMPLEMENTATION)
Scene::Scene() : Folder_()
{
	MESSAGE_CLASS_CREATED(Scene);
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::ReleaseQuadTreeAndRenderlist()
{
	if (!SceneTree)
	{
		LogDump::LogA("quadtrees and renderlists not allocated\n");
		return;
	}
}

void Scene::LoadResourceBlockIntoSceneBuffer(const char* assetname, AssetInfo::ActualAssetInfo* assetinfo)
{
	File assetfile(assetname, 161, true);

	assetinfo->m_ResourceDataBufferPtr = g_Blocks->LoadResourceBlock(&assetfile, assetinfo->m_ResourceAllocatedAlignedBufferPtr, &assetinfo->m_ResourceDataBufferSize, NONE);
	DWORD64 starttick = __rdtsc();

	//g_Blocks->_878030();
	//g_Blocks->_875EB0();
	//g_Blocks->_877AE0();

	LogDump::LogA("Timings: FixupAssetRefsInLoadedAssetBlocks: %f\n", (__rdtsc() - starttick) / Performance::ClockGetCycles());

	if (assetinfo->m_ResourceDataBufferPtr)
		LogDump::LogA("read asset block file: %s\n", assetname);
	else
		LogDump::LogA("Asset file could not be loaded: %s\n", assetname);
}

void Scene::CreateSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, const Node* summarynode, unsigned int savesize)
{
	LogDump::LogA("save point creation pending!\n");
	if (savesize)
	{
		m_SaveSlotIndex = slotind;
		m_SaveLoadState = STATE_SAVE;
		m_MemoryCardIndex = memcardind;
		m_SavePointOperationError = STATUS_OK;
		m_SaveDir = savedirectory;
		m_SaveGameSize = savesize;
		m_SaveData = (Node*)summarynode;
	}
	else
		LogDump::LogA("Trying to create empty savepoint??? Bailing out.\n");
}

void Scene::RestoreSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, const Node* summarynode, const MemoryCards* memcards)
{
	m_SaveLoadState = STATE_LOAD;
	m_MemoryCardIndex = memcardind;
	m_SaveSlotIndex = slotind;
	m_SavePointOperationError = STATUS_OK;
	m_SaveDir = savedirectory;
	m_SaveData = (Node*)summarynode;
	m_MemoryCards = (MemoryCards*)memcards;
}

void Scene::LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, const Node* summarynode)
{
	m_SaveLoadState = STATE_LOAD_SUMMARY;
	m_MemoryCardIndex = memcardind;
	m_SaveSlotIndex = slotind;
	m_SavePointOperationError = STATUS_OK;
	m_SaveDir = savedirectory;
	m_SaveData = (Node*)summarynode;

	char slotstr[10] = {};
	sprintf(slotstr, "Slot%02d", m_SaveSlotIndex);
	SavePoint savepoint(MemoryCardInfo[m_MemoryCardIndex], m_SaveDir.m_szString, slotstr, SAVEPOINT_SAVE_SIZE);

	if (savepoint.Open(STATUS_SUCCESS))
	{
		if (!g_SceneSaveLoad->LoadSaveSummary(&savepoint, m_SaveData))
			m_SavePointOperationError = STATUS_CANT_READ_SAVE_DATA;
	}
	else
		m_SavePointOperationError = STATUS_CANT_OPEN_FILE;

	if (m_SavePointOperationError == STATUS_CANT_READ_SAVE_DATA)
	{
		if (!MemoryCardInfo[m_MemoryCardIndex]->m_SaveFolderPath.m_nLength)
		{
			LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");

			m_SavePointOperationError = STATUS_SAVEDIR_NOT_READY;
			m_SaveLoadState = STATE_DONE;
		}

		if (!MemoryCardInfo[m_MemoryCardIndex]->m_Formatted ||
			!File::IsDirectoryValid(MemoryCardInfo[m_MemoryCardIndex]->m_SaveFolderPath.m_szString))
		{
			m_SavePointOperationError = STATUS_SAVEDIR_NOT_READY;
			m_SaveLoadState = STATE_DONE;
		}
	}

	m_SaveLoadState = STATE_DONE;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::Start()
{
	if (m_PlayMode != 1)
		return;

	m_PlayMode = 0;
	UpdateLoadedBlocks(1, 0);
	EnumSceneCamerasAndUpdate();
	RealTimeMs = NULL;
	GameTimeMs = NULL;
	NextUpdateTime = NULL;
	TotalFrames = NULL;
	NewFrameNumber = NULL;

	//	FIXME: this is stupid! Make something about it, like templated structure for passing parameters...
	//const char* params[] = { NULL, "start" };
	//tBuiltin->GetMessageId(params);
	//if ((int)params[0] >= 0)
		//TriggerScriptForAllChildren((int)params[0], this, nullptr);

	_896810();
	g_SceneSaveLoad->_874940();

	if (IsRewindBufferInUse)
	{
		AllocateRewindBuffer();
		ResetRewindBuffer(true);
	}

	m_RewindResumeTimeMs = NULL;
	TotalFrames = NULL;
	NewFrameNumber = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::Load(const char* sceneName)
{
	m_StartTimeMs = Performance::GetMilliseconds();

	FreeRewindBuffer();
	ReleaseQuadTreeAndRenderlist();

	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Initialization", Performance::GetMilliseconds() - m_StartTimeMs, Performance::GetMilliseconds() - m_StartTimeMs);

	g_Progress->Reset();
	g_Progress->AddLoadbarPhase("load scene", 25 * Performance::ClockGetCycles(), true);
	g_Progress->_40E7F0(1, __rdtsc());

	if (g_Blocks->m_LoadBlocks)
	{
		String scene_path;
		g_Blocks->GetFullResourcePath(scene_path, sceneName, "", ResType::PLATFORM_PC);
		scene_path.Append("/");

		char pathdummy[1024] = {};
		char scene_fname[128] = {};
		File::ExtractFilePath(sceneName, pathdummy, scene_fname, pathdummy);
		scene_path.Append(scene_fname);

		String block_path_shared, block_path_localised;
		Folder_::GetResourcePathRelative(block_path_shared, scene_path, NONE, 0);
		Folder_::GetResourcePathRelative(block_path_localised, scene_path, NONE, Script::GetCurrentCountryCode());
		File::FindFileEverywhere(block_path_shared.m_szString);
		File::FindFileEverywhere(block_path_localised.m_szString);

		int mainAssetAllocMem = Allocators::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations();
		LogDump::LogA("asset block before: %0.1f Kb\n", mainAssetAllocMem * 0.0009765625f);

		if (strcmp(Allocators::AllocatorsList[Blocks::GetResourceBlockTypeNumber(NONE)]->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
			((FrameBasedSubAllocator*)Allocators::AllocatorsList[Blocks::GetResourceBlockTypeNumber(NONE)])->_47A120();
		
		LoadingAssetBlock = true;
		Allocators::AllocatorsList[DEFRAGMENTING]->field_1C->field_20 = false;
		LoadResourceBlockIntoSceneBuffer(block_path_shared.m_szString, &m_AssetBlockInfo->m_AssetInfo_Shared);
		LoadResourceBlockIntoSceneBuffer(block_path_localised.m_szString, &m_AssetBlockInfo->m_AssetInfo_Localised);
		
		m_BlockId = m_BlockId | 0x80000000;
		LoadingAssetBlock = false;
		LogDump::LogA("asset block took %0.1f Kb\n", (mainAssetAllocMem - Allocators::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations()) * 0.0009765625f);
	}

	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Load main asset block", Performance::GetMilliseconds() - m_StartTimeMs, Performance::GetMilliseconds() - m_StartTimeMs);

	m_StartTimeMs = Performance::GetMilliseconds();
}

void Scene::RefreshChildNodes()
{
	RefreshQuadTree();
	//	TODO: is type cast correct?
	for (Scene* child = (Scene*)m_FirstChild; child; child = (Scene*)child->m_NextSibling)
		child->RefreshChildNodes();
}

void Scene::FinishCreation(const char* logTitle)
{
	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", logTitle, Performance::GetMilliseconds() - m_StartTimeMs, Performance::GetMilliseconds() - m_StartTimeMs);
	m_StartTimeMs = Performance::GetMilliseconds();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::UpdateActiveCameraPosition()
{

}

#pragma message(TODO_IMPLEMENTATION)
void Scene::EnumSceneCamerasAndUpdate()
{

}

#pragma message(TODO_IMPLEMENTATION)
void Scene::UpdateLoadedBlocks(int unk1, Node* unk2)
{
	//	NOTE: this is potentially a method that updates current scenery objects.
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::_896810()
{

}

#pragma message(TODO_IMPLEMENTATION)
void Scene::AllocateRewindBuffer()
{

}

void Scene::FreeRewindBuffer()
{
	if (IsRewindBufferInUse && m_RewindBuffer1)
	{
		LogDump::LogA("Free rewind buffer\n");
		ResetRewindBuffer(true);

		delete m_RewindBuffer1;
		delete m_RewindBuffer2;

		m_RewindBuffer1 = nullptr;
		m_RewindBuffer2 = nullptr;
	}
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::ResetRewindBuffer(bool)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::TriggerScriptForAllChildren(int scriptId, Scene* sceneNode, int* unk)
{
	//TriggerGlobalScript(scriptId, unk);
	//for (Node* children = sceneNode->m_FirstChild; children; children = children->m_NextSibling)
		//TriggerScriptForAllChildren(scriptId, children, unk);
}