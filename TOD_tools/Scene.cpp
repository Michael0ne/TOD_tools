#include "Scene.h"

Scene* g_Scene = (Scene*)0xA3DCBC;

bool Scene::Update()
{
	bool(__cdecl * _Update)() = (bool(__cdecl*)())0x93CEB0;

	return _Update();
}

bool Scene::Instantiate(const char* szSceneName)
{
	bool(__thiscall * _Instantiate)(const char* _scenename) = (bool(__thiscall*)(const char*))0x93CE00;

	return _Instantiate(szSceneName);
}

void Scene::Start()
{
	void(__thiscall * _Start)(Scene * _this) = (void(__thiscall*)(Scene*))0x89A100;

	_Start(this);
}

void Scene::UpdateCamera()
{
	void(__thiscall * _UpdCamera)(Scene * _this) = (void(__thiscall*)(Scene*))0x893480;

	_UpdCamera(this);
}