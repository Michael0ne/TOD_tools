#pragma once
#include "Node.h"
#include "ModelAsset.h"
#include "MeshColorAsset.h"
#include "FrameBuffer.h"

class Folder_;

class Model : public Node
{
    friend class Node;
    friend class Entity;
    friend class AssetManager;
    friend class Scene;

    struct AttachedEffect
    {
        Node           *m_Node;
        unsigned int    m_Index;    //  NOTE: index of the pivot it's attachedto?
    };

protected:
    AssetLoader         m_ModelRes;
    MeshColorAsset     *m_ModelLighting;
    uint32_t            field_5C;
    Folder_            *m_AssocFolder;
    uint16_t            m_ActiveTextureSet;
    uint16_t            m_SoloPivot;
    uint8_t             field_68;
    uint8_t             field_69;
    uint8_t             ModelOpacity;
    uint8_t             QuadTreeLodValue;

    union
    {
        struct
        {
            unsigned    Opacity : 8;    //  NOTE: multiply by 0.0039215689 to get actual opacity float value between 0..1.
            unsigned    _8 : 1;
            unsigned    _9 : 1;
            unsigned    _10 : 1;
            unsigned    _11 : 1;
            unsigned    _12 : 1;
            unsigned    _13 : 1;
            unsigned    _14 : 1;
            unsigned    PlaceInHud : 1;
            unsigned    UseVirtualHud : 1;
            unsigned    BacksideTransparent : 1;
            unsigned    BlocksStaticLight : 1;
            unsigned    AddBlend : 1;
            unsigned    DisableZWrite : 1;
            unsigned    DynamicallyLit : 1;
            unsigned    StaticallyLit : 1;
            unsigned    SingleColorMode : 1;
            unsigned    _24 : 1;
            unsigned    _25 : 1;
            unsigned    DepthSorted : 1;
            unsigned    _27 : 1;
            unsigned    _28 : 1;
            unsigned    _29 : 1;
            unsigned    UseHardAlphaFactor : 1;
            unsigned    _31 : 1;
        }               m_FlagBits;
        unsigned int    m_Flags;
    }                   m_ModelFlags;

    union
    {
        struct
        {
            unsigned    GlobalId : 15;
            unsigned    _16 : 4;
            unsigned    HardAlphaFactor : 8;
            unsigned    _28 : 4;
        }               m_FlagBits;
        unsigned int    m_AlphaFlags;
    }                   m_AlphaFlags;

    int                *field_74;   //  NOTE: sizeof = 94, ctor at 0x88A2C0.
    int                 field_78;
    FrameBuffer        *m_FrameBuffer[6];
    int                 field_94;
    int                 field_98;
    int                 field_9C;
    std::vector<AttachedEffect> m_AttachedEffects;
    std::vector<int>    m_List_2;
    std::vector<int>    m_List_3;
    std::vector<int>    m_List_4;
    int                 field_E0;
    int                 field_E4;
    int                 field_E8;
    int                 m_PivotHideList[5];

public:
    Model();    //  @884BA0
    virtual ~Model();   //  @884D70
    virtual Vector4f*   GetBounds(Vector4f& outBounds); //  @884450
    virtual void        stub19();   //  @8CB190
    virtual void        SetStaticLightingEnabled(const bool enabled);   //  @884190

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

    static void         Register(); //  @889800

private:
    void                SetModelAsset(Model* model);    //  @888F80
    void                _884530();  //  @884530
    void                FindPivot(int* args) const; //  @88A110
    void                SetModelResFromModel(int* args);  //  @88AAB0
    void                ForceInstantiate();   //  @883EB0
    const char*         GetModelRes() const;    //  @884350
    void                SetModelRes(const char* const arg);    //  @888E10
    const float         GetOpacity() const; //  @883F90
    void                SetOpacity(float op); //  @883FB0
    void                SetAddBlend(const bool enabled);    //  @883D40
    const bool          GetAddBlend() const;    //  @883D30
    void                SetZWriteDisabled(const bool disabled); //  @883D80
    const bool          GetZWriteDisabled() const;  //  @883D70
    void                SetUseHardAlphaFactor(const bool enabled);  //  @883D00
    const bool          GetUseHardAlphaFactor() const;  //  @883CF0
    void                SetHardAlphaFactor(const float fac);    //  @884050
    const float         GetHardAlphaFactor() const; //  @884030
    void                SetActiveTextureSet(const unsigned int set);    //  @884150
    const short         GetActiveTextureSet() const;    //  @884140
    const int           GetNumberOfTextureSets() const; //  @8844F0
    const short         GetSoloPivot() const;   //  @883DB0
    void                SetSoloPivot(const short piv);  //  @883ED0
    void                SetPlaceInHud(const bool place);    //  @883DC0
    const bool          GetPlaceInHud() const;  //  @883DF0
    void                SetUseVirtualHud(const bool use);   //  @883E00
    const bool          GetUseVirtualHud() const;   //  @883E30
    void                SetDynamicallyLit(const bool lit);    //  @8840B0
    const bool          GetDynamicallyLit() const;  //  @8840A0
    void                SetStaticallyLit(const bool lit);  //  @65D3A0
    const bool          GetStaticallyLit() const;   //  @884100
    void                SetBacksideTransparent(const bool tr);  //  @884120
    const bool          GetBacksideTransparent() const; //  @884110
    void                SetSingleColorMode(const bool enabled); //  @883EF0
    const bool          GetSingleColorMode() const; //  @883E70
    void                SetBlocksStaticLight(const bool enabled);   //  @883E80
    const bool          GetBlocksStaticLight() const;   //  @883EA0
    void                SetPivotHideList(int* list);  //  @888DD0
    const int*          GetPivotHideList();    //  @67B890
    void                SetDepthSorted(const bool enabled); //  @883E40
    const bool          GetDepthSorted() const; //  @883E60
    void                GetMsep1() const;   //  @42F4F0
    void                SetMsep1() const;   //  @883EC0
    void                GetPivotRelativePos(int* args) const;   //  @88A250
    void                GetPivotRelativePos_Impl(Vector4f& outPos, const unsigned int pivotId) const;   //  @884690
    void                GetMsep2() const;   //  @42F4F0
    void                SetMsep2() const;   //  @883EC0
    void                GetMsep3() const;   //  @42F4F0
    void                SetMsep3() const;   //  @883EC0

    void                RenderIndexedPivot(const uint32_t pivotIndex);  //  @884980

    void                SetLightingFromAsset(AssetLoader* assload, Folder_* associatedFolderPtr);   //  @888DE0

    static Model*       Create(AllocatorIndex);   //  @88AA70
};

extern EntityType* tModel;  //  @

ASSERT_CLASS_SIZE(Model, 256);