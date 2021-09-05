#include "Model.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "StringType.h"
#include "Light.h"
#include "SkinnedMeshBuffer.h"

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

void Model::SetStaticLightingEnabled(const bool enabled)
{
 if (m_ModelFlags.m_FlagBits.StaticallyLit != enabled)
  m_Id |= 8;

 m_ModelFlags.m_FlagBits.StaticallyLit = enabled;
 m_Id |= 64;

 Light::GetGlobalList()->m_StaticLightsTotal++;
}

#pragma message(TODO_IMPLEMENTATION)
void Model::Register()
{
 tModel = new EntityType("Model");
 tModel->InheritFrom(tNode);
 tModel->SetCreator((EntityType::CREATOR)Create);
}

#pragma message(TODO_IMPLEMENTATION)
void Model::SetModelAsset(Model* model)
{
 const char* val = m_ModelRes ? m_ModelRes->AddResToOpenListAndReturnName() : nullptr;
 SetParam(10, val, tSTRING);
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
 *args = m_ModelRes->PivotIndexByName((const char* const)args[1]);
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
 return m_ModelRes ? m_ModelRes->AddResToOpenListAndReturnName() : nullptr;
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
  SetParam(12, &v, tNUMBER);
 }

 m_ModelFlags.m_FlagBits.Opacity = (unsigned int)op;
}

void Model::SetAddBlend(const bool enabled)
{
 m_ModelFlags.m_FlagBits.AddBlend = enabled;
 m_Id |= 8;
}

const bool Model::GetAddBlend() const
{
 return m_ModelFlags.m_FlagBits.AddBlend;
}

void Model::SetZWriteDisabled(const bool disabled)
{
 m_ModelFlags.m_FlagBits.DisableZWrite = disabled;
 m_Id |= 8;
}

const bool Model::GetZWriteDisabled() const
{
 return m_ModelFlags.m_FlagBits.DisableZWrite;
}

void Model::SetUseHardAlphaFactor(const bool enabled)
{
 m_ModelFlags.m_FlagBits.UseHardAlphaFactor = enabled;
 m_Id |= 8;
}

const bool Model::GetUseHardAlphaFactor() const
{
 return m_ModelFlags.m_FlagBits.UseHardAlphaFactor;
}

void Model::SetHardAlphaFactor(const float fac)
{
 m_AlphaFlags.m_FlagBits.HardAlphaFactor = (unsigned int)(fac * 255.f);
 m_Id |= 8;
}

const float Model::GetHardAlphaFactor() const
{
 return m_AlphaFlags.m_FlagBits.HardAlphaFactor * (float)(1 / 255);
}

void Model::SetActiveTextureSet(const unsigned int set)
{
 if (m_ActiveTextureSet != set)
 {
  SetParam(11, &m_ActiveTextureSet, tINTEGER);
  m_ActiveTextureSet = set;
  m_Id |= 8;
 }
}

const short Model::GetActiveTextureSet() const
{
 return m_ActiveTextureSet;
}

#pragma message(TODO_IMPLEMENTATION)
const int Model::GetNumberOfTextureSets() const
{
 if (!m_ModelRes)
  return 1;

 if (m_ModelRes->m_MeshList.size() <= 0)
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
 m_Id |= 8;
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
  m_Id |= 8;

 m_Id |= 64;
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
 m_Id |= 8;
 //m_PivotHideList = list; // TODO: this is a struct, copy c-tor invocation here.
}

#pragma message(TODO_IMPLEMENTATION)
const int* Model::GetPivotHideList()
{
 m_Id |= 8;
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

Model* Model::Create(AllocatorIndex)
{
 return new Model();
}