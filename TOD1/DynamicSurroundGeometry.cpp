#include "DynamicSurroundGeometry.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "StringType.h"

EntityType* tDynamicSurroundGeometry;
int DynamicSurroundGeometry::TotalCreated;

#pragma message(TODO_IMPLEMENTATION)
DynamicSurroundGeometry::DynamicSurroundGeometry() : Node(NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(DynamicSurroundGeometry);
}

#pragma message(TODO_IMPLEMENTATION)
DynamicSurroundGeometry::~DynamicSurroundGeometry()
{
    MESSAGE_CLASS_DESTROYED(DynamicSurroundGeometry);
}

void DynamicSurroundGeometry::Render()
{
}

const int DynamicSurroundGeometry::GetCategory() const
{
    return m_Category;
}

void DynamicSurroundGeometry::SetCategory(const int category)
{
    m_Category = category;
}

const int DynamicSurroundGeometry::GetMaxCount() const
{
    return m_MaxCount;
}

void DynamicSurroundGeometry::SetMaxCount(const int maxcount)
{
    m_MaxCount = maxcount;
}

const float DynamicSurroundGeometry::GetRadius() const
{
    return m_Radius;
}

void DynamicSurroundGeometry::SetRadius(const float radius)
{
    m_Radius = radius;
}

const float DynamicSurroundGeometry::GetObjectDensity() const
{
    return m_ObjectDensity;
}

void DynamicSurroundGeometry::SetObjectDensity(const float density)
{
    m_ObjectDensity = density;
}

const float DynamicSurroundGeometry::GetSpawnRate() const
{
    return m_SpawnRate;
}

void DynamicSurroundGeometry::SetSpawnRate(const float spawnrate)
{
    m_SpawnRate = spawnrate < 25.f ? 25.f : spawnrate;
}

const char* const DynamicSurroundGeometry::GetTexture1() const
{
    return m_Texture[0] ? m_Texture[0].m_AssetPtr->GetName() : nullptr;
}

void DynamicSurroundGeometry::SetTexture1(const char* const texture1)
{
    if (!texture1)
        return;

    m_Texture[0] = AssetLoader(texture1);
    TryInstantiate();
}

const char* const DynamicSurroundGeometry::GetTexture2() const
{
    return m_Texture[1] ? m_Texture[1].m_AssetPtr->GetName() : nullptr;
}

void DynamicSurroundGeometry::SetTexture2(const char* const texture2)
{
    if (!texture2)
        return;

    m_Texture[1] = AssetLoader(texture2);
    TryInstantiate();
}

const char* const DynamicSurroundGeometry::GetTexture3() const
{
    return m_Texture[2] ? m_Texture[2].m_AssetPtr->GetName() : nullptr;
}

void DynamicSurroundGeometry::SetTexture3(const char* const texture3)
{
    if (!texture3)
        return;

    m_Texture[2] = AssetLoader(texture3);
    TryInstantiate();
}

const char* const DynamicSurroundGeometry::GetTexture4() const
{
    return m_Texture[3] ? m_Texture[3].m_AssetPtr->GetName() : nullptr;
}

void DynamicSurroundGeometry::SetTexture4(const char* const texture4)
{
    if (!texture4)
        return;

    m_Texture[3] = AssetLoader(texture4);
    TryInstantiate();
}

const float DynamicSurroundGeometry::GetSize() const
{
    return m_Size;
}

void DynamicSurroundGeometry::SetSize(const float size)
{
    m_Size = size;
}

const float DynamicSurroundGeometry::GetInitialHeight() const
{
    return m_InitialHeight;
}

void DynamicSurroundGeometry::SetInitialHeight(const float initialheight)
{
    m_InitialHeight = initialheight;
}

const float DynamicSurroundGeometry::GetMass() const
{
    return m_Mass;
}

void DynamicSurroundGeometry::SetMass(const float mass)
{
    m_Mass = mass;
}

const float DynamicSurroundGeometry::GetMassSize() const
{
    return m_MassSize;
}

void DynamicSurroundGeometry::SetMassSize(const float masssize)
{
    m_MassSize = masssize;
}

const float DynamicSurroundGeometry::GetWindFactor() const
{
    return m_WindFactor;
}

void DynamicSurroundGeometry::SetWindFactor(const float windfactor)
{
    m_WindFactor = windfactor;
}

const float DynamicSurroundGeometry::GetWindLift() const
{
    return m_WindLift;
}

void DynamicSurroundGeometry::SetWindLift(const float windlift)
{
    m_WindLift = windlift;
}

const float DynamicSurroundGeometry::GetWindLiftHeight() const
{
    return m_WindLiftHeight;
}

void DynamicSurroundGeometry::SetWindLiftHeight(const float windliftheight)
{
    m_WindLiftHeight = windliftheight;
}

const float DynamicSurroundGeometry::GetAirResistanceY() const
{
    return m_AirResistanceY;
}

void DynamicSurroundGeometry::SetAirResistanceY(const float airresy)
{
    m_AirResistanceY = airresy;
}

const float DynamicSurroundGeometry::GetAirResistanceXZ() const
{
    return m_AirResistanceXZ;
}

void DynamicSurroundGeometry::SetAirResistanceXZ(const float airresxz)
{
    m_AirResistanceXZ = airresxz;
}

const float DynamicSurroundGeometry::GetRotResistanceY() const
{
    return m_RotResistanceY;
}

void DynamicSurroundGeometry::SetRotResistanceY(const float rotresy)
{
    m_RotResistanceY = rotresy;
}

const float DynamicSurroundGeometry::GetRotResistanceXZ() const
{
    return m_RotResistanceXZ;
}

void DynamicSurroundGeometry::SetRotResistanceXZ(const float rotresxz)
{
    m_RotResistanceXZ = rotresxz;
}

const float DynamicSurroundGeometry::GetForceOffset() const
{
    return m_ForceOffset;
}

void DynamicSurroundGeometry::SetForceOffset(const float offset)
{
    m_ForceOffset = offset;
}

const float DynamicSurroundGeometry::GetForceRotate() const
{
    return m_ForceRotate;
}

void DynamicSurroundGeometry::SetForceRotate(const float rotate)
{
    m_ForceRotate = rotate;
}

void DynamicSurroundGeometry::Register()
{
    tDynamicSurroundGeometry = new EntityType("DynamicSurroundGeometry");
    tDynamicSurroundGeometry->InheritFrom(tNode);
    tDynamicSurroundGeometry->SetCreator((CREATOR)Create);

    tDynamicSurroundGeometry->RegisterProperty(tINTEGER, "Category", (EntityGetterFunction)&GetCategory, (EntitySetterFunction)&SetCategory, "control=dropdown|0=0|1=1|2=2|3=3|4=4|5=5|6=6|7=7|8=8|9=9|10=10|11=11|12=12|13=13");
    tDynamicSurroundGeometry->RegisterProperty(tINTEGER, "MaxCount", (EntityGetterFunction)&GetMaxCount, (EntitySetterFunction)&SetMaxCount, "control=string");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "Radius", (EntityGetterFunction)&GetRadius, (EntitySetterFunction)&SetRadius, "control=slider|min=0|max=250");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "ObjectDensity", (EntityGetterFunction)&GetObjectDensity, (EntitySetterFunction)&SetObjectDensity, "control=slider|min=0|max=0.1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "SpawnRate", (EntityGetterFunction)&GetSpawnRate, (EntitySetterFunction)&SetSpawnRate, "control=slider|min=0|max=25");
    tDynamicSurroundGeometry->RegisterProperty(tSTRING, "Texture1", (EntityGetterFunction)&GetTexture1, (EntitySetterFunction)&SetTexture1, "control=resource|type=*.bmp");
    tDynamicSurroundGeometry->RegisterProperty(tSTRING, "Texture2", (EntityGetterFunction)&GetTexture2, (EntitySetterFunction)&SetTexture2, "control=resource|type=*.bmp");
    tDynamicSurroundGeometry->RegisterProperty(tSTRING, "Texture3", (EntityGetterFunction)&GetTexture3, (EntitySetterFunction)&SetTexture3, "control=resource|type=*.bmp");
    tDynamicSurroundGeometry->RegisterProperty(tSTRING, "Texture4", (EntityGetterFunction)&GetTexture4, (EntitySetterFunction)&SetTexture4, "control=resource|type=*.bmp");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "Size", (EntityGetterFunction)&GetSize, (EntitySetterFunction)&SetSize, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "InitialHeight", (EntityGetterFunction)&GetInitialHeight, (EntitySetterFunction)&SetInitialHeight, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "Mass", (EntityGetterFunction)&GetMass, (EntitySetterFunction)&SetMass, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "MassSize", (EntityGetterFunction)&GetMassSize, (EntitySetterFunction)&SetMassSize, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "WindFactor", (EntityGetterFunction)&GetWindFactor, (EntitySetterFunction)&SetWindFactor, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "WindLift", (EntityGetterFunction)&GetWindLift, (EntitySetterFunction)&SetWindLift, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "WindLiftHeight", (EntityGetterFunction)&GetWindLiftHeight, (EntitySetterFunction)&SetWindLiftHeight, "control=slider|min=0|max=50");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "AirResistanceY", (EntityGetterFunction)&GetAirResistanceY, (EntitySetterFunction)&SetAirResistanceY, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "AirResistanceXZ", (EntityGetterFunction)&GetAirResistanceXZ, (EntitySetterFunction)&SetAirResistanceXZ, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "RotResistanceY", (EntityGetterFunction)&GetRotResistanceY, (EntitySetterFunction)&SetRotResistanceY, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "RotResistanceXZ", (EntityGetterFunction)&GetRotResistanceXZ, (EntitySetterFunction)&SetRotResistanceXZ, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "ForceOffset", (EntityGetterFunction)&GetForceOffset, (EntitySetterFunction)&SetForceOffset, "control=slider|min=0|max=1");
    tDynamicSurroundGeometry->RegisterProperty(tNUMBER, "ForceRotate", (EntityGetterFunction)&GetForceRotate, (EntitySetterFunction)&SetForceRotate, "control=slider|min=0|max=1");

    tDynamicSurroundGeometry->PropagateProperties();
}

DynamicSurroundGeometry* DynamicSurroundGeometry::Create(AllocatorIndex)
{
    return new DynamicSurroundGeometry;
}