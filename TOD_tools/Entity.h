#pragma once

#include "stdafx.h"
#include "Types.h"

#include "MemoryAllocators.h"
#include "StringsPool.h"
#include "ScriptTypes.h"

#define ENTITY_CLASS_SIZE 120

class AuxQuadTree;
class Properties;
class Fragment;
class Entity;
class CollisionList;
class CollisionProbe;

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

class EntityPosition
{
public:
	Quaternion<float> m_vOrientation;
	Vector4<float> m_vPosition2;
	Vector4<float> m_vPosition3;
	Vector4<float> m_vPosition4;
	Vector4<float> m_vPosition5;
	Vector4<float> m_vPosition6;
	Entity* m_pOwner;
public:
	EntityPosition(Entity* owner);	//	@892300	//	NOTE: is it copy constructor?
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
	Entity* (__cdecl* Create)(int allocatorId);
	EntityPosition__vtbl* lpPositionVtable;
	int m_nRenderOrderGroup;
	int field_2C;
	AuxQuadTree* m_pAuxQuadTree;
	Entity* m_pNextSibling;
	CollisionList* m_pIgnoreCollisionList;
	EntityPosition* m_vPosition;
	Entity* m_pParent;
	Entity* m_pChild;
	Fragment* m_pFragment;
	char* m_szName;
	int* m_pUnkResourcePtr;
	int* m_pUnkStructPtr;
	int* m_p58;
	int field_5C;
	int field_60;
	int field_64;
	int field_68;
	int field_6C;
	int field_70;
	int field_74;

public:
	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[Allocators::ALLOCATOR_DEFAULT]->allocate(size);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::MemoryAllocators::ReleaseMemory(ptr, 0);
	}

	Entity()
	{
		debug("Entity created at %X\n", this);
	}

	~Entity()
	{
		debug("Entity destroyed!\n");
	}

	explicit Entity(bool bExplicitConstructor);	//	@86C770

	D3DMATRIX* GetMatrix(D3DMATRIX* outMatrix)	//	@87BC60
	{
		return (*(D3DMATRIX * (__thiscall*)(Entity*, D3DMATRIX*))0x87BC60)(this, outMatrix);
	}
	Vector4f* ConvertToWorldSpace(Vector4f* vPos1, Vector4f* vPos2);	//	@88BDE0
	void _CopyPropertiesOnCreation(Entity* from)	//	@869E20
	{
		(*(void (__thiscall*)(Entity*, Entity*))0x869E20)(this, from);
	}

	Entity*			_FindParentFolder();	//	@88D430
	void			SetNewPos(const Vector4f& vPos);	//	@891280
	void			SetPos(const Vector4f& vPos);	//	@88D6C0
	void			FastSetPos(const Vector4f& vPos);	//	@88BB20
	static void		ResolveObject(const Entity& _this, int unk);	//	@8C7C30
	void			SetParam(int index, void* param, ScriptTypes::ScriptType* type);	//	@86A3C0
	AuxQuadTree*	GetEntityQuadTreeOrParentQuadTree();	//	@88C260
	void			SetParent(Entity* parent);	//	@88E8A0

	//	Getters
	const char*		GetTypename();	//	@891160
	const char*		GetScript();	//	@86A230
	int				GetOrder();		//	@87F4A0
	int				GetRenderOrderGroup();	//	@67B880
	Entity*			GetParent();	//	@48F380
	bool			IsDisabledOnCutscene();	//	@728D00
	const char*		GetFragment();	//	@88DEA0
	int				GetFlags();		//	@495DB0
	signed int		GetRepresentation();	//	@7BD7C0
	Vector4f*		GetPos(Vector4f* outPos);		//	@483620
	Orientation*	GetOrient(Orientation* outOrientation);	//	@483680
	bool			IsAuxQuadTreeUsed();	//	@88C870
	const char*		GetName();		//	@48C3E0
	float			GetLodThreshold();	//	@53FA20
	float			GetFadeThreshold();	//	@53FA50
	bool			GetSlowFade();	//	@5A1870
	float			GetTraverseDistance();	//	@75E510
	Entity*			GetScene();	//	@88C570
	Entity*			GetFirstChild();	//	@495DC0
	Entity*			GetNextSibling();	//	@495DD0
	float			GetLodDistance();	//	@493F70
	float			GetLodFade();	//	@571950
	int				GetLod();	//	@501080
	bool			IsTaggetForUnload();	//	@88D490
	time_t			GetUniqueId0();		//	@88B9C0
	int				GetUniqueId1();		//	@88B9F0
	int				GetUserType();		//	@48F390
	const char*		GetIgnoreList();	//	@88ECD0

	//	Global Scripts
	bool			sIsDisabled();		//	@88C8D0
	bool			sIsSuspended();		//	@88E430
	void			sMove(const Vector4f& newPos);	//	@88E470
	//	skipped...
	Entity*			sCreateNode(const char* szTypeName);	//	@88D0C0
	void			sDestroyNode(Entity* pEnt);		//	@88D0E0
	void			sAnnotatePoint(const Vector4f& vPoint, int unk1, int unk2);	//	@88C620
	void			sSetCurrentCamera(Entity* pCamera);	//	@88C750
	Entity*			sGetCurrentCamera();	//	@88C770
	CollisionProbe* sGetSharedProbe();	//	@88C790
	Vector4f*		sProject(const Vector4f& vPos);		//	@88C7F0
	Vector2<int>	sGetScreenSize();	//	@88C7B0
	unsigned int	sGetPlatform();	//	@88C860
	void			sForceLodCalculation();	//	@88D0F0
	void			sIgnoreNode(Entity* node);	//	@88F4D0
	void			sRemoveIgnoredNode(Entity* node);	//	@88F450
	void			sResetIgnoreList();	//	@88F490
	void			sPurgeNames(bool bOnlyChildren);	//	@891CC0
	void			sFastSetPos(const Vector4f& vPos);	//	@4CEEB0
	void			sFastSetOrient(const Orientation& vOrient);	//	@5A1830
	void			sTouchPivot();	//	@5A16D0
	void			sTouchThisPivot();	//	@891170
	int				sGetBlockId();	//	@5A18B0
	int				sGetBlockIdBelow();	//	@88D140

	static String&	IgnoredCollisionNodes;
};

extern Entity * g_Entity;

static_assert(sizeof(Entity) == ENTITY_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Entity));