#include "Light.h"

List<Light>& Light::g_LightsList = *(List<Light>*)0xA3D820;

List<Light>& Light::GetLightsList()
{
	return g_LightsList;
}

void Light::OverrideLights(bool unk)
{
	void(__thiscall * _OverrideLights)(List<Light> * _list, bool _unk) = (void(__thiscall*)(List<Light>*, bool))0x880DC0;

	_OverrideLights(&g_LightsList, unk);
}