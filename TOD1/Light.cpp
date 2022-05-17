#include "Light.h"
#include "GfxInternal_Dx9.h"
#include "BuiltinType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "TruthType.h"
#include "Scene.h"

Light* Light::AmbientLight;
Light* Light::DirectionalLight;
unsigned int Light::TotalLights;
Light::LightsList* Light::GlobalList;
int Light::StaticLights;

EntityType* tLight;

Light::~Light()
{
    MESSAGE_CLASS_DESTROYED(Light);
}

void Light::Destroy()
{
    --TotalLights;

    GetGlobalList()->RemoveLight(this);

    Node::Destroy();
}

void Light::Instantiate()
{
    DirectX::XMMATRIX mat;
    Vector4f dir;

    Node::Instantiate();
    GetWorldMatrix(mat);

    GetLocalSpaceDirection(dir, BuiltinType::InVector);

}

const bool Light::HasDynamicEmission() const
{
    return m_Flags.DynamicEmission;
}

void Light::SetHasDynamicEmission(const bool emission)
{
    GetGlobalList()->RemoveLight(this);

    m_Flags.DynamicEmission = emission;

    GetGlobalList()->AddLightToList(this);
}

const bool Light::HasStaticEmission() const
{
    return m_Flags.StaticEmission;
}

void Light::SetHasStaticEmission(const bool emission)
{
    if (emission == m_Flags.StaticEmission || !m_Flags.DynamicEmission)
        return;

    if (m_Flags.Type == eLightType::AMBIENT || m_Flags.Type == eLightType::DIRECTIONAL)
    {
        GetGlobalList()->_880D90(Scene::SceneInstance);
    }
    else
    {
        Vector4f pos;
        GetWorldPos(pos);
        GetGlobalList()->SetRange(pos, m_LightProperties.m_Range);
    }

    m_Flags.StaticEmission = emission;
}

const eLightType Light::GetLightType() const
{
    return m_Flags.Type;
}

Light::Light() : Node(NODE_MASK_POSITION)
{
    MESSAGE_CLASS_CREATED(Light);

    if (m_LightProperties.m_Flags.LightType != 0)
        m_LightProperties.m_Flags.Enabled = true;

    m_Flags.Type = eLightType::POINT;
    m_Flags.DynamicEmission = true;
    m_Flags._6 = true;
    m_LightColor = BuiltinType::ColorWhite;
    m_StaticColor = BuiltinType::ColorWhite;
    m_Flags.StaticallyLit = false;
    ++TotalLights;
    m_Flags.NegativeLight = false;
    m_Vec_1 = BuiltinType::ZeroVector;
    m_Vec_2 = { 0, 1, 0, 0 };
    field_C8 = 0x10000000;

    GetGlobalList()->AddLightToList(this);
}

#pragma message(TODO_IMPLEMENTATION)
void Light::LightsList::AddLightToList(Light* light)
{
}

void Light::SetLightType(const eLightType ltype)
{
    GetGlobalList()->RemoveLight(this);
    const int thisLightType = (int)m_Flags.Type - 2;
    int newLightType = -1;

    m_Flags.Type = ltype;

    switch (thisLightType)
    {
    case 0:
        newLightType = 2;
        break;
    case 1:
        newLightType = 0;
        break;
    default:
        newLightType = 3;
    }

    if (m_LightProperties.m_Flags.LightType != newLightType)
        m_LightProperties.m_Flags.LightType = newLightType;

    GetGlobalList()->AddLightToList(this);
}

void Light::GetLightColorRGB(ColorRGB& outColor) const
{
    outColor = m_LightColor;
}

void Light::SetLightColorRGB(const ColorRGB& color)
{
    m_LightColor = color;
    m_LightProperties.m_Color = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);
    m_LightProperties.m_Flags.Enabled = true;
}

void Light::GetStaticColorRGB(ColorRGB& outColor) const
{
    if (m_Flags.Type != eLightType::NONE)
        outColor = m_LightColor;
    else
        outColor = m_StaticColor;
}

void Light::SetStaticColorRGB(const ColorRGB& color)
{
    m_Flags.StaticallyLit = true;
    m_StaticColor = color;
}

const float Light::GetBrightness() const
{
    return m_LightProperties.m_Brightness;
}

void Light::SetBrightness(const float brightness)
{
    float actualBrightness = clamp<float>(brightness, 0.f, 2.f);

    if (m_LightProperties.m_Brightness != actualBrightness)
    {
        m_LightProperties.m_Flags.Enabled = true;
        m_LightProperties.m_Brightness = actualBrightness;
    }
}

const float Light::GetRange() const
{
    return m_LightProperties.m_Range;
}

void Light::SetRange(const float range)
{
    if (m_LightProperties.m_Range != range)
    {
        m_LightProperties.m_Flags.Enabled = true;
        m_LightProperties.m_Range = range;
    }
}

const bool Light::IsNegativeLight() const
{
    return m_Flags.NegativeLight;
}

void Light::SetIsNegativeLight(const bool negative)
{
    m_Flags.NegativeLight = negative;
}

void Light::InitLightsList()
{
    GlobalList = new LightsList;

    AmbientLight = (Light*)tLight->CreateNode();
    DirectionalLight = (Light*)tLight->CreateNode();

    AmbientLight->SetLightType(eLightType::AMBIENT);
    AmbientLight->SetLightColorRGB({ 0.64999998, 0.64999998, 0.64999998, 1 });

    DirectionalLight->SetLightType(eLightType::DIRECTIONAL);
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

Light::LightsList* Light::GetGlobalList()
{
    return GlobalList;
}

void Light::Register()
{
    tLight = new EntityType("Light");
    tLight->InheritFrom(tNode);
    tLight->SetCreator((CREATOR)Create);

    tLight->RegisterProperty(tTRUTH, "dynamic_emission", (EntityGetterFunction)&HasDynamicEmission, (EntitySetterFunction)&SetHasDynamicEmission, "control=truth|name=|");
    tLight->RegisterProperty(tTRUTH, "static_emission", (EntityGetterFunction)&HasStaticEmission, (EntitySetterFunction)&SetHasStaticEmission, "control=truth|name=|");
    tLight->RegisterProperty(tINTEGER, "light_type", (EntityGetterFunction)&GetLightType, (EntitySetterFunction)&SetLightType, "control=dropdown|Ambient=1|Directional=2|Point=3");
    tLight->RegisterProperty(tVECTOR, "light_color_rgb", (EntityGetterFunction)&GetLightColorRGB, (EntitySetterFunction)&SetLightColorRGB, "control=colorrgb");
    tLight->RegisterProperty(tVECTOR, "static_color_rgb", (EntityGetterFunction)&GetStaticColorRGB, (EntitySetterFunction)&SetStaticColorRGB, "control=colorrgb");
    tLight->RegisterProperty(tNUMBER, "brightness", (EntityGetterFunction)&GetBrightness, (EntitySetterFunction)&SetBrightness, "control=slider|min=0|max=10");
    tLight->RegisterProperty(tNUMBER, "range", (EntityGetterFunction)&GetRange, (EntitySetterFunction)&SetRange, "control=slider|min=0|max=1000");
    tLight->RegisterProperty(tTRUTH, "is_negative_light", (EntityGetterFunction)&IsNegativeLight, (EntitySetterFunction)&SetIsNegativeLight, "control=truth|name=|");

    tLight->PropagateProperties();
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
    m_Flags.Enabled = true;
    m_Flags.LightType = 1;
}

Light_Properties::~Light_Properties()
{
    g_GfxInternal_Dx9->RemoveLightFromScene(this);
}

void Light_Properties::SetPosition(const Vector3f& position)
{
    if (m_Position == position)
        return;

    m_Position = position;
    m_Flags.Enabled = true;
}

void Light_Properties::SetDirection(const Vector3f& direction)
{
    if (m_Direction == direction)
        return;

    m_Direction = direction;
    m_Flags.Enabled = true;
}

#pragma message(TODO_IMPLEMENTATION)
Light::LightsList::LightsList()
{
    MESSAGE_CLASS_CREATED(LightsList);

    m_StaticLights.clear();
    //  TODO: allocate space for something.
    field_10 = new int[6];  //  TODO: ctor at 0x882370.
    m_StaticLightsTotal = 0;
}

#pragma message(TODO_IMPLEMENTATION)
void Light::LightsList::RemoveLight(Light* light)
{
    if (light == AmbientLight || light == DirectionalLight)
        return;
}

void Light::LightsList::_880D90(Node* node)
{
    Node* child = node->m_FirstChild;
    node->m_Id._3 = 8;

    if (!child)
        return;

    while (child)
    {
        _880D90(child);
        child = child->m_NextSibling;
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Light::LightsList::SetRange(const Vector4f& lightPos, const float range) const
{
    if (!Scene::MainQuadTree)
        return;
}

void Light::LightsList::AddStaticLight(Light* light)
{
    m_StaticLights.push_back(light);
}

void Light::LightsList::RemoveAt(const unsigned int pos)
{
    m_StaticLights.erase(m_StaticLights.begin() + pos);
}

void Light::LightsList::OverrideLights(const bool overrideLights)
{
    m_LightsOverride = overrideLights;
    m_StaticLightsTotal++;

    if (Scene::SceneInstance)
    {
        Node* child = Scene::SceneInstance->m_FirstChild;
        Scene::SceneInstance->m_Id._3 = 8;

        while (child)
        {
            _880D90(child);
            child = child->m_NextSibling;
        }
    }
}