#include "Scene.h"
#include "SceneSaveLoad.h"
#include "LogDump.h"
#include "Performance.h"
#include "Progress.h"
#include "ScriptDatabase.h"
#include "SavePoint.h"
#include "FrameBuffer.h"
#include "CollisionProbe.h"
#include "GfxInternal.h"
#include "Camera.h"
#include "FrameBasedSubAllocator.h"
#include "ModelAsset.h"

EntityType* tScene = nullptr;
Scene* Scene::SceneInstance = nullptr;
AuxQuadTree* Scene::SceneTree;

unsigned int Scene::QuadTreesAllocated;
Scene::QuadTree* Scene::QuadTrees;
short Scene::_A120E8 = -1;

int Scene::RealTimeMs;
int Scene::GameTimeMs;
int Scene::NextUpdateTime;
int Scene::TotalFrames;
int Scene::NewFrameNumber;
bool Scene::IsRewindBufferInUse = true;
bool Scene::LoadingAssetBlock;
float Scene::FrameRate;
float Scene::FrameRate_1;
UINT64 Scene::CreationTime;
int* Scene::_A3CEE4;
int Scene::_A3CEE8;
int Scene::_A3DA80[100];
int Scene::_A3D8D8[100];
int Scene::_A3DC38[4];
int Scene::UpdateOrBlockingListSize;
bool Scene::_A3D858;

int Scene::PreBlocksUnloadedCommand;
int Scene::BlocksUnloadedCommand;
int Scene::InvalidatePlaceholderModelCommand = -1;
int Scene::RewindOrRetryFinishedCommand = -1;

void Scene::CreateQuadTrees(const unsigned int num, const AllocatorIndex allocind)
{
	if (QuadTrees)
		return;

	QuadTreesAllocated = num;
	QuadTrees = new QuadTree[num];

	for (unsigned int i = 0; i < num - 1; ++i)
		QuadTrees[i].m_Index = i;

	QuadTrees[num - 1].m_Index = -1;
}

Scene::Scene() : Folder_()
{
	MESSAGE_CLASS_CREATED(Scene);

	m_PlayMode = MODE_UNKNOWN_1;
	m_NodesWithUpdateOrBlockingScripts = NULL;
	m_Buffer_1 = new FrameBuffer(0, 36, 2);
	m_Buffer_2 = new FrameBuffer(0, 36, 2);
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
		m_FrameBuffers[i] = new FrameBuffer(100, 20, 2);
		g_GfxInternal->SetBufferRenderBufferPointerByIndex(i, m_FrameBuffers[i]);
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

	ClearNodesLists();
	g_GfxInternal->SetRenderBufferIsEmpty(true);
	g_AssetManager->ResetSceneChildrenNodes(0);
	const unsigned int resind = g_AssetManager->GetFreeResourceTypeListItem(0);

	if (resind)
		for (Asset* a = g_AssetManager->m_ResourcesInstancesList[resind]; a; a = g_AssetManager->GetAssetIfExists(a))
			if (a->GetInstancePtr() == ModelAsset::Instance)
				((ModelAsset*)a)->_856E60();

	_895F50();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::LoadResourceBlockIntoSceneBuffer(const char* assetname, AssetInfo::ActualAssetInfo* assetinfo)
{
	File assetfile(assetname, 161, true);

	assetinfo->m_ResourceDataBufferPtr = g_AssetManager->LoadResourceBlock(&assetfile, (int*)&assetinfo->m_ResourceAllocatedAlignedBufferPtr, &assetinfo->m_ResourceDataBufferSize, Asset::BlockTypeNumber::NONE);
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

void Scene::CreateSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode, unsigned int savesize)
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
		m_SaveData = summarynode;
	}
	else
		LogDump::LogA("Trying to create empty savepoint??? Bailing out.\n");
}

void Scene::RestoreSavePoint(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode, MemoryCards* memcards)
{
	m_SaveLoadState = STATE_LOAD;
	m_MemoryCardIndex = memcardind;
	m_SaveSlotIndex = slotind;
	m_SavePointOperationError = STATUS_OK;
	m_SaveDir = savedirectory;
	m_SaveData = summarynode;
	m_MemoryCards = memcards;
}

void Scene::LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, const char* const savedirectory, Node* summarynode)
{
	m_SaveLoadState = STATE_LOAD_SUMMARY;
	m_MemoryCardIndex = memcardind;
	m_SaveSlotIndex = slotind;
	m_SavePointOperationError = STATUS_OK;
	m_SaveDir = savedirectory;
	m_SaveData = summarynode;

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
void Scene::ProfileMemory(const int)
{
	LogDump::LogA("(memory profiler is not available in this build)\n");
	//	TODO: calls profiler method of MemoryManager class.
}

void Scene::ClearNodesLists()
{
	UpdateOrBlockingListSize = NodesWithUpdateOrBlockingScripts.size();

	for (unsigned int i = 0; i < NodesWithUpdateOrBlockingScripts.size(); ++i)
		NodesWithUpdateOrBlockingScripts[i].m_Node->ClearFromBlockingList();

	NodesWithUpdateOrBlockingScripts.clear();

	m_NodesWithUpdateOrBlockingScripts = NULL;

	for (unsigned int i = 0; i < NodesList.size(); ++i)
		NodesList[i].m_QuadTree->m_Owner->ClearFromSceneList();

	NodesList.clear();
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::_895F50()
{
}

Folder_* Scene::GetFolderByIndex(const unsigned int index) const
{
	return (Folder_*)m_LoadedBlocks[index];
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::LoadSceneSession(void) const
{
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::InstantiateAssetsToLists()
{
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::Start()
{
	if (m_PlayMode != MODE_UNKNOWN_1)
		return;

	m_PlayMode = MODE_INGAME;
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
	int alloctotalbefore;

	FreeRewindBuffer();
	ReleaseQuadTreeAndRenderlist();

	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Initialization", Performance::GetMilliseconds() - m_StartTimeMs, Performance::GetMilliseconds() - m_StartTimeMs);

	g_Progress->Reset();
	g_Progress->AddLoadbarPhase("load scene", 25 * Performance::ClockGetCycles(), true);
	g_Progress->_40E7F0(1, __rdtsc());

	if (g_AssetManager->m_LoadBlocks)
	{
		char scene_path[1024] = {};
		g_AssetManager->GetPlatformSpecificPath(scene_path, sceneName, "", Asset::PlatformId::PC);
		strcat(scene_path, "/");

		char pathdummy[1024] = {};
		char scene_fname[128] = {};
		File::ExtractFilePath(sceneName, pathdummy, scene_fname, pathdummy);
		strcat(scene_path, scene_fname);

		String block_path_shared, block_path_localised;
		Folder_::GetResourcePathRelative(block_path_shared, scene_path, Asset::BlockTypeNumber::NONE, 0);
		Folder_::GetResourcePathRelative(block_path_localised, scene_path, Asset::BlockTypeNumber::NONE, Script::GetCurrentCountryCode());
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

		//if (strcmp(MemoryManager::AllocatorsList[Asset::ResourceBase::GetResourceBlockTypeNumber(ResType::BlockTypeNumber::NONE)]->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
			//((FrameBasedSubAllocator*)MemoryManager::AllocatorsList[ResType::ResourceBase::GetResourceBlockTypeNumber(ResType::BlockTypeNumber::NONE)])->_47A120();
		
		LoadingAssetBlock = true;
		//Allocators::AllocatorsList[DEFRAGMENTING]->field_1C->field_20 = false;
		LoadResourceBlockIntoSceneBuffer(block_path_shared.m_szString, &m_AssetBlockInfo->m_AssetInfo_Shared);
		LoadResourceBlockIntoSceneBuffer(block_path_localised.m_szString, &m_AssetBlockInfo->m_AssetInfo_Localised);
		
		m_BlockId = m_BlockId | 0x80000000;
		LoadingAssetBlock = false;
		alloctotalbefore = MemoryManager::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations();
		LogDump::LogA("asset block took %0.1f Kb\n", (mainAssetAllocMem - MemoryManager::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations()) * 0.0009765625f);
	}

	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Load main asset block", Performance::GetMilliseconds() - m_StartTimeMs, Performance::GetMilliseconds() - m_StartTimeMs);

	m_StartTimeMs = Performance::GetMilliseconds();

	g_AssetManager->m_ActiveBlockId = (8 * m_BlockId) >> 3;
	_A3D858 = 1;

	//if (strcmp(MemoryManager::AllocatorsList[Asset::ResourceBase::GetResourceBlockTypeNumber(ResType::BlockTypeNumber::NONE)]->GetAllocatorName(), "FrameBasedSubAllocator") == NULL)
			//((FrameBasedSubAllocator*)MemoryManager::AllocatorsList[ResType::ResourceBase::GetResourceBlockTypeNumber(ResType::BlockTypeNumber::NONE)])->_47A120();

	UINT64 fragmentloadstarttime = __rdtsc();
	g_AssetManager->_878030();
	SetFragment(sceneName);
	g_AssetManager->_877AE0();
	LogDump::LogA("Timings: SetFragment: %f\n", (__rdtsc() - fragmentloadstarttime) / Performance::ClockGetCycles());
	const int alloctotal = MemoryManager::AllocatorsList[MAIN_ASSETS]->GetTotalAllocations();
	LogDump::LogA("Scene graph took %0.1f KB\n", (alloctotal - alloctotalbefore) * 0.0009765625f);
	LogDump::LogA("Asset block after: %0.1f KB\n", alloctotal * 0.0009765625f);
	
	const int freeassetind = g_AssetManager->GetFreeResourceTypeListItem(0);
	if (freeassetind)
	{
		for (FragmentAsset* frgm = (FragmentAsset*)g_AssetManager->m_ResourcesInstancesList[freeassetind]; frgm; frgm = (FragmentAsset*)g_AssetManager->GetAssetIfExists(frgm))
		{
			if (frgm->GetInstancePtr() == FragmentAsset::Instance)
			{
				if ((frgm->field_24 & 1) == 0)
				{
					LogDump::LogA("NOT UNLOADING FRAGMENT: %s\n", frgm->m_ResourcePath);
					continue;
				}

				if (!frgm->field_20)
				{
					frgm->field_20 = nullptr;
					continue;
				}

				delete frgm->field_20;
			}
		}
	}

	//Allocators::AllocatorsList[DEFRAGMENTING]->field_1C->field_20 = true;
	_A3D858 = false;
	g_AssetManager->m_ActiveBlockId = -1;
	_896BA0();
	AllocateRewindBuffer();
	DWORD currtimems = Performance::GetMilliseconds();
	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Scene load and apply", currtimems - m_StartTimeMs, currtimems - m_StartTimeMs);
	m_StartTimeMs = currtimems;
	LoadSceneSession();
	currtimems = Performance::GetMilliseconds();
	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Load current asset blocks", currtimems - m_StartTimeMs, currtimems - m_StartTimeMs);
	m_StartTimeMs = currtimems;
	currtimems = Performance::GetMilliseconds();
	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", "Compile 'test' folder scripts", currtimems - m_StartTimeMs, currtimems - m_StartTimeMs);
	m_StartTimeMs = currtimems;
	GlobalScript::InstantiateGlobalScripts();
	EnumSceneCamerasAndUpdate();
	TotalFrames = 0;
	NewFrameNumber = 0;
	InstantiateAssetsToLists();
	g_Progress->Complete();
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

void Scene::EnumSceneCamerasAndUpdate()
{
	m_EditorCamera = nullptr;
	m_GameCamera = nullptr;

	for (Node* n = m_FirstChild; n; n = n->m_NextSibling)
	{
		EntityType* scent = n->m_ScriptEntity;
		if (!scent)
			continue;

		while (tCamera != scent)
		{
			scent = scent->m_Parent;
			if (!scent)
				break;
		}

		if (!m_EditorCamera && strcmp(n->m_Name, "editorcamera") == NULL)
			m_EditorCamera = (EditorCamera*)n;

		if (!m_GameCamera && strcmp(n->m_Name, "gamecamera") == NULL)
			m_GameCamera = (Camera*)n;
	}

	StoreGameCamera();
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

	m_RewindBuffer1->m_List_1.resize(1);
	m_RewindBuffer1->field_1C = (void*)&(m_RewindBuffer1->m_List_1.begin());
	m_RewindBuffer1->m_Size = NULL;
	m_RewindBuffer1->field_20 = NULL;

	m_RewindBuffer2->m_List_1.resize(1);
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
void Scene::_896BA0()
{
}

void Scene::TriggerScriptForAllChildren(int scriptId, Node* node, int* args)
{
	node->TriggerGlobalScript(scriptId, args);

	for (Node* n = node->m_FirstChild; n; n = n->m_NextSibling)
		TriggerScriptForAllChildren(scriptId, n, args);
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