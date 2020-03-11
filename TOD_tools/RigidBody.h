#pragma once

#include "Entity.h"

class RigidBody
{
		void* m_pVtbl;
		int field_4;
		int field_8;
		int field_C;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		void* Creator;
		void* field_24;
		int field_28;
		int field_2C;
		int field_30;
		int field_34;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		float m_fCollMass;
		D3DMATRIX m_Matrix;
		int field_94;
		int field_98;
		int field_9C;
		int field_A0;
		int field_A4;
		int field_A8;
		int field_AC;
		int field_B0;
		int field_B4;
		int field_B8;
		int field_BC;
		int field_C0;
		int field_C4;
		int field_C8;
		int field_CC;
		int field_D0;
		int m_fElasticity;
		int field_D8;
		Vector4f m_vUnkVec_1;
		Vector4f m_vPosition;
		Orientation m_vOrientation;
		Vector4f m_vLinearSpeed;
		Vector4f m_vAngularMomentum;
		int field_12C;
		int field_130;
		int field_134;
		int field_138;
		Vector4f m_vUnkVec_6;
		int field_14C;
		int field_150;
		int field_154;
		int field_158;
		int field_15C;
		int field_160;
		int field_164;
		int field_168;
		int field_16C;
		int field_170;
		int field_174;
		int field_178;
		int field_17C;
		int field_180;
		int field_184;
		int field_188;
		int field_18C;
		int field_190;
		int field_194;
		int field_198;
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
		Vector4f m_vAngularSpeed;
		int field_20C;
		int field_210;
		int field_214;
		int field_218;
		int field_21C;
		int field_220;
		int field_224;
		int field_228;
		int field_22C;
		int field_230;
		int field_234;
		int field_238;
		int field_23C;
		byte m_bGrounded[4];
};

extern RigidBody* g_pRigidBody;

static_assert(sizeof(RigidBody) == 0x244, MESSAGE_WRONG_CLASS_SIZE("RigidBody"));