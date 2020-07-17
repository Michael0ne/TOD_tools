#pragma once

#include "Node.h"

#define CHARACTER_CLASS_SIZE 596

class Character : public Node
{
protected:
	unsigned int m_Flags_2;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	unsigned int m_Flags;
	unsigned int m_Flags_1;
	int field_6C;
	int field_70;
	int field_74;
	Vector4f m_PhysWorldGravityVector;
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
	int field_B0;
	int field_B4;
	int field_B8;
	int field_BC;
	int field_C0;
	int field_C4;
	int* m_PhysPony;
	float m_fCC;
	float m_PhysPonyRestlength;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int field_F0;
	Orientation m_Orient_1;
	Orientation m_Orient_2;
	int* m_PhysBreastL;
	float m_f118;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	Orientation m_Orient_3;
	int* m_PhysBreastR;
	float m_f140;
	int field_144;
	int field_148;
	int field_14C;
	int field_150;
	Orientation m_Orient_4;
	int* m_PhysBelly;
	float m_f168;
	int field_16C;
	int field_170;
	int field_174;
	int field_178;
	Orientation m_Orient_5;
	int* m_PhysTrouserL;
	int field_190;
	float m_f194;
	float m_PhysTrouserSidewayTolerance;
	float m_PhysTrouserWilderness;
	int* m_PhysTrouserR;
	int field_1A4;
	float m_f1A8;
	int field_1AC;
	int field_1B0;
	int field_1B4;
	float m_Opacity;
	List<int> m_List_1;
	int m_TextureSets;
	int* m_ResourceInfo;
	int field_1D4;
	List<int> m_List_2;
	List<int> m_List_3;
	List<int> m_List_4;
	Vector4f m_Pos_1;
	List<int> m_AttachedNodesList;
	List<int> m_List_6;
	List<int> m_List_7;
	int field_248;
	int field_24C;
	int field_250;
public:
	Character();	//	@914320
};

static_assert(sizeof(Character) == CHARACTER_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Character));