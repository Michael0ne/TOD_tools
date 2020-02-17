#pragma once

#include "stdafx.h"
#include "Entity.h"

class EditorCamera
{
private:
	Entity entity;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	int field_8C;
	int field_90;
	int field_94;
	int field_98;
	int field_9C;
	int field_A0;
	int field_A4;
	int field_A8;
	int field_AC;
	float m_fB0;
	float m_fB4;
	float m_fB8;
	float m_fBC;
	float m_fC0;
	Vector4<float> m_vUnkVector1;
	byte field_D4[4];
	float m_fD8;
	float m_fDC;
	byte field_E0[4];
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	int field_F4;
	int field_F8;
	int field_FC;
	int field_100;
	int field_104;
	byte field_108[4];
	Vector4<float> m_vUnkVector2;
	Vector4<float> m_vUnkVector3;
	int field_12C;
	int field_130;
	int field_134;
};

static_assert(sizeof(EditorCamera) == 0x138, MESSAGE_WRONG_CLASS_SIZE("EditorCamera"));