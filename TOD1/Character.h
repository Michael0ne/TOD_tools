#pragma once
#include "Node.h"
#include "ModelAsset.h"

class FrameBuffer;
class PhysSystem;

class Character : public Node
{
    struct AttachedNode
    {
        Node                   *m_NodeMain;
        Node                   *m_NodeSecond;
        Node                   *m_NodeThird;
    };

protected:
    union
    {
        struct
        {
            unsigned            _0 : 1;
            unsigned            _1 : 1;
            unsigned            _2 : 1;
            unsigned            _3 : 1;
            unsigned            _4 : 1;
            unsigned            _5 : 1;
            unsigned            _6 : 1;
            unsigned            _7 : 1;
            unsigned            _8 : 1;
            unsigned            _9 : 1;
            unsigned            _10 : 1;
            unsigned            _11 : 1;
            unsigned            _12 : 1;
            unsigned            _13 : 1;
            unsigned            _14 : 1;
            unsigned            _15 : 1;
            unsigned            _16 : 1;
            unsigned            _17 : 1;
            unsigned            _18 : 1;
            unsigned            _19 : 1;
            unsigned            _20 : 1;
            unsigned            _21 : 1;
            unsigned            _22 : 1;
            unsigned            IsPlayerCharacter : 1;
            unsigned            _24 : 1;
            unsigned            _25 : 1;
            unsigned            _26 : 1;
            unsigned            _27 : 1;
            unsigned            _28 : 1;
            unsigned            _29 : 1;
            unsigned            _30 : 1;
            unsigned            _31 : 1;
        };
    }                           m_Flags;
    FrameBuffer                *m_FrameBuffers[4];
    union
    {
        struct
        {
            unsigned            GamePivotPhysAttachmentIndex : 7;
            unsigned            Freeze : 1;
            unsigned            BipPivotPhysAttachmentIndex : 7;
            unsigned            BoneControl : 1;
        };
    }                           m_Flags_1;
    union
    {
        struct
        {
            unsigned            LCalfPhysAttachmentIndex : 7;
            unsigned            RCalfPhysAttachmentIndex : 7;
            unsigned            DrawPhysData : 1;
            unsigned            PhysicsOn : 1;
        };
    }                           m_Flags_2;
    int                         field_6C;
    int                         field_70;
    union
    {
        struct
        {
            unsigned            _0 : 1;
            unsigned            _1 : 1;
            unsigned            _2 : 1;
            unsigned            _3 : 1;
            unsigned            _4 : 1;
            unsigned            _5 : 1;
            unsigned            _6 : 1;
            unsigned            _7 : 1;
            unsigned            _8 : 1;
            unsigned            _9 : 1;
            unsigned            _10 : 1;
            unsigned            _11 : 1;
            unsigned            _12 : 1;
            unsigned            _13 : 1;
            unsigned            _14 : 1;
            unsigned            _15 : 1;
            unsigned            _16 : 1;
            unsigned            _17 : 1;
            unsigned            _18 : 1;
            unsigned            _19 : 1;
            unsigned            _20 : 1;
            unsigned            PhysPonyZAxislimit : 1;
        };
    }                           m_Flags_3;
    Vector4f                    m_PhysWorldGravityVector;
    D3DMATRIX                   m_Matrix;
    PhysSystem                 *m_PhysPony;
    float                       m_PhysPonyZAxisEnd;
    float                       m_PhysPonyRestlength;
    int                         field_D4;
    int                         field_D8;
    int                         field_DC;
    int                         field_E0;
    Vector4f                    field_E4;
    Orientation                 m_Orient_1;
    Orientation                 m_Orient_2;
    PhysSystem                 *m_PhysBreastL;
    float                       m_PhysBreastLRestLength;
    int                         field_11C;
    int                         field_120;
    int                         field_124;
    int                         field_128;
    Orientation                 m_Orient_3;
    PhysSystem                 *m_PhysBreastR;
    float                       field_140;
    int                         field_144;
    int                         field_148;
    int                         field_14C;
    int                         field_150;
    Orientation                 m_Orient_4;
    PhysSystem                 *m_PhysBelly;
    float                       m_PhysBellyRestlength;
    int                         field_16C;
    int                         field_170;
    int                         field_174;
    int                         field_178;
    Orientation                 m_Orient_5;
    PhysSystem                 *m_PhysTrouserL;
    int                         field_190;
    float                       field_194;
    float                       m_PhysTrouserSidewayTolerance;
    float                       m_PhysTrouserWilderness;
    PhysSystem                 *m_PhysTrouserR;
    float                       field_1A4;
    float                       m_PhysTrouserRXLimit;
    float                       field_1AC;
    int                         field_1B0;
    int                         field_1B4;
    float                       m_Opacity;
    std::vector<int>            m_List_1;
    int                         m_TextureSets;
    AssetLoader                 m_ModelRes;
    std::vector<Node*>          m_PhysAttachments;
    std::vector<int>            m_List_3;
    std::vector<int>            m_List_4;
    Vector4f                    m_BipPivotPos;
    std::vector<AttachedNode>   m_AttachedNodesList;
    std::vector<int>            m_AttachedLights;
    std::vector<int>            m_List_5;
    int                         field_248;
    int                         field_24C;
    int                         field_250;
public:
    Character(); // @914320
    virtual                     ~Character();   //  @910DA0
    virtual void                Instantiate() override; //  @911070
    virtual Vector4f*           GetBounds(Vector4f& outBounds);   //  @908220

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    void                UpdateAnimation(); // @90B3F0
    void                Animate();  //  @910CE0
    void                AttachNode(const unsigned int index, Node* node, const String& nodeName1, const String& nodeName2); //  @910FC0
    void                DetachNode(const unsigned int index);   //  @915D00
    const int           GetPhysAttachmentIndex(const char* attachmentName) const;   //  @9083F0
    const char*         GetModelRes() const;    //  @9159E0
    void                SetModelRes(const char* const modelname);   //  @9147E0
    const bool          IsFrozen() const;   //  @908140
    void                SetIsFrozen(const bool frozen); //  @5AB010
    const bool          IsBoneControl() const;  //  @7552B0
    void                SetIsBoneControl(const bool bonecontrol);   //  @50B410
    const float         GetOpacity() const; //  @4A5B90
    void                SetOpacity(const float opacity);    //  @4A5BA0
    void                SetRestPose();  //  @90B1E0
    const bool          ShouldDrawPhysData() const; //  @908100
    void                SetShouldDrawPhysData(const bool drphdata); //  @9080E0
    const int           GetActiveTextureSet() const;    //  @908190
    void                SetActiveTextureSet(const int texset);  //  @90B380
    const size_t        GetTextureSetSize() const;  //  @90B340
    const char* const   GetAttachment() const;  //  @90F9F0
    void                SetAttachment(const char* attachment);    //  @90FC00
    const bool          IsPlayerCharacter() const;  //  @908150
    void                SetIsPlayerCharacter(const bool playerchar);    //  @908160
    const bool          IsPhysicsOn() const;    //  @908130
    void                SetPhysicsOn(const bool physicson); //  @908110
    void                GetPhysWorldGravityVec(Vector4f& outGravityVec) const;  //  @907AD0
    void                SetPhysWorldGravityVec(const Vector4f& gravityVec); //  @907B00
    const bool          GetCharacterSep1() const;   //  @42F4F0
    void                SetCharacterSep1(const bool);   //  @883EC0
    const float         GetPhysPonyDamping() const; //  @907B20
    void                SetPhysPonyDamping(const float damping);    //  @907B40

    static void         Register(); //  @914A20
    static Character*   Create(AllocatorIndex); //  @9149A0

    static String       CurrentAttachedNode;    //  @A13258
    static int          UpdateBoneCtrlCommand;  //  @A1322C

private:
    void                InitPhysicsSystems();   //  @908490
};

extern EntityType* tCharacter;  //  @A3E128

ASSERT_CLASS_SIZE(Character, 596);