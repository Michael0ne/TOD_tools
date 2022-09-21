#pragma once
#include "Node.h"
#include "AssetManager.h"

struct AssetInfo
{
    struct ActualAssetInfo
    {
        void* m_ResourceDataBufferPtr = nullptr; // NOTE: actual resource data read from file written here.
        void* m_ResourceAllocatedAlignedBufferPtr = nullptr; // NOTE: pointer to buffer with resource data size written here.
        unsigned int m_ResourceDataBufferSize = NULL;
    };
    ActualAssetInfo m_AssetInfo_Shared;
    ActualAssetInfo m_AssetInfo_Localised;

    inline AssetInfo() // NOTE: constructor always inlined.
        : m_AssetInfo_Shared(), m_AssetInfo_Localised()
    {
        MESSAGE_CLASS_CREATED(AssetInfo);
    };
};

ASSERT_CLASS_SIZE(AssetInfo, 24);

// NOTE: actual path to header file "/Kernel/Scenegraph/Folder.h".
class Folder_ : public Node
{
    friend class Scene;
    friend class Node;

protected:
    struct
    {
        unsigned    m_BlockId : 28;
        unsigned    _29 : 1;
        unsigned    m_TaggedForUnload : 1;
        unsigned    _31 : 1;
    };
    AssetInfo      *m_AssetBlockInfo;

    void            DestroyAllChildren(); // @88C1D0
    void            ReadAssetBlockFile(AssetInfo::ActualAssetInfo *assinfo, const char* const assfname) const; // @87E790
    void            FixDanglingAssets(AssetInfo::ActualAssetInfo* assinfo);  //  @87EB90

    static void     GetResourcePathRelative(String& outPath, String resourceName, Asset::BlockTypeNumber blockType, const char* languageCode); // @882DF0
public:
    Folder_();

    void            UnloadBlocks(); // @87EE80

    const int       GetBlockId() const; // @87E630
    void            SetBlockId(unsigned int blockid); // @87E6A0
    void            UnloadAssets(); //  @87F000
    void            LoadAssetBlock();   //  @87E8D0
    void            LoadFragment(); //  @87EDB0
    Node*           _87E640() const;    //  @87E640

    static void  Register(); // @87E810
    static Folder_* Create(); // @87E730

    static int      CurrentBlockId; // @A11A30
    static int      _A11B84[9]; //  @A11B84
};

extern EntityType* tFolder; // @A3D810

ASSERT_CLASS_SIZE(Folder_, 88);