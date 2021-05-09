#include "GeometryEffect.h"

#pragma message(TODO_IMPLEMENTATION)
GeometryEffect::GeometryEffect() : Model()
{
	MESSAGE_CLASS_CREATED(GeometryEffect);

	//m_List_1 = List<Entity>(0x21B00);

	m_EffectLifeTime = 2.0f;
	m_Flags = m_Flags & 0xFFFFFF80;
	field_148 = nullptr;
	field_14C = nullptr;
	field_100 = 0;
	field_134 = 0;
	m_EffectFadeThreshold = 0.02f;
	m_Flags = m_Flags & 0xFFFFFF7F;
	m_EffectRadius = 5.0f;
	m_Flags &= 0xFFFF7FFF;
}

#pragma message(TODO_IMPLEMENTATION)
GeometryEffect::~GeometryEffect()
{
	MESSAGE_CLASS_DESTROYED(GeometryEffect);
}