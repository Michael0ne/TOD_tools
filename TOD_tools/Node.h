#pragma once

#include "Entity.h"
#include "AuxQuadTree.h"

#define NODE_CLASS_SIZE 80

#define NODE_MASK_EMPTY		0
#define NODE_MASK_POSITION	1
#define NODE_MASK_QUADTREE	2
#define NODE_MASK_FRAGMENT	4

#define NODE_FLAG_DISABLED_ON_CUTSCENE 0x10000000

class NodePositionInterface
{
public:
	virtual Vector4f*	GetPosition(Vector4f*) = 0;
	virtual void		GetOrientation(Orientation*) = 0;
	virtual void		CopyPositionToMatrix(D3DMATRIX*) = 0;
	virtual Orientation* GetWorldRotation(Orientation*) = 0;
};

class NodePosition : NodePositionInterface
{
public:
	virtual Vector4f*	GetPosition(Vector4f*) override;	//	@484CD0
	virtual void		GetOrientation(Orientation*) override;	//	@484CF0
	virtual void		CopyPositionToMatrix(D3DMATRIX*) override;	//	@484D40
	virtual Orientation* GetWorldRotation(Orientation*) override;	//	@484D80
};

#define POSITION_CLASS_SIZE 100

class Position
{
public:
	Orientation			m_Orientation;	//	NOTE: not sure this is orientation.
	Vector4f			m_Position;
	Vector4f			m_RightVector;
	Vector4f			m_UpVector;
	Vector4f			m_ForwardVector;
	Vector4f			m_PositionVector;
	class Node*			m_Owner;

public:
	Position(class Node* owner);	//	@892300
	~Position();

	void				GetMatrixForNode(D3DMATRIX& outMat);	//	@892940
	void				ApplyMatrixFromQuadTree();	//	@8923A0
	void				GetMatrix(D3DMATRIX& outMat) const;	//	@88B910
	void				SetTransformationFromMatrix(const D3DMATRIX* mat);	//	@892280
};

static_assert(sizeof(Position) == POSITION_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Position));

//	NOTE: this is actual base class for game 'entities'.
class Node : public Entity
{
public:
	NodePosition*		m_NodePosition;	//	NOTE: is Position class virtual methods only?
	unsigned int		m_Flags;
	short				m_GlobalIdInBlockigList;
	short				m_GlobalIdInSceneList;
	AuxQuadTree*		m_QuadTree;
	Node*				m_NextSibling;
	class CollisionList* m_CollisionIgnoreList;
	Position*			m_Position;
	Node*				m_Parent;
	Node*				m_FirstChild;
	class Fragment*		m_Fragment;
	char*				m_Name;

	virtual				~Node();								//	@86C6D0
	virtual void		Destroy();								//	@88F770
	virtual void		_484CC0(int);							//	@484CC0
	virtual Entity*		FindNode(const char* nodeName);			//	@88EED0
	virtual void		_88EC20(int unk);						//	@88EC20	//	NOTE: this could be 'SetFlagsForChildren'.
	virtual void		RefreshQuadTree();						//	@88DE70	//	NOTE: this could be 'Instantiate'.
	virtual void		Update();								//	@8CB190
	virtual void		_88C300();								//	@88C300	//	NOTE: void _88C300() { *(unsigned char*)0xA3D890 = 1; };
	virtual void		nullsub_2();							//	@8CB190	//	NOTE: could be 'Render' method.
	virtual char		_484DB0(int, int);						//	@484DB0	//	NOTE: char _484DB0(int, int) { return 0; }
	virtual float		_8F8650(int, int);						//	@8F8650	//	NOTE: float _8F8650(int, int) { return -1.0f; }
	virtual void		nullsub_3(int);							//	@88C600
	virtual void		_88C610();								//	@88C610
	virtual void		nullsub_4(int);							//	@883EC0
	virtual void		nullsub_5();							//	@8CB190
	virtual void		nullsub_6(int);							//	@883EC0	//	NOTE: this could be 'GetBoundsRectangle'.
	virtual String*		_484E80(String* unk);					//	@484E80
	virtual Vector4f*	GetBounds(Vector4f& unk);				//	@484DC0

public:
	Node();	//	NOTE: this is not in EXE, but required for List class.
	Node(unsigned char allocationBitmask);	//	@88D4B0

	void* operator new (size_t size)
	{
		return Allocators::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			Allocators::ReleaseMemory(ptr, 0);
	}

	const char*			GetTypename() const;	//	@891160
	const char*			GetScript() const;	//	@86A230

	void				SetParam(int index, void* param, ScriptTypes::ScriptType* type);	//	@86A3C0
	void				SetOrient(const Orientation& orient);	//	@88DB20
	Vector4f*			GetPos(Vector4f& outVec);	//	@483620
	void				GetWorldMatrix(D3DMATRIX& outMat);	//	@4842C0
};

extern ScriptTypes::ScriptType_Entity* tNode;	//	@A3D884

static_assert(sizeof(Node) == NODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Node));