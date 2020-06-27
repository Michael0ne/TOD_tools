#pragma once

#include "stdafx.h"
#include "Types.h"

#define DYNAMIC_SURROUND_GEOMETRY_CLASS_SIZE 276

class DynamicSurroundGeometry
{
private:
	void* lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	void* lpPositionVtbl;
	int field_28;
	int field_2C;
	int* field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	int m_szTexture_1;
	int field_54;
	int m_szTexture_2;
	int field_5C;
	int m_szTexture_3;
	int field_64;
	int m_szTexture_4;
	int field_6C;
	float m_fObjectDensity;
	float m_fSpawnRate;
	float m_fSize;
	float m_fInitialHeight;
	float m_fMass;
	float m_fMassSize;
	float m_fWindFactor;
	float m_fWindLift;
	float m_fWindLiftHeight;
	float m_fAirResistanceY;
	float m_fAirResistanceXZ;
	float m_fRotResistanceY;
	float m_fRotResistanceXZ;
	float m_fForceOffset;
	float m_fForceRotate;
	float m_fAC;
	float m_fB0;
	float m_fB4;
	float m_fB8;
	int m_nCategory;
	int m_nMaxCount;
	float m_fRadius;
	float m_fC8;
	int field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	int field_108;
	int field_10C;
	float m_f110;

public:

};

static_assert(sizeof(DynamicSurroundGeometry) == DYNAMIC_SURROUND_GEOMETRY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(DynamicSurroundGeometry));