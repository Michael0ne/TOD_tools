#include "Node.h"
#include "Fragment.h"
#include "AssetManager.h"
#include "LogDump.h"
#include "CollisionList.h"
#include "Scene.h"
#include "TransactionBuffer.h"
#include "SceneSaveLoad.h"
#include "VectorType.h"
#include "Camera.h"

EntityType* tNode;

NodeMatrix::NodeMatrix(Node* owner)
{
	MESSAGE_CLASS_CREATED(NodeMatrix);

	m_Orientation = Orientation();
	m_Position = Vector4f();
	m_Owner = owner;
}

NodeMatrix::~NodeMatrix()
{
	MESSAGE_CLASS_DESTROYED(NodeMatrix);
}

void NodeMatrix::GetMatrixForNode(D3DMATRIX& outMat)
{
	if (m_Owner->m_Id & NODE_MASK_QUADTREE)
		ApplyMatrixFromQuadTree();
	GetMatrix(outMat);
}

#pragma message(TODO_IMPLEMENTATION)
void NodeMatrix::ApplyMatrixFromQuadTree()
{
}

void NodeMatrix::GetMatrix(D3DMATRIX& outMat) const
{
	*(Vector4f*)&(outMat._11) = m_RightVector;
	*(Vector4f*)&(outMat._21) = m_UpVector;
	*(Vector4f*)&(outMat._31) = m_ForwardVector;
	*(Vector4f*)&(outMat._41) = m_PositionVector;
}

void NodeMatrix::SetTransformationFromMatrix(const D3DMATRIX* mat)
{
	m_RightVector = *(Vector4f*)&(mat->_11);
	m_UpVector = *(Vector4f*)&(mat->_21);
	m_ForwardVector = *(Vector4f*)&(mat->_31);
	m_PositionVector = *(Vector4f*)&(mat->_41);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::Destroy()
{
}

void Node::_484CC0(int)
{
	m_Id |= 10;
}

#pragma message(TODO_IMPLEMENTATION)
Entity* Node::FindNode(const char* nodeName)
{
	return (*(Entity * (__thiscall*)(Node*, const char*))0x88EED0)(this, nodeName);
}

void Node::SetFlags(int flags)
{
	if (!m_Flags.m_FlagBits.Volatile && (flags & 0x20) == 0 && flags != (m_Flags.m_Flags & 0xFFF))
	{
		unsigned int _flags = m_Flags.m_Flags & 0xFFF;
		SetParam(2, &_flags, (BaseType*)tINTEGER);
	}

	if (((flags ^ m_Flags.m_FlagBits.Dynamic) & 2) != 0)
	{
		if (m_QuadTree && m_QuadTree->field_38)
		{
			CollisionList* collst = _8A0810(this)->m_Owner->m_CollisionIgnoreList;
			if (collst)
				collst->field_78 |= 0x80000000;
		}
	}

	m_Flags.m_Flags ^= (flags ^ (unsigned short)m_Flags.m_Flags) & 0xFFF;
	_88E6A0(this);

	if (m_QuadTree || (m_Parent && m_Parent->GetEntityQuadTreeOrParentQuadTree()))
		m_Parent->GetEntityQuadTreeOrParentQuadTree()->m_Owner->m_Id |= 8;
}

void Node::RefreshQuadTree()
{
	if (m_CollisionIgnoreList)
		*(int*)((int*)m_CollisionIgnoreList + 0x78) |= 0x80000000;

	m_Id &= 0xFE;

	if (m_QuadTree)
		m_QuadTree->Refresh();

	m_Id |= 0x40;
}

void Node::Update()
{
	return;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88C300()
{
	*(char*)0xA3D890 = (char)1;
}

void Node::nullsub_2()
{
	return;
}

char Node::_484DB0(int, int)
{
	return 0;
}

float Node::_8F8650(int, int)
{
	return -1.0f;
}

void Node::nullsub_3(int)
{
	return;
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88C610()
{
	(*(void(__thiscall*)(void*, Node*))0x86CFF0)(m_ScriptEntity, this);
}

void Node::nullsub_4(int)
{
	return;
}

void Node::nullsub_5()
{
	return;
}

void Node::nullsub_6(int)
{
	return;
}

String* Node::GetResourceName(String* unk)
{
	*unk = String();

	return unk;
}

Vector4f* Node::GetBounds(Vector4f& unk) const
{
	return (unk = Vector4f(), &unk);
}

Node::Node(unsigned char allocationBitmask)
{
	MESSAGE_CLASS_CREATED(Node);

	m_Flags.m_FlagBits.HasFragment = true;
	m_Flags.m_FlagBits._30 = true;
	m_Flags.m_FlagBits._15 = true;
	m_Flags.m_FlagBits._14 = true;
	m_Flags.m_FlagBits._13 = true;
	m_Flags.m_FlagBits._12 = true;

	m_GlobalIdInSceneList = NULL;
	m_QuadTree = nullptr;
	m_NextSibling = nullptr;
	m_CollisionIgnoreList = nullptr;
	m_Position = nullptr;
	m_Parent = nullptr;
	m_FirstChild = nullptr;
	m_Fragment = nullptr;
	m_Name = nullptr;

	m_NodePosition = new NodePosition();

	if (allocationBitmask & NODE_MASK_POSITION)
		m_Position = new NodeMatrix(this);

	if (allocationBitmask & NODE_MASK_QUADTREE)
		m_QuadTree = new AuxQuadTree(this);

	if (allocationBitmask & NODE_MASK_FRAGMENT)
		m_Fragment = new Fragment(this);

	m_Flags.m_FlagBits.HasFragment = m_Fragment != nullptr;

	m_GlobalIdInSceneList = m_GlobalIdInSceneList | 0xFFFFFFFF;
	
	m_Flags.m_FlagBits._12 = true;
	m_Flags.m_FlagBits._13 = true;
	m_Flags.m_FlagBits._14 = true;
	m_Flags.m_FlagBits._15 = true;
}

#pragma message(TODO_IMPLEMENTATION)
Node::~Node()
{
	MESSAGE_CLASS_DESTROYED(Node);
}

const char* Node::GetTypename() const
{
	return m_ScriptEntity->m_TypeName.m_szString;
}

const char* Node::GetScript() const
{
	return m_ScriptEntity->m_Script ? m_ScriptEntity->m_Script->m_Name.m_szString : nullptr;
}

unsigned int Node::GetFlags() const
{
	return m_Flags.m_Flags & 0xFFF;
}

void Node::SetParam(const int index, const void* param, BaseType* type)
{
	if (!m_ScriptEntity)
		return;

	if (m_Flags.m_FlagBits.HasFragment && m_Flags.m_FlagBits.Volatile)
		return;

	unsigned char paramInd = 1 << (index & 7);
	char* paramsMap = (char*)this + index / 8;

	if ((paramInd & paramsMap[8]) == NULL)
		_869EC0(index, param, *type);
	if ((paramInd & paramsMap[13]) == NULL)
		_869F80(index, param, *type);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetOrient(const Orientation& orient)
{
	(*(void(__thiscall*)(Node*, const Orientation&))0x88DB20)(this, orient);
}

Vector4f* Node::GetPos(Vector4f& outVec)
{
	if (m_Position)
		return (outVec = Vector4f(m_Position->m_Position.x, m_Position->m_Position.y, m_Position->m_Position.z, m_Position->m_Position.a), &outVec);
	else
		return (outVec = Vector4f(), &outVec);
}

void Node::GetWorldMatrix(D3DXMATRIX& outMat) const
{
	if (m_Position)
		m_Position->GetMatrixForNode(outMat);
	else
		D3DXMatrixIdentity(&outMat);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetParent(const Node* parent)
{
	if (m_Parent == parent)
		return;

	SetParam(1, &m_Parent, tEntity);

	if (parent)
	{
		Node* _pa = (Node*)parent;
		while (true)
		{
			if (_pa->m_Parent == this)
				break;
			_pa = _pa->m_Parent;
			if (!_pa)
				break;
		}

		if (_pa)
			_pa->SetParent(nullptr);
	}

	//if (!m_QuadTree && m_Parent)
		//SetChildrenPositionToSame();	//	@88D1E0
}

void Node::DestroyAddon()
{
	if (m_Name && strstr(m_Name, "ADD_"))
	{
		SetParent(nullptr);
		Destroy();
		LogDump::LogA("addon destroyed\n");
	}
	else
		for (Node* node_ = m_FirstChild; node_; node_ = node_->m_NextSibling)
			node_->DestroyAddon();
}

void Node::SetName(const char* name)
{
	if (m_Name)
		MemoryManager::ReleaseMemory(m_Name, false);

	if (!name || *name == NULL)
		m_Name = nullptr;
	else
	{
		m_Name = (char*)MemoryManager::AllocatorsList[g_AssetManager->GetAllocatorType()]->Allocate(strlen(name) + 1, NULL, NULL);
		strcpy(m_Name, name);
	}
}

#pragma message(TODO_IMPLEMENTATION)
void Node::SetPos(const Vector4f& newpos)
{
	if (!m_Position)
		return;

	if (m_Position->m_Owner != this)
		return;

	if (m_Position->m_Position.x == newpos.x ||
		m_Position->m_Position.y == newpos.y ||
		m_Position->m_Position.z == newpos.z ||
		Script::_A11C90 == 1.f)
		return;

	SetParam(3, &m_Position, tVECTOR);

	if (m_CollisionIgnoreList)
	{
		if (Script::_A11C90 != 1.f)
		{
			float vecdiffx = newpos.x - m_CollisionIgnoreList->m_Position_1.x;
			float vecdiffy = newpos.y - m_CollisionIgnoreList->m_Position_1.y;
			float vecdiffz = newpos.z - m_CollisionIgnoreList->m_Position_1.z;
			if ( ((vecdiffz * vecdiffz) + (vecdiffy * vecdiffy) + (vecdiffx * vecdiffx)) <= 100.f)
			{
				float negflt = 1.f - Script::_A11C90;
				m_Position->m_Position =
				{
					(m_CollisionIgnoreList->m_Position_1.x * negflt) + (newpos.x * Script::_A11C90),
					(m_CollisionIgnoreList->m_Position_1.y * negflt) + (newpos.y * Script::_A11C90),
					(m_CollisionIgnoreList->m_Position_1.z * negflt) + (newpos.z * Script::_A11C90),
					0.f
				};

				_484CC0(0);

				if (m_QuadTree)
					m_QuadTree->_8A3320();

				for (Node* sibling = m_FirstChild; sibling; sibling = sibling->m_NextSibling)
				{
					sibling->_484CC0(0);
					if (sibling->m_QuadTree)
						sibling->m_QuadTree->_8A3320();

					for (Node* siblingA = sibling->m_FirstChild; siblingA; siblingA = siblingA->m_NextSibling)
					{
						siblingA->_484CC0(0);
						siblingA->_88D230(0);
					}
				}

				if (m_QuadTree ||
					(m_Parent && (m_QuadTree = m_Parent->GetEntityQuadTreeOrParentQuadTree())))
					m_QuadTree->m_Owner->_484CC0(0);

				return;
			}
		}

		m_CollisionIgnoreList->m_Position_1 = { newpos.x, newpos.y, newpos.z };
	}
	else
	{
		if (Camera::PlayerPos && m_ScriptEntity == tBone)
		{
			//	TODO ...
		}
	}

	m_Position->m_Position = newpos;
	_484CC0(0);

	if (m_QuadTree)
		m_QuadTree->_8A3320();

	for (Node* sibling = m_FirstChild; sibling; sibling = sibling->m_NextSibling)
	{
		sibling->_484CC0(0);
		if (sibling->m_QuadTree)
			sibling->m_QuadTree->_8A3320();

		for (Node* siblingA = sibling->m_FirstChild; siblingA; siblingA = siblingA->m_NextSibling)
		{
			siblingA->_484CC0(0);
			siblingA->_88D230(0);
		}
	}

	if (m_QuadTree ||
		(m_Parent && (m_QuadTree = m_Parent->GetEntityQuadTreeOrParentQuadTree())))
		m_QuadTree->m_Owner->_484CC0(0);
}

const char* Node::GetFragment() const
{
	if (m_Fragment)
		if (m_Flags.m_FlagBits.HasFragment)	//	TODO: check if this is correct!
			return m_Fragment->m_Name;
		else
			if (m_Fragment->m_FragmentRes)
				return m_Fragment->m_FragmentRes->AddResToOpenListAndReturnName();
			else
				return nullptr;
	else
		return nullptr;
}

void Node::ForceLodCalculation(unsigned int a1)
{
	if (m_QuadTree)
		m_QuadTree->CalculateLodForAllChildren();
	else
		for (Node* chld = m_FirstChild; chld; chld = chld->m_NextSibling)
			chld->ForceLodCalculation(a1);
}

#pragma message(TODO_IMPLEMENTATION)
void Node::_88E6A0(Node* node)
{
}

AuxQuadTree* Node::GetEntityQuadTreeOrParentQuadTree() const
{
	if (m_QuadTree)
		return m_QuadTree;

	Node* parentnode = m_Parent;
	while (true)
	{
		if (!parentnode)
			break;
		if (parentnode->m_QuadTree)
			return parentnode->m_QuadTree;

		parentnode = parentnode->m_Parent;
	}

	return nullptr;
}

void Node::_869EC0(const unsigned int paramind, const void* paramptr, BaseType& paramtype)
{
	if (!Scene::_A3CEE4)
		return;

	*Scene::_A3CEE4++ = paramind & 0xFFF | ((m_Id & 0x7FFF00) << 8) | (((short)m_Id & 0x7000) - 1) & 0xF000;
	Scene::_A3CEE4 += paramtype.stub9((char*)paramptr, (char*)Scene::_A3CEE4);

	TransactionBuffer* tb = Scene::SceneInstance->m_RewindBuffer1;
	if (tb->m_Buffer[tb->m_Chunks - (int)Scene::_A3CEE4] < 1024)
		tb->_8AA1F0(&Scene::_A3CEE4);

	field_8[paramind / 8] |= 1 << (paramind & 7);
}

void Node::_869F80(const unsigned int paramind, const void* paramptr, BaseType& paramtype)
{
	int* v1 = g_SceneSaveLoad->_873BA0(m_Id >> 8);
	if (v1)
	{
		*v1 = paramind & 0xFFF | ((m_Id & 0x7FFF00) << 8) | (((short)m_Id & 0x7000) - 1) & 0xF000;
		v1++;
		g_SceneSaveLoad->_873C00(m_Id >> 8, &v1[paramtype.stub9((char*)paramptr, (char*)v1)]);
	}

	field_8[paramind / 8 + 5] |= 1 << (paramind & 7);
}

AuxQuadTree* Node::_8A0810(Node* node)
{
	if (node->m_QuadTree)
	{
		AuxQuadTree* qdtr = node->m_QuadTree;
		while (!qdtr->m_Owner->m_CollisionIgnoreList)
		{
			qdtr = qdtr->field_4;
			if (!qdtr)
				return node->m_QuadTree;
		}

		return qdtr;
	}
	else
	{
		Node* nd = node->m_Parent;
		if (nd)
		{
			while (!nd->m_QuadTree)
			{
				nd = nd->m_Parent;
				if (!nd)
					return nullptr;
			}

			AuxQuadTree* qdtr = nd->m_QuadTree;
			if (qdtr)
			{
				while (!qdtr->m_Owner->m_CollisionIgnoreList)
				{
					qdtr = qdtr->field_4;
					if (!qdtr)
						return nd->m_QuadTree;
				}

				return nd->m_QuadTree;
			}
		}
		else
			return nullptr;
	}

	return nullptr;
}

void Node::Register()
{
	tNode = new EntityType("Node");
	tNode->SetCreator((EntityType::CREATOR)Create);
}

Node* Node::Create(AllocatorIndex)
{
	return new Node();
}

Vector4f* NodePosition::GetPosition(Vector4f* outPos)
{
	((Node*)(this - 9))->GetPos(*outPos);
	return outPos;
}

void NodePosition::GetOrientation(Orientation* outOrient)
{
	if (this + 6)
		*outOrient = *(Orientation*)(this + 6);
	else
		*outOrient = Orientation();
}

void NodePosition::CopyPositionToMatrix(D3DMATRIX* outMat)
{
	D3DXMATRIX tempMat;

	((Node*)(this - 9))->GetWorldMatrix(tempMat);
	outMat->_11 = tempMat._41;
	outMat->_12 = tempMat._42;
	outMat->_13 = tempMat._43;
	outMat->_14 = tempMat._44;
}

#pragma message(TODO_IMPLEMENTATION)
Orientation* NodePosition::GetWorldRotation(Orientation* rot)
{
	D3DXMATRIX tempMat;

	((Node*)(this - 9))->GetWorldMatrix(tempMat);

	return nullptr;
}