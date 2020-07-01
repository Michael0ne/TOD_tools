#pragma once

#include "stdafx.h"
#include "Types.h"
#include "List.h"

#define RIGIDBODY_CLASS_SIZE 580

class Entity;

class RigidBody
{
private:
	void* m_pVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	void* Creator;
	void* lpPositionVtbl;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	Entity* m_pEntity;
	int field_44;
	int field_48;
	int field_4C;
	float m_fCollMass;
	D3DMATRIX m_Matrix;
	float field_94;
	float field_98;
	float field_9C;
	float field_A0;
	float field_A4;
	float field_A8;
	float field_AC;
	float field_B0;
	float field_B4;
	float field_B8;
	float field_BC;
	float field_C0;
	float field_C4;
	float field_C8;
	float field_CC;
	float field_D0;
	float m_fElasticity;
	int field_D8;
	Vector4f m_vUnkVec_1;
	Vector4f m_vPosition;
	Orientation m_vOrientation;
	Vector4f m_vLinearSpeed;
	Vector4f m_vAngularMomentum;
	Vector4f m_vUnkVec_7;
	Vector4f m_vUnkVec_6;
	int field_14C;
	int field_150;
	int field_154;
	int field_158;
	int field_15C;
	int field_160;
	int field_164;
	int field_168;
	Vector4f m_vUnkVec_8;
	Vector4f m_vUnkVec_9;
	Vector4f m_vUnkVec_10;
	int field_19C;
	int field_1A0;
	int field_1A4;
	int field_1A8;
	int field_1AC;
	int field_1B0;
	int field_1B4;
	int field_1B8;
	int field_1BC;
	int field_1C0;
	int field_1C4;
	int field_1C8;
	int field_1CC;
	int field_1D0;
	int field_1D4;
	int field_1D8;
	int field_1DC;
	int field_1E0;
	int field_1E4;
	int field_1E8;
	Vector4f m_vLinearVelocity;
	Vector4f m_vCollAngularSpeed;
	float field_20C;
	float field_210;
	float field_214;
	int field_218;
	float field_21C;
	float field_220;
	float field_224;
	int field_228;
	char field_22C[4];
	List<int> m_Unknown_1;
	bool m_bGrounded;

	void		SetBoxInertialMatrix_Impl(const Vector4f& box);	//	@9332C0

public:
	RigidBody();	//	@9376E0
};

static_assert(sizeof(RigidBody) == RIGIDBODY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(RigidBody));