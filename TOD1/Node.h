#pragma once
#include "Fragment.h"
#include "Entity.h"
#include "AuxQuadTree.h"
#include <directxmath\include\DirectXMath.h>

#define NODE_MASK_EMPTY		0
#define NODE_MASK_POSITION	1
#define NODE_MASK_QUADTREE	2
#define NODE_MASK_FRAGMENT	4

class INodeMatrix
{
public:
    virtual Vector4f*	GetPosition(Vector4f*) = 0;
    virtual void		GetOrientation(Orientation*) = 0;
    virtual void		CopyPositionToMatrix(DirectX::XMMATRIX*) = 0;
    virtual Orientation* GetWorldRotation(Orientation*) = 0;
};

class NodePosition : INodeMatrix
{
public:
    virtual Vector4f*	GetPosition(Vector4f*) override;	//	@484CD0
    virtual void		GetOrientation(Orientation*) override;	//	@484CF0
    virtual void		CopyPositionToMatrix(DirectX::XMMATRIX*) override;	//	@484D40
    virtual Orientation* GetWorldRotation(Orientation*) override;	//	@484D80
};

class NodeMatrix
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
    NodeMatrix(class Node* owner);	//	@892300
    ~NodeMatrix();

    void				GetMatrixForNode(DirectX::XMMATRIX& outMat);	//	@892940
    void				ApplyMatrixFromQuadTree();	//	@8923A0
    void				GetMatrix(DirectX::XMMATRIX& outMat) const;	//	@88B910
    void				SetTransformationFromMatrix(const DirectX::XMMATRIX& mat);	//	@892280
};

ASSERT_CLASS_SIZE(NodeMatrix, 100);

//	NOTE: this is actual base class for game 'entities'.
class Node : public Entity
{
public:
    NodePosition*		m_NodePosition;	//	NOTE: is Position class virtual methods only?
    union
    {
        struct
        {
            unsigned Disable : 1;
            unsigned Dynamic : 1;
            unsigned Invisible : 1;
            unsigned Lock : 1;
            unsigned Open : 1;
            unsigned Volatile : 1;
            unsigned CollisionDisabled : 1;
            unsigned DBR : 1;
            unsigned LSR : 1;
            unsigned PurgeNames : 1;
            unsigned ERR : 1;
            unsigned FRR : 1;
            unsigned _12 : 1;
            unsigned _13 : 1;
            unsigned _14 : 1;
            unsigned _15 : 1;
            unsigned _16 : 1;
            unsigned _17 : 1;
            unsigned _18 : 1;
            unsigned _19 : 1;
            unsigned _20 : 1;
            unsigned _21 : 1;
            unsigned _22 : 1;
            unsigned _23 : 1;
            unsigned _24 : 1;
            unsigned _25 : 1;
            unsigned _26 : 1;
            unsigned DisabledOnCutscene : 1;
            unsigned _28 : 1;
            unsigned HasFragment : 1;
            unsigned _30 : 1;
        }				m_FlagBits;
        unsigned int	m_Flags;
    }					m_Flags;
    short				m_GlobalIdInBlockigList;
    short				m_GlobalIdInSceneList;
    AuxQuadTree*		m_QuadTree;
    Node*				m_NextSibling;
    class CollisionList* m_CollisionIgnoreList;
    NodeMatrix*			m_Position;
    Node*				m_Parent;
    Node*				m_FirstChild;
    Fragment*			m_Fragment;
    char*				m_Name;

    virtual				~Node();								//	@86C6D0
    virtual void		Destroy();                              //	@88F770
    virtual void		_484CC0(int);                           //	@484CC0
    virtual Entity*		FindNode(const char* nodeName);         //	@88EED0
    virtual void		SetFlags(int);                          //	@88EC20	//	NOTE: this could be 'SetFlagsForChildren'.
    virtual void		Instantiate();                          //	@88DE70
    virtual void		Update();                               //	@8CB190
    virtual void		_88C300();                              //	@88C300	//	NOTE: void _88C300() { *(unsigned char*)0xA3D890 = 1; };
    virtual void		Render();                               //	@8CB190
    virtual char		ProcessCollision(int, int);             //	@484DB0	//	NOTE: char _484DB0(int, int) { return 0; }
    virtual float		_8F8650(int, int);						//	@8F8650	//	NOTE: float _8F8650(int, int) { return -1.0f; }
    virtual void		nullsub_3(int);							//	@88C600
    virtual void		_88C610();								//	@88C610	//	NOTE: this could be 'ExecuteScript'.
    virtual void		nullsub_4(int);							//	@883EC0
    virtual void		DestroyFrameBuffers();					//	@8CB190
    virtual void		nullsub_6(const std::vector<Node*>);    //	@883EC0 //  NOTE: used when checking for dangling entities.
    virtual String*		GetResourceName(String* unk);           //	@484E80
    virtual Vector4f*	GetBounds(Vector4f& unk) const;         //	@484DC0

public:
    Node() {};	//	NOTE: this is not in EXE, but required for List class.
    Node(unsigned char allocationBitmask);	//	@88D4B0

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    const char*			GetTypename() const;	//	@891160
    const char*			GetScript() const;	//	@86A230
    unsigned int		GetFlags() const;	//	@495DB0

    void				SetParam(const int index, const void* param, BaseType* type);	//	@86A3C0	//	NOTE: probably it's 'SetScriptParam'.
    void				SetOrient(const Orientation& orient);	//	@88DB20
    Vector4f*			GetPos(Vector4f& outVec);	//	@483620
    void				GetWorldMatrix(DirectX::XMMATRIX& outMat) const;	//	@4842C0
    void				SetParent(const Node* parent);	//	@88E9A0
    void				DestroyAddon();	//	@88EB00	//	NOTE: actually unused.
    void				SetName(const char*);	//	@88D610
    void				SetPos(const Vector4f&);	//	@88D6C0
    const char*			GetFragment() const;	//	@88DEA0
    void				ForceLodCalculation(unsigned int);	//	@88D100
    void				_88E6A0(Node* node);	//	@88E6A0
    AuxQuadTree*		GetEntityQuadTreeOrParentQuadTree() const;	//	@88C260
    void                _869EC0(const unsigned int paramind, const void* paramptr, BaseType& paramtype);	//	@869EC0
    void                _869F80(const unsigned int paramind, const void* paramptr, BaseType& paramtype);	//	@869F80
    void                Project_Impl(Vector2f& outvec, const Vector4f& invec);	//	@87DA10
    void				TriggerGlobalScript(int scriptId, void* args);	//	@86A340
    void                _88C310(struct CameraMatrix* cammat);  //  @88C310
    void                InstantiateAllChildren();   //  @88C2B0
    void                DestroyChildren();  //  @8752A0
    void                ClearFromBlockingList();    //  @88E950
    void                ClearFromSceneList();   //  @88E980
    void                _86A190();  //  @86A190
    void                SetFragment(const char* const fragmentpath);    //  @88F8A0
    void                TryInstantiate(); //   @88C2D0

    static AuxQuadTree* _8A0810(Node* node);	//	@8A0810
    static void         _891E70(const String& s, String& sout); //  @891E70

    static void			Register();	//	@88FCD0
    static Node*		Create(AllocatorIndex);	//	@88D680

    struct NodeInfo
    {
        Node           *m_Node;
        bool            m_Enabled;
    };

    struct NodePosInfo
    {
        int             field_0;
        int             field_4;
        AuxQuadTree    *m_QuadTree;
    };

    static std::vector<NodeInfo>    NodesWithUpdateOrBlockingScripts;   //  @A11CC0
    static std::vector<NodePosInfo> NodesList;  //  @A3D8B0
};

extern EntityType* tNode;	//	@A3D884

ASSERT_CLASS_SIZE(Node, 80);