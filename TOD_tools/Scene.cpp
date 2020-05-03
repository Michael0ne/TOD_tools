#include "Scene.h"

#include "Renderer.h"
#include "LoadScreen.h"
#include "StreamedSoundBuffers.h"

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

	Allocators::MemoryAllocators::DefragmentIfNecessary(Allocators::AllocatorsList->ALLOCATOR_DEFRAGMENTING->field_1C);

	__int64 timeStart = __rdtsc();

	g_Scene->lpVtbl->Update(g_Scene);

	g_Renderer->_SetBufferStateByIndex(3, 0);

	bool loadingEnabled = false;

	if (!g_CurrentLoadScreen->m_bEnabled) {
		g_Scene->lpVtbl->_896370(g_Scene);
		loadingEnabled = true;
	}

	Scene::UnkInt_1++;

	g_Scene->m_nLoadTime = __rdtsc() - timeStart;

	if (Audio::g_StreamedSoundBuffers && g_Scene->m_pCameraEntity) {
		//Audio::g_StreamedSoundBuffers->SetListener3DPos(0);
	}

	//	TODO: implementation!

	return true;
}

bool Scene::Instantiate(const char* szSceneName)
{
	bool(__cdecl * _Instantiate)(const char* _scenename) = (bool(__cdecl*)(const char*))0x93CE00;

	return _Instantiate(szSceneName);
}

void Scene::Start()
{
	void(__thiscall * _Start)(Scene * _this) = (void(__thiscall*)(Scene*))0x89A100;

	_Start(this);
}

void Scene::ForceLodCalculation(int unk)
{
	if (m_pQuadTree)
		m_pQuadTree->CalculateLodForAllChildren();
	else
		for (EditorCamera* pEditorCamera = m_pEditorCamera; pEditorCamera; pEditorCamera = (EditorCamera*)field_34)
			ForceLodCalculation(unk);
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