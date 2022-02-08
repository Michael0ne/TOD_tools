#include "AnimSlot.h"
#include "StringType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"
#include "BuiltinType.h"

EntityType* tAnimSlot;

AnimSlot::AnimSlot() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(AnimSlot);

    m_TargetAnimation = nullptr;
    field_54 = 1;
    m_Flags.StallFirstFrame = true;
    m_Speed = m_CrossBlendSpeed = 1.0f;
    m_LoopMode = LOOP;
    m_AnimFlags = FREEZE_ROOT_NODE;

    m_OverrideAverage = {};
    m_OverrideEscape = {};
}

AnimSlot::~AnimSlot()
{
    MESSAGE_CLASS_DESTROYED(AnimSlot);
}

const char* const AnimSlot::GetTarget1() const
{
    return m_TargetAnimation ? m_TargetAnimation->AddResToOpenListAndReturnName() : nullptr;
}

void AnimSlot::SetTarget1(const char* const target1)
{
    AssetLoader assload(target1);
    m_TargetAnimation = (AnimationAsset*)assload.m_AssetPtr;

    if (m_TargetAnimation->m_ResourceTimestamp)
    {
        m_LoopMode = (LoopMode)m_TargetAnimation->m_LoopMode;
        m_Speed = GetSpeed();
    }
}

AnimSlot::LoopMode AnimSlot::GetLoopmode1() const
{
    return m_LoopMode;
}

void AnimSlot::SetLoopmode1(const LoopMode loopmode)
{
    m_LoopMode = loopmode;
}

const float AnimSlot::GetSpeed() const
{
    return m_Speed;
}

void AnimSlot::SetSpeed(const float speed)
{
    m_Speed = speed;
}

const float AnimSlot::GetCrossBlendSpeed() const
{
    return m_CrossBlendSpeed;
}

void AnimSlot::SetCrossBlendSpeed(const float speed)
{
    m_CrossBlendSpeed = speed;
}

const float AnimSlot::GetCrossBlendFrames() const
{
    return m_CrossBlendSpeed;
}

void AnimSlot::SetCrossBlendFrames(const int frames)
{
    m_CrossBlendSpeed = 25.f / frames;
}

const bool AnimSlot::GetShouldStallFirstFrame() const
{
    return m_Flags.StallFirstFrame;
}

void AnimSlot::SetShouldStallFirstFrame(const bool stallff)
{
    m_Flags.StallFirstFrame = stallff;
}

const float AnimSlot::GetOverrideAverageX() const
{
    return m_OverrideAverage.x;
}

void AnimSlot::SetOverrideAverageX(const float averagex)
{
    m_OverrideAverage.x = averagex;
}

const float AnimSlot::GetOverrideAverageY() const
{
    return m_OverrideAverage.y;
}

void AnimSlot::SetOverrideAverageY(const float averagey)
{
    m_OverrideAverage.y = averagey;
}

const float AnimSlot::GetOverrideAverageZ() const
{
    return m_OverrideAverage.z;
}

void AnimSlot::SetOverrideAverageZ(const float averagez)
{
    m_OverrideAverage.z = averagez;
}

const float AnimSlot::GetOverrideEscapeX() const
{
    return m_OverrideEscape.x;
}

void AnimSlot::SetOverrideEscapeX(const float escapex)
{
    m_OverrideEscape.x = escapex;
}

const float AnimSlot::GetOverrideEscapeY() const
{
    return m_OverrideEscape.y;
}

void AnimSlot::SetOverrideEscapeY(const float escapey)
{
    m_OverrideEscape.y = escapey;
}

const float AnimSlot::GetOverrideEscapeZ() const
{
    return m_OverrideEscape.z;
}

void AnimSlot::SetOverrideEscapeZ(const float escapez)
{
    m_OverrideEscape.z = escapez;
}

const AnimSlot::AnimFlags AnimSlot::GetAnimFlags() const
{
    return m_AnimFlags;
}

void AnimSlot::SetAnimFlags(const AnimFlags flags)
{
    m_AnimFlags = flags;
}

#pragma message(TODO_IMPLEMENTATION)
void AnimSlot::GetGamePivotStartPos(int* args) const
{
/*
    Vector4f pivotpos;

    if (m_TargetAnimation && m_Flags._1 >= 0)
        m_TargetAnimation->GetPositionForPivot(pivotpos, m_Flags._1, 0);

    args[0] = pivotpos.x;
    args[1] = pivotpos.y;
    args[2] = pivotpos.z;
*/
}

#pragma message(TODO_IMPLEMENTATION)
void AnimSlot::GetGamePivotEndPos(int* args) const
{
/*
    Vector4f pivotpos;

    if (m_TargetAnimation && m_Flags._1 >= 0)
        m_TargetAnimation->GetPositionForPivot(pivotpos, m_Flags._1, m_TargetAnimation->GetNumberOfFrames(m_Flags._1) - 1);

    args[0] = pivotpos.x;
    args[1] = pivotpos.y;
    args[2] = pivotpos.z;
*/
}

void AnimSlot::GetGamePivotPos(int* args) const
{
    Vector4f pos;
    GetGamePivotPos_Impl(pos, *(float*)(args + 3));

    args[0] = (int)pos.x;
    args[1] = (int)pos.y;
    args[2] = (int)pos.z;
}

void AnimSlot::Register()
{
    tAnimSlot = new EntityType("AnimSlot");
    tAnimSlot->InheritFrom(tNode);
    tAnimSlot->SetCreator((CREATOR)Create);

    tAnimSlot->RegisterProperty(tSTRING, "target1", (EntityGetterFunction)&GetTarget1, NULL, NULL, NULL, (EntitySetterFunction)&SetTarget1, NULL, NULL, NULL, "control=resource|type=*.animation", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tINTEGER, "loopmode1", (EntityGetterFunction)&GetLoopmode1, NULL, NULL, NULL, (EntitySetterFunction)&SetLoopmode1, NULL, NULL, NULL, "control=dropdown|LOOP=0|ONESHOT=1|RANDOM=2", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "speed", (EntityGetterFunction)&GetSpeed, NULL, NULL, NULL, (EntitySetterFunction)&SetSpeed, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "crossblendspeed", (EntityGetterFunction)&GetCrossBlendSpeed, NULL, NULL, NULL, (EntitySetterFunction)&SetCrossBlendSpeed, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "crossblendframes", (EntityGetterFunction)&GetCrossBlendFrames, NULL, NULL, NULL, (EntitySetterFunction)&SetCrossBlendFrames, NULL, NULL, NULL, "control=string", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tTRUTH, "stallfirstframe", (EntityGetterFunction)&GetShouldStallFirstFrame, NULL, NULL, NULL, (EntitySetterFunction)&SetShouldStallFirstFrame, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);

    tAnimSlot->RegisterProperty(tNUMBER, "override_average_x", (EntityGetterFunction)&GetOverrideAverageX, NULL, NULL, NULL, (EntitySetterFunction)&SetOverrideAverageX, NULL, NULL, NULL, "control=slider|min=-10|max=10", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "override_average_y", (EntityGetterFunction)&GetOverrideAverageY, NULL, NULL, NULL, (EntitySetterFunction)&SetOverrideAverageY, NULL, NULL, NULL, "control=slider|min=-10|max=10", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "override_average_z", (EntityGetterFunction)&GetOverrideAverageZ, NULL, NULL, NULL, (EntitySetterFunction)&SetOverrideAverageZ, NULL, NULL, NULL, "control=slider|min=-10|max=10", NULL, NULL, -1);

    tAnimSlot->RegisterProperty(tNUMBER, "override_escape_x", (EntityGetterFunction)&GetOverrideEscapeX, NULL, NULL, NULL, (EntitySetterFunction)&SetOverrideEscapeX, NULL, NULL, NULL, "control=slider|min=-10|max=10", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "override_escape_y", (EntityGetterFunction)&GetOverrideEscapeY, NULL, NULL, NULL, (EntitySetterFunction)&SetOverrideEscapeY, NULL, NULL, NULL, "control=slider|min=-10|max=10", NULL, NULL, -1);
    tAnimSlot->RegisterProperty(tNUMBER, "override_escape_z", (EntityGetterFunction)&GetOverrideEscapeZ, NULL, NULL, NULL, (EntitySetterFunction)&SetOverrideEscapeZ, NULL, NULL, NULL, "control=slider|min=-10|max=10", NULL, NULL, -1);

    tAnimSlot->RegisterProperty(tINTEGER, "anim_flags", (EntityGetterFunction)&GetAnimFlags, NULL, NULL, NULL, (EntitySetterFunction)&SetAnimFlags, NULL, NULL, NULL, "control=flags|flag0=freeze root node|flag1=override average z|flag2=override escape x|flag3=override escape y|flag4=override escape z|flag5=override average x|flag6=override all", NULL, NULL, -1);

    tAnimSlot->RegisterScript("getgamepivotstartpos:vector", (EntityFunctionMember)&GetGamePivotStartPos, NULL, NULL, NULL, nullptr, nullptr);
    tAnimSlot->RegisterScript("getgamepivotendpos:vector", (EntityFunctionMember)&GetGamePivotEndPos, NULL, NULL, NULL, nullptr, nullptr);
    tAnimSlot->RegisterScript("getgamepivotpos(number):vector", (EntityFunctionMember)&GetGamePivotPos, NULL, NULL, NULL, nullptr, nullptr);

    tAnimSlot->PropagateProperties();
}

AnimSlot* AnimSlot::Create(AllocatorIndex)
{
    return new AnimSlot;
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f AnimSlot::GetGamePivotPos_Impl(Vector4f& outPos, const float pivotindex) const
{
/*
    int info[8]{};

    if (m_TargetAnimation && m_Flags._1 >= 0 && pivotindex >= 0)
    {
        m_TargetAnimation->GetGamePivotInfo(m_Flags._1, pivotindex, info, m_LoopMode == LOOP, -1, -1);
        outPos = { (float)info[0], (float)info[1], (float)info[2], 0 };
    }
    else
        outPos = BuiltinType::ZeroVector;
*/
    return outPos;
}