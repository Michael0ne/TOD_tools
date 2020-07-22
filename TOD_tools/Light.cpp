#include "Light.h"

int& Light::TotalLights = *(int*)0xA3D81C;	//	@A3D81C
List<Light>& Light::LightsList = *(List<Light>*)0xA3D820;	//	@A3D820

Light::Light() : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(Light);

	m_LightProperties = Light_Properties();
	m_Flags = m_Flags & 0xFFFFFFFB | 3;

	if (m_LightProperties.field_24 & 31)
		m_LightProperties.field_24 = m_LightProperties.field_24 & 0xFFFFFFE0 | 32;

	m_Flags = m_Flags & 0xFFFFFFEF | 72;
	m_LightColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
	m_StaticColor = ColorRGB(1.0f, 1.0f, 1.0f, 1.0f);
	m_Flags = m_Flags & 0xFFFFFF7F;
	m_Flags = m_Flags & 0xFFFFFFDF;

	m_Vec_1 = Vector4f();
	m_Vec_2 = Vector4f(0.0f, 1.0f, 0.0f, 0.0f);
	field_C8 = 0x10000000;

	AddLightToList(&LightsList, this);

	++TotalLights;
}

void Light::AddLightToList(void* list, Light* light)
{
	(*(void(__thiscall*)(void*, Light*))0x8812A0)(list, light);
}

void Light::OverrideLights(bool unk)
{
	(*(void(__thiscall*)(void*, bool))0x880DC0)(&LightsList, unk);
}

Light_Properties::Light_Properties()
{
	m_Vec_1 = Vector3f();
	m_Vec_2 = Vector3f(0.0f, 0.0f, 1.0f);
	field_0 = -1;
	m_Range = 100.0f;
	m_Brightness = 1.0f;
	field_24 = field_24 & 0xFFFFFFE2 | 34;
}