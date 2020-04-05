#include "Scene.h"

bool Scene::Update()
{
	bool(__cdecl * _Update)() = (bool(__cdecl*)())0x93CEB0;

	return _Update();
}