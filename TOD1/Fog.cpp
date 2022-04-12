#include "Fog.h"
#include "BuiltinType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "Scene.h"

EntityType* tFog;

Fog::~Fog()
{
    MESSAGE_CLASS_DESTROYED(Fog);
}

void Fog::Render()
{
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[2], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[3], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[4], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[5], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[6], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[7], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[8], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[9], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[10], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[11], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[12], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[13], false);
    SetPropertiesForFrameBuffer(Scene::SceneInstance->m_FrameBuffers[16], true);
}

void Fog::Register()
{
    tFog = new EntityType("Fog");
    tFog->InheritFrom(tNode);
    tFog->SetCreator((CREATOR)Create);

    tFog->RegisterProperty(tINTEGER, "fog_type", (EntityGetterFunction)&GetFogType, (EntitySetterFunction)&SetFogType, "control=string");
    tFog->RegisterProperty(tNUMBER, "start", (EntityGetterFunction)&GetStart, (EntitySetterFunction)&SetStart, "control=string", 10);
    tFog->RegisterProperty(tNUMBER, "end", (EntityGetterFunction)&GetEnd, (EntitySetterFunction)&SetEnd, "control=string", 11);
    tFog->RegisterProperty(tNUMBER, "density", (EntityGetterFunction)&GetDensity, (EntitySetterFunction)&SetDensity, "control=string");
    tFog->RegisterProperty(tVECTOR, "color_rgb", (EntityGetterFunction)&GetColorRGB, (EntitySetterFunction)&SetColorRGB, "control=colorrgb", 12);

    tFog->PropagateProperties();
}

Fog* Fog::Create(AllocatorIndex)
{
    return new Fog();
}

const int Fog::GetFogType() const
{
    return m_FogType;
}

void Fog::SetFogType(const int fogtype)
{
    m_FogType = fogtype;
}

const float Fog::GetStart() const
{
    return m_FogStart;
}

void Fog::SetStart(const float fogstart)
{
    StoreProperty(10, &m_FogStart, tNUMBER);
    m_FogStart = fogstart;
}

const float Fog::GetEnd() const
{
    return m_FogEnd;
}

void Fog::SetEnd(float fogend)
{
    StoreProperty(11, &m_FogEnd, tNUMBER);
    m_FogEnd = fogend;
}

const float Fog::GetDensity() const
{
    return m_Density;
}

void Fog::SetDensity(const float fogdensity)
{
    m_Density = fogdensity;
}

const ColorRGB& Fog::GetColorRGB() const
{
    return m_FogColor;
}

void Fog::SetColorRGB(const ColorRGB& fogcolor)
{
    m_FogColor = fogcolor;
}