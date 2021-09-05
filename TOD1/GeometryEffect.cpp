#include "GeometryEffect.h"

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
 m_GeometryEffectFlags.m_FlagBits.Frozen = 1;

 for (Effect* eff = m_Effect; eff; eff = eff->m_ParentEffect)
  eff->m_Lifetime = m_EffectLifeTime - *args;
}