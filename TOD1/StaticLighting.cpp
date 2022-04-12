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

    if (!m_LightingFile)
        return;

    Node* n = _8A6EB0();
    if (n)
        _8A6E10(n, n);
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