#include "AnimLayer.h"
#include "TruthType.h"
#include "AssetManager.h"
#include "AnimSlot.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"
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

const bool AnimLayer::IsPlaying() const
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
    //tAnimLayer->RegisterProperty(tTRUTH, "looping", (EntityGetterFunction)&IsLooping, (EntitySetterFunction)&SetIsLooping, nullptr, 11);
    //tAnimLayer->RegisterProperty(tTRUTH, "freeze", (EntityGetterFunction)&IsFrozen, (EntitySetterFunction)&SetIsFrozen, nullptr, 12);
    //tAnimLayer->RegisterProperty(tINTEGER, "target_anim", (EntityGetterFunction)&GetTargetAnim, (EntitySetterFunction)&SetTargetAnim, nullptr, 13);
    //tAnimLayer->RegisterProperty(tNUMBER, "weight", (EntityGetterFunction)&GetWeight, (EntitySetterFunction)&SetWeight, nullptr, 14);
    //tAnimLayer->RegisterProperty(tEntity, "crosstarget1", (EntityGetterFunction)&GetCrossTarget1, (EntitySetterFunction)&SetCrossTarget1, nullptr, 15);
    //tAnimLayer->RegisterProperty(tEntity, "crosstarget2", (EntityGetterFunction)&GetCrossTarget2, (EntitySetterFunction)&SetCrossTarget2, nullptr, 16);
    //tAnimLayer->RegisterProperty(tNUMBER, "playpos1", (EntityGetterFunction)&GetPlayPos1, (EntitySetterFunction)&SetPlayPos1, nullptr, 17);
    //tAnimLayer->RegisterProperty(tNUMBER, "playpos2", (EntityGetterFunction)&GetPlayPos2, (EntitySetterFunction)&SetPlayPos2, nullptr, 18);
    //tAnimLayer->RegisterProperty(tNUMBER, "crossblendfactor", (EntityGetterFunction)&GetCrossBlendFactor, (EntitySetterFunction)&SetCrossBlendFactor, nullptr, 19);
    //tAnimLayer->RegisterProperty(tNUMBER, "crossblendspeed", (EntityGetterFunction)&GetCrossBlendSpeed, (EntitySetterFunction)&SetCrossBlendSpeed, nullptr, 21);
    //tAnimLayer->RegisterProperty(tNUMBER, "targetcrossblendfactor", (EntityGetterFunction)&GetTargetCrossBlendFactor, (EntitySetterFunction)&SetTargetCrossBlendFactor, nullptr, 20);
    //tAnimLayer->RegisterProperty(tINTEGER, "event", (EntityGetterFunction)&GetEvent, (EntitySetterFunction)&SetEvent, nullptr, 22);
    //tAnimLayer->RegisterProperty(tINTEGER, "blendfinishedevent", (EntityGetterFunction)&GetBlendFinishedEvent, (EntitySetterFunction)&SetBlendFinishedEvent, nullptr, 23);
    //tAnimLayer->RegisterProperty(tTRUTH, "stallfirstframe", (EntityGetterFunction)&StallFirstFrame, (EntitySetterFunction)&SetStallFirstFrame, nullptr);
    //tAnimLayer->RegisterProperty(tEntity, "targetanim", (EntityGetterFunction)&GetTargetAnim, nullptr, nullptr);
    //tAnimLayer->RegisterProperty(tEntity, "blendinanim", (EntityGetterFunction)&GetBlendAnim, nullptr, nullptr);
    //tAnimLayer->RegisterProperty(tEntity, "blendoutanim", (EntityGetterFunction)&GetBlendOutAnim, nullptr, nullptr);
    //tAnimLayer->RegisterProperty(tEntity, "motionlayer", (EntityGetterFunction)&GetMotionLayer, (EntitySetterFunction)&SetMotionLayer, nullptr, 24);
    //tAnimLayer->RegisterProperty(tTRUTH, "overridetarget1", (EntityGetterFunction)&OverrideTarget1, (EntitySetterFunction)&SetOverrideTarget1, nullptr, 25);
    //tAnimLayer->RegisterProperty(tTRUTH, "overridetarget2", (EntityGetterFunction)&OverrideTarget2, (EntitySetterFunction)&SetOverrideTarget2, nullptr, 26);
    //tAnimLayer->RegisterProperty(tVECTOR, "overridegamepivotpos1", (EntityGetterFunction)&GetOverrideGamePivotPos1, (EntitySetterFunction)&SetOverrideGamePivotPos1, nullptr, 27);
    //tAnimLayer->RegisterProperty(tVECTOR, "overridegamepivotpos2", (EntityGetterFunction)&GetOverrideGamePivotPos2, (EntitySetterFunction)&SetOverrideGamePivotPos2, nullptr, 28);
    //tAnimLayer->RegisterProperty(tTRUTH, "getclearoverridetarget1onblendfinished", (EntityGetterFunction)&GetClearOverrideTarget1OnBlendFinished, (EntitySetterFunction)&SetClearOverrideTarget1OnBlendFinished, nullptr, 29);
    //tAnimLayer->RegisterProperty(tTRUTH, "getclearoverridetarget2onblendfinished", (EntityGetterFunction)&GetClearOverrideTarget2OnBlendFinished, (EntitySetterFunction)&SetClearOverrideTarget2OnBlendFinished, nullptr, 30);
    //tAnimLayer->RegisterProperty(tNUMBER, "speedmultiplier", (EntityGetterFunction)&GetSpeedMultiplier, (EntitySetterFunction)&SetSpeedMultiplier, nullptr, 31);

    tAnimLayer->RegisterScript("play(entity,integer)", (EntityFunctionMember)&Play);
    tAnimLayer->RegisterScript("blendto(entity,integer)", (EntityFunctionMember)&BlendTo);
    //tAnimLayer->RegisterScript("setblendfinishedevent(integer)", (EntityFunctionMember)&SetBlendFinishedEvent);
    //tAnimLayer->RegisterScript("stop", (EntityFunctionMember)&Stop);
    //tAnimLayer->RegisterScript("synctoprevious", (EntityFunctionMember)&SyncToPrevious);
    //tAnimLayer->RegisterScript("setplaypos(number)", (EntityFunctionMember)&SetPlayPos);
    //tAnimLayer->RegisterScript("getplaypos:number", (EntityFunctionMember)&GetPlayPos);
    //tAnimLayer->RegisterScript("getvelocity:vector", (EntityFunctionMember)&GetVelocity, 0, 0, 0, "GetVelocityMSG");
    //tAnimLayer->RegisterScript("getgamepivotstartpos:vector", (EntityFunctionMember)&GetGamePivotStartPos);
    //tAnimLayer->RegisterScript("getgamepivotendpos:vector", (EntityFunctionMember)&GetGamePivotEndPos);
    //tAnimLayer->RegisterScript("getgamepivotstartorient:quaternion", (EntityFunctionMember)&GetGamePivotStartOrient);
    //tAnimLayer->RegisterScript("getgamepivotendorient:quaternion", (EntityFunctionMember)&GetGamePivotEndOrient);
    //tAnimLayer->RegisterScript("getgamepivotpos:vector", (EntityFunctionMember)&GetGamePivotPos);
    //tAnimLayer->RegisterScript("getgamepivotorient:quaternion", (EntityFunctionMember)&GetGamePivotOrient);
    //tAnimLayer->RegisterScript("getloopmode:integer", (EntityFunctionMember)&GetLoopMode);
    //tAnimLayer->RegisterScript("getoverrideblendout:truth", (EntityFunctionMember)&GetOverrideBlendOut);
    //tAnimLayer->RegisterScript("getoverrideblendin:truth", (EntityFunctionMember)&GetOverrideBlendIn);
    //tAnimLayer->RegisterScript("setoverrideblendout(truth)", (EntityFunctionMember)&SetOverrideBlendOut);
    //tAnimLayer->RegisterScript("setoverrideblendin(truth)", (EntityFunctionMember)&SetOverrideBlendIn);
    //tAnimLayer->RegisterScript("clearblendoutoverrideonblendfinished(truth)", (EntityFunctionMember)&ClearBlendOutOverrideOnBlendFinished);
    //tAnimLayer->RegisterScript("clearblendinoverrideonblendfinished(truth)", (EntityFunctionMember)&ClearBlendInOverrideOnBlendFinished);
    //tAnimLayer->RegisterScript("getoverrideblendoutpos:vector", (EntityFunctionMember)&GetOverrideBlendOutPos);
    //tAnimLayer->RegisterScript("getoverrideblendinpos:vector", (EntityFunctionMember)&GetOverrideBlendInPos);
    //tAnimLayer->RegisterScript("setoverrideblendoutpos(vector)", (EntityFunctionMember)&SetOverrideBlendOutPos);
    //tAnimLayer->RegisterScript("setoverrideblendinpos(vector)", (EntityFunctionMember)&SetOverrideBlendInPos);
    tAnimLayer->RegisterScript("calcandsetgamepivot", (EntityFunctionMember)&CalcAndSetGamePivot);

    tAnimLayer->PropagateProperties();
}

AnimLayer* AnimLayer::Create(AllocatorIndex)
{
    return new AnimLayer;
}