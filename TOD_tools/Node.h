#pragma once

#include "Entity.h"
#include "AuxQuadTree.h"

#define NODE_CLASS_SIZE 80

#define NODE_MASK_EMPTY		0
#define NODE_MASK_POSITION	1
#define NODE_MASK_QUADTREE	2
#define NODE_MASK_FRAGMENT	4

#define NODE_FLAG_DISABLED_ON_CUTSCENE 00010000000000000000000000000000b

//	NOTE: this is actual base class for game 'entities'.
class Node : public Entity
{
protected:
	void* lpPositionVtable;	//	NOTE: is Position class virtual methods only?
	unsigned int m_Flags;
	int field_2C;	//	NOTE: it looks like short[2].
	AuxQuadTree* m_QuadTree;
	Node* m_NextSibling;
	class CollisionList* m_CollisionIgnoreList;
	class Position* m_Position;
	Node* m_Parent;
	Node* m_FirstChild;
	class Fragment* m_Fragment;
	char* m_Name;

	virtual Node*	scalar_destructor(bool freeMemory);		//	@86C6D0
	virtual void	destructor();							//	@88F770

	virtual void	_484CC0(int);							//	@484CC0
	virtual Entity* FindNode(const char* nodeName);			//	@88EED0
	virtual void	_88EC20(int);							//	@88EC20
	virtual void	RefreshQuadTree();						//	@88DE70
	virtual void	nullsub_1();							//	@8CB190
	virtual void	_88C300();								//	@88C300	//	NOTE: void _88C300() { *(unsigned char*)0xA3D890 = 1; };
	virtual void	nullsub_2();							//	@8CB190
	virtual char	_484DB0(int, int);						//	@484DB0	//	NOTE: char _484DB0(int, int) { return 0; }
	virtual float	_8F8650(int, int);						//	@8F8650	//	NOTE: float _8F8650(int, int) { return -1.0f; }
	virtual void	nullsub_3(int);							//	@88C600
	virtual void	_88C610();								//	@88C610
	virtual void	nullsub_4(int);							//	@883EC0
	virtual void	nullsub_5();							//	@8CB190
	virtual void	nullsub_6(int);							//	@883EC0
	virtual String* _484E80(String*);						//	@484E80
	virtual Vector4f* _484DC0(Vector4f*);					//	@484DC0

public:
	Node(unsigned char allocationBitmask);	//	@88D4B0
	~Node()
	{
		MESSAGE_CLASS_DESTROYED(Node);
	}

	void			SetParam(int index, void* param, ScriptTypes::ScriptType* type);	//	@86A3C0

	const char*		GetTypename();	//	@891160
	const char*		GetScript();	//	@86A230
	int				GetOrder();		//	@87F4A0
	int				GetRenderOrderGroup();	//	@67B880
	ScriptTypes::ScriptType_Entity* GetParent();	//	@48F380
	bool			IsDisabledOnCutscene();	//	@728D00
	const char*		GetFragment();		//	@88DEA0
	unsigned int	GetFlags();		//	@495DB0
	int				GetRepresentation();	//	@7BD7C0
	void			GetPos(Vector4f* outPos);	//	@483620
	void			GetOrient(Orientation* outOrient);	//	@483680
	bool			IsUsingQuadTree();	//	@88C870
	const char*		GetName();	//	@48C3E0
	float			GetLodThreshold();	//	@53FA20
	float			GetFadeThreshold();	//	@53FA50
	bool			IsSlowFade();	//	@5A1870
	float			GetTraverseDistance();	//	@75E510
	ScriptTypes::ScriptType_Entity* GetScene();	//	@88C570
	ScriptTypes::ScriptType_Entity* GetFirstChild();	//	@495DC0
	ScriptTypes::ScriptType_Entity* GetNextSibling();	//	@495DD0
	float			GetLodDistance();	//	@493F70
	float			GetLodFade();	//	@571950
	int				GetLod();	//	@501080
	bool			IsTaggetForUnload();	//	@88D490
	int				GetUniqueId0();	//	@88B9C0
	int				GetUniqueId1();	//	@88B9F0
	int				GetUserType();	//	@48F390
	const char*		GetIgnoreList();	//	@88ECD0

};

static_assert(sizeof(Node) == NODE_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(Node));