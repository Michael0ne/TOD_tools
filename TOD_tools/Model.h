#pragma once

#include "Node.h"

#define MODEL_CLASS_SIZE 256

class Model : public Node
{
	friend class GeometryEffect;
protected:
	int* m_ModelRes;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	char m_ActiveTextureSet;
	__int16 m_SoloPivot;
	int field_68;
	int m_Flags;
	int field_70;
	int field_74;
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
	List<int> m_List_1;
	List<int> m_List_2;
	List<int> m_List_3;
	List<int> m_List_4;
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	List<int> m_List_6;

public:
	Model();	//	@884BA0
};

static_assert(sizeof(Model) == MODEL_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Model));