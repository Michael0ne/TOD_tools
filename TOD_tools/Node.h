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
	virtual void		GetWorldMatrix(D3DMATRIX*) = 0;
	virtual Orientation* GetWorldRotation(Orientation*) = 0;
};

class NodePosition : NodePositionInterface
{
public:
	virtual Vector4f*	GetPosition(Vector4f*) override;	//	@484CD0
	virtual void		GetOrientation(Orientation*) override;	//	@484CF0
	virtual void		GetWorldMatrix(D3DMATRIX*) override;	//	@484D40
	virtual Orientation* GetWorldRotation(Orientation*) override;	//	@484D80
};

//	NOTE: this is actual base class for game 'entities'.
class Node : public Entity
{
public:
	NodePosition*		m_NodePosition;	//	NOTE: is Position class virtual methods only?
	unsigned int		m_Flags;
	int					field_2C;	//	NOTE: it looks like short[2].
	AuxQuadTree*		m_QuadTree;
	Node*				m_NextSibling;
	class CollisionList* m_CollisionIgnoreList;
	class Position*		m_Position;
	ScriptTypes::ScriptType_Entity* m_Parent;
	Node*				m_FirstChild;
	class Fragment*		m_Fragment;
	char*				m_Name;

	virtual void	scalar_destructor(bool freeMemory);		//	@86C6D0
	virtual void	Destroy();								//	@88F770
	virtual void	_484CC0(int);							//	@484CC0
	virtual Entity* FindNode(const char* nodeName);			//	@88EED0
	virtual void	_88EC20(int unk);						//	@88EC20
	virtual void	RefreshQuadTree();						//	@88DE70
	virtual void	Update();								//	@8CB190
	virtual void	_88C300();								//	@88C300	//	NOTE: void _88C300() { *(unsigned char*)0xA3D890 = 1; };
	virtual void	nullsub_2();							//	@8CB190
	virtual char	_484DB0(int, int);						//	@484DB0	//	NOTE: char _484DB0(int, int) { return 0; }
	virtual float	_8F8650(int, int);						//	@8F8650	//	NOTE: float _8F8650(int, int) { return -1.0f; }
	virtual void	nullsub_3(int);							//	@88C600
	virtual void	_88C610();								//	@88C610
	virtual void	nullsub_4(int);							//	@883EC0
	virtual void	nullsub_5();							//	@8CB190
	virtual void	nullsub_6(int);							//	@883EC0	//	NOTE: this could be 'GetBoundsRectangle'.
	virtual String* _484E80(String* unk);					//	@484E80
	virtual Vector4f* _484DC0(Vector4f* unk);				//	@484DC0

public:
	Node(unsigned char allocationBitmask);	//	@88D4B0
	~Node();

	const char*		GetTypename() const;	//	@891160
	const char*		GetScript() const;	//	@86A230

	void			SetParam(int index, void* param, ScriptTypes::ScriptType* type);	//	@86A3C0
	void			SetOrient(const Orientation& orient);	//	@88DB20
};

extern ScriptTypes::ScriptType_Entity* tNode;	//	@A3D884

static_assert(sizeof(Node) == NODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Node));