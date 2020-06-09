#pragma once

#include "stdafx.h"

#include "List.h"
#include "Entity.h"

#define LIGHT_CLASS_SIZE 228

//	NOTE: Maybe light point properties?
struct Light_unkStru1 {
	int field_0;
	Vector3<float> m_vPosition1;
	Vector3<float> m_vPosition2;
	float m_f1C;
	float m_f20;
	int field_24;
};

class Light : public Entity
{
private:
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	float m_fRange;
	float m_fBrightness;
	int m_nLightType;
	unsigned int m_nEmissionFlags;	//	TODO: should be enum!
	Vector4f m_vUnkPos3;
	Vector4f m_vUnkPos4;
	int field_BC;
	int field_C0;
	int field_C4;
	int field_C8;
	int field_CC;
	int field_D0;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;

	static List<Light>& g_LightsList;

public:
	static List<Light>& GetLightsList();	//	@880D80

	static void OverrideLights(bool unk);	//	@880DC0
};

extern Light* g_Light;

static_assert(sizeof(Light) == LIGHT_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Light));