#pragma once

#include "Node.h"

#define DYNAMIC_SURROUND_GEOMETRY_CLASS_SIZE 276

class DynamicSurroundGeometry : public Node
{
public:
	int* m_Texture_1;
	int field_54;
	int* m_Texture_2;
	int field_5C;
	int* m_Texture_3;
	int field_64;
	int* m_Texture_4;
	int field_6C;
	float m_ObjectDensity;
	float m_SpawnRate;
	float m_Size;
	float m_InitialHeight;
	float m_Mass;
	float m_MassSize;
	float m_WindFactor;
	float m_WindLift;
	float m_WindLiftHeight;
	float m_AirResistanceY;
	float m_AirResistanceXZ;
	float m_RotResistanceY;
	float m_RotResistanceXZ;
	float m_ForceOffset;
	float m_ForceRotate;
protected:
	float m_fAC;
	float m_fB0;
	float m_fB4;
	float m_fB8;
public:
	int m_Category;
	int m_MaxCount;
	float m_Radius;	//	NOTE: range is from 0.f to 250.f
protected:
	float m_fC8;
	int field_CC;
	List<int> m_List_1;
	List<int> m_List_2;
	int field_F0;
	int field_F4;
	int field_F8;
	List<int> m_List_3;
	int field_10C;
	float m_f110;

public:
	DynamicSurroundGeometry();	//	@8D8FF0

	static int&		TotalCreated;	//	@A3E0E0
};

static_assert(sizeof(DynamicSurroundGeometry) == DYNAMIC_SURROUND_GEOMETRY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(DynamicSurroundGeometry));