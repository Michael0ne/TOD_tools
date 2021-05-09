#pragma once
#include "Model.h"

struct GeometryEffect_120
{
	Orientation m_Vec_1;
	Vector4f m_Vec_2;
	class GeometryEffect* m_Parent;
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
	unsigned int m_GeometryEffectFlags;
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
	int m_List_1[4];
	GeometryEffect_120* field_148;
	GeometryEffect_120* field_14C;
	float m_EffectRadius;
	class Matrix6* m_ActiveCameraMatrix;
	Vector4f m_CameraPos;
	float m_EffectFadeThreshold;

public:
	virtual ~GeometryEffect();
	GeometryEffect();
};

ASSERT_CLASS_SIZE(GeometryEffect, 364);