#include "SurroundGeometry.h"
#include "FrameBuffer.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "StringType.h"

EntityType* tSurroundGeometry;

int SurroundGeometry::TotalCreated;

const int SurroundGeometry::GetCategory() const
{
    return m_Category;
}

void SurroundGeometry::SetCategory(const int category)
{
    m_Category = category;
}

const float SurroundGeometry::GetFarDistance() const
{
    return sqrtf(m_FarDistance);
}

void SurroundGeometry::SetFarDistance(const float distance)
{
    m_FarDistance = distance;
    TryInstantiate();
}

const float SurroundGeometry::GetCloseDistance() const
{
    return sqrtf(m_CloseDistance);
}

void SurroundGeometry::SetCloseDistance(const float distance)
{
    m_CloseDistance = distance;
    TryInstantiate();
}

const int SurroundGeometry::GetMaxCount() const
{
    return m_MaxCount;
}

void SurroundGeometry::SetMaxCount(const int count)
{
    m_MaxCount = count;
    TryInstantiate();
}

const float SurroundGeometry::GetVariation() const
{
    return m_Variation;
}

void SurroundGeometry::SetVariation(const float variation)
{
    m_Variation = variation;
    TryInstantiate();
}

const char* const SurroundGeometry::GetModel1() const
{
    return m_Model1 ? m_Model1.m_AssetPtr->GetName() : nullptr;
}

void SurroundGeometry::SetModel1(const char* const model)
{
    if (!model)
        return;

    m_Model1 = AssetLoader(model);
    TryInstantiate();
}

const float SurroundGeometry::GetProperbility1() const
{
    return m_Properbility1;
}

void SurroundGeometry::SetProperbility1(const float properbil)
{
    m_Properbility1 = properbil;
    TryInstantiate();
}

const char* const SurroundGeometry::GetModel2() const
{
    return m_Model2 ? m_Model2.m_AssetPtr->GetName() : nullptr;
}

void SurroundGeometry::SetModel2(const char* const model)
{
    if (!model)
        return;

    m_Model2 = AssetLoader(model);
    TryInstantiate();
}

const float SurroundGeometry::GetProperbility2() const
{
    return m_Properbility2;
}

void SurroundGeometry::SetProperbility2(const float properbil)
{
    m_Properbility2 = properbil;
    TryInstantiate();
}

const char* const SurroundGeometry::GetModel3() const
{
    return m_Model3 ? m_Model3.m_AssetPtr->GetName() : nullptr;
}

void SurroundGeometry::SetModel3(const char* const model)
{
    if (!model)
        return;

    m_Model3 = AssetLoader(model);
    TryInstantiate();
}

const float SurroundGeometry::GetProperbility3() const
{
    return m_Properbility3;
}

void SurroundGeometry::SetProperbility3(const float properbil)
{
    m_Properbility3 = properbil;
    TryInstantiate();
}

const char* const SurroundGeometry::GetModel4() const
{
    return m_Model4 ? m_Model4.m_AssetPtr->GetName() : nullptr;
}

void SurroundGeometry::SetModel4(const char* const model)
{
    if (!model)
        return;

    m_Model4 = AssetLoader(model);
    TryInstantiate();
}

const float SurroundGeometry::GetProperbility4() const
{
    return m_Properbility4;
}

void SurroundGeometry::SetProperbility4(const float properbil)
{
    m_Properbility4 = properbil;
    TryInstantiate();
}

const float SurroundGeometry::GetRandomSeed() const
{
    return (float)m_RandomSeed;
}

void SurroundGeometry::SetRandomSeed(const float seed)
{
    m_RandomSeed = (int)seed;
}

void SurroundGeometry::GenerateRandomSeed()
{
    m_RandomSeed = (int)(m_RandomSeed * 32867) + 1;
}

void SurroundGeometry::Register()
{
    tSurroundGeometry = new EntityType("SurroundGeometry");
    tSurroundGeometry->InheritFrom(tNode);
    tSurroundGeometry->SetCreator((CREATOR)Create);

    tSurroundGeometry->RegisterProperty(tINTEGER, "Category", (EntityGetterFunction)&GetCategory, (EntitySetterFunction)&SetCategory, "control=dropdown|0=0|1=1|2=2|3=3|4=4|5=5|6=6|7=7|8=8|9=9|10=10|11=11|12=12|13=13");
    tSurroundGeometry->RegisterProperty(tNUMBER, "FarDistance", (EntityGetterFunction)&GetFarDistance, (EntitySetterFunction)&SetFarDistance, "control=slider|min=0|max=30");
    tSurroundGeometry->RegisterProperty(tNUMBER, "CloseDistance", (EntityGetterFunction)&GetCloseDistance, (EntitySetterFunction)&SetCloseDistance, "control=slider|min=0|max=30");
    tSurroundGeometry->RegisterProperty(tINTEGER, "MaxCount", (EntityGetterFunction)&GetMaxCount, (EntitySetterFunction)&SetMaxCount, "control=slider|min=10|max=200");
    tSurroundGeometry->RegisterProperty(tNUMBER, "Variation", (EntityGetterFunction)&GetVariation, (EntitySetterFunction)&SetVariation, "control=slider|min=0|max=1");
    tSurroundGeometry->RegisterProperty(tSTRING, "Model1", (EntityGetterFunction)&GetModel1, (EntitySetterFunction)&SetModel1, "control=resource|type=*.model");
    tSurroundGeometry->RegisterProperty(tNUMBER, "Properbility1", (EntityGetterFunction)&GetProperbility1, (EntitySetterFunction)&SetProperbility1, "control=slider|min=0|max=1");
    tSurroundGeometry->RegisterProperty(tSTRING, "Model2", (EntityGetterFunction)&GetModel2, (EntitySetterFunction)&SetModel2, "control=resource|type=*.model");
    tSurroundGeometry->RegisterProperty(tNUMBER, "Properbility2", (EntityGetterFunction)&GetProperbility2, (EntitySetterFunction)&SetProperbility2, "control=slider|min=0|max=1");
    tSurroundGeometry->RegisterProperty(tSTRING, "Model3", (EntityGetterFunction)&GetModel3, (EntitySetterFunction)&SetModel3, "control=resource|type=*.model");
    tSurroundGeometry->RegisterProperty(tNUMBER, "Properbility3", (EntityGetterFunction)&GetProperbility3, (EntitySetterFunction)&SetProperbility3, "control=slider|min=0|max=1");
    tSurroundGeometry->RegisterProperty(tSTRING, "Model4", (EntityGetterFunction)&GetModel4, (EntitySetterFunction)&SetModel4, "control=resource|type=*.model");
    tSurroundGeometry->RegisterProperty(tNUMBER, "Properbility4", (EntityGetterFunction)&GetProperbility4, (EntitySetterFunction)&SetProperbility4, "control=slider|min=0|max=1");
    tSurroundGeometry->RegisterProperty(tNUMBER, "RandomSeed", (EntityGetterFunction)&GetRandomSeed, (EntitySetterFunction)&SetRandomSeed, nullptr);

    tSurroundGeometry->RegisterScript("GenerateRandomSeed", (EntityFunctionMember)&GenerateRandomSeed, 0, 0, 0, "control=button|text=GenerateRandomSeed");

    tSurroundGeometry->PropagateProperties();
}

SurroundGeometry* SurroundGeometry::Create(AllocatorIndex)
{
    return new SurroundGeometry;
}