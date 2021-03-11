#pragma once
#include "Node.h"

class RigidBody : public Node
{
protected:
	float m_CollMass;
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
	float m_Elasticity;
	int field_D8;
	Vector4f m_UnkVec_1;
	Vector4f m_Position_t;
	Orientation m_Orientation_t;
	Vector4f m_LinearSpeed;
	Vector4f m_AngularMomentum;
	Vector4f m_UnkVec_7;
	Orientation m_UnkVec_6;
	int field_14C;
	int field_150;
	int field_154;
	int field_158;
	int field_15C;
	int field_160;
	int field_164;
	int field_168;
	Vector4f m_UnkVec_8;
	Vector4f m_UnkVec_9;
	Vector4f m_UnkVec_10;
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
	Vector4f m_LinearVelocity;
	Vector4f m_CollAngularSpeed;
	float field_20C;
	float field_210;
	float field_214;
	int field_218;
	float field_21C;
	float field_220;
	float field_224;
	int field_228;
	char field_22C;
	char field_22D;
	__int16 field_22E;
	int m_Unknown_1[4];
	bool m_Grounded;

public:
	RigidBody();	//	@9376E0

	void		SetBoxInertialMatrix(const Vector4f* vec);	//	@9332C0
};

ASSERT_CLASS_SIZE(RigidBody, 580);