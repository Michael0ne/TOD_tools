#pragma once

#include "Model.h"

#define GEOMETRY_EFFECT_CLASS_SIZE 364

struct GeometryEffect_120
{
	Orientation m_Vec_1;
	Vector4f m_Vec_2;
	GeometryEffect* m_Parent;
	float field_24;
	struct GeometryEffect_120* field_28;
	struct GeometryEffect_120* field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	float field_74;
};

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
	GeometryEffect_120* field_148;
	GeometryEffect_120* field_14C;
	float m_EffectRadius;
	class Matrix6* m_ActiveCameraMatrix;
	Vector4f m_CameraPos;
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