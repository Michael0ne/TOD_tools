#include "Scene.h"
#include "SceneNode.h"
#include "Builtin.h"

int& Scene::_A3DCCC = *(int*)0xA3DCCC;
int& Scene::GameTime = *(int*)0xA3DCD4;
int& Scene::_A3DCD0 = *(int*)0xA3DCD0;
int& Scene::_A3DCE4 = *(int*)0xA3DCE4;
int& Scene::NewFrameNumber = *(int*)0xA3DCE0;
bool& Scene::IsRewindBufferInUse = *(bool*)0xA1207C;

Scene::Scene() : Folder_()
{
	MESSAGE_CLASS_CREATED(Scene);

	m_List_1 = List<int>(0x13B00);
	m_List_2 = List<int>(0x1A300);
	m_List_3 = List<int>(0x25B00);
	m_List_4 = List<int>(0x1A300);
	m_List_5 = List<int>(0x1CB00);
	m_List_6 = List<int>(0x27B00);	//	NOTE: 3 lists below are initialized using while loop and pointers, why?
	m_List_7 = List<int>(0x27B00);
	m_List_8 = List<int>(0x27B00);

	m_sSaveDir = String();

	m_PlayMode = MODE_UNKNOWN_1;
	m_NodesWithUpdateOrBlockingScripts = 0;

	m_Buffer_1 = new Scene_Buffer(0, 36, 2);
	m_Buffer_2 = new Scene_Buffer(0, 36, 2);
	//	m_pSharedProbe = tCollisionProbe->CreateNode();

	patch(0xA3DCBC, this, 4);

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

//	TODO: implementation!
void Scene::Start()
{
	if (m_PlayMode != 1)
		return;

	m_PlayMode = 0;
	UpdateLoadedBlocks(1, 0);
	EnumSceneCamerasAndUpdate();
	_A3DCCC = NULL;
	GameTime = NULL;
	_A3DCD0 = NULL;
	_A3DCE4 = NULL;
	NewFrameNumber = NULL;

	int startScriptId = tBuiltin->GetMessageId("start");
	if (startScriptId >= 0)
		TriggerScriptForAllChildren(startScriptId, this, nullptr);

	_896810();
	tSceneNode->_874940();

	if (IsRewindBufferInUse)
	{
		AllocateRewindBuffer();
		FreeRewindBuffer(1);
	}

	m_RewindResumeTimeMs = NULL;
	_A3DCE4 = NULL;
	NewFrameNumber = NULL;
}

//	TODO: implementation!
bool Scene::GameUpdate()
{
	return (*(bool (*)())0x93CEB0)();
}

//	TODO: implementation!
Scene_Buffer::Scene_Buffer(int unk1, char unk2, int unk3)
{
}