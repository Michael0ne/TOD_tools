#include "Scene.h"
#include "SceneSaveLoad.h"
#include "LogDump.h"
#include "Performance.h"
#include "Progress.h"
#include "ScriptDatabase.h"
#include "SavePoint.h"
#include "Buffer92.h"
#include "CollisionProbe.h"
#include "GfxInternal.h"
#include "Camera.h"
#include "FrameBasedSubAllocator.h"

EntityType* tScene = nullptr;
Scene* Scene::SceneInstance = nullptr;
AuxQuadTree* Scene::SceneTree;

int Scene::RealTimeMs;
int Scene::GameTimeMs;
int Scene::NextUpdateTime;
int Scene::TotalFrames;
int Scene::NewFrameNumber;
bool Scene::IsRewindBufferInUse = true;
bool Scene::LoadingAssetBlock;
const unsigned int Scene::RewindBufferSize_1 = 204800;
const unsigned int Scene::RewindBufferSize_2 = 309248;

float Scene::FrameRate;
UINT64 Scene::CreationTime;
int Scene::_A3CEE4;
int Scene::_A3CEE8;
int Scene::_A3DA80[100];
int Scene::_A3D8D8[100];
int Scene::_A3DC38[4];

int Scene::PreBlocksUnloadedCommand;
int Scene::BlocksUnloadedCommand;
int Scene::InvalidatePlaceholderModelCommand = -1;
int Scene::RewindOrRetryFinishedCommand = -1;

Scene::Scene() : Folder_()
{
	MESSAGE_CLASS_CREATED(Scene);

	m_PlayMode = 1;
	m_NodesWithUpdateOrBlockingScripts = NULL;
	m_Buffer_1 = new RenderBuffer92(0, 36, 2);
	m_Buffer_2 = new RenderBuffer92(0, 36, 2);
	m_SharedProbe = (CollisionProbe*)tCollisionProbe->CreateNode();
	SceneInstance = this;
	m_GameCamera = nullptr;
	m_EditorCamera = nullptr;
	m_QuadTreesAllocated = false;
	StoreGameCamera();
	m_CameraPosition = { 10000.f, -10000.f, 50000.f, 0.f };
	m_SaveGameSize = NULL;
	NextUpdateTime = NULL;
	RealTimeMs = NULL;
	GameTimeMs = NULL;
	m_RewindResumeTimeMs = NULL;
	m_InitMode = NULL;
	CreationTime = Performance::GetMilliseconds();
	m_TimeMultiplier = 1.f;
	field_118 = 1.f;
	m_RewindTimeMultiplier = 1.f;
	m_FixedFramerate = false;
	m_FixedFramerateVal = 0.039999999f;

	for (unsigned int i = 0; i < 31; i++)
	{
		m_SceneBufferArray[i] = new RenderBuffer92(100, 20, 2);
		g_GfxInternal->SetBufferRenderBufferPointerByIndex(i, m_SceneBufferArray[i]);
	}

	m_List_5.resize(1024);
	m_RewindBuffer1 = nullptr;
	m_RewindBuffer2 = nullptr;
	_A3CEE4 = NULL;
	_A3CEE8 = NULL;
	field_C8 = NULL;
	m_SaveLoadState = STATE_DONE;
	m_SaveData = nullptr;
	m_MemoryCards = nullptr;
	m_WindMode = NULL;
	field_1A8 = NULL;
	m_WindPause = false;
	field_1AA = NULL;
	m_FlushRewindRequested = false;
	m_SharedProbe->SetFlags(0x20);

	for (unsigned int i = 0; i < 8; i++)
	{
		m_LoadedBlocks[i] = nullptr;
		field_1D0[i] = NULL;
	}

	memset(_A3DA80, NULL, sizeof(_A3DA80));
	memset(_A3D8D8, NULL, sizeof(_A3D8D8));
	memset(_A3DC38, NULL, sizeof(_A3DC38));

	field_268 = NULL;
	m_ActiveCamera = nullptr;
	
	RewindOrRetryFinishedCommand = GetCommandByName("rewind_or_retry_finished(integer)");
	BuildSceneTree();
	AllocateRewindBuffer();

	m_AssetBlockInfo = new AssetInfo();
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

	assetinfo->m_ResourceDataBufferPtr = g_Blocks->LoadResourceBlock(&assetfile, (int*)assetinfo->m_ResourceAllocatedAlignedBufferPtr, &assetinfo->m_ResourceDataBufferSize, ResType::BlockTypeNumber::NONE);
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
		g_Blocks->GetPlatformSpecificPath(scene_path, sceneName, "", ResType::PLATFORM_PC);
		scene_path.Append("/");

		char pathdummy[1024] = {};
		char scene_fname[128] = {};
		File::ExtractFilePath(sceneName, pathdummy, scene_fname, pathdummy);
		scene_path.Append(scene_fname);

		String block_path_shared, block_path_localised;
		Folder_::GetResourcePathRelative(block_path_shared, scene_path, ResType::BlockTypeNumber::NONE, 0);
		Folder_::GetResourcePathRelative(block_path_localised, scene_path, ResType::BlockTypeNumber::NONE, Script::GetCurrentCountryCode());
#ifdef INCLUDE_FIXES
		if (!File::FindFileEverywhere(block_path_shared.m_szString))
		{
			LogDump::LogA("Asset shared block not found: \"%s\"!\n", block_path_shared.m_szString);
			return;
		}
#else
		File::FindFileEverywhere(block_path_shared.m_szString);
#endif
#ifdef INCLUDE_FIXES
		if (!File::FindFileEverywhere(block_path_localised.m_szString))
		{
			LogDump::LogA("Asset localization block not found: \"%s\"!\n", block_path_shared.m_szString);
			return;
		}
#else
		File::FindFileEverywhere(block_path_localised.m_szString);
#endif

		int mainAssetAllocMem = MemoryManager::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations();
		LogDump::LogA("asset block before: %0.1f Kb\n", mainAssetAllocMem * 0.0009765625f);

		if (strcmp(MemoryManager::AllocatorsList[ResType::ResourceBase::GetResourceBlockTypeNumber(ResType::BlockTypeNumber::NONE)]->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
			((FrameBasedSubAllocator*)MemoryManager::AllocatorsList[ResType::ResourceBase::GetResourceBlockTypeNumber(ResType::BlockTypeNumber::NONE)])->_47A120();
		
		LoadingAssetBlock = true;
		//Allocators::AllocatorsList[DEFRAGMENTING]->field_1C->field_20 = false;
		LoadResourceBlockIntoSceneBuffer(block_path_shared.m_szString, &m_AssetBlockInfo->m_AssetInfo_Shared);
		LoadResourceBlockIntoSceneBuffer(block_path_localised.m_szString, &m_AssetBlockInfo->m_AssetInfo_Localised);
		
		m_BlockId = m_BlockId | 0x80000000;
		LoadingAssetBlock = false;
		LogDump::LogA("asset block took %0.1f Kb\n", (mainAssetAllocMem - MemoryManager::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations()) * 0.0009765625f);
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

void Scene::StoreGameCamera()
{
	if (m_GameCamera && m_EditorCamera != nullptr && m_PlayMode != 1)
		m_ActiveCamera = m_QuadTreesAllocated ? (Camera*)m_EditorCamera : m_GameCamera;
	else
		if (m_EditorCamera)
			m_ActiveCamera = (Camera*)m_EditorCamera;
		else
			if (m_GameCamera)
				m_ActiveCamera = m_GameCamera;
			else
				m_ActiveCamera = nullptr;

	if (m_ActiveCamera)
	{
		Camera::StoreActiveCameraPosition();

		if ((((m_CameraPosition.x - Camera::ActiveCameraPosition.x) * (m_CameraPosition.x - Camera::ActiveCameraPosition.x)) +
			((m_CameraPosition.z - Camera::ActiveCameraPosition.z) * (m_CameraPosition.z - Camera::ActiveCameraPosition.z)) +
			((m_CameraPosition.y - Camera::ActiveCameraPosition.y) * (m_CameraPosition.y - Camera::ActiveCameraPosition.y))) > 100.f)
		{
			LogDump::LogA("Camera has moved more than 10m. Forcing Lod-calculation\n");
			ForceLodCalculation(0);
		}

		m_CameraPosition = Camera::ActiveCameraPosition;
	}
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
	if (IsRewindBufferInUse && !m_RewindBuffer1)
	{
		if (MemoryManager::AllocatorsList[CUTSCENE_OR_REWIND]->stub19() > 0)
		{
			LogDump::LogA("cannot allocate rewind buffer - memory block is in use!\n");
			return;
		}

		LogDump::LogA("Allocate rewind buffer...\n");
		m_RewindBuffer1 = new TransactionBuffer(RewindBufferSize_1);
		m_RewindBuffer2 = new TransactionBuffer(RewindBufferSize_2);
		field_268 = NULL;
	}

	m_RewindBuffer1->m_List_1.clear();
	m_RewindBuffer1->field_1C = (void*)&(m_RewindBuffer1->m_List_1.begin());
	m_RewindBuffer1->m_Size = NULL;
	m_RewindBuffer1->field_20 = NULL;

	m_RewindBuffer2->m_List_1.clear();
	m_RewindBuffer2->field_1C = (void*)&(m_RewindBuffer2->m_List_1.begin());
	m_RewindBuffer2->m_Size = NULL;
	m_RewindBuffer2->field_20 = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::BuildSceneTree()
{
	if (SceneTree)
		return;

	/*g_Blocks->ResetSceneChildrenNodes(true);
	AllocateQuadTrees();
	AdjustNodesListsSize();*/
}

void Scene::FreeRewindBuffer()
{
	if (IsRewindBufferInUse && m_RewindBuffer1)
	{
		LogDump::LogA("Free rewind buffer\n");
		ResetRewindBuffer(true);

		delete m_RewindBuffer1;
		delete m_RewindBuffer2;
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

#pragma message(TODO_IMPLEMENTATION)
void Scene::Register()
{
	tScene = new EntityType("Scene");
	tScene->InheritFrom(tNode);
	tScene->SetCreator((EntityType::CREATOR)Create);

	//	TODO: register properties/scripts.

	PreBlocksUnloadedCommand = RegisterGlobalCommand("pre_blocks_unloaded", true);
	BlocksUnloadedCommand = RegisterGlobalCommand("blocks_unloaded", true);
	InvalidatePlaceholderModelCommand = RegisterGlobalCommand("invalidate_placeholder_model", true);

	tScene->_86E9B0();
}

Scene* Scene::Create(AllocatorIndex)
{
	return new Scene();
}