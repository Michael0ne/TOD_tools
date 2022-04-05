#include "Cloth.h"
#include "PhysSystem.h"
#include "AssetManager.h"
#include "ModelAsset.h"
#include "FrameBuffer.h"
#include "MeshBuffer_Dx9.h"

#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"
#include "StringType.h"

EntityType* tCloth;
const float Cloth::MaximumImpulse = 9999999.0;

Cloth::Cloth() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Cloth);

    m_ModelAsset = nullptr;
    field_68 = 1;
    m_FrameBuffer = nullptr;
    m_MeshBuffer = nullptr;
    field_A8 = NULL;
    field_AC = 1;
    field_B0 = -1;
    m_PhysSystem = new PhysSystem(120, this, 1);
    m_Weight = 1;
    m_Damping = 1;
    m_Opacity = 1;
    m_WindInfluence = 10;
    m_Flags.Iterations = 2;
    m_BoundingRadius = { 0, 0, 0, 0.5 };

    m_PhysSystem->m_Weight = 1;
    m_PhysSystem->m_Damping = 0.0099999998f;
    m_PhysSystem->m_NumIterations = 2;
}

Cloth::~Cloth()
{
    MESSAGE_CLASS_DESTROYED(Cloth);

    delete m_FrameBuffer;
    delete m_PhysSystem;
    delete m_MeshBuffer;

    g_AssetManager->DecreaseResourceReferenceCount(m_ModelAsset);
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::Instantiate()
{
    Node::Instantiate();
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::Update()
{
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::Render()
{
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::ApplyImpulse_Impl(const Vector4f& v1, const Vector4f& v2, const float force)
{
}

Cloth* Cloth::Create(AllocatorIndex)
{
    return new Cloth;
}

const char Cloth::GetActiveTextureSet() const
{
    return m_Flags.TextureSet;
}

void Cloth::SetActiveTextureSet(const char set)
{
    int textureset = 1;

    if (m_ModelAsset && m_ModelAsset->m_MeshList.size())
        textureset = m_ModelAsset->m_MeshList[m_ModelAsset->m_MeshList.size()].m_SkinnedMeshesList[0]->m_TextureSets.size();

    if (set >= textureset)
        m_Flags.TextureSet = GetNumberOfTexturesSets() - 1;
    else
        m_Flags.TextureSet = set;

    m_Id._3 = true;
}

const int Cloth::GetNumberOfTexturesSets() const
{
    if (!m_ModelAsset)
        return 1;

    if (!m_ModelAsset->m_MeshList.size())
        return 1;

    unsigned int sets = 0;

    for (int meshIndex = 0; meshIndex < m_ModelAsset->m_MeshList.size(); ++meshIndex)
    {
        if (++sets >= m_ModelAsset->m_MeshList.size())
            return 1;
        else
            if (!m_ModelAsset->m_MeshList[meshIndex].m_SkinnedMeshesList.size())
                break;
    }

    return m_ModelAsset->m_MeshList[sets].m_SkinnedMeshesList.size();
}

const float Cloth::GetOpacity() const
{
    return m_Opacity;
}

void Cloth::SetOpacity(const float opacity)
{
    m_Opacity = opacity;
}

const bool Cloth::ShouldAddBlend() const
{
    return m_Flags.AddBlend;
}

void Cloth::SetShouldAddBlend(const bool add)
{
    m_Flags.AddBlend = add;
}

const char* Cloth::GetModelRes() const
{
    return m_ModelAsset ? m_ModelAsset->GetName() : nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void Cloth::SetModelRes(const char* modelres)
{
    AssetLoader assload(modelres);
    m_ModelAsset = (ModelAsset*)assload.m_AssetPtr;

    delete m_MeshBuffer;
    delete m_PhysSystem;

    //m_MeshBuffer = new MeshBuffer_Dx9(m_ModelAsset->m_MeshList[0].m_SkinnedMeshesList[0], 1);
    //m_PhysSystem = new PhysSystem(, this, 1);
}

const float Cloth::GetWeight() const
{
    return m_Weight;
}

void Cloth::SetWeight(const float weight)
{
    m_Weight = weight >= 0.1f ? weight : 0.1f;
    m_PhysSystem->m_Weight = weight >= 0.1f ? weight : 0.1f;
}

const float Cloth::GetWindInfluence() const
{
    return m_WindInfluence;
}

void Cloth::SetWindInfluence(const float influence)
{
    m_WindInfluence = influence;
    m_PhysSystem->m_WindInfluence = influence;
}

const float Cloth::GetDamping() const
{
    return m_Damping;
}

void Cloth::SetDamping(const float damping)
{
    m_Damping = damping;
    m_PhysSystem->m_Damping = damping * 0.009f;
}

const int Cloth::GetIterations() const
{
    return m_Flags.Iterations;
}

void Cloth::SetIterations(const char iterations)
{
    m_Flags.Iterations = iterations;
    m_PhysSystem->m_NumIterations = iterations;
}

const float Cloth::GetBoundingRadius() const
{
    return m_BoundingRadius.a;
}

void Cloth::SetBoundingRadius(const float radius)
{
    m_BoundingRadius.a = radius;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const bool Cloth::ShouldAlwaysUpdate() const
{
    return m_Flags.AllwaysUpdate;
}

void Cloth::SetShouldAlwaysUpdate(const bool update)
{
    m_Flags.AllwaysUpdate = update;
}

void Cloth::ApplyImpulse(int* args)
{
    const Vector4f vec1((float)args[0], (float)args[1], (float)args[2], 0);
    const Vector4f vec2((float)args[3], (float)args[4], (float)args[5], 0);
    const float force = args[6];

    ApplyImpulse_Impl(vec1, vec2, force);
}

void Cloth::Register()
{
    tCloth = new EntityType("Cloth");
    tCloth->InheritFrom(tNode);
    tCloth->SetCreator((CREATOR)Create);

    tCloth->RegisterProperty(tINTEGER, "active_texture_set", (EntityGetterFunction)&GetActiveTextureSet, (EntitySetterFunction)&SetActiveTextureSet, "control=slider|min=0|max=16");
    tCloth->RegisterProperty(tINTEGER, "number_of_textures_sets", (EntityGetterFunction)&GetNumberOfTexturesSets, nullptr, "control=string");
    tCloth->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1");
    tCloth->RegisterProperty(tTRUTH, "addblend", (EntityGetterFunction)&ShouldAddBlend, (EntitySetterFunction)&SetShouldAddBlend, "control=truth|name=");
    tCloth->RegisterProperty(tSTRING, "modelres", (EntityGetterFunction)&GetModelRes, (EntitySetterFunction)&SetModelRes, "control=resource|type=*.model");
    tCloth->RegisterProperty(tNUMBER, "Weight", (EntityGetterFunction)&GetWeight, (EntitySetterFunction)&SetWeight, "control=slider|min=0|max=5");
    tCloth->RegisterProperty(tNUMBER, "WindInfluence", (EntityGetterFunction)&GetWindInfluence, (EntitySetterFunction)&SetWindInfluence, "control=slider|min=0|max=100");
    tCloth->RegisterProperty(tNUMBER, "Damping", (EntityGetterFunction)&GetDamping, (EntitySetterFunction)&SetDamping, "control=slider|min=0|max=1");
    tCloth->RegisterProperty(tINTEGER, "Iterations", (EntityGetterFunction)&GetIterations, (EntitySetterFunction)&SetIterations, "control=slider|min=1|max=5");
    tCloth->RegisterProperty(tNUMBER, "BoundingRadius", (EntityGetterFunction)&GetBoundingRadius, (EntitySetterFunction)&SetBoundingRadius, "control=slider|min=0|max=20");
    tCloth->RegisterProperty(tTRUTH, "AllwaysUpdate", (EntityGetterFunction)&ShouldAlwaysUpdate, (EntitySetterFunction)&SetShouldAlwaysUpdate, "control=truth");

    tCloth->RegisterScript("ApplyImpulse(vector,vector,number)", (EntityFunctionMember)&ApplyImpulse, NULL, NULL, NULL, nullptr, nullptr);

    tCloth->PropagateProperties();
}