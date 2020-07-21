#pragma once

#include "Model.h"

#define BULLET_CLASS_SIZE 360

class Bullet : public Model
{
protected:
	float m_Speed;
	float m_Range;
	int m_DamageType;
	float m_ImpulseMultiplier;
	int field_110;
	int field_114;
	List<int> m_List_1;
	List<int> m_List_2;
	int field_138;
	List<int> m_List_3;
	List<int> m_List_4;
	int field_15C;
	int field_160;
	int field_164;

public:
	Bullet();	//	@8C9ED0
};

static_assert(sizeof(Bullet) == BULLET_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Bullet));