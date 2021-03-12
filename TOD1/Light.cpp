#include "Light.h"
#include "GfxInternal_Dx9.h"

Light* Light::LightType1;
Light* Light::LightType2;
unsigned int Light::TotalLights;
LightsListStruct Light::LightsList;
ScriptType_Entity* tLight;

#pragma message(TODO_IMPLEMENTATION)
Light::~Light()
{
	MESSAGE_CLASS_DESTROYED(Light);
}

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

#pragma message(TODO_IMPLEMENTATION)
void Light::AddLightToList(void* list, Light* light)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Light::SetLightType(LightType)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Light::SetLightColorRGB(const ColorRGB&)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Light::OverrideLights(bool unk)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Light::InitLightsList()
{
	/*
	LightsList = new List<Light>;
	LightType1 = (Light*)tLight->CreateNode();
	LightType2 = (Light*)tLight->CreateNode();

	LightType1->SetLightType(LIGHT_TYPE_1);
	LightType1->SetLightColorRGB({ .6f, .6f, .6f, 1.f });
	LightType2->SetLightType(LIGHT_TYPE_2);
	LightType2->SetLightColorRGB({ 1.f, 1.f, 1.f, 1.f });
	LightType2->SetOrient({ .85f, -0.13f, -0.37f, -0.32f });
	LightType2->SetPos({ 0.f, 0.f, 0.f, 0.f });

	LightsList->AddElement(LightType1);
	LightsList->AddElement(LightType2);
	*/
}

void Light::ClearLightsList()
{
	//delete LightsList;
}

Light_Properties::Light_Properties()
{
	m_Vec_1;
	m_Vec_2 = Vector3f(0.0f, 0.0f, 1.0f);
	field_0 = -1;
	m_Range = 100.0f;
	m_Brightness = 1.0f;
	field_24 = NULL & 0xFFFFFFE2 | 34;
}

Light_Properties::~Light_Properties()
{
	//g_GfxInternal_Dx9->_45E5D0(*this);
}