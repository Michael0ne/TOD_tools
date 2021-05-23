#include "Light.h"
#include "GfxInternal_Dx9.h"

Light* Light::AmbientLight;
Light* Light::DirectionalLight;
unsigned int Light::TotalLights;
Light::LightsList* Light::GlobalList;

EntityType* tLight;

#pragma message(TODO_IMPLEMENTATION)
Light::~Light()
{
	MESSAGE_CLASS_DESTROYED(Light);
}

#pragma message(TODO_IMPLEMENTATION)
Light::Light() : Node(NODE_MASK_POSITION)
{
	MESSAGE_CLASS_CREATED(Light);
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

void Light::InitLightsList()
{
	GlobalList = new LightsList;

	AmbientLight = (Light*)tLight->CreateNode();
	DirectionalLight = (Light*)tLight->CreateNode();

	AmbientLight->SetLightType(AMBIENT);
	AmbientLight->SetLightColorRGB({ 0.64999998, 0.64999998, 0.64999998, 1 });

	DirectionalLight->SetLightType(DIRECTIONAL);
	DirectionalLight->SetLightColorRGB({ 0.64999998, 0.64999998, 0.64999998, 1 });
	DirectionalLight->SetOrient({ 0.85898501, -0.139645, -0.37349701, -0.321161 });
	DirectionalLight->SetPos({});

	GlobalList->m_StaticLights.push_back(AmbientLight);
	GlobalList->m_StaticLights.push_back(DirectionalLight);
}

void Light::ClearLightsList()
{
	delete GlobalList;
}

#pragma message(TODO_IMPLEMENTATION)
void Light::Register()
{
	tLight = new EntityType("Light");
	tLight->InheritFrom(tNode);
	tLight->SetCreator((EntityType::CREATOR)Create);

	tLight->_86E9B0();
}

Light* Light::Create(AllocatorIndex)
{
	return new Light;
}

Light_Properties::Light_Properties()
{
	MESSAGE_CLASS_CREATED(Light_Properties);

	m_Position = { 0, 0, 0 };
	m_Direction = { 0, 0, 1 };
	m_Color = -1;
	m_Range = 100;
	m_Brightness = 1;
	m_Flags = m_Flags & 0xFFFFFFE2 | 34;
}

Light_Properties::~Light_Properties()
{
	g_GfxInternal_Dx9->RemoveLightFromScene(this);
}

#pragma message(TODO_IMPLEMENTATION)
Light::LightsList::LightsList()
{
	MESSAGE_CLASS_CREATED(LightsList);
}