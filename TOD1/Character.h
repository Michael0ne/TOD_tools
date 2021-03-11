#pragma once
#include "Node.h"
#include "ResourcesTypes.h"

class Character : public Node
{
protected:
	unsigned int m_Flags_2;
	int field_54;
	int field_58;
	int field_5C;
	class Scene_Buffer* m_SceneBuffer;
	unsigned int m_Flags;
	unsigned int m_Flags_1;
	int field_6C;
	int field_70;
	unsigned int field_74;
	Vector4f m_PhysWorldGravityVector;
	D3DMATRIX m_Matrix;
	class PhysSystem* m_PhysPony;
	float m_PhysPonyZAxisEnd;
	float m_PhysPonyRestlength;
	int field_D4;
	int field_D8;
	int field_DC;
	int field_E0;
	Vector4f field_E4;
	Orientation m_Orient_1;
	Orientation m_Orient_2;
	class PhysSystem* m_PhysBreastL;
	float m_PhysBreastLRestLength;
	int field_11C;
	int field_120;
	int field_124;
	int field_128;
	Orientation m_Orient_3;
	class PhysSystem* m_PhysBreastR;
	float m_f140;
	int field_144;
	int field_148;
	int field_14C;
	int field_150;
	Orientation m_Orient_4;
	class PhysSystem* m_PhysBelly;
	float m_PhysBellyRestlength;
	int field_16C;
	int field_170;
	int field_174;
	int field_178;
	Orientation m_Orient_5;
	class PhysSystem* m_PhysTrouserL;
	int field_190;
	float m_f194;
	float m_PhysTrouserSidewayTolerance;
	float m_PhysTrouserWilderness;
	class PhysSystem* m_PhysTrouserR;
	float field_1A4;
	float m_PhysTrouserRXLimit;
	float field_1AC;
	int field_1B0;
	int field_1B4;
	float m_Opacity;
	int	m_List_1[4];
	int m_TextureSets;
	ResType::Model* m_ModelRes;
	int field_1D4;
	int m_BonesList[4];
	int m_List_3[4];
	int m_List_4[4];
	Vector4f m_Pos_1;
	int m_AttachedNodesList[4];
	int m_AttachedLights[4];
	int m_List_5[4];
	int field_248;
	int field_24C;
	int field_250;
public:
	Character();	//	@914320

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	void		UpdateAnimation();	//	@90B3F0
};

ASSERT_CLASS_SIZE(Character, 596);