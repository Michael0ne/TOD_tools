#include "Model.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "StringType.h"
#include "TruthType.h"
#include "Light.h"
#include "SkinnedMeshBuffer.h"
#include "Scene.h"
#include "Placeholder.h"

EntityType* tModel;

#pragma message(TODO_IMPLEMENTATION)
Model::Model() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE | NODE_MASK_FRAGMENT)
{
    MESSAGE_CLASS_CREATED(Model);
}

#pragma message(TODO_IMPLEMENTATION)
Model::~Model()
{
    MESSAGE_CLASS_DESTROYED(Model);
}

Vector4f* Model::GetBounds(Vector4f& unk)
{
    if (0)
    {
        if (m_ModelRes.m_AssetPtr)
            ((ModelAsset*)m_ModelRes.m_AssetPtr)->GetBoundingRadius(unk);
        else
            m_Id._3 = 8;
    }
    else
        unk = { 0, 0, 0, 1000.f };

    return &unk;
}

void Model::stub19()
{
}

void Model::SetStaticLightingEnabled(const bool enabled)
{
    if (m_ModelFlags.m_FlagBits.StaticallyLit != enabled)
        m_Id._3 = true;

    m_ModelFlags.m_FlagBits.StaticallyLit = enabled;
    m_Id._3 = 8;

    Light::GetGlobalList()->m_StaticLightsTotal++;
}

void Model::Register()
{
    tModel = new EntityType("Model");
    tModel->InheritFrom(tNode);
    tModel->SetCreator((CREATOR)Create);

    //  NOTE: msep - Message SEParator?
    tModel->RegisterProperty(tTRUTH, "msep1", (EntityGetterFunction)&GetMsep1, (EntitySetterFunction)&SetMsep1, "control=drawline|name=Assets");
    tModel->RegisterProperty(tSTRING, "modelres", (EntityGetterFunction)&GetModelRes, (EntitySetterFunction)&SetModelAsset, "control=resource|type=*.model", 10);
    tModel->RegisterProperty(tTRUTH, "msep2", (EntityGetterFunction)&GetMsep2, (EntitySetterFunction)&SetMsep2, "control=drawline|name=Rendering settings");
    tModel->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1", 12);
    tModel->RegisterProperty(tTRUTH, "addblend", (EntityGetterFunction)&GetAddBlend, (EntitySetterFunction)&SetAddBlend, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "disablezwrite", (EntityGetterFunction)&GetZWriteDisabled, (EntitySetterFunction)&SetZWriteDisabled, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "use_hard_alpha_factor", (EntityGetterFunction)&GetUseHardAlphaFactor, (EntitySetterFunction)&SetUseHardAlphaFactor, "control=truth|name=");
    tModel->RegisterProperty(tNUMBER, "hard_alpha_factor", (EntityGetterFunction)&GetHardAlphaFactor, (EntitySetterFunction)&SetHardAlphaFactor, "control=slider|min=0|max=1");
    tModel->RegisterProperty(tINTEGER, "active_texture_set", (EntityGetterFunction)&GetActiveTextureSet, (EntitySetterFunction)&SetActiveTextureSet, "control=slider|min=0|max=32", 11);
    tModel->RegisterProperty(tINTEGER, "number_of_textures_sets", (EntityGetterFunction)&GetNumberOfTextureSets, nullptr, nullptr);
    tModel->RegisterProperty(tINTEGER, "solo_pivot", (EntityGetterFunction)&GetSoloPivot, (EntitySetterFunction)&SetSoloPivot, "control=slider|min=0|max=32");
    tModel->RegisterProperty(tTRUTH, "place_in_hud", (EntityGetterFunction)&GetPlaceInHud, (EntitySetterFunction)&SetPlaceInHud, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "use_virtual_hud", (EntityGetterFunction)&GetUseVirtualHud, (EntitySetterFunction)&SetUseVirtualHud, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "msep3", (EntityGetterFunction)&GetMsep3, (EntitySetterFunction)&SetMsep3, "control=drawline|name=Light settings");
    tModel->RegisterProperty(tTRUTH, "dynamically_lit", (EntityGetterFunction)&GetDynamicallyLit, (EntitySetterFunction)&SetDynamicallyLit, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "statically_lit", (EntityGetterFunction)&GetStaticallyLit, (EntitySetterFunction)&SetStaticallyLit, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "backside_transparent", (EntityGetterFunction)&GetBacksideTransparent, (EntitySetterFunction)&SetBacksideTransparent, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "single_color_mode", (EntityGetterFunction)&GetSingleColorMode, (EntitySetterFunction)&SetSingleColorMode, "control=truth|name=");
    tModel->RegisterProperty(tTRUTH, "blocks_static_light", (EntityGetterFunction)&GetBlocksStaticLight, (EntitySetterFunction)&SetBlocksStaticLight, "control=truth|name=");
    tModel->RegisterProperty(DataType::LoadScript("list(integer)"), "pivot_hide_list", (EntityGetterFunction)&GetPivotHideList, (EntitySetterFunction)&SetPivotHideList, nullptr);
    tModel->RegisterProperty(tTRUTH, "depth_sorted", (EntityGetterFunction)&GetDepthSorted, (EntitySetterFunction)&SetDepthSorted, "control=truth|name=");

    tModel->RegisterScript("findpivot(string):integer", (EntityFunctionMember)&FindPivot);
    tModel->RegisterScript("setmodelresfrommodel(entity)", (EntityFunctionMember)&SetModelResFromModel);
    tModel->RegisterScript("getpivotrelativepos(integer):vector", (EntityFunctionMember)&GetPivotRelativePos);
    tModel->RegisterScript("forceinstantiate", (EntityFunctionMember)&ForceInstantiate);

    tModel->PropagateProperties();
}

#pragma message(TODO_IMPLEMENTATION)
void Model::SetModelAsset(Model* model)
{
    const char* val = m_ModelRes.m_AssetPtr ? m_ModelRes.m_AssetPtr->GetName() : nullptr;
    StoreProperty(10, val, tSTRING);
}

void Model::_884530()
{
    for (auto it = m_AttachedEffects.cbegin(); it != m_AttachedEffects.cend(); it++)
    {
        if (it->m_Node && it->m_Node->m_ScriptEntity == tNode && strcmp(it->m_Node->GetScript(), "") == NULL && it->m_Node->m_Flags.m_Flags != NULL)
        {
            it->m_Node->SetFlags(it->m_Node->m_Flags.m_Flags & 0xFFE); // NOTE: remove 'Disabled' flag.
            it->m_Node->SetFlags(it->m_Node->m_Flags.m_Flags & 0xF7F | 0x80); // NOTE: set flag 'DER'.
        }
    }
}

void Model::FindPivot(int* args) const
{
    *args = ((ModelAsset*)m_ModelRes.m_AssetPtr)->PivotIndexByName((const char* const)args[1]);
}

void Model::SetModelResFromModel(int* args)
{
    SetModelAsset((Model*)*args);
}

void Model::ForceInstantiate()
{
    Instantiate();
}

const char* Model::GetModelRes() const
{
    return m_ModelRes.m_AssetPtr ? m_ModelRes.m_AssetPtr->GetName() : nullptr;
}

void Model::SetModelRes(const char* const arg)
{
    char* respath = (char*)arg;
    if (Scene::_A3D858 && this && m_ScriptEntity != nullptr)
    {
        EntityType* placeholderEntity = m_ScriptEntity;
        while (tPlaceholder != placeholderEntity)
        {
            placeholderEntity = placeholderEntity->m_Parent;
            if (!placeholderEntity)
                break;
        }

        if (placeholderEntity)
        {
            if (arg && strlen(arg))
                respath = nullptr;
        }
    }

    if (m_ModelRes.m_AssetPtr)
        respath = (char*)m_ModelRes.m_AssetPtr->GetName();
    else
        respath = nullptr;

    StoreProperty(10, &respath, tSTRING);
    m_ModelRes = AssetLoader(respath);
    
    //  TODO: flags...

    TryInstantiate();
    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_Id._3 = 9;

    if (m_ModelRes.m_AssetPtr && strstr(m_ModelRes.m_AssetPtr->GetName(), "_Marker_"))
    {
        SetFadeThreshold(0.0099999998f);
        LogDump::LogA("repaired marker fade threshold to 0.01\n");
    }

    stub19();
}

const float Model::GetOpacity() const
{
    return m_ModelFlags.m_FlagBits.Opacity * (float)(1 / 255);
}

void Model::SetOpacity(float op)
{
    op *= 255.f;
    if (op != m_ModelFlags.m_FlagBits.Opacity)
    {
        m_ModelFlags.m_FlagBits._25 = true;
        const float v = m_ModelFlags.m_FlagBits.Opacity * (float)(1 / 255);
        StoreProperty(12, &v, tNUMBER);
    }

    m_ModelFlags.m_FlagBits.Opacity = (unsigned int)op;
}

void Model::SetAddBlend(const bool enabled)
{
    m_ModelFlags.m_FlagBits.AddBlend = enabled;
    m_Id._3 = true;
}

const bool Model::GetAddBlend() const
{
    return m_ModelFlags.m_FlagBits.AddBlend;
}

void Model::SetZWriteDisabled(const bool disabled)
{
    m_ModelFlags.m_FlagBits.DisableZWrite = disabled;
    m_Id._3 = true;
}

const bool Model::GetZWriteDisabled() const
{
    return m_ModelFlags.m_FlagBits.DisableZWrite;
}

void Model::SetUseHardAlphaFactor(const bool enabled)
{
    m_ModelFlags.m_FlagBits.UseHardAlphaFactor = enabled;
    m_Id._3 = true;
}

const bool Model::GetUseHardAlphaFactor() const
{
    return m_ModelFlags.m_FlagBits.UseHardAlphaFactor;
}

void Model::SetHardAlphaFactor(const float fac)
{
    m_AlphaFlags.m_FlagBits.HardAlphaFactor = (unsigned int)(fac * 255.f);
    m_Id._3 = true;
}

const float Model::GetHardAlphaFactor() const
{
    return m_AlphaFlags.m_FlagBits.HardAlphaFactor * (float)(1 / 255);
}

void Model::SetActiveTextureSet(const unsigned int set)
{
    if (m_ActiveTextureSet != set)
    {
        StoreProperty(11, &m_ActiveTextureSet, tINTEGER);
        m_ActiveTextureSet = set;
        m_Id._3 = true;
    }
}

const short Model::GetActiveTextureSet() const
{
    return m_ActiveTextureSet;
}

#pragma message(TODO_IMPLEMENTATION)
const int Model::GetNumberOfTextureSets() const
{
    if (!m_ModelRes.m_AssetPtr)
        return 1;

    if (((ModelAsset*)m_ModelRes.m_AssetPtr)->m_MeshList.size() <= 0)
        return 1;

    //for (unsigned int i = 0; i < m_ModelRes->m_MeshList.size(); ++i)
    // if (!m_ModelRes->m_MeshList[i].m_SkinnedMeshesList)
    //  return m_ModelRes->m_MeshList[i].m_SkinnedMeshesList->m_TextureSets;

    return 1;
}

const short Model::GetSoloPivot() const
{
    return m_SoloPivot;
}

void Model::SetSoloPivot(const short piv)
{
    m_SoloPivot = piv;
    TryInstantiate();
    m_Id._3 = true;
}

void Model::SetPlaceInHud(const bool place)
{
    m_ModelFlags.m_FlagBits.PlaceInHud = place;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const bool Model::GetPlaceInHud() const
{
    return m_ModelFlags.m_FlagBits.PlaceInHud;
}

void Model::SetUseVirtualHud(const bool use)
{
    m_ModelFlags.m_FlagBits.UseVirtualHud = use;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const bool Model::GetUseVirtualHud() const
{
    return m_ModelFlags.m_FlagBits.UseVirtualHud;
}

void Model::SetDynamicallyLit(const bool lit)
{
    if (m_ModelFlags.m_FlagBits.DynamicallyLit != lit)
        m_Id._3 = true;

    m_Id._3 = 8;
    m_ModelFlags.m_FlagBits.DynamicallyLit = lit;
    Light::GetGlobalList()->m_StaticLightsTotal++;
}

const bool Model::GetDynamicallyLit() const
{
    return m_ModelFlags.m_FlagBits.DynamicallyLit;
}

void Model::SetStaticallyLit(const bool lit)
{
    SetStaticLightingEnabled(lit);
}

const bool Model::GetStaticallyLit() const
{
    return m_ModelFlags.m_FlagBits.StaticallyLit;
}

void Model::SetBacksideTransparent(const bool tr)
{
    m_ModelFlags.m_FlagBits.BacksideTransparent = tr;
}

const bool Model::GetBacksideTransparent() const
{
    return m_ModelFlags.m_FlagBits.BacksideTransparent;
}

void Model::SetSingleColorMode(const bool enabled)
{
    if (m_ModelFlags.m_FlagBits.SingleColorMode != enabled)
        TryInstantiate();

    m_ModelFlags.m_FlagBits.SingleColorMode = enabled;
}

const bool Model::GetSingleColorMode() const
{
    return m_ModelFlags.m_FlagBits.SingleColorMode;
}

void Model::SetBlocksStaticLight(const bool enabled)
{
    m_ModelFlags.m_FlagBits.BlocksStaticLight = enabled;
}

const bool Model::GetBlocksStaticLight() const
{
    return m_ModelFlags.m_FlagBits.BlocksStaticLight;
}

#pragma message(TODO_IMPLEMENTATION)
void Model::SetPivotHideList(int* list)
{
    m_Id._3 = true;
    //m_PivotHideList = list; // TODO: this is a struct, copy c-tor invocation here.
}

#pragma message(TODO_IMPLEMENTATION)
const int* Model::GetPivotHideList()
{
    m_Id._3 = true;
    return m_PivotHideList;
}

void Model::SetDepthSorted(const bool enabled)
{
    m_ModelFlags.m_FlagBits.DepthSorted = enabled;
}

const bool Model::GetDepthSorted() const
{
    return m_ModelFlags.m_FlagBits.DepthSorted;
}

void Model::GetMsep1() const
{
}

void Model::SetMsep1() const
{
}

void Model::GetPivotRelativePos(int* args) const
{
    Vector4f pivotPos;
    GetPivotRelativePos_Impl(pivotPos, args[3]);

    *(Vector4f*)args = pivotPos;
}

#pragma message(TODO_IMPLEMENTATION)
void Model::GetPivotRelativePos_Impl(Vector4f& outPos, const unsigned int pivotId) const
{
}

void Model::GetMsep2() const
{
}

void Model::SetMsep2() const
{
}

void Model::GetMsep3() const
{
}

void Model::SetMsep3() const
{
}

void Model::SetLightingFromAsset(AssetLoader* assload, Folder_* associatedFolderPtr)
{
    m_ModelLighting = (MeshColorAsset*)assload->m_AssetPtr;
    m_AssocFolder = associatedFolderPtr;

    TryInstantiate();

    m_Id._3 = true;
}

Model* Model::Create(AllocatorIndex)
{
    return new Model();
}