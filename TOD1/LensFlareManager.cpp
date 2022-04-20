#include "LensFlareManager.h"
#include "CollisionProbe.h"
#include "NumberType.h"

EntityType* tLensFlareManager;

LensFlareManager::LensFlareManager() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(LensFlareManager);

    field_64 = NULL;
    m_FrameBuffer = nullptr;
    m_FadeDistance = 100;
    m_LineCheckLength = 100;
    field_68 = { 10000, 10000, 10000, 0 };
    m_SizeReduction = 0;
    m_EmitterSize = 0;

    for (unsigned int i = 0; i < 4; ++i)
    {
        m_CollisionProbes[i] = (CollisionProbe*)tCollisionProbe->CreateNode();
        m_CollisionProbes[i]->m_ScriptEntity = tCollisionProbe;
        m_CollisionProbes[i]->SetFlags(m_CollisionProbes[i]->m_Flags.m_Flags & 0xFDF | 0x20);
        m_CollisionProbes[i]->Reset_Impl();
        m_CollisionProbes[i]->SetPos({ 1000, 1000, 1000, 0 });
    }
}

LensFlareManager::~LensFlareManager()
{
    MESSAGE_CLASS_DESTROYED(LensFlareManager);
}

void LensFlareManager::Destroy()
{
    delete m_FrameBuffer;
    for (unsigned int i = 0; i < 4; ++i)
        delete m_CollisionProbes[i];

    Node::Destroy();
}

const float LensFlareManager::GetSizeReduction() const
{
    return m_SizeReduction;
}

void LensFlareManager::SetSizeReduction(const float sizereduction)
{
    m_SizeReduction = sizereduction;
}

const float LensFlareManager::GetFadeDistance() const
{
    return m_FadeDistance;
}

void LensFlareManager::SetFadeDistance(const float fadedistance)
{
    m_FadeDistance = fadedistance;
}

const float LensFlareManager::GetEmitterSize() const
{
    return m_EmitterSize;
}

void LensFlareManager::SetEmitterSize(const float emittersize)
{
    m_EmitterSize = emittersize;
}

const float LensFlareManager::GetOcclusionFactor() const
{
    return m_OcclusionFactor;
}

const float LensFlareManager::GetLineCheckLength() const
{
    return m_LineCheckLength;
}

void LensFlareManager::SetLineCheckLength(const float length)
{
    m_LineCheckLength = length;
}

void LensFlareManager::Register()
{
    tLensFlareManager = new EntityType("LensFlareManager");
    tLensFlareManager->InheritFrom(tNode);
    tLensFlareManager->SetCreator((CREATOR)Create);

    tLensFlareManager->RegisterProperty(tNUMBER, "SizeReduction", (EntityGetterFunction)&GetSizeReduction, (EntitySetterFunction)&SetSizeReduction, "control=slider|min=0|max=1");
    tLensFlareManager->RegisterProperty(tNUMBER, "FadeDistance", (EntityGetterFunction)&GetFadeDistance, (EntitySetterFunction)&SetFadeDistance, "control=slider|min=10|max=100");
    tLensFlareManager->RegisterProperty(tNUMBER, "EmitterSize", (EntityGetterFunction)&GetEmitterSize, (EntitySetterFunction)&SetEmitterSize, "control=slider|min=0|max=10");
    tLensFlareManager->RegisterProperty(tNUMBER, "OcclusionFactor", (EntityGetterFunction)&GetOcclusionFactor, nullptr, nullptr);
    tLensFlareManager->RegisterProperty(tNUMBER, "LineCheckLength", (EntityGetterFunction)&GetLineCheckLength, (EntitySetterFunction)&SetLineCheckLength, "control=slider|min=10|max=2000");

    tLensFlareManager->PropagateProperties();
}

LensFlareManager* LensFlareManager::Create(AllocatorIndex)
{
    return new LensFlareManager;
}