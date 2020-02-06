#pragma once

#include "stdafx.h"

class Node__vtbl;
class Properties;
class AuxQuadTree;
class Entity;
class CollisionList;
class NodePosition;
class Node;
class Fragment;

//	NOTE: Maybe light point properties?
struct Light_unkStru1 {
	int field_0;
	Vector3<float> m_vPosition1;
	Vector3<float> m_vPosition2;
	float m_f1C;
	float m_f20;
	int field_24;
};

struct Creator_Light {
	void *lpVtbl;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
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
	ColorRGB m_vColorDynamic;
	ColorRGB m_vColorStatic;
	Light_unkStru1 *m_pUnkStru1;
	int field_74;
	int field_78;
	int field_7C;
	int field_80;
	int field_84;
	int field_88;
	float m_fRange;
	float m_fBrightness;
	int m_nLightType;
	unsigned int m_nEmissionFlags;	//	TODO: should be enum!
	Vector4<float> m_vUnkPos3;
	Vector4<float> m_vUnkPos4;
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
};

class Light
{
	Node__vtbl *lpVtbl;
	Properties *m_pProperties;
	int field_8;
	char *m_szFragment;
	__int16 field_10;
	__int16 m_nOrder;
	int m_nId;
	int field_18;
	int field_1C;
	Creator_Light *m_pCreator;
	void *lpUnkVtbl;
	int m_nRenderOrderGroup;
	int field_2C;
	AuxQuadTree *m_pAuxQuadTree;
	Entity *m_pNextSibling;
	CollisionList *m_pIgnoreCollisionList;
	NodePosition *m_vPosition;
	Node *m_pParent;
	Entity *m_pChild;
	Fragment *m_pFragment;
	char *m_szName;
	int field_50;
	int field_54;
	int field_58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
	int field_78;
public:
};

