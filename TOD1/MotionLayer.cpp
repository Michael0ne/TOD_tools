#include "MotionLayer.h"
#include "VectorType.h"
#include "QuaternionType.h"
#include "TruthType.h"

EntityType* tMotionLayer;

void MotionLayer::Motion(int* args)
{
    Motion_Impl((AnimSlot*)args[0], args[1]);
}

void MotionLayer::GetWorldStartPos(Vector4f& outStartPos) const
{
    outStartPos = WorldStartPos;
}

void MotionLayer::SetWorldStartPos(const Vector4f& startPos)
{
    StoreProperty(32, &WorldStartPos, tVECTOR);
    WorldStartPos = startPos;
}

void MotionLayer::GetWorldStartOrient(Orientation& outStartOrient) const
{
    outStartOrient = WorldStartOrient;
}

void MotionLayer::SetWorldStartOrient(const Orientation& startOrient)
{
    StoreProperty(33, &WorldStartOrient, tQUATERNION);
    WorldStartOrient = startOrient;
}

void MotionLayer::GetWorldEndPos(Vector4f& outEndPos) const
{
    outEndPos = WorldEndPos;
}

void MotionLayer::SetWorldEndPos(const Vector4f& endPos)
{
    StoreProperty(34, &WorldEndPos, tVECTOR);
    WorldEndPos = endPos;
}

void MotionLayer::GetWorldEndOrient(Orientation& outEndOrient) const
{
    outEndOrient = outEndOrient;
}

void MotionLayer::SetWorldEndOrient(const Orientation& endOrient)
{
    StoreProperty(35, &WorldEndOrient, tQUATERNION);
    WorldEndOrient = endOrient;
}

const bool MotionLayer::GetUpdateAbsolute() const
{
    return UpdateAbsolute;
}

void MotionLayer::SetUpdateAbsolute(const bool update)
{
    StoreProperty(36, &UpdateAbsolute, tTRUTH);
    UpdateAbsolute = update;
}

void MotionLayer::UseAbsoluteEndValues(int* args)
{
    UseAbsoluteEndValues_Impl();
}

void MotionLayer::UseAbsoluteStartValues(int* args)
{
    UseAbsoluteStartValues_Impl();
}

void MotionLayer::Motion_Impl(AnimSlot* animation, const int val)
{
    if (!animation)
    {
        LogDump::LogA("NULL animation\n");
        return;
    }

    BlendTo_Impl(animation, val);
    if (!Flags.Playing)
    {
        Flags.Playing = true;
        SetWeight(1.f);
        Flags.Playing = false;
    }

    UpdatePivotValues();
}

#pragma message(TODO_IMPLEMENTATION)
void MotionLayer::UpdatePivotValues()
{
}

#pragma message(TODO_IMPLEMENTATION)
void MotionLayer::UseAbsoluteEndValues_Impl()
{
}

#pragma message(TODO_IMPLEMENTATION)
void MotionLayer::UseAbsoluteStartValues_Impl()
{
}

void MotionLayer::Register()
{
    tMotionLayer = new EntityType("MotionLayer");
    tMotionLayer->InheritFrom(tAnimLayer);
    tMotionLayer->SetCreator((CREATOR)Create);

    tMotionLayer->RegisterScript("motion(entity,integer)", (EntityFunctionMember)&Motion);

    tMotionLayer->RegisterProperty(tVECTOR, "worldstartpos", (EntityGetterFunction)&GetWorldStartPos, (EntitySetterFunction)&SetWorldStartPos, nullptr, 32);
    tMotionLayer->RegisterProperty(tQUATERNION, "worldstartorient", (EntityGetterFunction)&GetWorldStartOrient, (EntitySetterFunction)&SetWorldStartOrient, nullptr, 33);
    tMotionLayer->RegisterProperty(tVECTOR, "worldendpos", (EntityGetterFunction)&GetWorldEndPos, (EntitySetterFunction)&SetWorldEndPos, nullptr, 34);
    tMotionLayer->RegisterProperty(tQUATERNION, "worldendorient", (EntityGetterFunction)&GetWorldOrient, (EntitySetterFunction)&SetWorldOrient, nullptr, 35);

    tMotionLayer->RegisterScript("useabsoluteendvalues", (EntityFunctionMember)&UseAbsoluteEndValues);
    tMotionLayer->RegisterScript("useabsolutestartvalues", (EntityFunctionMember)&UseAbsoluteStartValues);
    tMotionLayer->RegisterProperty(tTRUTH, "updateabsolute", (EntityGetterFunction)&GetUpdateAbsolute, (EntitySetterFunction)&SetUpdateAbsolute, nullptr, 36);

    tMotionLayer->PropagateProperties();
}

MotionLayer* MotionLayer::Create(AllocatorIndex)
{
    return new MotionLayer;
}