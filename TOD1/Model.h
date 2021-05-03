#pragma once
#include "Node.h"
#include "ModelAsset.h"

DECLARE_SCRIPT_ENTITY_CLASS(Model, Node)
	DECLARE_SCRIPT_ENTITY_PROPERTY(modelres, const char*, ModelAsset*)
protected:
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
	int m_List_1[4];	//	NOTE: maybe list of attached models?
	int m_List_2[4];
	int m_List_3[4];
	int m_List_4[4];
	int field_E0;
	int field_E4;
	int field_E8;
	int field_EC;
	int m_List_6[4];

DECLARE_SCRIPT_ENTITY_CLASS_END(Model, 256)