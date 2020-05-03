#pragma once

#include "stdafx.h"
#include "Types.h"

#define ENTITY_CLASS_SIZE 120

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
	void(__thiscall* Destroy)(Entity* _this);
	void(__thiscall* Destroy1)(Entity* _this);
	void(__thiscall* TouchPivot)(Entity* _this);
	Entity* (__thiscall* FindNode)(Entity* _this, char* szName);
	void(__thiscall* stub4)(Entity* _this);
	void(__thiscall* stub5)(Entity* _this);
	int(__cdecl* Update)();
};

struct EntityPosition__vtbl
{
	void(__thiscall* GetPosition)(Entity* _this, Vector4f& outVec);	//	@484CD0
	void(__thiscall* GetUnkPos)(Entity* _this, Vector4f& outVec);	//	@484CF0
	void(__thiscall* GetWorldMatrix)(Entity* _this, D3DMATRIX& outMat);	//	@484D40
	void(__thiscall* GetWorldRotation)(Entity* _this, Orientation& outOrient);	//	@9CE80C
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
	EntityPosition__vtbl* lpPositionVtable;
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
	D3DMATRIX* GetMatrix(D3DMATRIX* outMatrix)	//	@87BC60
	{
		return (*(D3DMATRIX * (__thiscall*)(Entity*, D3DMATRIX*))0x87BC60)(this, outMatrix);
	}
};

extern Entity * g_Entity;

static_assert(sizeof(Entity) == ENTITY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE("Entity"));