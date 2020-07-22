#pragma once

#include "Model.h"

#define GEOMETRY_EFFECT_CLASS_SIZE 364

class GeometryEffect : public Model
{
protected:
	int field_100;
	float m_EffectLifeTime;
	unsigned int m_Flags;
	int field_10C;
	int field_110;
	int field_114;
	int field_118;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	int field_12C;
	int field_130;
	int field_134;
	List<int> m_List_1;
	int* field_148;
	int* field_14C;
	float m_EffectRadius;
	int field_154;
	int field_158;
	int field_15C;
	int field_160;
	int field_164;
	float m_EffectFadeThreshold;

public:
	GeometryEffect() : Model()	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(GeometryEffect);

		m_List_1 = List<int>(0x21B00);

		m_EffectLifeTime = 2.0f;
		m_Flags = m_Flags & 0xFFFFFF80;
		field_148 = nullptr;
		field_14C = nullptr;
		field_100 = 0;
		field_7C = 0;
		field_134 = 0;
		m_EffectFadeThreshold = 0.02f;
		m_Flags = m_Flags & 0xFFFFFF7F;
		m_EffectRadius = 5.0f;
		((Model*)this)->m_Flags &= 0xFFFF7FFF;
	}
};

static_assert(sizeof(GeometryEffect) == GEOMETRY_EFFECT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(GeometryEffect));