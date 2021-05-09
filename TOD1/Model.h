#pragma once
#include "Node.h"
#include "ModelAsset.h"
#include "MeshColorAsset.h"
#include "Buffer92.h"

class Model : public Node
{
    struct AttachedEffect
    {
        Node           *m_Node;
        unsigned int    m_Index;    //  NOTE: index of the pivot it's attachedto?
    };

protected:
    ModelAsset         *m_ModelRes;
    int                 field_54;
    MeshColorAsset     *m_LightingRes;
    int                 field_5C;
    Folder_            *m_AssocFolder;
    short               m_ActiveTextureSet;
    short               m_SoloPivot;
    char                field_68[4];
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
    int                 field_70;
    int                *field_74;
    int                 field_78;
    RenderBuffer92     *m_SceneBuffer;
    int                 field_80;
    int                 field_84;
    int                 field_88;
    int                 field_8C;
    int                 field_90;
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
    int                 field_EC;
    std::vector<int>    m_PivotHideList;

public:
    Model();    //  @884BA0

    virtual ~Model();   //  @884D70

    static void         Register(); //  @

private:
    void                _884530();  //  @884530
    static Model*       Create(AllocatorIndex);   //  @
};

extern EntityType* tModel;  //  @

ASSERT_CLASS_SIZE(Model, 256);