#include "GeometryEffect.h"
#include "NumberType.h"
#include "TruthType.h"
#include "IntegerType.h"
#include "GEKeyFrame.h"
#include "Scene.h"

EntityType* tGeometryEffect;

GeometryEffect::~GeometryEffect()
{
    MESSAGE_CLASS_DESTROYED(GeometryEffect);
}

Effect* GeometryEffect::AddEffect()
{
    Effect* effect = new Effect();

    if (m_Effect)
        m_Effect->m_Sibling = effect;

    effect->m_ParentEffect = m_Effect;
    m_Effect = effect;
    m_TotalEffects++;

    return effect;
}

void GeometryEffect::RemoveEffect(Effect* effect)
{
    if (effect == m_ActiveEffect)
        m_ActiveEffect = nullptr;

    if (effect == m_Effect)
        m_Effect = effect->m_ParentEffect;

    if (effect->m_ParentEffect)
        effect->m_ParentEffect->m_Sibling = effect->m_Sibling;

    if (effect->m_Sibling)
        effect->m_Sibling->m_ParentEffect = effect->m_ParentEffect;

    --m_TotalEffects;

    delete effect;
}

void GeometryEffect::AddKeyFrame(GEKeyFrame* keyframe)
{
    unsigned int i = 0;
    if (m_KeyFramesList.size() > 0)
    {
        for (; i < m_KeyFramesList.size(); ++i)
            if (m_KeyFramesList[i]->m_KeyTime > keyframe->m_KeyTime)
                break;
    }

    if (i > m_KeyFramesList.size())
        m_KeyFramesList.resize(i + 1);

    m_KeyFramesList[i + 1] = m_KeyFramesList[i];
    m_KeyFramesList[i] = keyframe;
}

#pragma message(TODO_IMPLEMENTATION)
void GeometryEffect::KillAllEffects_Impl(const Effect* effect)
{
    //  TODO: not sure about 'effect' type.
    Effect* currentEffect = m_Effect;
    m_Effect = nullptr;

    if (m_Effect)
        currentEffect->m_ParentEffect = nullptr;
}

void GeometryEffect::MakeNewActiveEffect()
{
    if (m_ActiveEffect)
        RemoveEffect(m_ActiveEffect);

    m_ActiveEffect = AddEffect();
    m_ActiveEffect->m_Parent = this;
    m_ActiveEffect->m_Position = BuiltinType::ZeroVector;
    m_ActiveEffect->m_Orient = BuiltinType::Orient;
    m_ActiveEffect->m_Lifetime = m_EffectLifeTime;
}

void GeometryEffect::TriggerEffect(int* args)
{
    Start(nullptr);
    MakeNewActiveEffect();
}

void GeometryEffect::TriggerAtPos(int* args)
{
    Effect* newEffect = AddEffect();

    newEffect->m_Position = { (float)args[0], (float)args[1], (float)args[2], 0 };
    newEffect->m_Orient = { (float)args[3], (float)args[4], (float)args[5], (float)args[6] };
    newEffect->m_Lifetime = m_EffectLifeTime;
    newEffect->m_StartTime = Scene::GameTimeMs;
}

void GeometryEffect::TriggerEntity(int* args)
{
    Effect* newEffect = AddEffect();

    newEffect->m_Parent = (GeometryEffect*)args[0];
    newEffect->m_Position = BuiltinType::ZeroVector;
    newEffect->m_Orient = BuiltinType::Orient;
    newEffect->m_Lifetime = m_EffectLifeTime;
    newEffect->m_StartTime = Scene::GameTimeMs;
}

void GeometryEffect::TriggerEntityPos(int* args)
{
    Effect* newEffect = AddEffect();

    newEffect->m_Parent = (GeometryEffect*)args[0];
    newEffect->m_Position = { (float)args[1], (float)args[2], (float)args[3], 0 };
    newEffect->m_Orient = { (float)args[4], (float)args[5], (float)args[6], (float)args[7] };
    newEffect->m_Lifetime = m_EffectLifeTime;
    newEffect->m_StartTime = Scene::GameTimeMs;
}

void GeometryEffect::KillAllEffects(int* args)
{
    KillAllEffects_Impl(nullptr);
}

void GeometryEffect::PauseAll(int* args)
{
    m_Flags.Frozen = *args;
}

void GeometryEffect::SetTimeAndFreeze(float* args)
{
    m_Flags.Frozen = 1;

    for (Effect* eff = m_Effect; eff; eff = eff->m_ParentEffect)
        eff->m_Lifetime = m_EffectLifeTime - *args;
}

const float GeometryEffect::GetEffectFadeThreshold() const
{
    return m_EffectFadeThreshold;
}

void GeometryEffect::SetEffectFadeThreshold(const float th)
{
    m_EffectFadeThreshold = th;
}

const float GeometryEffect::GetEffectRadius() const
{
    return m_EffectRadius;
}

void GeometryEffect::SetEffectRadius(const float radius)
{
    m_EffectRadius = radius;
}

const bool GeometryEffect::GetGesep0() const
{
    return false;
}

void GeometryEffect::SetGesep0(const int)
{
}

const float GeometryEffect::GetEffectLifeTime() const
{
    return m_EffectLifeTime;
}

void GeometryEffect::SetEffectLifeTime(const float lt)
{
    m_EffectLifeTime = lt;
}

const eEffectBlendMode GeometryEffect::GetBlendMode() const
{
    return (eEffectBlendMode)m_Flags.BlendMode;
}

void GeometryEffect::SetBlendMode(const eEffectBlendMode blmode)
{
    m_Flags.BlendMode = blmode;
}

const bool GeometryEffect::IsLooping() const
{
    return m_Flags.Looping;
}

void GeometryEffect::SetIsLooping(const bool looping)
{
    m_Flags.Looping = looping;
}

const bool GeometryEffect::TriggerAtStart() const
{
    return m_Flags.TriggerAtStart;
}

void GeometryEffect::SetTriggerAtStart(const bool trig)
{
    m_Flags.TriggerAtStart = trig;
}

const bool GeometryEffect::IsTargetBracket() const
{
    return m_Flags.TargetBracket;
}

void GeometryEffect::SetIsTargetBracket(const bool target)
{
    m_Flags.TargetBracket = target;
}

void GeometryEffect::Start(int* args)
{
    m_KeyFramesList.clear();
    if (!m_FirstChild)
        return;

    Node* child = m_FirstChild;
    do
    {
        EntityType* tEnt = child->m_ScriptEntity;
        if (tEnt)
        {
            while (tGEKeyFrame != tEnt)
            {
                tEnt = tEnt->Parent;
                if (!tEnt)
                {
                    child = child->m_NextSibling;
                    break;
                }
            }
            if (!tEnt)
            {
                child = child->m_NextSibling;
                continue;
            }

            AddKeyFrame((GEKeyFrame*)child);
        }
        child = child->m_NextSibling;
    } while (child);
}

void GeometryEffect::Register()
{
    tGeometryEffect = new EntityType("GeometryEffect");
    tGeometryEffect->InheritFrom(tModel);
    tGeometryEffect->SetCreator((CREATOR)Create);

    tGeometryEffect->RegisterProperty(tNUMBER, "EffectFadeThreshold", (EntityGetterFunction)&GetEffectFadeThreshold, (EntitySetterFunction)&SetEffectFadeThreshold, "control=slider|min=0|max=0.1");
    tGeometryEffect->RegisterProperty(tNUMBER, "EffectRadius", (EntityGetterFunction)&GetEffectRadius, (EntitySetterFunction)&SetEffectRadius, "control=slider|min=0|max=50");
    tGeometryEffect->RegisterProperty(tTRUTH, "gesep0", (EntityGetterFunction)&GetGesep0, (EntitySetterFunction)&SetGesep0, "control=drawline|name=Global Settings");
    tGeometryEffect->RegisterProperty(tNUMBER, "EffectLifeTime", (EntityGetterFunction)&GetEffectLifeTime, (EntitySetterFunction)&SetEffectLifeTime, "control=slider|min=0|max=6");
    tGeometryEffect->RegisterProperty(tINTEGER, "BlendMode", (EntityGetterFunction)&GetBlendMode, (EntitySetterFunction)&SetBlendMode, "control=dropdown|Normal=0|Add=1");
    tGeometryEffect->RegisterProperty(tTRUTH, "Looping", (EntityGetterFunction)&IsLooping, (EntitySetterFunction)&SetIsLooping, "control=truth");
    tGeometryEffect->RegisterProperty(tTRUTH, "TriggerAtStart", (EntityGetterFunction)&TriggerAtStart, (EntitySetterFunction)&SetTriggerAtStart, "control=truth|name=");
    tGeometryEffect->RegisterProperty(tTRUTH, "is_target_bracket", (EntityGetterFunction)&IsTargetBracket, (EntitySetterFunction)&SetIsTargetBracket, "control=truth");

    tGeometryEffect->RegisterScript("TriggerEffect", (EntityFunctionMember)&TriggerEffect, 0, 0, 0, "control=button|text=Trigger");
    tGeometryEffect->RegisterScript("Trigger(vector,quaternion)", (EntityFunctionMember)&TriggerAtPos);
    tGeometryEffect->RegisterScript("Trigger(entity)", (EntityFunctionMember)&TriggerEntity);
    tGeometryEffect->RegisterScript("Trigger(entity,vector,quaternion)", (EntityFunctionMember)&TriggerEntityPos);
    tGeometryEffect->RegisterScript("KillAllEffects", (EntityFunctionMember)&KillAllEffects);
    tGeometryEffect->RegisterScript("PauseAll(truth)", (EntityFunctionMember)&PauseAll);
    tGeometryEffect->RegisterScript("SetTimeAndFreeze(number)", (EntityFunctionMember)&SetTimeAndFreeze);
    tGeometryEffect->RegisterScript("start", (EntityFunctionMember)&Start);

    tGeometryEffect->PropagateProperties();
}

GeometryEffect* GeometryEffect::Create(AllocatorIndex)
{
    return new GeometryEffect;
}