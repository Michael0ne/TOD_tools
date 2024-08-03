#include "AnimSlot.h"
#include "StringType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "TruthType.h"
#include "BuiltinType.h"

EntityType* tAnimSlot;

void AnimSlot::GetGamePivotStartOrient(Orientation& orient) const
{
    if (TargetAnimation && Flags.GamePivotIndex >= 0)
        TargetAnimation.GetAsset<AnimationAsset>()->GetPivotOrient(orient, Flags.GamePivotIndex, 0);
    else
        orient = BuiltinType::Orient;
}

void AnimSlot::GetGamePivotEndOrient(Orientation& orient) const
{
    if (TargetAnimation && Flags.GamePivotIndex >= 0)
        TargetAnimation.GetAsset<AnimationAsset>()->GetPivotOrient(orient, Flags.GamePivotIndex, TargetAnimation.GetAsset<AnimationAsset>()->GetPivotEndFrame(Flags.GamePivotIndex));
    else
        orient = BuiltinType::Orient;
}

AnimSlot::AnimSlot() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(AnimSlot);

    TargetAnimation = nullptr;
    Flags.StallFirstFrame = true;
    Speed = CrossBlendSpeed = 1.0f;
    LoopMode = LOOP;
    AnimFlags = FREEZE_ROOT_NODE;

    OverrideAverage = {};
    OverrideEscape = {};
}

AnimSlot::~AnimSlot()
{
    MESSAGE_CLASS_DESTROYED(AnimSlot);
}

const char* const AnimSlot::GetTarget1() const
{
    return TargetAnimation ? TargetAnimation.GetAsset<AnimationAsset>()->GetName() : nullptr;
}

void AnimSlot::SetTarget1(const char* const target1)
{
    TargetAnimation = AssetLoader(target1);

    if (TargetAnimation.GetAsset<AnimationAsset>()->m_ResourceTimestamp)
    {
        LoopMode = (LoopMode_t)TargetAnimation.GetAsset<AnimationAsset>()->LoopMode;
        Speed = GetSpeed();
    }
}

AnimSlot::LoopMode_t AnimSlot::GetLoopmode1() const
{
    return LoopMode;
}

void AnimSlot::SetLoopmode1(const LoopMode_t loopmode)
{
    LoopMode = loopmode;
}

const float AnimSlot::GetSpeed() const
{
    return Speed;
}

void AnimSlot::SetSpeed(const float speed)
{
    Speed = speed;
}

const float AnimSlot::GetCrossBlendSpeed() const
{
    return CrossBlendSpeed;
}

void AnimSlot::SetCrossBlendSpeed(const float speed)
{
    CrossBlendSpeed = speed;
}

const float AnimSlot::GetCrossBlendFrames() const
{
    return CrossBlendSpeed;
}

void AnimSlot::SetCrossBlendFrames(const int frames)
{
    CrossBlendSpeed = 25.f / frames;
}

const bool AnimSlot::GetShouldStallFirstFrame() const
{
    return Flags.StallFirstFrame;
}

void AnimSlot::SetShouldStallFirstFrame(const bool stallff)
{
    Flags.StallFirstFrame = stallff;
}

const float AnimSlot::GetOverrideAverageX() const
{
    return OverrideAverage.x;
}

void AnimSlot::SetOverrideAverageX(const float averagex)
{
    OverrideAverage.x = averagex;
}

const float AnimSlot::GetOverrideAverageY() const
{
    return OverrideAverage.y;
}

void AnimSlot::SetOverrideAverageY(const float averagey)
{
    OverrideAverage.y = averagey;
}

const float AnimSlot::GetOverrideAverageZ() const
{
    return OverrideAverage.z;
}

void AnimSlot::SetOverrideAverageZ(const float averagez)
{
    OverrideAverage.z = averagez;
}

const float AnimSlot::GetOverrideEscapeX() const
{
    return OverrideEscape.x;
}

void AnimSlot::SetOverrideEscapeX(const float escapex)
{
    OverrideEscape.x = escapex;
}

const float AnimSlot::GetOverrideEscapeY() const
{
    return OverrideEscape.y;
}

void AnimSlot::SetOverrideEscapeY(const float escapey)
{
    OverrideEscape.y = escapey;
}

const float AnimSlot::GetOverrideEscapeZ() const
{
    return OverrideEscape.z;
}

void AnimSlot::SetOverrideEscapeZ(const float escapez)
{
    OverrideEscape.z = escapez;
}

const AnimSlot::AnimFlags_t AnimSlot::GetAnimFlags() const
{
    return AnimFlags;
}

void AnimSlot::SetAnimFlags(const AnimFlags_t flags)
{
    AnimFlags = flags;
}

void AnimSlot::GetGamePivotStartPos(int* args) const
{
    Vector4f pivotPos{};

    const auto animAsset = TargetAnimation.GetAsset<AnimationAsset>();
    if (animAsset)
    {
        if (Flags.GamePivotIndex >= 0)
            animAsset->GetPivotPos(pivotPos, Flags.GamePivotIndex, 0);
    }

    *reinterpret_cast<Vector4f*>(args) = pivotPos;
}

void AnimSlot::GetGamePivotEndPos(int* args) const
{
    Vector4f pivotPos{};

    const auto animAsset = TargetAnimation.GetAsset<AnimationAsset>();
    if (animAsset)
    {
        if (Flags.GamePivotIndex >= 0)
        {
            const auto pivotEndFrame = animAsset->GetPivotEndFrame(Flags.GamePivotIndex);
            animAsset->GetPivotPos(pivotPos, Flags.GamePivotIndex, pivotEndFrame - 1);
        }
    }

    *reinterpret_cast<Vector4f*>(args) = pivotPos;
}

void AnimSlot::GetGamePivotPos(int* args) const
{
    Vector4f pos;
    GetGamePivotPos_Impl(pos, *(float*)(args + 3));

    *reinterpret_cast<Vector4f*>(args) = pos;
}

void AnimSlot::GetGamePivotOrient(Orientation& orient, const float_t playPos) const
{
    orient = BuiltinType::Orient;
    Orientation pivotOrient;

    if (TargetAnimation && Flags.GamePivotIndex >= 0 && playPos >= 0.f)
        TargetAnimation.GetAsset<AnimationAsset>()->GetPivotInformation(Flags.GamePivotIndex, playPos, pivotOrient, LoopMode == 0, -1, 1);
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

void AnimSlot::GetGamePivotPos_Impl(Vector4f& outPos, const float playPos) const
{
    outPos = BuiltinType::ZeroVector;

    const auto animAsset = TargetAnimation.GetAsset<AnimationAsset>();
    if (!animAsset)
        return;

    if (Flags.GamePivotIndex < 0 || playPos < 0.f)
        return;

    Orientation pivotOrient{};
    animAsset->GetPivotInformation(Flags.GamePivotIndex, playPos, pivotOrient, LoopMode == LOOP, -1, 1);

    outPos = { pivotOrient.x, pivotOrient.y, pivotOrient.z, 0.f };
}