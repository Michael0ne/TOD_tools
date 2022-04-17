#include "AnimLayer.h"
#include "TruthType.h"
#include "AssetManager.h"
#include "AnimSlot.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "TransactionBuffer.h"

EntityType* tAnimLayer;

AnimLayer::AnimLayer() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(AnimLayer);

    m_GamePivotOrient = Orientation(0.0f, 0.0f, 0.0f, 1.0f);
    field_D4 = 0;
    m_PlayPos_1 = 0;
    field_FC = 0;
    m_PlayPos_2 = 0;
    m_TargetAnim = 1;
    m_BlendFinishedEvent = -1;
    m_Event = -1;
    m_Flags.StallFirstFrame = true;
    m_TargetAnim_Entity = nullptr;
    m_TargetAnim_Entity_2 = nullptr;
    m_MotionLayer = 0;
    m_Weight = 0;
    m_SpeedMultiplier = 1;
    m_CrossBlendFactor = 0;
    m_OverrideTarget.Target1 = false;
    m_OverrideTarget.Target2 = false;
    m_ClearOverrideTargetOnBlendFinished.Target1 = false;
    m_ClearOverrideTargetOnBlendFinished.Target2 = false;
}

AnimLayer::~AnimLayer()
{
    MESSAGE_CLASS_DESTROYED(AnimLayer);

    g_AssetManager->DecreaseResourceReferenceCount(field_FC);
    g_AssetManager->DecreaseResourceReferenceCount(field_D4);
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::Play(AnimLayer* animation, const unsigned int event)
{
/*
    if (!animation)
    {
        LogDump::LogA("NULL animation\n");
        return;
    }

    SetIsPlaying(true);
    SetIsLooping(false);

    if (m_TargetAnim != 1)
        StoreProperty(13, &m_TargetAnim, tINTEGER);

    SetCrossTarget1(animation);
    SetTargetCrossBlendFactor(0);

    if (m_CrossBlendSpeed != 0)
        StoreProperty(21, &m_CrossBlendSpeed, tNUMBER);

    m_CrossBlendSpeed = 0;
    SetPlayPos2(0);
*/
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::BlendTo(MotionAnimSlot* animSlot, const int event)
{
}

const bool AnimLayer::GetIsPlaying() const
{
    return m_Flags.Playing;
}

void AnimLayer::SetIsPlaying(const bool playing)
{
    const bool isplaying = m_Flags.Playing;

    if (m_Flags.Playing != playing)
        StoreProperty(10, &isplaying, tTRUTH);

    m_Flags.Playing = playing;
}

void AnimLayer::SetIsLooping(const bool loop)
{
    const bool loopflag = m_Flags.Looping;

    if (loop != loopflag)
        StoreProperty(11, &loopflag, tTRUTH);

    m_Flags.Looping = loop;
}

void AnimLayer::GetGamePivotPos(Vector4f& outPos) const
{
    outPos = m_GamePivotPos;
}

void AnimLayer::GetGamePivot(Vector4f& outPos) const
{
    outPos = m_GamePivotPosition;
}

void AnimLayer::GetGamePivotOrientA(Orientation& outOrient) const
{
    outOrient = m_GamePivotOrient;
}

void AnimLayer::GetGamePivotOrient(Orientation& outOrient) const
{
    outOrient = m_GamePivotOrient;
}

void AnimLayer::GetLoopMode(int& outLoopMode) const
{
    outLoopMode = m_TargetAnim == 1 ? m_TargetAnim_Entity->m_LoopMode : m_TargetAnim_Entity_2->m_LoopMode;
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::SetCrossTarget1(AnimLayer* target)
{
}

void AnimLayer::SetTargetCrossBlendFactor(const float blend)
{
    if (m_CrossBlendFactor != blend)
        StoreProperty(19, &m_CrossBlendFactor, tNUMBER);

    if (m_TargetAnim_Entity)
    {
        if (TransactionBuffer::_A11C90 == 1)
        {
            m_CrossBlendFactor = blend;
            field_140 = blend;
        }
        else
            m_CrossBlendFactor = ((1 - TransactionBuffer::_A11C90) * field_140) + (TransactionBuffer::_A11C90 * blend);
    }
    else
        if (m_TargetAnim_Entity_2)
            m_CrossBlendFactor = 1;
        else
            SetIsPlaying(false);
}

const float AnimLayer::GetWeight() const
{
    return m_Weight;
}

void AnimLayer::SetWeight(const float weight)
{
    if (weight != m_Weight)
        StoreProperty(14, &m_Weight, tNUMBER);

    if (TransactionBuffer::_A11C90 == 1.f)
    {
        m_Weight = weight;
        field_13C = weight;
    }
    else
    {
        m_Weight = ((1.f - TransactionBuffer::_A11C90) * field_13C) + (TransactionBuffer::_A11C90 * weight);
    }
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::Register()
{
    tAnimLayer = new EntityType("AnimLayer");
    tAnimLayer->InheritFrom(tNode);
    tAnimLayer->SetCreator((CREATOR)Create);

    tAnimLayer->RegisterProperty(tTRUTH, "playing", (EntityGetterFunction)&GetIsPlaying, NULL, NULL, NULL, (EntitySetterFunction)&SetIsPlaying, NULL, NULL, NULL, nullptr, NULL, NULL, 10);
}

AnimLayer* AnimLayer::Create(AllocatorIndex)
{
    return new AnimLayer;
}