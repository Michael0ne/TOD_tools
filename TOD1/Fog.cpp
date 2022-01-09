#include "Fog.h"
#include "BuiltinType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"

EntityType* tFog;

Fog::Fog() : Node(NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Fog);

    m_FogStart = 100.0f;
    m_FogEnd = 1000.0f;
    m_FogType = 1;
    m_Density = 0.02f;
    m_FogColor = BuiltinType::ColorWhite;

    m_QuadTree->field_1C = m_QuadTree->field_1C & 0xFFFFFF | m_QuadTree->field_1C & 0xFF000000 | 0x8000000;
}

Fog::~Fog()
{
    MESSAGE_CLASS_DESTROYED(Fog);
}

void Fog::Register()
{
    tFog = new EntityType("Fog");
    tFog->InheritFrom(tNode);
    tFog->SetCreator((EntityType::CREATOR)Create);

    tFog->RegisterProperty(tINTEGER, "fog_type", &GetFogType, 0, 0, 0, &SetFogType, 0, 0, 0, "control=string", 0, 0, -1);
    tFog->RegisterProperty(tNUMBER, "start", &GetStart, 0, 0, 0, &SetStart, 0, 0, 0, "control=string", 0, 0, 10);
    tFog->RegisterProperty(tNUMBER, "end", &GetEnd, 0, 0, 0, &SetEnd, 0, 0, 0, "control=string", 0, 0, 11);
    tFog->RegisterProperty(tNUMBER, "density", &GetDensity, 0, 0, 0, &SetDensity, 0, 0, 0, "control=string", 0, 0, -1);
    tFog->RegisterProperty(tVECTOR, "color_rgb", &GetColorRGB, 0, 0, 0, &SetColorRGB, 0, 0, 0, "control=colorrgb", 0, 0, 12);

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
    SetParam(10, &m_FogStart, tNUMBER);
    m_FogStart = fogstart;
}

const float Fog::GetEnd() const
{
    return m_FogEnd;
}

void Fog::SetEnd(float fogend)
{
    SetParam(11, &m_FogEnd, tNUMBER);
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