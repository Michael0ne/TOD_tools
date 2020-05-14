#include "Entity.h"

#include "Builtin.h"
#include "AuxQuadTree.h"
#include "Blocks.h"
#include "Properties.h"
#include "Fragment.h"
#include "Scene.h"
#include "CollisionList.h"
#include "CollisionProbe.h"
#include "GfxInternal_Dx9.h"

String& Entity::IgnoredCollisionNodes = *(String*)0xA11CD0;

Entity::Entity(bool bExplicitConstructor)
{
	//	TODO: redo!
	Entity* tmp = Create(g_Blocks->GetAllocatorType());
	tmp->_CopyPropertiesOnCreation(this);
}

Vector4f* Entity::ConvertToWorldSpace(Vector4f* vPos1, Vector4f* vPos2)
{
	//	TODO: implementation!
	return (*(Vector4f * (__thiscall*)(Entity*, Vector4f*, Vector4f*))0x88BDE0)(this, vPos1, vPos2);
}

Entity* Entity::_FindParentFolder()
{
	//	TODO: implementation!
	return (*(Entity * (__thiscall*)(Entity*))0x88D430)(this);
}

void Entity::SetNewPos(const Vector4f& vPos)
{
	Vector4f newPos;

	GetPos(&newPos);

	newPos.x += vPos.x;
	newPos.y += vPos.y;
	newPos.z += vPos.z;
	
	SetPos(newPos);
}

void Entity::SetPos(const Vector4f& vPos)
{
	(*(void(__thiscall*)(Entity*, const Vector4f*))0x88D6C0)(this, &vPos);
}

void Entity::FastSetPos(const Vector4f& vPos)
{
	(*(void(__thiscall*)(Entity*, const Vector4f*))0x88BB20)(this, &vPos);
}

void Entity::ResolveObject(const Entity& _this, int unk)
{
	(*(void (*)(const Entity*, int))0x8C7C30)(&_this, unk);
}

void Entity::SetParam(int index, void* param, ScriptTypes::ScriptType* type)
{
	(*(void(__thiscall*)(Entity*, int, void*, ScriptTypes::ScriptType*))0x86A3C0)(this, index, param, type);
}

AuxQuadTree* Entity::GetEntityQuadTreeOrParentQuadTree()
{
	if (m_pAuxQuadTree)
		return m_pAuxQuadTree;

	Entity* parent_ = m_pParent;

	while (true) {
		if (!parent_)
			break;

		if (parent_->m_pAuxQuadTree)
			return parent_->m_pAuxQuadTree;

		parent_ = parent_->m_pParent;
	}

	return nullptr;
}

void Entity::SetParent(Entity* parent)
{
	(*(void(__thiscall*)(Entity*, Entity*))0x88E8A0)(this, parent);
}

const char* Entity::GetTypename()
{
	return m_pProperties->m_sTypeName;
}

const char* Entity::GetScript()
{
	if (m_pProperties->m_pScript)
		return m_pProperties->m_pScript->m_szName;
	else
		return NULL;
}

int Entity::GetOrder()
{
	return m_nOrder;
}

int Entity::GetRenderOrderGroup()
{
	return m_nRenderOrderGroup << 16 >> 28;
}

Entity* Entity::GetParent()
{
	return m_pParent;
}

bool Entity::IsDisabledOnCutscene()
{
	return (m_nRenderOrderGroup >> 28) & 1;
}

const char* Entity::GetFragment()
{
	if (!m_pFragment)
		return NULL;

	if (!(m_nRenderOrderGroup & 0x40000000))
		return m_pFragment->m_szName;

	if (m_pFragment->m_pUnkStructPtr)
		return m_pFragment->_GetResourcePath();

	return NULL;
}

/*
 *	0 - disable, 1 - dynamic, 2 - invisible, 3 - lock, 4 - open, 5 - volatile, 6 - disable collision, 7 - d.b.r., 8 - l.s.r., 9 - purge names, 10 - e.r.r., 11 - f.r.r.
*/
int Entity::GetFlags()
{
	return m_nRenderOrderGroup & 4095;
}

signed int Entity::GetRepresentation()
{
	signed int flags = 0;

	if (m_vPosition)
		flags = 1;

	if (m_pAuxQuadTree)
		flags |= 2;

	if (m_nRenderOrderGroup < 0)
		flags |= 4;

	return flags;
}

Vector4f* Entity::GetPos(Vector4f* outPos)
{
	if (m_vPosition)
		outPos = &m_vPosition->m_vPosition2;
	else
		*outPos = Builtin::m_ZeroVector;

	return outPos;
}

Orientation* Entity::GetOrient(Orientation* outOrientation)
{
	if (m_vPosition)
		outOrientation = &m_vPosition->m_vOrientation;
	else
		*outOrientation = Builtin::m_Orientation;

	return outOrientation;
}

bool Entity::IsAuxQuadTreeUsed()
{
	if (m_pAuxQuadTree)
		return (m_pAuxQuadTree->field_4D >> 6) & 1;
	else
		return false;
}

const char* Entity::GetName()
{
	return m_szName ? m_szName : NULL;
}

float Entity::GetLodThreshold()
{
	if (m_pAuxQuadTree)
		return *(float*)&m_pAuxQuadTree->m_nFlags * (float)0.0049999999;
	else
		return 0.0f;
}

float Entity::GetFadeThreshold()
{
	if (m_pAuxQuadTree)
		return (*(int*)&m_pAuxQuadTree->m_nFlags & 127) * (float)0.0099999998;
	else
		return 0.0f;
}

bool Entity::GetSlowFade()
{
	if (m_pAuxQuadTree)
		return *(int*)&m_pAuxQuadTree->m_nFlags >> 7;
	else
		return false;
}

float Entity::GetTraverseDistance()
{
	if (m_pAuxQuadTree)
		return *(float*)&m_pAuxQuadTree->m_nFlags;
	else
		return 0.0f;
}

Entity* Entity::GetScene()
{
	return (Entity*)g_Scene;
}

Entity* Entity::GetFirstChild()
{
	return m_pChild;
}

Entity* Entity::GetNextSibling()
{
	return m_pNextSibling;
}

float Entity::GetLodDistance()
{
	if (m_pAuxQuadTree)
		return (float)sqrt(m_pAuxQuadTree->m_nLodDistance & 0x7FFFFFFF);
	else
		return 0.0f;
}

float Entity::GetLodFade()
{
	if (m_pAuxQuadTree)
		return (float)m_pAuxQuadTree->m_bLodFade * (float)0.0039215689;
	else
		return 0.0f;
}

int Entity::GetLod()
{
	if (m_pAuxQuadTree)
		return m_pAuxQuadTree->m_bLod;
	else
		return 0;
}

bool Entity::IsTaggetForUnload()
{
	Entity* fold_ = _FindParentFolder();

	if (fold_ && fold_ != this)
		return (*m_pUnkResourcePtr >> 30) & 1;
	else
		return false;
}

time_t Entity::GetUniqueId0()
{
	if (m_pFragment)
		return m_pFragment->m_nUniqueId0;
	else
		return NULL;
}

int Entity::GetUniqueId1()
{
	if (m_pFragment)
		return m_pFragment->m_nUniqueId1;
	else
		return NULL;
}

int Entity::GetUserType()
{
	if (m_pAuxQuadTree)
		return m_pAuxQuadTree->m_nUserType;
	else
		return NULL;
}

const char* Entity::GetIgnoreList()
{
	if (!m_pIgnoreCollisionList)
		return nullptr;

	*IgnoredCollisionNodes.m_szString = NULL;
	IgnoredCollisionNodes.m_nLength = 0;

	if (m_pIgnoreCollisionList->m_nSize <= 0)
		return IgnoredCollisionNodes.m_szString;

	int index = 0;
	CollisionList* colProbeList = m_pIgnoreCollisionList;

	do {
		CollisionProbe* colProbe = colProbeList->m_pCollisionProbe[index];

		if (colProbe) {
			String strtemp_;
			strtemp_.Format("%010d;", ((Entity*)colProbe)->m_nId >> 8);
			IgnoredCollisionNodes.Append(strtemp_.m_szString);

			colProbeList = m_pIgnoreCollisionList;
		}

		++index;
	} while (index < m_pIgnoreCollisionList->m_nSize);

	return IgnoredCollisionNodes.m_szString;
}

bool Entity::sIsDisabled()
{
	return (m_nRenderOrderGroup | (m_nRenderOrderGroup >> 16)) & 1;
}

bool Entity::sIsSuspended()
{
	//	TODO: wtf is this?
	return Create && *((int*)Create + 1) && (*(int*)(*((int*)Create + 1) + 52) >> 26) & 1;
}

void Entity::sMove(const Vector4f& newPos)
{
	SetNewPos(newPos);
}

Entity* Entity::sCreateNode(const char* szTypeName)
{
	//	TODO: implementation!
	return (*(Entity * (CALLBACK*)(const char*))0x88D0C0)(szTypeName);
}

void Entity::sDestroyNode(Entity* pEnt)
{
	pEnt->lpVtbl->Destroy(pEnt);
}

void Entity::sAnnotatePoint(const Vector4f& vPoint, int unk1, int unk2)
{
	//	0x8935C0 is nullsub. Possible replace with something usable?
	(*(void (*)(const Vector4f*, int, int))0x8935C0)(&vPoint, unk1, unk2);
}

void Entity::sSetCurrentCamera(Entity* pCamera)
{
	g_Scene->m_pCamera = pCamera;
	g_Scene->UpdateCamera();
}

Entity* Entity::sGetCurrentCamera()
{
	return g_Scene->m_pCameraEntity;
}

CollisionProbe* Entity::sGetSharedProbe()
{
	return g_Scene->m_pSharedProbe;
}

Vector4f* Entity::sProject(const Vector4f& vPos)
{
	//	TODO: ambigious call to Entity::Project @87DA10
	return (*(Vector4f * (CALLBACK*)(const Vector4f*))0x88C7F0)(&vPos);
}

Vector2<int> Entity::sGetScreenSize()
{
	Vector2<int> res_;

	GfxInternal_Dx9::GetScreenResolution(res_);

	return res_;
}

unsigned int Entity::sGetPlatform()
{
	//	NOTE: insert platform-detection here?
	return 0;
}

void Entity::sForceLodCalculation()
{
	if (m_pAuxQuadTree)
		m_pAuxQuadTree->CalculateLod();
}

void Entity::sIgnoreNode(Entity* node)
{
	if (!m_pIgnoreCollisionList)
		return;

	SetParam(7, (void*)GetIgnoreList(), *ScriptTypes::tSTRING);
	m_pIgnoreCollisionList->AddEntity(node);
}

void Entity::sRemoveIgnoredNode(Entity* node)
{
	if (!m_pIgnoreCollisionList)
		return;

	SetParam(7, (void*)GetIgnoreList(), *ScriptTypes::tSTRING);
	m_pIgnoreCollisionList->RemoveEntity(node);
}

void Entity::sResetIgnoreList()
{
	if (!m_pIgnoreCollisionList)
		return;

	SetParam(7, (void*)GetIgnoreList(), *ScriptTypes::tSTRING);
	m_pIgnoreCollisionList->Init();
}

void Entity::sPurgeNames(bool bOnlyChildren)
{
	//	NOTE: Implementation inlined from 88E150
	if (m_nRenderOrderGroup & 512)
		bOnlyChildren = true;
	else if (!bOnlyChildren) {
		for (Entity* ent_ = m_pChild; ent_; ent_ = m_pNextSibling)
			ent_->sPurgeNames(bOnlyChildren);

		return;
	}

	if (m_szName)
		delete m_szName;

	m_szName = nullptr;
}

void Entity::sFastSetPos(const Vector4f& vPos)
{
	FastSetPos(vPos);
}

int Entity::sGetBlockId()
{
	return ((m_nId >> 28) & 7) - 1;
}

int Entity::sGetBlockIdBelow()
{
	//	TODO: implementation!
	return NULL;
}

EntityPosition::EntityPosition(Entity* owner)
{
	m_vOrientation = Builtin::m_Orientation;
	m_vPosition2 = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pOwner = owner;
}