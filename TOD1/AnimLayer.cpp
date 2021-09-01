#include "AnimLayer.h"
#include "TruthType.h"
#include "AssetManager.h"
#include "AnimSlot.h"

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

const bool AnimLayer::GetIsPlaying() const
{
    return m_Flags.Playing;
}

void AnimLayer::SetIsPlaying(const bool playing)
{
    const bool isplaying = m_Flags.Playing;

    if (m_Flags.Playing != playing)
        SetParam(10, &isplaying, tTRUTH);

    m_Flags.Playing = playing;
}

void AnimLayer::GetGamePivotPos(Vector4f& outPos) const
{
    outPos = m_GamePivotPos;
}

void AnimLayer::GetGamePivotOrient(Orientation& outOrient) const
{
    outOrient = m_GamePivotOrient;
}

void AnimLayer::GetLoopMode(int& outLoopMode) const
{
    outLoopMode = m_TargetAnim == 1 ? m_TargetAnim_Entity->m_LoopMode_1 : m_TargetAnim_Entity_2->m_LoopMode_1;
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::Register()
{
    tAnimLayer = new EntityType("AnimLayer");
    tAnimLayer->InheritFrom(tNode);
    tAnimLayer->SetCreator((EntityType::CREATOR)Create);
    
    tAnimLayer->RegisterProperty(tTRUTH, "playing", &GetIsPlaying, NULL, NULL, NULL, &SetIsPlaying, NULL, NULL, NULL, nullptr, NULL, NULL, 10);
}

AnimLayer* AnimLayer::Create(AllocatorIndex)
{
    return new AnimLayer;
}