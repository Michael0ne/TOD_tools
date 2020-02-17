#pragma once

#include "stdafx.h"

class Node;
class AuxQuadTree;
class Fragment;
class CollisionList;
class Properties;
class NodePosition;
class Entity;

//	TODO: this is ugly. Most of class methods are virtual, make them so.
struct Entity__vtbl
{
	void(__thiscall* Destroy)(Node* _this);
	void(__thiscall* Destroy1)(Node* _this);
	void(__thiscall* TouchPivot)(Node* _this);
	Entity* (__thiscall* FindNode)(Node* _this, char* szName);
	void(__thiscall* stub4)(Node* _this);
	void(__thiscall* stub5)(Node* _this);
	int(__cdecl* Update)();
};

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *-------------- Base class for game objects -----------------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
class Entity
{
	Entity__vtbl* lpVtbl;
	Properties* m_pProperties;
	int field_8;
	char* m_szFragment;
	__int16 field_10;
	__int16 m_nOrder;
	int m_nId;
	int field_18;
	int field_1C;
	void* m_pCreator;
	void* lpUnkVtbl;
	int m_nRenderOrderGroup;
	int field_2C;
	AuxQuadTree* m_pAuxQuadTree;
	Entity* m_pNextSibling;
	CollisionList* m_pIgnoreCollisionList;
	NodePosition* m_vPosition;
	Entity* m_pParent;
	Entity* m_pChild;
	Fragment* m_pFragment;
	char* m_szName;
	int field_50;
	int field_54;
	int* m_p58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;
public:
};

extern Entity * g_pEntity;

static_assert(sizeof(Entity) == 0x78, MESSAGE_WRONG_CLASS_SIZE("Entity"));