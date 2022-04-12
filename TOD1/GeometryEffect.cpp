#include "GeometryEffect.h"
#include "NumberType.h"
#include "TruthType.h"
#include "IntegerType.h"

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

void GeometryEffect::SetTimeAndFreeze(float* args)
{
    m_Flags.Frozen = 1;

    for (Effect* eff = m_Effect; eff; eff = eff->m_ParentEffect)
        eff->m_Lifetime = m_EffectLifeTime - *args;
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
    tGeometryEffect->RegisterProperty(tTRUTH, "TriggerAtStart", (EntityGetterFunction)&ShouldTriggerAtStart, (EntitySetterFunction)&SetTriggerAtStart, "control=truth|name=");
    tGeometryEffect->RegisterProperty(tTRUTH, "is_target_bracket", (EntityGetterFunction)&IsTargetBracket, (EntitySetterFunction)&SetIsTargetBracket, "control=truth");

    tGeometryEffect->RegisterScript("TriggerEffect", (EntityFunctionMember)&TriggerEffect, 0, 0, 0, "control=button|text=Trigger");
    tGeometryEffect->RegisterScript("Trigger(vector,quaternion)", (EntityFunctionMember)&TriggerAtPosition);
    tGeometryEffect->RegisterScript("Trigger(entity)", (EntityFunctionMember)&TriggerOnEntity);
    tGeometryEffect->RegisterScript("Trigger(entity,vector,quaternion)", (EntityFunctionMember)&TriggerOnEntityPosition);
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