#include "Scene.h"

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

	m_pCamera = nullptr;
	field_64 = 0;
	m_bQuadTreesAllocated = 0;

	//	TODO: initialize lots more stuff here...
}

inline void Scene::SetFixedFramerate(float framerate)
{
	m_FixedFramerate = true;
	m_FixedFramerateVal = 1.0f / framerate;
}

void Scene::Start()
{
	(*(void(__thiscall*)(Scene*))0x89A100)(this);
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