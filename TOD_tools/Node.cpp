#include "Node.h"
#include "Position.h"
#include "Fragment.h"

void Node::scalar_destructor(bool freeMemory)
{
	if (freeMemory)
		Allocators::ReleaseMemory(this, false);
}

void Node::destructor()
{
	Node* first_child = m_FirstChild;
	Node* next_sibling = nullptr;
	if (first_child)
	{
		do 
		{
			next_sibling = first_child->m_NextSibling;
			delete first_child;
			first_child = next_sibling;
		} while (next_sibling);
	}
	m_FirstChild = nullptr;
	//SetParent(nullptr);

	//	TODO: implementation!
}

void Node::_484CC0(int)
{
	m_Id |= 10;
}

Entity* Node::FindNode(const char* nodeName)
{
	return (*(Entity * (__thiscall*)(Node*, const char*))0x88EED0)(this, nodeName);
}

void Node::_88EC20(int unk)
{
	(*(void(__thiscall*)(Node*, int))0x88EC20)(this, unk);
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

String* Node::_484E80(String* unk)
{
	*unk = String();

	return unk;
}

Vector4f* Node::_484DC0(Vector4f* unk)
{
	*unk = Vector4f();

	return unk;
}

Node::Node(unsigned char allocationBitmask)
{
	MESSAGE_CLASS_CREATED(Node);

	m_Flags = m_Flags & 0xC000F000 | 0x40000000;
	field_2C = 0;
	field_10 = 0;
	m_QuadTree = nullptr;
	m_NextSibling = nullptr;
	m_CollisionIgnoreList = nullptr;
	m_Position = nullptr;
	m_Parent = nullptr;
	m_FirstChild = nullptr;
	m_Fragment = nullptr;
	m_Name = nullptr;

	if (allocationBitmask & NODE_MASK_POSITION)
		m_Position = new Position(this);

	if (allocationBitmask & NODE_MASK_QUADTREE)
		m_QuadTree = new AuxQuadTree(this);

	if (allocationBitmask & NODE_MASK_FRAGMENT)
		m_Fragment = new Fragment(this);

	//	NOTE: this just changes sign?
	if (m_Fragment)
		m_Flags = m_Flags | 0x80000000;
	else
		m_Flags = m_Flags & 0x7FFFFFFF;

	field_2C = field_2C | 0xFFFFFFFF;
	m_Flags = m_Flags | 0xF000;
}

//	TODO: implementation!
void Node::SetParam(int index, void* param, ScriptTypes::ScriptType* type)
{
	(*(void(__thiscall*)(Node*, int, void*, ScriptTypes::ScriptType*))0x86A3C0)(this, index, param, type);
}

//	TODO: implementation!
void Node::SetOrient(const Orientation& orient)
{
	(*(void(__thiscall*)(Node*, const Orientation&))0x88DB20)(this, orient);
}