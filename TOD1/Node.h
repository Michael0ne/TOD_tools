#pragma once
#include "Fragment.h"
#include "Entity.h"
#include "AuxQuadTree.h"
#include <directxmath\include\DirectXMath.h>

#define NODE_MASK_EMPTY     0
#define NODE_MASK_POSITION  1
#define NODE_MASK_QUADTREE  2
#define NODE_MASK_FRAGMENT  4

class Folder_;
class CollisionProbe;
class Camera;

class INodeMatrix
{
public:
    virtual Vector4f*   GetPosition(Vector4f*) = 0;
    virtual void        GetOrientation(Orientation*) = 0;
    virtual void        CopyPositionToMatrix(DirectX::XMMATRIX*) = 0;
    virtual Orientation*GetWorldRotation(Orientation*) = 0;
};

class NodePosition : INodeMatrix
{
public:
    virtual Vector4f*   GetPosition(Vector4f*) override;    //  @484CD0
    virtual void        GetOrientation(Orientation*) override;  //  @484CF0 //  NOTE: unused.
    virtual void        CopyPositionToMatrix(DirectX::XMMATRIX*) override;  //  @484D40
    virtual Orientation*GetWorldRotation(Orientation*) override;    //  @484D80
};

class NodeMatrix
{
public:
    Orientation         m_Orientation;  //  NOTE: not sure if this is orientation.
    Vector4f            m_Position;
    Vector4f            m_RightVector;
    Vector4f            m_UpVector;
    Vector4f            m_ForwardVector;
    Vector4f            m_PositionVector;
    class Node         *m_Owner;

public:
    NodeMatrix(class Node* owner);  //  @892300
    ~NodeMatrix();

    void                GetMatrixForNode(DirectX::XMMATRIX& outMat);    //  @892940
    void                ApplyMatrixFromQuadTree();  //  @8923A0
    void                GetMatrix(DirectX::XMMATRIX& outMat) const; //  @88B910
    void                SetTransformationFromMatrix(const DirectX::XMMATRIX& mat);  //  @892280
};

ASSERT_CLASS_SIZE(NodeMatrix, 100);

class CollisionInfo;

//  NOTE: this is actual base class for game 'entities'.
class Node : public Entity
{
public:
    NodePosition       *m_NodePosition; //  NOTE: is Position class virtual methods only?
    union
    {
        struct
        {
            unsigned Disable : 1;               //  0
            unsigned Dynamic : 1;               //  1
            unsigned Invisible : 1;             //  2
            unsigned Lock : 1;                  //  3
            unsigned Open : 1;                  //  4
            unsigned Volatile : 1;              //  5
            unsigned CollisionDisabled : 1;     //  6
            unsigned DBR : 1;                   //  7
            unsigned LSR : 1;                   //  8
            unsigned PurgeNames : 1;            //  9
            unsigned ERR : 1;                   //  10
            unsigned FRR : 1;                   //  11

            unsigned RenderOrderGroup : 3;      //  12
            unsigned _15 : 1;                   //  15
            unsigned _16 : 1;                   //  16
            unsigned _17 : 1;                   //  17
            unsigned _18 : 1;                   //  18
            unsigned _19 : 1;                   //  19
            unsigned _20 : 1;                   //  20
            unsigned _21 : 1;                   //  21
            unsigned _22 : 1;                   //  22
            unsigned _23 : 1;                   //  23
            unsigned _24 : 1;                   //  24
            unsigned _25 : 1;                   //  25
            unsigned _26 : 1;                   //  26
            unsigned _27 : 1;                   //  27
            unsigned DisabledOnCutscene : 1;    //  28
            unsigned _29 : 1;                   //  29
            unsigned HasFragment : 1;           //  30
        };
        unsigned int    m_Flags;
    }                   m_Flags;
    short               m_GlobalIdInBlockigList;
    short               m_GlobalIdInSceneList;
    AuxQuadTree        *m_QuadTree;
    Node               *m_NextSibling;
    CollisionInfo      *m_Collision;
    NodeMatrix         *m_Position;
    Node               *m_Parent;
    Node               *m_FirstChild;
    Fragment           *m_Fragment;
    char               *m_Name;

    virtual             ~Node();                                //  @86C6D0
    virtual void        Destroy();                              //  @88F770
    virtual void        _484CC0(int);                           //  @484CC0
    virtual Node*       FindNode(const char* nodeName);         //  @88EED0
    virtual void        SetFlags(int);                          //  @88EC20 //  NOTE: this could be 'SetFlagsForChildren'.
    virtual void        Instantiate();                          //  @88DE70
    virtual void        Update();                               //  @8CB190
    virtual void        _88C300();                              //  @88C300 //  NOTE: void _88C300() { *(unsigned char*)0xA3D890 = 1; };
    virtual void        Render();                               //  @8CB190
    virtual char        ProcessCollision(int, int);             //  @484DB0 //  NOTE: char _484DB0(int, int) { return 0; }
    virtual float       _8F8650(int, int);                      //  @8F8650 //  NOTE: float _8F8650(int, int) { return -1.0f; }
    virtual void        nullsub_3(int);                         //  @88C600
    virtual void        SaveData();                             //  @88C610 //  NOTE: this is called when a save is initiated.
    virtual void        nullsub_4(int);                         //  @883EC0
    virtual void        DestroyFrameBuffers();                  //  @8CB190
    virtual void        nullsub_6(const std::vector<void*>&);    //  @883EC0
    virtual String*     GetResourceName(String* unk);           //  @484E80
    virtual Vector4f*   GetBounds(Vector4f& outBounds);         //  @484DC0

public:
    Node(unsigned char allocationBitmask);  //  @88D4B0

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    const char*         GetTypename() const;    //  @891160
    const char*         GetScript() const;  //  @86A230
    unsigned int        GetFlags() const;   //  @495DB0

    void                SaveScriptThreadData(); //  @86ACB0
    void                StoreScriptData();  //  @86B610
    void                SetScriptData(Defragmentator* defrag, EntityScriptData* data);  //  @48B700
    void                GetWorldPos(Vector4f& pos) const; //  @484370
    void                StoreProperty(const int index, const void* param, DataType* type);   //  @86A3C0
    void                SetOrient(const Orientation& orient);   //  @88DB20
    Vector4f*           GetPos(Vector4f& outVec);   //  @483620
    void                GetWorldMatrix(DirectX::XMMATRIX& outMat) const;    //  @4842C0
    Node*               GetParent() const;  //  @48F380
    void                SetParent(const Node* parent);  //  @88E9A0
    void                DestroyAddon(); //  @88EB00 //  NOTE: actually unused.
    void                SetName(const char* const name);   //  @88D610
    void                SetPos(const Vector4f&);    //  @88D6C0
    const char*         GetFragment() const;    //  @88DEA0
    void                CalculateLod(unsigned int);  //  @88D100
    void                _88E6A0(Node* node);    //  @88E6A0
    AuxQuadTree*        GetEntityQuadTreeOrParentQuadTree() const;  //  @88C260
    void                _869EC0(const unsigned int paramind, const void* paramptr, DataType& paramtype);    //  @869EC0
    void                _869F80(const unsigned int paramind, const void* paramptr, DataType& paramtype);    //  @869F80
    void                TriggerGlobalScript(int scriptId, void* args);  //  @86A340
    void                _88C310(struct CameraMatrix* cammat);  //  @88C310
    void                InstantiateAllChildren();   //  @88C2B0
    void                DestroyChildren();  //  @8752A0
    void                ClearFromBlockingList();    //  @88E950
    void                ClearFromSceneList();   //  @88E980
    void                SetFragment(const char* const fragmentpath);    //  @88F8A0
    void                TryInstantiate(); //   @88C2D0
    void                FindNode_Impl(int* args);  //  @88EB60
    void                ConvertFromWorldSpace(Vector4f& outPos, const Vector4f& inPos); //  @88BC90
    void                SetWorldOrient(const Orientation& orientation);   //  @87F240
    void                _86B4B0(const uint32_t propertyId);    //  @86B4B0 //  NOTE: make space for field?
    void                _86A930(const int size, const uint32_t* value, uint32_t* const outval, const int a4) const;   //  @86A930
    void                _86AA10(const int size, const uint32_t* value, uint32_t* const outval, const int a4) const;   //  @86AA10
    void                RotateLocalX(const float x);    //  @483DE0
    void                RotateLocalY(const float y);    //  @891A50
    void                RotateLocalZ(const float z);    //  @484050
    void                ConvertToWorldSpace(Vector4f& outPos, const Vector4f& inPos);   //  @88BDE0
    void                SetRotationX(const float x);    //  @891300
    void                SetRotationY(const float y);    //  @891360
    void                SetRotationZ(const float z);    //  @8913C0
    void                TouchThisPivot(const int);  //  @891170
    void                IsSuspended(bool* suspended) const; //  @88E430
    void                IsDisabled(bool* disabled) const;   //  @88C8D0
    const int           GetUserType() const;    //  @48F390
    const char          GetLod() const; //  @501080
    const short         GetOrder() const;   //  @87F4A0
    void                SetOrder(const short order);    //  @4843B0
    void                SetChildrenPositionToSame();    //  @88D3B0
    void                SetAndUpdateChildrenAuxQuadTree(AuxQuadTree* qdtree);   //  @88D1E0
    void                SetEntityPositionRecursively(NodeMatrix* position); //  @88D1A0
    const int           GetRenderOrderGroup() const;    //  @67B880
    void                SetRenderOrderGroup(int renderordergroup);    //  @67FB40
    const bool          ShouldDisableOnCutscene() const;    //  @728D00
    void                SetShouldDisableOnCutscene(const bool disable); //  @88BA20
    const int           GetRepresentation() const;  //  @7BD7C0
    void                GetOrient(Orientation& orientation) const;    //  @483680
    const bool          ShouldUseAuxQuadTree() const;   //  @88C870
    void                SetShouldUseAuxQuadTree(const bool use);    //  @88C890
    const char* const   GetName() const;    //  @48C3E0
    const float         GetLodThreshold() const;    //  @53FA20
    void                SetLodThreshold(float threshold); //  @5406A0
    void                UpdateQuadTreeLodThreshold();  //  @88BA60
    const float         GetFadeThreshold() const;   //  @53FA50
    void                SetFadeThreshold(float threshold);  //  @540730
    void                UpdateModelFadeThreshold();  //  @88BAA0
    const bool          ShouldSlowFade() const; //  @5A1870
    void                SetShouldSlowFade(const bool slowfade); //  @5A1890
    const float         GetTraverseDistance() const;    //  @75E510
    void                SetTraverseDistance(const float distance);  //  @500BF0
    Entity*             GetScene() const;   //  @88C570
    Entity*             GetFirstChild() const;  //  @495DC0
    Entity*             GetNextSibling() const; //  @495DD0
    const float         GetLodDistance() const; //  @493F70
    const float         GetLodFade() const; //  @571950
    const bool          GetIsTaggedForUnload() const;   //  @88D490
    Folder_*            FindParentFolder() const;   //  @88D430
    const int           GetUniqueId0() const;   //  @88B9C0
    void                SetUniqueId0(const int id); //  @88B9D0
    const int           GetUniqueId1() const;   //  @88B9F0
    void                SetUniqueId1(const int id); //  @88BA00
    void                SetUserType(const int type);    //  @48F3A0
    const char* const   GetIgnoreList() const;  //  @88ECD0
    void                SetIgnoreList(const char* list);    //  @88EDC0
    void                Move(const Vector4f& pos);  //  @88E470
    void                SetNewPos(const Vector4f& pos); //  @891280
    void                MoveLocal(const Vector4f& pos); //  @88E4B0
    void                MoveLocal_Impl(const Vector4f& pos);    //  @4836D0 //  NOTE: moves this node within it's coordinates (pos coords are relative).
    void                GetBlockIdBelow(int* outBlockId) const; //  @88D140
    void                GetBlockId(int* outBlockId) const;  //  @5A18B0
    void                SetOrientFast_Impl(const Orientation& orient);  //  @5A1710
    void                FastSetOrient(const Orientation& orient);   //  @5A1830
    void                SetPosFast_Impl(const Vector4f& pos);   //  @88BB20
    void                PurgeNames(const int dummy);    //  @891CC0
    void                PurgeNames_Impl(bool onlyChildren);   //  @88E150
    void                ResetIgnoreList();  //  @88F490
    void                ForceLodCalculation();  //  @88D0F0
    void                GetPlatform(int* args) const;   //  @88C860
    void                GetScreenSize(Vector3f& size) const;    //  @88C7B0
    void                GetSharedProbe(CollisionProbe** probe) const;   //  @88C790
    void                GetCurrentCamera(Camera** camera) const;    //  @88C770
    void                SetCurrentCamera(Camera** camera) const;  //  @88C750
    void                CreateNode(int* args) const;    //  @88D0C0
    void                CommitCollision();  //  @88C8F0
    void                SetSafePos(const Vector4f& pos, const Orientation& orient); //  @88D000
    void                Rotate(float* args);    //  @88E4F0
    void                RotateX(const float* args); //  @88E560
    void                RotateY(const float* args); //  @88E570
    void                RotateZ(const float* args); //  @88E580
    void                RotateLocal(const float* args); //  @88E590
    void                TouchPivot(const int dummy);    //  @5A16D0
    void                FastSetPos(int* args);  //  @4CEEB0
    void                RemoveIgnoreNode(int* args);    //  @88F450
    void                IgnoreNode(int* args);  //  @88F4D0
    void                AnnotateSphere(int* args) const;  //  @88C6F0
    void                AnnotateLine(int* args) const;  //  @88C670
    void                AnnotatePoint(int* args) const; //  @88C620
    void                DestroyNode(int* args) const; //  @88D0E0
    void                ResolveObjectUsingProbe(int* args) const;   //  @88D0A0
    void                ResolveObject(int* args) const; //  @88D080
    void                GetContactRegion(int* args) const;  //  @88FB40
    void                GetContactMaterialID(int* args) const;  //  @88CFD0
    void                GetContactSurfacePropFields(int* args) const;   //  @88CFA0
    void                GetRealNode(int* args) const;   //  @88CF00
    void                GetContactNode(int* args) const; //  @88CED0
    void                GetSelfRealNode(int* args) const;   //  @88CEA0
    void                GetPeerContactPoint(int* args) const;   //  @88CE30
    void                GetContactPoint(int* args) const;   //  @88CDC0
    void                GetContactNormal(int* args) const;  //  @88CD50
    void                Contacts(int* args) const;  //  @88CD30
    void                SetContactFilter(int* args) const;  //  @88CD10
    void                SetWorldPos(int* args);   //  @88E600
    void                GetWorldOrient(int* args) const;    //  @88CAE0
    void                ConvertDirectionFromWorldSpace(int* args) const;    //  @88CA20
    void                ConvertDirectionToWorldSpace(int* args) const;  //  @88C9C0
    void                CallPropertySetter(const unsigned short propertyId, const void* data);  //  @86A6C0
    void                _86B560(const uint32_t propertyIndex, const uint32_t* param) const; //  @86B560
    Node*               FindNodeSlowRecursive(const char* const nodeName); //  @88DFF0
    void                StorePropertyData(const uint32_t propertyIndex, const uint32_t* const propertyValue, const DataType* propertyType);   //  @86A020
    void                GetScriptEntityPropertyValue(const int propertyId, uint8_t* outPropertyValue) const;  //  @86A6A0
    void                TriggerScript(const uint16_t scriptId, const uint16_t scriptIdA, const uint8_t* scriptParams) const;    //  @86A2B0
    void                SaveScriptData() const;  //  @86AED0
    void                SaveScriptPropertyData(const uint32_t propertyIndex, const uint32_t* param) const; //  @86ABD0
    void                _86BB80(const uint32_t propertyIndex, const uint32_t* param) const; //  @86BB80

private:
    void                Rotate_Impl(const Orientation& orient); //  @891420
    void                RotateX_Impl(const float orientX);  //  @891640
    void                RotateY_Impl(const float orientY);  //  @483BD0
    void                RotateZ_Impl(const float orientZ);  //  @891850
    void                SetOrientation(const Orientation& orient);  //  @8806C0
    void                GetBlockIDBelow(int* args) const;   //  @88D140
    void                GetBlockID(int* args) const;    //  @5A18B0
    void                FastSetPos_Impl(const Vector4f& pos);   //  @88BB20
    void                Project(float* args); //  @88C7F0
    void                HasContactFlags(int* args) const; //  @88CF60
    void                GetContactFlags(int* args) const;   //  @88CF30

    static void         ResolveObjectUsingProbe_Impl(const Node* node, int* a1, CollisionProbe* probe); //  @8C73C0
    static void         ResolveObject_Impl(const Node* node, const int a2);  //  @8C7C30

public:
    void                GetLocalSpaceDirection(Vector4f& outDir, const Vector4f& inOffset) const; //  @88BF10
    void                GetWorldSpaceDirection(Vector4f& outDir, const Vector4f& inOffset) const; //  @88C060
    void                _88D230(const int a1);  //  @88D230
    void                GetAllUniqueIds(std::vector<Utils::UniqueId>& outList) const;  //  @88F510

public:
    static void         _891E70(const String& s, String& sout); //  @891E70

    static void         Register(); //  @88FCD0
    static Node*        Create(AllocatorIndex); //  @88D680

    struct NodeInfo
    {
        Node           *m_Node;
        union
        {
            signed      m_Id : 16;
            signed      m_GlobalId : 6;
            signed      PositionInStack : 2;
        }               m_Flags;
    };

    struct PackedBoundsInfo
    {
        uint16_t    BoundX;
        uint16_t    BoundY;
        uint16_t    BoundZ;
        uint8_t     BoundA; //  NOTE: lowest bit is used to indicate associated quad tree is scene's or not.
        AuxQuadTree*QDTree;
    };

    static std::vector<NodeInfo>    NodesWithUpdateOrBlockingScripts;   //  @A11CC0
    static std::vector<PackedBoundsInfo> NodesList;  //  @A3D8B0

    static String       IgnoredCollisionNodes;  //  @A11CD0
};

extern EntityType* tNode;   //  @A3D884

ASSERT_CLASS_SIZE(Node, 80);