#include <algorithm>

#include "StaticLighting.h"
#include "TruthType.h"
#include "StringType.h"
#include "IntegerType.h"
#include "NumberType.h"

EntityType* tStaticLighting;

#pragma message(TODO_IMPLEMENTATION)
StaticLighting::~StaticLighting()
{
    MESSAGE_CLASS_DESTROYED(StaticLighting);
}

void StaticLighting::Instantiate()
{
    Node::Instantiate();

    if (!m_LightingAsset)
        return;

    Node* n = _8A6EB0();
    if (n)
        _8A6E10(n, n);
}

const bool StaticLighting::GetLsep1() const
{
    return false;
}

void StaticLighting::SetLsep1(const int)
{
}

const char* const StaticLighting::GetLightingFile() const
{
    return m_LightingAsset.m_AssetPtr ? m_LightingAsset.m_AssetPtr->GetName() : nullptr;
}

void StaticLighting::SetLightingFile(const char* const filename)
{
    m_LightingAsset = AssetLoader(filename);
    TryInstantiate();
}

const StaticLighting::eBlockId StaticLighting::GetTargetBlockId() const
{
    return m_TargetBlockId;
}

void StaticLighting::SetTargetBlockId(const eBlockId blockid)
{
    m_TargetBlockId = blockid;
}

const StaticLighting::eLightingMethod StaticLighting::GetLightingMethod() const
{
    return m_LightingMethod;
}

void StaticLighting::SetLightingMethod(const eLightingMethod method)
{
    m_LightingMethod = method;
}

const bool StaticLighting::GetLsep2() const
{
    return false;
}

void StaticLighting::SetLsep2(const int)
{
}

const int StaticLighting::GetNumPhotons() const
{
    return m_NumPhotons;
}

void StaticLighting::SetNumPhotons(const int numphotons)
{
    m_NumPhotons = numphotons;
}

const float StaticLighting::GetChanceOfReflection() const
{
    return m_ChanceOfReflection;
}

void StaticLighting::SetChanceOfReflection(const float chance)
{
    m_ChanceOfReflection = clamp<float>(chance, 0.f, 0.949f);
}

const float StaticLighting::GetGammaCorrection() const
{
    return m_GammaCorrection;
}

void StaticLighting::SetGammaCorrection(const float gammacorr)
{
    m_GammaCorrection = gammacorr;
}

const float StaticLighting::GetSingleColorMultiplier() const
{
    return m_SingleColorMultiplier;
}

void StaticLighting::SetSingleColorMultiplier(const float mult)
{
    m_SingleColorMultiplier = mult;
}

const bool StaticLighting::GetLsep3() const
{
    return false;
}

void StaticLighting::SetLsep3(const int)
{
}

const float StaticLighting::GetMaxSampleRadius() const
{
    return m_MaxSampleRadius;
}

void StaticLighting::SetMaxSampleRadius(const float radius)
{
    m_MaxSampleRadius = radius;
}

const int StaticLighting::GetSamplesPerVert() const
{
    return m_SamplesPerVert;
}

void StaticLighting::SetSamplesPerVert(const int samppervert)
{
    m_SamplesPerVert = samppervert;
}

const int StaticLighting::GetMinSamplePhotons() const
{
    return m_MinSamplePhotons;
}

void StaticLighting::SetMinSamplePhotons(const int sampphotons)
{
    m_MinSamplePhotons = sampphotons;
}

const float StaticLighting::GetMaxLodSampleRadius() const
{
    return m_MaxLodSampleRadius;
}

void StaticLighting::SetMaxLodSampleRadius(const float sampradius)
{
    m_MaxLodSampleRadius = sampradius;
}

const bool StaticLighting::GetLsep4() const
{
    return false;
}

void StaticLighting::SetLsep4(const int)
{
}

const float StaticLighting::GetCutoffArea() const
{
    return m_CutoffArea;
}

void StaticLighting::SetCutoffArea(const float cutoffarea)
{
    m_CutoffArea = cutoffarea;
}

const bool StaticLighting::GetLsep5() const
{
    return false;
}

void StaticLighting::SetLsep5(const int)
{
}

void StaticLighting::CalculateAll(int* args)
{
}

void StaticLighting::CalculateAnchor(int* args)
{
}

void StaticLighting::RedoSampling(int* args)
{
}

void StaticLighting::RedoSamplingAnchor(int* args)
{
}

void StaticLighting::Save(int* args)
{
}

void StaticLighting::SaveAs(int* args)
{
}

void StaticLighting::Register()
{
    tStaticLighting = new EntityType("StaticLighting");
    tStaticLighting->InheritFrom(tNode);
    tStaticLighting->SetCreator((CREATOR)Create);

    tStaticLighting->RegisterProperty(tTRUTH, "lsep1", (EntityGetterFunction)&GetLsep1, (EntitySetterFunction)&SetLsep1, "control=drawline|name=Settings for Static Lighting");
    tStaticLighting->RegisterProperty(tSTRING, "lightingfile", (EntityGetterFunction)&GetLightingFile, (EntitySetterFunction)&SetLightingFile, "control=resource");
    tStaticLighting->RegisterProperty(tINTEGER, "target_block_id", (EntityGetterFunction)&GetTargetBlockId, (EntitySetterFunction)&SetTargetBlockId, "control=dropdown|Main=0|Map=1|Submap=2|Mission=3|Cutscene=4");
    tStaticLighting->RegisterProperty(tINTEGER, "lighting_method", (EntityGetterFunction)&GetLightingMethod, (EntitySetterFunction)&SetLightingMethod, "control=dropdown|Phong=0|Photon_mapping=1|Geometry_Photon_Tracing=2");
    tStaticLighting->RegisterProperty(tTRUTH, "lsep2", (EntityGetterFunction)&GetLsep2, (EntitySetterFunction)&SetLsep2, "control=drawline|name=Settings for Photon mapping and Geometry Photon Tracing");
    tStaticLighting->RegisterProperty(tINTEGER, "num_photons", (EntityGetterFunction)&GetNumPhotons, (EntitySetterFunction)&SetNumPhotons, "control=string");
    tStaticLighting->RegisterProperty(tNUMBER, "chance_of_reflection", (EntityGetterFunction)&GetChanceOfReflection, (EntitySetterFunction)&SetChanceOfReflection, "control=slider|min=0.0|max=0.95");
    tStaticLighting->RegisterProperty(tNUMBER, "gamma_correction", (EntityGetterFunction)&GetGammaCorrection, (EntitySetterFunction)&SetGammaCorrection, "control=slider|min=1.0|max=10.0");
    tStaticLighting->RegisterProperty(tNUMBER, "singlecolormultiplier", (EntityGetterFunction)&GetSingleColorMultiplier, (EntitySetterFunction)&SetSingleColorMultiplier, "control=slider|min=0.1|max=2.0");
    tStaticLighting->RegisterProperty(tTRUTH, "lsep3", (EntityGetterFunction)&GetLsep3, (EntitySetterFunction)&SetLsep3, "control=drawline|name=Settings for Photon mapping");
    tStaticLighting->RegisterProperty(tNUMBER, "max_sample_radius", (EntityGetterFunction)&GetMaxSampleRadius, (EntitySetterFunction)&SetMaxSampleRadius, "control=slider|min=0.1|max=10");
    tStaticLighting->RegisterProperty(tINTEGER, "samples_per_vert", (EntityGetterFunction)&GetSamplesPerVert, (EntitySetterFunction)&SetSamplesPerVert, "control=slider|min=1|max=100");
    tStaticLighting->RegisterProperty(tINTEGER, "min_sample_photons", (EntityGetterFunction)&GetMinSamplePhotons, (EntitySetterFunction)&SetMinSamplePhotons, "control=slider|min=1|max=300");
    tStaticLighting->RegisterProperty(tNUMBER, "max_lod_sample_radius", (EntityGetterFunction)&GetMaxLodSampleRadius, (EntitySetterFunction)&SetMaxLodSampleRadius, "control=slider|min=0.1|max=10");
    tStaticLighting->RegisterProperty(tTRUTH, "lsep4", (EntityGetterFunction)&GetLsep4, (EntitySetterFunction)&SetLsep4, "control=drawline|name=Settings for Geometry Photon Tracing");
    tStaticLighting->RegisterProperty(tNUMBER, "cutoff_area", (EntityGetterFunction)&GetCutoffArea, (EntitySetterFunction)&SetCutoffArea, "control=slider|min=0.1|max=10");
    tStaticLighting->RegisterProperty(tTRUTH, "lsep5", (EntityGetterFunction)&GetLsep5, (EntitySetterFunction)&SetLsep5, "control=drawline|name=Controls for calculation and saving");
    tStaticLighting->RegisterScript("calculate_all", (EntityFunctionMember)&CalculateAll, 0, 0, 0, "control=button|text=CalculateAll");
    tStaticLighting->RegisterScript("calculate_anchor", (EntityFunctionMember)&CalculateAnchor, 0, 0, 0, "control=button|text=CalculateAnchorOnly");
    tStaticLighting->RegisterScript("redo_sampling", (EntityFunctionMember)&RedoSampling, 0, 0, 0, "control=button|text=RedoSampling");
    tStaticLighting->RegisterScript("redo_sampling_anchor", (EntityFunctionMember)&RedoSamplingAnchor, 0, 0, 0, "control=button|text=RedoSamplingAnchorOnly");
    tStaticLighting->RegisterScript("save", (EntityFunctionMember)&Save, 0, 0, 0, "control=button|text=Save");
    tStaticLighting->RegisterScript("saveas", (EntityFunctionMember)&SaveAs, 0, 0, 0, "control=button|text=SaveAs");

    tStaticLighting->PropagateProperties();
}

#pragma message(TODO_IMPLEMENTATION)
Node* StaticLighting::_8A6EB0()
{
    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void StaticLighting::_8A6E10(Node*, Node*)
{
}

StaticLighting* StaticLighting::Create(AllocatorIndex)
{
    return new StaticLighting;
}