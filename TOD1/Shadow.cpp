#include "Shadow.h"
#include "AssetManager.h"
#include "Scene.h"

#include "StringType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "TruthType.h"

int Shadow::TotalShadows;
Vector4f Shadow::_A12B70 = { 1000, 1000, 1000, 0 };
std::vector<Vector4f> Shadow::_A12B60;
std::vector<int> Shadow::_A3E09C;
MeshBuffer* Shadow::ShadowMeshBuffer;

EntityType* tShadow;

Shadow::Shadow() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Shadow);
}

Shadow::~Shadow()
{
    MESSAGE_CLASS_DESTROYED(Shadow);
}

void Shadow::Destroy()
{
    TotalShadows--;

    if (TotalShadows == 1)
        delete ShadowMeshBuffer;

    delete m_FrameBuffer_1;
    delete m_FrameBuffer_2;

    m_CollisionProbe->Destroy();

    field_80 = 0;

    Node::Destroy();
}

void Shadow::Instantiate()
{
    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
    m_QuadTree->Refresh();

    if (m_BoneName.Empty() || m_Parent == Scene::SceneInstance)
        m_Bone = nullptr;
    else
        m_Bone = (Node*)m_Parent->FindNode(m_BoneName.m_Str);

    field_80 = 0;
    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_Id._3 = true;
}

#pragma message(TODO_IMPLEMENTATION)
void Shadow::Render()
{
}

void Shadow::Register()
{
    tShadow = new EntityType("Shadow");
    tShadow->InheritFrom(tNode);
    tShadow->SetCreator((CREATOR)Create);

    tShadow->RegisterProperty(tSTRING, "Texture", (EntityGetterFunction)&GetTexture, (EntitySetterFunction)&SetTexture, "control=resource|type=*.bmp");
    tShadow->RegisterProperty(tNUMBER, "Shadowradius", (EntityGetterFunction)&GetShadowRadius, (EntitySetterFunction)&SetShadowRadius, nullptr);
    tShadow->RegisterProperty(tNUMBER, "ShadowradiusX", (EntityGetterFunction)&GetShadowRadiusX, (EntitySetterFunction)&SetShadowRadiusX, "control=slider|min=0|max=10");
    tShadow->RegisterProperty(tNUMBER, "ShadowradiusY", (EntityGetterFunction)&GetShadowRadiusY, (EntitySetterFunction)&SetShadowRadiusY, "control=slider|min=0|max=10");
    tShadow->RegisterProperty(tSTRING, "Bonename", (EntityGetterFunction)&GetBoneName, (EntitySetterFunction)&SetBoneName, "control=string");
    tShadow->RegisterProperty(tINTEGER, "Complexity", (EntityGetterFunction)&GetComplexity, (EntitySetterFunction)&SetComplexity, "control=dropdown|Simple=0|MeshPrecise=1");
    tShadow->RegisterProperty(tTRUTH, "AlignRotation", (EntityGetterFunction)&GetAlignRotation, (EntitySetterFunction)&SetAlignRotation, "control=truth");
    tShadow->RegisterProperty(tNUMBER, "Opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1");

    tShadow->RegisterScript("command_clear_hints", (EntityFunctionMember)&ClearHints);

    tShadow->PropagateProperties();
}

Shadow* Shadow::Create(AllocatorIndex)
{
    return new Shadow;
}

void Shadow::ClearHints()
{
    m_HintNode = nullptr;
}

void Shadow::SetOpacity(float opacity)
{
    if (opacity > 1.f)
        opacity = 1.f;
    else if (opacity <= 0.f)
        opacity = 0.f;

    m_Flags.Opacity = (unsigned char)(opacity * 255.f);
    m_Id._3 = true;
}

const float Shadow::GetOpacity() const
{
    return (float)(m_Flags.Opacity * (1.f / 255.f));
}

void Shadow::SetAlignRotation(const bool align)
{
    m_Flags.AlignRotation = align;
    m_Id._3 = true;
}

const bool Shadow::GetAlignRotation() const
{
    return m_Flags.AlignRotation;
}

void Shadow::SetComplexity(const ShadowComplexity complexity)
{
    m_Flags.Complexity = complexity;
    field_80 = nullptr;
    m_Id._3 = true;
}

const ShadowComplexity Shadow::GetComplexity() const
{
    //  NOTE: original code implies that ShadowComplexity is a unsigned short, but only 2 values are present in the release build.
    return (ShadowComplexity)m_Flags.Complexity;
}

void Shadow::SetBoneName(const char* const bonename)
{
    if (!bonename || !bonename[0] || m_Parent == Scene::SceneInstance)
    {
        m_Bone = nullptr;
        m_Id._3 = true;

        return;
    }

    m_BoneName = bonename;
    m_Bone = (Node*)m_Parent->FindNode(bonename);
    m_Id._3 = true;
}

const char* const Shadow::GetBoneName() const
{
    return m_BoneName.m_Str;
}

void Shadow::SetShadowRadiusY(const float radiusy)
{
    m_ShadowRadiusY = radiusy;
    m_Id._3 = true;
}

const float Shadow::GetShadowRadiusY() const
{
    return m_ShadowRadiusY;
}

void Shadow::SetShadowRadiusX(const float radiusx)
{
    m_ShadowRadiusX = radiusx;
    m_Id._3 = true;
}

const float Shadow::GetShadowRadiusX() const
{
    return m_ShadowRadiusX;
}

void Shadow::SetShadowRadius(const float radius)
{
    if (Node::m_Flags.ERR)
        return;

    m_ShadowRadiusX = radius;
    m_ShadowRadiusY = radius;
    SetFlags(0xBFF | 0x400);
    LogDump::LogA("Fixed up shadow radius\n");
    m_Id._3 = true;
}

const float Shadow::GetShadowRadius() const
{
    return m_ShadowRadiusX;
}

void Shadow::SetTexture(const char* const texturepath)
{
    m_Texture = AssetLoader(texturepath);
    m_Id._3 = true;
}

const char* const Shadow::GetTexture() const
{
    return m_Texture.m_AssetPtr ? m_Texture.m_AssetPtr->GetName() : nullptr;
}