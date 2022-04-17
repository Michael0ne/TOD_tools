#include "MotionLayer.h"
#include "VectorType.h"
#include "QuaternionType.h"
#include "TruthType.h"

EntityType* tMotionLayer;

void MotionLayer::Motion(int* args)
{
    Motion_Impl((Node*)args[0], args[1]);
}

void MotionLayer::GetWorldStartPos(Vector4f& outStartPos) const
{
    outStartPos = m_WorldStartPos;
}

void MotionLayer::SetWorldStartPos(const Vector4f& startPos)
{
    StoreProperty(32, &m_WorldStartPos, tVECTOR);
    m_WorldStartPos = startPos;
}

void MotionLayer::GetWorldStartOrient(Orientation& outStartOrient) const
{
    outStartOrient = m_WorldStartOrient;
}

void MotionLayer::SetWorldStartOrient(const Orientation& startOrient)
{
    StoreProperty(33, &m_WorldStartOrient, tQUATERNION);
    m_WorldStartOrient = startOrient;
}

void MotionLayer::GetWorldEndPos(Vector4f& outEndPos) const
{
    outEndPos = m_WorldEndPos;
}

void MotionLayer::SetWorldEndPos(const Vector4f& endPos)
{
    StoreProperty(34, &m_WorldEndPos, tVECTOR);
    m_WorldEndPos = endPos;
}

void MotionLayer::GetWorldEndOrient(Orientation& outEndOrient) const
{
    outEndOrient = outEndOrient;
}

void MotionLayer::SetWorldEndOrient(const Orientation& endOrient)
{
    StoreProperty(35, &m_WorldEndOrient, tQUATERNION);
    m_WorldEndOrient = endOrient;
}

const bool MotionLayer::UpdateAbsolute() const
{
    return m_UpdateAbsolute;
}

void MotionLayer::SetUpdateAbsolute(const bool update)
{
    StoreProperty(36, &m_UpdateAbsolute, tTRUTH);
    m_UpdateAbsolute = update;
}

void MotionLayer::UseAbsoluteEndValues(int* args)
{
    UseAbsoluteEndValues_Impl();
}

void MotionLayer::UseAbsoluteStartValues(int* args)
{
    UseAbsoluteStartValues_Impl();
}

void MotionLayer::Motion_Impl(Node* animation, const int val)
{
    if (!animation)
    {
        LogDump::LogA("NULL animation\n");
        return;
    }

    BlendTo((MotionAnimSlot*)animation, val);
    if (!m_Flags.Playing)
    {
        m_Flags.Playing = true;
        SetWeight(1.f);
        m_Flags.Playing = false;
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
    tMotionLayer->RegisterProperty(tTRUTH, "updateabsolute", (EntityGetterFunction)&UpdateAbsolute, (EntitySetterFunction)&SetUpdateAbsolute, nullptr, 36);

    tMotionLayer->PropagateProperties();
}

MotionLayer* MotionLayer::Create(AllocatorIndex)
{
    return new MotionLayer;
}