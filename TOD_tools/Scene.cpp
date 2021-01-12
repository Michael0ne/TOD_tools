#include "Scene.h"
#include "SceneSaveLoad.h"
#include "LogDump.h"
#include "Performance.h"

int& Scene::RealTimeMs = *(int*)0xA3DCCC;
int& Scene::GameTimeMs = *(int*)0xA3DCD4;
int& Scene::_A3DCD0 = *(int*)0xA3DCD0;
int& Scene::_A3DCE4 = *(int*)0xA3DCE4;
int& Scene::NewFrameNumber = *(int*)0xA3DCE0;
bool& Scene::IsRewindBufferInUse = *(bool*)0xA1207C;
ScriptType_Entity* tScene = nullptr;

#pragma message(TODO_IMPLEMENTATION)
Scene::Scene() : Folder_()
{
	MESSAGE_CLASS_CREATED(Scene);

	m_List_1 = List<int>(0x13B00);
	m_QuadTreesList = List<AuxQuadTree>(0x1A300);
	m_ParticleSystemsList = List<ParticleSystem>(0x25B00);
	m_CollisionListList = List<CollisionList>(0x1A300);
	m_List_5 = List<int>(0x1CB00);
	m_List_6 = List<int>(0x27B00);	//	NOTE: 3 lists below are initialized using while loop and pointers, why?
	m_List_7 = List<int>(0x27B00);
	m_List_8 = List<int>(0x27B00);

	m_sSaveDir;

	SceneInstance = this;

	m_PlayMode = MODE_UNKNOWN_1;
	m_NodesWithUpdateOrBlockingScripts = 0;

	m_Buffer_1 = new Scene_Buffer(0, 36, 2);
	m_Buffer_2 = new Scene_Buffer(0, 36, 2);
	//	m_pSharedProbe = tCollisionProbe->CreateNode();

	m_EditorCamera = nullptr;
	m_GameCamera = nullptr;
	m_QuadTreesAllocated = 0;

	//	TODO: initialize lots more stuff here...
}

void Scene::SetFixedFramerate(float framerate)
{
	m_FixedFramerate = true;
	m_FixedFramerateVal = 1.0f / framerate;
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
	_A3DCD0 = NULL;
	_A3DCE4 = NULL;
	NewFrameNumber = NULL;

	//	FIXME: this is stupid! Make something about it, like templated structure for passing parameters...
	const char* params[] = { NULL, "start" };
	tBuiltin->GetMessageId(params);
	if ((int)params[0] >= 0)
		TriggerScriptForAllChildren((int)params[0], this, nullptr);

	_896810();
	g_SceneSaveLoad->_874940();

	if (IsRewindBufferInUse)
	{
		AllocateRewindBuffer();
		FreeRewindBuffer(1);
	}

	m_RewindResumeTimeMs = NULL;
	_A3DCE4 = NULL;
	NewFrameNumber = NULL;
}

#pragma message(TODO_IMPLEMENTATION)
void Scene::Load(const char* sceneName)
{
	(*(void(__thiscall*)(Scene*, const char*))0x8980C0)(this, sceneName);
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
	LogDump::LogA("Load Time: '%s'. %dms of %dms.\n", logTitle, Performance::GetMilliseconds() - m_TimeMs, Performance::GetMilliseconds() - m_nLoadTime[1]);
	m_TimeMs = Performance::GetMilliseconds();
}

#pragma message(TODO_IMPLEMENTATION)
bool Scene::GameUpdate()
{
	return (*(bool (*)())0x93CEB0)();
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

#pragma message(TODO_IMPLEMENTATION)
void Scene::FreeRewindBuffer(char unk)
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
Scene_Buffer::Scene_Buffer(int unk1, char unk2, int unk3)
{
}