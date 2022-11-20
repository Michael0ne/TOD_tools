#include "AnimLayer.h"
#include "TruthType.h"
#include "AssetManager.h"
#include "AnimSlot.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "TransactionBuffer.h"
#include "Character.h"
#include "BuiltinType.h"

EntityType* tAnimLayer;

AnimLayer::AnimLayer() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(AnimLayer);

    GamePivotOrientation = BuiltinType::Orient;
    TargetAnimationNodeIndex = 1;
    BlendFinishedEvent = -1;
    Event = -1;
    Flags.StallFirstFrame = true;
    TargetAnimNode_1 = nullptr;
    TargetAnimNode_2 = nullptr;
    MotionLayerNode = 0;
    Weight = 0;
    SpeedMultiplier = 1;
    CrossBlendFactor = 0;
    OverrideTarget.Target1 = false;
    OverrideTarget.Target2 = false;
    ClearOverrideTargetOnBlendFinished.Target1 = false;
    ClearOverrideTargetOnBlendFinished.Target2 = false;
}

AnimLayer::~AnimLayer()
{
    MESSAGE_CLASS_DESTROYED(AnimLayer);
}

const bool AnimLayer::IsPlaying() const
{
    return Flags.Playing;
}

void AnimLayer::SetIsPlaying(const bool playing)
{
    const bool isplaying = Flags.Playing;

    if (Flags.Playing != playing)
        StoreProperty(10, &isplaying, tTRUTH);

    Flags.Playing = playing;
}

const bool AnimLayer::IsLooping() const
{
    return Flags.Looping;
}

void AnimLayer::SetIsLooping(const bool loop)
{
    const bool loopflag = Flags.Looping;

    if (loop != loopflag)
        StoreProperty(11, &loopflag, tTRUTH);

    Flags.Looping = loop;
}

const bool AnimLayer::IsFrozen() const
{
    return Flags.Freeze;
}

void AnimLayer::SetIsFrozen(const bool frozen)
{
    const bool isfrozen = Flags.Freeze;

    if (frozen != isfrozen)
        StoreProperty(12, &isfrozen, tTRUTH);

    Flags.Freeze = frozen;
}

const uint32_t AnimLayer::GetTargetAnim() const
{
    return TargetAnimationNodeIndex;
}

void AnimLayer::SetTargetAnim(const uint32_t targetAnim)
{
    if (TargetAnimationNodeIndex != targetAnim)
        StoreProperty(13, &TargetAnimationNodeIndex, tINTEGER);

    TargetAnimationNodeIndex = targetAnim;
}

void AnimLayer::GetGamePivotPos(Vector4f& outPos) const
{
    outPos = GamePivotPos_1;
}

void AnimLayer::GetGamePivot(Vector4f& outPos) const
{
    outPos = GamePivotPosition;
}

void AnimLayer::GetGamePivotOrientA(Orientation& outOrient) const
{
    outOrient = GamePivotOrientation;
}

void AnimLayer::GetGamePivotOrient(Orientation& outOrient) const
{
    outOrient = GamePivotOrientation;
}

void AnimLayer::GetLoopMode(int& outLoopMode) const
{
    outLoopMode = TargetAnimationNodeIndex == 1 ? TargetAnimNode_1->LoopMode : TargetAnimNode_2->LoopMode;
}

void AnimLayer::SetCrossTarget1(AnimSlot* target)
{
    AnimSlot* currentTargetAnimEntity = TargetAnimNode_1;
    if (target != TargetAnimNode_1)
        StoreProperty(15, &TargetAnimNode_1, tEntity);

    currentTargetAnimEntity = target;
    TargetData* currentCrossTarget = &CrossTargets[0];

    if (!target)
    {
        CrossTargets[0] = TargetData();
        return;
    }

    Character* targetCharacter = nullptr;
    if (m_Parent)
    {
        EntityType* scriptEntity = m_Parent->m_ScriptEntity;
        if (scriptEntity)
        {
            while (tCharacter != scriptEntity)
            {
                scriptEntity = scriptEntity->m_Parent;
                if (!scriptEntity)
                    break;
            }
        }

        targetCharacter = (Character*)m_Parent;
    }

    std::vector<uint32_t> physAttachmentsList(64);
    const AnimationAsset* targetAnimationAsset = target->TargetAnimation.GetAsset<AnimationAsset>();
    const uint32_t framesTotal = targetAnimationAsset->field_5C->Data.FramesTotal;

    if (framesTotal)
    {
        for (uint32_t boneIndex = 0; boneIndex < framesTotal; ++boneIndex)
        {
            const int32_t physAttachmentIndex = targetCharacter->GetPhysAttachmentIndex(targetAnimationAsset->GetBoneName(boneIndex));
            if (physAttachmentIndex >= 0)
                physAttachmentsList[physAttachmentIndex] = boneIndex;
        }
    }

    CrossTargets[0] = TargetData(target->TargetAnimation, target->LoopMode, physAttachmentsList);
}

Node* AnimLayer::GetCrossTarget2() const
{
    return TargetAnimNode_2;
}

void AnimLayer::SetCrossTarget2(AnimSlot* target)
{
    AnimSlot* currentTargetAnimEntity = TargetAnimNode_2;
    if (target != TargetAnimNode_2)
        StoreProperty(15, &TargetAnimNode_2, tEntity);

    currentTargetAnimEntity = target;
    TargetData* currentCrossTarget = &CrossTargets[1];

    if (!target)
    {
        CrossTargets[0] = TargetData();
        return;
    }

    Character* targetCharacter = nullptr;
    if (m_Parent)
    {
        EntityType* scriptEntity = m_Parent->m_ScriptEntity;
        if (scriptEntity)
        {
            while (tCharacter != scriptEntity)
            {
                scriptEntity = scriptEntity->m_Parent;
                if (!scriptEntity)
                    break;
            }
        }

        targetCharacter = (Character*)m_Parent;
    }

    std::vector<uint32_t> physAttachmentsList(64);
    const AnimationAsset* targetAnimationAsset = target->TargetAnimation.GetAsset<AnimationAsset>();
    const uint32_t framesTotal = targetAnimationAsset->field_5C->Data.FramesTotal;

    if (framesTotal)
    {
        for (uint32_t boneIndex = 0; boneIndex < framesTotal; ++boneIndex)
        {
            const int32_t physAttachmentIndex = targetCharacter->GetPhysAttachmentIndex(targetAnimationAsset->GetBoneName(boneIndex));
            if (physAttachmentIndex >= 0)
                physAttachmentsList[physAttachmentIndex] = boneIndex;
        }
    }

    CrossTargets[1] = TargetData(target->TargetAnimation, target->LoopMode, physAttachmentsList);
}

const float_t AnimLayer::GetPlayPos1() const
{
    return CrossTargets[0].PlayPos;
}

void AnimLayer::SetPlayPos1(const float_t playpos)
{
    const float_t currentPlayPos = CrossTargets[0].PlayPos;
    if (playpos != currentPlayPos)
        StoreProperty(17, &currentPlayPos, tNUMBER);

    if (TransactionBuffer::_A11C90 == 1.f)
    {
        field_134 = playpos;
        CrossTargets[0].PlayPos = playpos;
        if (!CrossTargets[0].LoopMode && playpos >= 1.f)
            CrossTargets[0].PlayPos = playpos - (float_t)(uint32_t)playpos;
    }
    else
    {
        float_t playPosCopy = playpos;
        if (playpos > field_134 && TargetAnimNode_1 && !TargetAnimNode_1->LoopMode)
            playPosCopy -= 1.f;

        float_t playPosAdjusted = ((1.f - TransactionBuffer::_A11C90) * field_134) + (TransactionBuffer::_A11C90 * playPosCopy);
        if (!TargetAnimNode_1 || TargetAnimNode_1->LoopMode)
        {
            if (playPosAdjusted >= 0.f)
            {
                if (playPosAdjusted > 1.f)
                    playPosAdjusted = 1.f;
            }
            else
            {
                playPosAdjusted = 0.f;
            }
        }
        else
        {
            while (playPosAdjusted < 0.f)
                playPosAdjusted += 1.f;
            while (playPosAdjusted > 1.f)
                playPosAdjusted -= 1.f;
        }

        CrossTargets[0].PlayPos = playPosAdjusted;
        if (!CrossTargets[0].LoopMode && playPosAdjusted >= 1.f)
            CrossTargets[0].PlayPos = playPosAdjusted - (float_t)(uint32_t)playPosAdjusted;
    }
}

const float_t AnimLayer::GetPlayPos2() const
{
    return CrossTargets[1].PlayPos;
}

void AnimLayer::SetPlayPos2(const float_t playpos)
{
    const float_t currentPlayPos = CrossTargets[1].PlayPos;
    if (playpos != currentPlayPos)
        StoreProperty(18, &currentPlayPos, tNUMBER);

    if (TransactionBuffer::_A11C90 == 1.f)
    {
        field_138 = playpos;
        CrossTargets[1].PlayPos = playpos;
        if (!CrossTargets[1].LoopMode && playpos >= 1.f)
            CrossTargets[1].PlayPos = playpos - (float_t)(uint32_t)playpos;
    }
    else
    {
        float_t playPosCopy = playpos;
        if (playpos > field_138 && TargetAnimNode_2 && !TargetAnimNode_2->LoopMode)
            playPosCopy -= 1.f;

        float_t playPosAdjusted = ((1.f - TransactionBuffer::_A11C90) * field_138) + (TransactionBuffer::_A11C90 * playPosCopy);
        if (!TargetAnimNode_2 || TargetAnimNode_2->LoopMode)
        {
            if (playPosAdjusted >= 0.f)
            {
                if (playPosAdjusted > 1.f)
                    playPosAdjusted = 1.f;
            }
            else
            {
                playPosAdjusted = 0.f;
            }
        }
        else
        {
            while (playPosAdjusted < 0.f)
                playPosAdjusted += 1.f;
            while (playPosAdjusted > 1.f)
                playPosAdjusted -= 1.f;
        }

        CrossTargets[1].PlayPos = playPosAdjusted;
        if (!CrossTargets[1].LoopMode && playPosAdjusted >= 1.f)
            CrossTargets[1].PlayPos = playPosAdjusted - (float_t)(uint32_t)playPosAdjusted;
    }
}

const float_t AnimLayer::GetCrossBlendFactor() const
{
    return CrossBlendFactor;
}

void AnimLayer::SetCrossBlendFactor(const float_t crossblendfactor)
{
    if (crossblendfactor != CrossBlendFactor)
        StoreProperty(19, &CrossBlendFactor, tNUMBER);

    if (TargetAnimNode_1)
    {
        if (TargetAnimNode_2)
        {
            if (TransactionBuffer::_A11C90 == 1.f)
            {
                CrossBlendFactor = crossblendfactor;
                field_140 = crossblendfactor;
            }
            else
            {
                CrossBlendFactor = ((1.f - TransactionBuffer::_A11C90) * field_140) + (TransactionBuffer::_A11C90 * crossblendfactor);
            }
        }
        else
        {
            CrossBlendFactor = 0.f;
        }
    }
    else
    {
        if (TargetAnimNode_2)
            CrossBlendFactor = 1.f;
        else
            SetIsPlaying(false);
    }
}

const float_t AnimLayer::GetCrossBlendSpeed() const
{
    return CrossBlendSpeed;
}

void AnimLayer::SetCrossBlendSpeed(const float_t crossblendspeed)
{
    if (crossblendspeed != CrossBlendSpeed)
        StoreProperty(21, &CrossBlendSpeed, tNUMBER);

    CrossBlendSpeed = crossblendspeed;
}

const float_t AnimLayer::GetTargetCrossBlendFactor() const
{
    return TargetAnimationNodeIndex == 1 ? 1.f - CrossBlendFactor : CrossBlendFactor;
}

void AnimLayer::SetTargetCrossBlendFactor(const float_t crossblendfactor)
{
    if (TargetAnimationNodeIndex == 1)
        SetCrossBlendFactor(1.f - crossblendfactor);
    else
        SetCrossBlendFactor(crossblendfactor);
}

const uint32_t AnimLayer::GetEvent() const
{
    return Event;
}

void AnimLayer::SetEvent(const uint32_t event)
{
    StoreProperty(22, &Event, tINTEGER);
    Event = event;
}

const uint32_t AnimLayer::GetBlendFinishedEvent() const
{
    return BlendFinishedEvent;
}

void AnimLayer::SetBlendFinishedEvent(const uint32_t event)
{
    StoreProperty(23, &BlendFinishedEvent, tINTEGER);
    BlendFinishedEvent = event;
}

const bool AnimLayer::ShouldStallFirstFrame() const
{
    return Flags.StallFirstFrame;
}

void AnimLayer::SetShouldStallFirstFrame(const bool enabled)
{
    Flags.StallFirstFrame = enabled;
}

Node* AnimLayer::GetTargetAnimNode() const
{
    return TargetAnimationNodeIndex == 1 ? TargetAnimNode_1 : TargetAnimNode_2;
}

Node* AnimLayer::GetBlendInAnimNode() const
{
    return GetTargetAnimNode();
}

Node* AnimLayer::GetBlendOutAnimNode() const
{
    return TargetAnimationNodeIndex == 1 ? TargetAnimNode_2 : TargetAnimNode_1;
}

MotionLayer* AnimLayer::GetMotionLayer() const
{
    return MotionLayerNode;
}

void AnimLayer::SetMotionLayer(MotionLayer* node)
{
    MotionLayerNode = node;
}

const bool AnimLayer::OverrideTarget1() const
{
    return OverrideTarget.Target1;
}

void AnimLayer::SetOverrideTarget1(const bool shouldoverride)
{
    const bool currentOverride1 = OverrideTarget.Target1;
    if (shouldoverride != currentOverride1)
        StoreProperty(25, &currentOverride1, tTRUTH);

    OverrideTarget.Target1 = shouldoverride;
}

const bool AnimLayer::OverrideTarget2() const
{
    return OverrideTarget.Target2;
}

void AnimLayer::SetOverrideTarget2(const bool shouldoverride)
{
    const bool currentOverride2 = OverrideTarget.Target2;
    if (shouldoverride != currentOverride2)
        StoreProperty(26, &currentOverride2, tTRUTH);

    OverrideTarget.Target2 = shouldoverride;
}

void AnimLayer::GetOverrideGamePivotPos1(Vector4f& pos) const
{
    pos = GamePivotPos_1;
}

void AnimLayer::SetOverrideGamePivotPos1(const Vector4f& pos)
{
    const Vector4f& currentPos = GamePivotPos_1;
    if (currentPos != pos)
        StoreProperty(27, &currentPos, tVECTOR);

    GamePivotPos_1 = pos;
}

void AnimLayer::GetOverrideGamePivotPos2(Vector4f& pos) const
{
    pos = GamePivotPos_2;
}

void AnimLayer::SetOverrideGamePivotPos2(const Vector4f& pos)
{
    const Vector4f& currentPos = GamePivotPos_2;
    if (currentPos != pos)
        StoreProperty(28, &currentPos, tVECTOR);

    GamePivotPos_2 = pos;
}

const bool AnimLayer::GetClearOverrideTarget1OnBlendFinished() const
{
    return ClearOverrideTargetOnBlendFinished.Target1;
}

void AnimLayer::SetClearOverrideTarget1OnBlendFinished(const bool clearoverride)
{
    const bool clearOverride = ClearOverrideTargetOnBlendFinished.Target1;
    if (clearOverride != clearoverride)
        StoreProperty(29, &clearOverride, tTRUTH);

    ClearOverrideTargetOnBlendFinished.Target1 = clearoverride;
}

const bool AnimLayer::GetClearOverrideTarget2OnBlendFinished() const
{
    return ClearOverrideTargetOnBlendFinished.Target2;
}

void AnimLayer::SetClearOverrideTarget2OnBlendFinished(const bool clearoverride)
{
    const bool clearOverride = ClearOverrideTargetOnBlendFinished.Target2;
    if (clearOverride != clearoverride)
        StoreProperty(30, &clearOverride, tTRUTH);

    ClearOverrideTargetOnBlendFinished.Target2 = clearoverride;
}

const float_t AnimLayer::GetSpeedMultiplier() const
{
    return SpeedMultiplier;
}

void AnimLayer::SetSpeedMultiplier(const float_t mult)
{
    if (SpeedMultiplier != mult)
        StoreProperty(31, &SpeedMultiplier, tNUMBER);

    SpeedMultiplier = mult;
}

void AnimLayer::Play(uint32_t* args)
{
    Play_Impl((AnimSlot*)args[0], args[1]);
}

void AnimLayer::BlendTo(uint32_t* args)
{
    BlendTo_Impl((AnimSlot*)args[0], args[1]);
}

void AnimLayer::SetBlendFinishedEventScripted(uint32_t* args)
{
    StoreProperty(23, &BlendFinishedEvent, tINTEGER);
    BlendFinishedEvent = args[0];
}

void AnimLayer::Stop(uint32_t* args)
{
    const bool isPlaying = Flags.Playing;
    if (isPlaying)
        StoreProperty(10, &isPlaying, tTRUTH);

    Flags.Playing = false;
}

void AnimLayer::SyncToPrevious(uint32_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        SetPlayPos1(CrossTargets[1].PlayPos);
    else
        if (TargetAnimationNodeIndex == 2)
            SetPlayPos2(CrossTargets[0].PlayPos);
}

void AnimLayer::SetPlayPos(uint32_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        SetPlayPos1((float_t)args[0]);
    else
        SetPlayPos2((float_t)args[0]);
}

void AnimLayer::GetPlayPos(float_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        *args = CrossTargets[0].PlayPos;
    else
        *args = CrossTargets[1].PlayPos;
}

void AnimLayer::GetVelocity(uint32_t* args)
{
    Vector4f velocity;
    GetVelocity_Impl(velocity);

    *(Vector4f*)args = velocity;
}

void AnimLayer::GetGamePivotStartPos(float_t* args) const
{
    AnimSlot* targetAnim = TargetAnimationNodeIndex == 1 ? TargetAnimNode_1 : TargetAnimNode_2;
    AnimationAsset* animationAsset = targetAnim->TargetAnimation.GetAsset<AnimationAsset>();

    Vector4f pivotPos;
    if (animationAsset && targetAnim->Flags.GamePivotIndex >= 0)
        animationAsset->GetPivotPos(pivotPos, targetAnim->Flags.GamePivotIndex, 0);

    *(Vector4f*)args = pivotPos;
}

void AnimLayer::GetGamePivotEndPos(float_t* args) const
{
    Vector4f pivotPos;
    GetGamePivotEndPos_Impl(pivotPos);

    *(Vector4f*)args = pivotPos;
}

void AnimLayer::GetGamePivotStartOrient(float_t* args) const
{
    Orientation pivotOrient;
    AnimSlot* animSlot = TargetAnimationNodeIndex == 1 ? TargetAnimNode_1 : TargetAnimNode_2;

    animSlot->GetGamePivotStartOrient(pivotOrient);

    *(Orientation*)args = pivotOrient;
}

void AnimLayer::GetGamePivotEndOrient(float_t* args) const
{
    Orientation pivotOrient;
    AnimSlot* animSlot = TargetAnimationNodeIndex == 1 ? TargetAnimNode_1 : TargetAnimNode_2;

    animSlot->GetGamePivotEndOrient(pivotOrient);

    *(Orientation*)args = pivotOrient;
}

void AnimLayer::GetOverrideBlendOut(uint32_t* args) const
{
    if (TargetAnimationNodeIndex == 1)
        *args = OverrideTarget.Target2;
    else
        *args = OverrideTarget.Target1;
}

void AnimLayer::GetOverrideBlendIn(uint32_t* args) const
{
    if (TargetAnimationNodeIndex == 1)
        *args = OverrideTarget.Target1;
    else
        *args = OverrideTarget.Target2;
}

void AnimLayer::SetOverrideBlendOut(uint32_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        SetOverrideTarget2(*args != 0);
    else
        SetOverrideTarget1(*args != 0);
}

void AnimLayer::SetOverrideBlendIn(uint32_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        SetOverrideTarget1(*args != 0);
    else
        SetOverrideTarget2(*args != 0);
}

void AnimLayer::ClearBlendOutOverrideOnBlendFinished(uint32_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        SetClearOverrideTarget2OnBlendFinished(*args != 0);
    else
        SetClearOverrideTarget1OnBlendFinished(*args != 0);
}

void AnimLayer::ClearBlendInOverrideOnBlendFinished(uint32_t* args)
{
    if (TargetAnimationNodeIndex == 1)
        SetClearOverrideTarget1OnBlendFinished(*args != 0);
    else
        SetClearOverrideTarget2OnBlendFinished(*args != 0);
}

void AnimLayer::GetOverrideBlendOutPos(float_t* args) const
{
    if (TargetAnimationNodeIndex == 1)
        *(Vector4f*)args = GamePivotPos_2;
    else
        *(Vector4f*)args = GamePivotPos_1;
}

void AnimLayer::GetOverrideBlendInPos(float_t* args) const
{
    if (TargetAnimationNodeIndex == 1)
        *(Vector4f*)args = GamePivotPos_1;
    else
        *(Vector4f*)args = GamePivotPos_2;
}

void AnimLayer::SetOverrideBlendOutPos(float_t* args)
{
    const Vector4f& pivotPos = *(Vector4f*)args;
    if (TargetAnimationNodeIndex == 1)
        SetOverrideGamePivotPos2(pivotPos);
    else
        SetOverrideGamePivotPos1(pivotPos);
}

void AnimLayer::SetOverrideBlendInPos(float_t* args)
{
    const Vector4f& pivotPos = *(Vector4f*)args;
    if (TargetAnimationNodeIndex == 1)
        SetOverrideGamePivotPos1(pivotPos);
    else
        SetOverrideGamePivotPos2(pivotPos);
}

void AnimLayer::SetTargetCrossBlendFactor_Impl(const float_t factor)
{
    if (CrossBlendFactor != factor)
        StoreProperty(19, &CrossBlendFactor, tNUMBER);

    if (TargetAnimNode_1)
    {
        if (TransactionBuffer::_A11C90 == 1)
        {
            CrossBlendFactor = factor;
            field_140 = factor;
        }
        else
            CrossBlendFactor = ((1 - TransactionBuffer::_A11C90) * field_140) + (TransactionBuffer::_A11C90 * factor);
    }
    else
    {
        if (TargetAnimNode_2)
            CrossBlendFactor = 1;
        else
            SetIsPlaying(false);
    }
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::GetVelocity_Impl(Vector4f& velocity) const
{
    velocity = BuiltinType::ZeroVector;

    if (!Flags.Playing)
        return;
}

void AnimLayer::GetGamePivotEndPos_Impl(Vector4f& pivotPos) const
{
    AnimSlot* animSlot = TargetAnimationNodeIndex == 1 ? TargetAnimNode_1 : TargetAnimNode_2;
    AnimationAsset* animationAsset = animSlot->TargetAnimation.GetAsset<AnimationAsset>();

    if (!animationAsset)
        return;

    const uint32_t gamePivotIndex = animSlot->Flags.GamePivotIndex;
    if (gamePivotIndex >= 0)
        animationAsset->GetPivotPos(pivotPos, gamePivotIndex, animationAsset->GetPivotEndFrame(gamePivotIndex));
}

void AnimLayer::Play_Impl(AnimSlot* animSlot, const uint32_t event)
{
    if (!animSlot)
    {
        LogDump::LogA("NULL animation\n");
        return;
    }

    SetIsPlaying(true);
    SetIsLooping(false);

    if (TargetAnimationNodeIndex != 1)
        StoreProperty(13, &TargetAnimationNodeIndex, tINTEGER);

    TargetAnimationNodeIndex = 1;

    SetCrossTarget1(animSlot);
    SetCrossBlendFactor(0.f);

    if (CrossBlendSpeed != 0.f)
        StoreProperty(21, &CrossBlendSpeed, tNUMBER);

    CrossBlendSpeed = 0.f;

    SetPlayPos1(0.f);
    StoreProperty(22, &Event, tINTEGER);
    Event = event;

    Flags.StallFirstFrame = true;

    AnimSlot* animSlot = TargetAnimationNodeIndex == 1 ? TargetAnimNode_1 : TargetAnimNode_2;
    AnimationAsset* animationAsset = animSlot->TargetAnimation.GetAsset<AnimationAsset>();

    if (animationAsset->m_List_1.size())
    {
        for (uint32_t i = 0; i < animationAsset->m_List_1.size(); ++i)
            animationAsset->m_List_1[i].field_14 = 1;
    }

    TargetAnimNode_1->GetGamePivotPos_Impl(GamePivotPosition, CrossTargets[0].PlayPos);
    TargetAnimNode_1->GetGamePivotOrient(GamePivotOrientation, CrossTargets[0].PlayPos);
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::BlendTo_Impl(AnimSlot* animSlot, const uint32_t event)
{
    if (!animSlot)
    {
        LogDump::LogA("NULL animation\n");
        return;
    }

    if (!Flags.Playing || CrossBlendSpeed >= 100.f)
    {
        Play_Impl(animSlot, event);
        return;
    }
}

const float_t AnimLayer::GetWeight() const
{
    return Weight;
}

void AnimLayer::SetWeight(const float_t weight)
{
    if (weight != Weight)
        StoreProperty(14, &Weight, tNUMBER);

    if (TransactionBuffer::_A11C90 == 1.f)
    {
        Weight = weight;
        field_13C = weight;
    }
    else
    {
        Weight = ((1.f - TransactionBuffer::_A11C90) * field_13C) + (TransactionBuffer::_A11C90 * weight);
    }
}

Node* AnimLayer::GetCrossTarget1() const
{
    return TargetAnimNode_1;
}

void AnimLayer::CalcAndSetGamePivot(int* args)
{
    CalcAndSetGamePivot_Impl();
}

#pragma message(TODO_IMPLEMENTATION)
void AnimLayer::CalcAndSetGamePivot_Impl()
{
}

void AnimLayer::Register()
{
    tAnimLayer = new EntityType("AnimLayer");
    tAnimLayer->InheritFrom(tNode);
    tAnimLayer->SetCreator((CREATOR)Create);

    tAnimLayer->RegisterProperty(tTRUTH, "playing", (EntityGetterFunction)&IsPlaying, (EntitySetterFunction)&SetIsPlaying, nullptr, 10);
    tAnimLayer->RegisterProperty(tTRUTH, "looping", (EntityGetterFunction)&IsLooping, (EntitySetterFunction)&SetIsLooping, nullptr, 11);
    tAnimLayer->RegisterProperty(tTRUTH, "freeze", (EntityGetterFunction)&IsFrozen, (EntitySetterFunction)&SetIsFrozen, nullptr, 12);
    tAnimLayer->RegisterProperty(tINTEGER, "target_anim", (EntityGetterFunction)&GetTargetAnim, (EntitySetterFunction)&SetTargetAnim, nullptr, 13);
    tAnimLayer->RegisterProperty(tNUMBER, "weight", (EntityGetterFunction)&GetWeight, (EntitySetterFunction)&SetWeight, nullptr, 14);
    tAnimLayer->RegisterProperty(tEntity, "crosstarget1", (EntityGetterFunction)&GetCrossTarget1, (EntitySetterFunction)&SetCrossTarget1, nullptr, 15);
    tAnimLayer->RegisterProperty(tEntity, "crosstarget2", (EntityGetterFunction)&GetCrossTarget2, (EntitySetterFunction)&SetCrossTarget2, nullptr, 16);
    tAnimLayer->RegisterProperty(tNUMBER, "playpos1", (EntityGetterFunction)&GetPlayPos1, (EntitySetterFunction)&SetPlayPos1, nullptr, 17);
    tAnimLayer->RegisterProperty(tNUMBER, "playpos2", (EntityGetterFunction)&GetPlayPos2, (EntitySetterFunction)&SetPlayPos2, nullptr, 18);
    tAnimLayer->RegisterProperty(tNUMBER, "crossblendfactor", (EntityGetterFunction)&GetCrossBlendFactor, (EntitySetterFunction)&SetCrossBlendFactor, nullptr, 19);
    tAnimLayer->RegisterProperty(tNUMBER, "crossblendspeed", (EntityGetterFunction)&GetCrossBlendSpeed, (EntitySetterFunction)&SetCrossBlendSpeed, nullptr, 21);
    tAnimLayer->RegisterProperty(tNUMBER, "targetcrossblendfactor", (EntityGetterFunction)&GetTargetCrossBlendFactor, (EntitySetterFunction)&SetTargetCrossBlendFactor, nullptr, 20);
    tAnimLayer->RegisterProperty(tINTEGER, "event", (EntityGetterFunction)&GetEvent, (EntitySetterFunction)&SetEvent, nullptr, 22);
    tAnimLayer->RegisterProperty(tINTEGER, "blendfinishedevent", (EntityGetterFunction)&GetBlendFinishedEvent, (EntitySetterFunction)&SetBlendFinishedEvent, nullptr, 23);
    tAnimLayer->RegisterProperty(tTRUTH, "stallfirstframe", (EntityGetterFunction)&ShouldStallFirstFrame, (EntitySetterFunction)&SetShouldStallFirstFrame, nullptr);
    tAnimLayer->RegisterProperty(tEntity, "targetanim", (EntityGetterFunction)&GetTargetAnimNode, nullptr, nullptr);
    tAnimLayer->RegisterProperty(tEntity, "blendinanim", (EntityGetterFunction)&GetBlendInAnimNode, nullptr, nullptr);
    tAnimLayer->RegisterProperty(tEntity, "blendoutanim", (EntityGetterFunction)&GetBlendOutAnimNode, nullptr, nullptr);
    tAnimLayer->RegisterProperty(tEntity, "motionlayer", (EntityGetterFunction)&GetMotionLayer, (EntitySetterFunction)&SetMotionLayer, nullptr, 24);
    tAnimLayer->RegisterProperty(tTRUTH, "overridetarget1", (EntityGetterFunction)&OverrideTarget1, (EntitySetterFunction)&SetOverrideTarget1, nullptr, 25);
    tAnimLayer->RegisterProperty(tTRUTH, "overridetarget2", (EntityGetterFunction)&OverrideTarget2, (EntitySetterFunction)&SetOverrideTarget2, nullptr, 26);
    tAnimLayer->RegisterProperty(tVECTOR, "overridegamepivotpos1", (EntityGetterFunction)&GetOverrideGamePivotPos1, (EntitySetterFunction)&SetOverrideGamePivotPos1, nullptr, 27);
    tAnimLayer->RegisterProperty(tVECTOR, "overridegamepivotpos2", (EntityGetterFunction)&GetOverrideGamePivotPos2, (EntitySetterFunction)&SetOverrideGamePivotPos2, nullptr, 28);
    tAnimLayer->RegisterProperty(tTRUTH, "getclearoverridetarget1onblendfinished", (EntityGetterFunction)&GetClearOverrideTarget1OnBlendFinished, (EntitySetterFunction)&SetClearOverrideTarget1OnBlendFinished, nullptr, 29);
    tAnimLayer->RegisterProperty(tTRUTH, "getclearoverridetarget2onblendfinished", (EntityGetterFunction)&GetClearOverrideTarget2OnBlendFinished, (EntitySetterFunction)&SetClearOverrideTarget2OnBlendFinished, nullptr, 30);
    tAnimLayer->RegisterProperty(tNUMBER, "speedmultiplier", (EntityGetterFunction)&GetSpeedMultiplier, (EntitySetterFunction)&SetSpeedMultiplier, nullptr, 31);

    tAnimLayer->RegisterScript("play(entity,integer)", (EntityFunctionMember)&Play);
    tAnimLayer->RegisterScript("blendto(entity,integer)", (EntityFunctionMember)&BlendTo);
    tAnimLayer->RegisterScript("setblendfinishedevent(integer)", (EntityFunctionMember)&SetBlendFinishedEventScripted);
    tAnimLayer->RegisterScript("stop", (EntityFunctionMember)&Stop);
    tAnimLayer->RegisterScript("synctoprevious", (EntityFunctionMember)&SyncToPrevious);
    tAnimLayer->RegisterScript("setplaypos(number)", (EntityFunctionMember)&SetPlayPos);
    tAnimLayer->RegisterScript("getplaypos:number", (EntityFunctionMember)&GetPlayPos);
    tAnimLayer->RegisterScript("getvelocity:vector", (EntityFunctionMember)&GetVelocity, 0, 0, 0, "GetVelocityMSG");
    tAnimLayer->RegisterScript("getgamepivotstartpos:vector", (EntityFunctionMember)&GetGamePivotStartPos);
    tAnimLayer->RegisterScript("getgamepivotendpos:vector", (EntityFunctionMember)&GetGamePivotEndPos);
    tAnimLayer->RegisterScript("getgamepivotstartorient:quaternion", (EntityFunctionMember)&GetGamePivotStartOrient);
    tAnimLayer->RegisterScript("getgamepivotendorient:quaternion", (EntityFunctionMember)&GetGamePivotEndOrient);
    tAnimLayer->RegisterScript("getgamepivotpos:vector", (EntityFunctionMember)&GetGamePivotPos);
    tAnimLayer->RegisterScript("getgamepivotorient:quaternion", (EntityFunctionMember)&GetGamePivotOrient);
    tAnimLayer->RegisterScript("getloopmode:integer", (EntityFunctionMember)&GetLoopMode);
    tAnimLayer->RegisterScript("getoverrideblendout:truth", (EntityFunctionMember)&GetOverrideBlendOut);
    tAnimLayer->RegisterScript("getoverrideblendin:truth", (EntityFunctionMember)&GetOverrideBlendIn);
    tAnimLayer->RegisterScript("setoverrideblendout(truth)", (EntityFunctionMember)&SetOverrideBlendOut);
    tAnimLayer->RegisterScript("setoverrideblendin(truth)", (EntityFunctionMember)&SetOverrideBlendIn);
    tAnimLayer->RegisterScript("clearblendoutoverrideonblendfinished(truth)", (EntityFunctionMember)&ClearBlendOutOverrideOnBlendFinished);
    tAnimLayer->RegisterScript("clearblendinoverrideonblendfinished(truth)", (EntityFunctionMember)&ClearBlendInOverrideOnBlendFinished);
    tAnimLayer->RegisterScript("getoverrideblendoutpos:vector", (EntityFunctionMember)&GetOverrideBlendOutPos);
    tAnimLayer->RegisterScript("getoverrideblendinpos:vector", (EntityFunctionMember)&GetOverrideBlendInPos);
    tAnimLayer->RegisterScript("setoverrideblendoutpos(vector)", (EntityFunctionMember)&SetOverrideBlendOutPos);
    tAnimLayer->RegisterScript("setoverrideblendinpos(vector)", (EntityFunctionMember)&SetOverrideBlendInPos);
    tAnimLayer->RegisterScript("calcandsetgamepivot", (EntityFunctionMember)&CalcAndSetGamePivot);

    tAnimLayer->PropagateProperties();
}

AnimLayer* AnimLayer::Create(AllocatorIndex)
{
    return new AnimLayer;
}

AnimLayer::TargetData::~TargetData()
{
    MESSAGE_CLASS_DESTROYED(TargetData);
}

AnimLayer::TargetData::TargetData(AssetLoader& animation, const uint32_t loopMode, const std::vector<uint32_t>& physAttachmentsList)
{
    MESSAGE_CLASS_CREATED(TargetData);

    PhysAttachmentsList = physAttachmentsList;
    TargetAsset = animation;
    field_20 = 0;
    LoopMode = loopMode;
    PlayPos = 0.f;
    field_22 = 0;
    field_24 = 0;
}