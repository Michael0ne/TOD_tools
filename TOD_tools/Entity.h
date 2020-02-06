#pragma once

#include "stdafx.h"

class Node;
class AuxQuadTree;
class Fragment;
class CollisionList;
class Properties;
class NodePosition;
class Entity;

struct Node__vtbl
{
	void(__thiscall *Destroy)(Node *this_);
	void(__thiscall *Destroy1)(Node *this_);
	void(__thiscall *TouchPivot)(Node *this_);
	Entity *(__thiscall *FindNode)(Node *this_, char *szName);
	void(__thiscall *stub4)(Node *this_);
	void(__thiscall *stub5)(Node *this_);
	void(__thiscall *stub6)(Node *this_);
};

struct Creator_Entity {
	void *lpVtbl;
	int field_4;
	int field_8;
	BYTE field_C;
	byte field_D;
	byte field_E;
	byte field_F;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
};

class Entity
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
	Creator_Entity *m_pCreator;
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

