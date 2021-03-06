#pragma once

#include "List.h"
#include "Cloth.h"

#define PHYSSYSTEM_CLASS_SIZE 120

class PhysSystem
{
protected:
	List<Vector4f>	m_List_1;
	List<int>	m_List_2;
	List<int>	m_List_3;
	Cloth*		m_Cloth;
	Vector4f	m_GravityVec;
	int			field_44;
	int			field_48;
	int			field_4C;
	int			field_50;
	float		m_Weight;
	float		m_Damping;
	float		field_5C;
	float		field_60;
	List<int>	m_List_4;
	short		m_NumIterations;
	short		field_76;

public:
	PhysSystem(unsigned int, Cloth*, bool);	//	@931240
};

static_assert(sizeof(PhysSystem) == PHYSSYSTEM_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(PhysSystem));