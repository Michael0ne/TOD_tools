#include "Scene.h"

#include "Renderer.h"
#include "LoadScreen.h"
#include "StreamedSoundBuffers.h"
#include "Performance.h"
#include "Blocks.h"

Scene* g_Scene = (Scene*)0xA3DCBC;
int& Scene::UnkInt_1 = *(int*)0xA3DCE4;

void Scene_Buffer92::Reset()
{
	field_14 &= 0xFFFFFFEF;
	field_34 = 0;
	field_38 = 0;
	field_20 = 0;
	field_24 = 0;
	field_48 = 0;
	field_4C = 0;
	field_0 = 0;
	field_8 = 0;
}

bool Scene::Update()
{
	if (!g_Scene)
		return true;

	g_Scene->m_pBuffer_92_1->Reset();
	g_Scene->m_pBuffer_92_2->Reset();

	Allocators::MemoryAllocators::DefragmentIfNecessary(Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFRAGMENTING]->field_1C);

	auto timeStart = __rdtsc();

	g_Scene->lpVtbl->Update(g_Scene);

	g_Renderer->_SetBufferStateByIndex(3, 0);

	bool loadingEnabled = false;

	if (!g_CurrentLoadScreen->m_bEnabled) {
		g_Scene->lpVtbl->_896370(g_Scene);
		loadingEnabled = true;
	}

	Scene::UnkInt_1++;

	*(DWORD64*)&g_Scene->m_nLoadTime = __rdtsc() - timeStart;

	if (Audio::g_StreamedSoundBuffers && g_Scene->m_pCameraEntity) {
		//Audio::g_StreamedSoundBuffers->SetListener3DPos(0);
	}

	//	TODO: implementation!

	return true;
}

bool Scene::Instantiate(const char* szSceneName)
{
	g_Scene = (Scene*)(new Entity(true));

	g_Blocks->SetSceneName(szSceneName);
	g_Scene->Load(szSceneName);
	g_Scene->UpdateLoadedBlocks(0, 0);
	g_Scene->m_nTimeMilliseconds = Performance::GetMilliseconds();
	g_Scene->RefreshChildNodes();
	g_Scene->FinishCreation("Scene instantiate all completed.");

	return true;
}

void Scene::Start()
{
	void(__thiscall * _Start)(Scene * _this) = (void(__thiscall*)(Scene*))0x89A100;

	_Start(this);
}

void Scene::Load(const char* szSceneName)
{
	(*(void (__thiscall*)(Scene*, const char*))0x8980C0)(this, szSceneName);
}

void Scene::UpdateLoadedBlocks(int unk1, int unk2)
{
	(*(void(__thiscall*)(Scene*, int, int))0x8986E0)(this, unk1, unk2);
}

void Scene::ForceLodCalculation(int unk)
{
	if (m_pQuadTree)
		m_pQuadTree->CalculateLodForAllChildren();
	else
		for (EditorCamera* pEditorCamera = m_pEditorCamera; pEditorCamera; pEditorCamera = (EditorCamera*)field_34)
			ForceLodCalculation(unk);
}

void Scene::RefreshChildNodes()
{
	(*(void(__thiscall*)(Scene*))0x88C2B0)(this);
}

void Scene::FinishCreation(const char* logtitle)
{
	debug("Load Time: '%s'. %dms of %dms.\n", logtitle, Performance::GetMilliseconds() - m_nTimeMilliseconds, Performance::GetMilliseconds() - *(DWORD64*)&m_nLoadTime);
	m_nTimeMilliseconds = Performance::GetMilliseconds();
}

void Scene::UpdateCamera()
{
	Camera* sceneCamera = nullptr;

	if (m_pCamera && (sceneCamera = m_pUnkEntityPtr) != NULL && m_nPlayMode != 1)
		m_pCameraEntity = m_bQuadTreesAllocated ? sceneCamera : sceneCamera;
	else
		if (m_pUnkEntityPtr)
			m_pCameraEntity = m_pUnkEntityPtr;
		else if (sceneCamera)
			m_pCameraEntity = sceneCamera;
		else
			m_pCameraEntity = nullptr;

	if (!m_pCameraEntity)
		return;

	Camera::StoreCameraMatrix();

	if ((
		((m_vCameraPosition.x - Camera::CameraPosition.x) * (m_vCameraPosition.x - Camera::CameraPosition.x)) +
		((m_vCameraPosition.y - Camera::CameraPosition.y) * (m_vCameraPosition.y - Camera::CameraPosition.y)) +
		((m_vCameraPosition.z - Camera::CameraPosition.z) * (m_vCameraPosition.z - Camera::CameraPosition.z))
		) > 100.0f
		)
	{
		debug("Camera has moved more than 10m. Forcing Lod-calculation\n");
		ForceLodCalculation(0);
	}

	m_vCameraPosition = Camera::CameraPosition;
}