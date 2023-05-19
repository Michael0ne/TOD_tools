#pragma once
#include "BaseAsset.h"
#include "TextureAsset.h"
#include "KapowEngineClass.h"
#include <vector>

class Node;
class Entity;

struct AssetHeaderStruct_t
{
    unsigned char               m_AssetId[36];
    int                         field_24;
    int                         field_28;
    int                         field_2C;
    int                         field_30;
    int                         field_34;

    struct Header_t
    {
        char                    m_OriginalKey[32];
        
        int                     field_20;
        int                     field_24;
        int                     field_28;

        int                     field_2C;
        int                     field_30;
        int                     field_34;
        int                     field_38;
        int                     field_3C;
        int                     field_40;
        int                     field_44;
        int                     field_48;
        int                     field_4C;

        Header_t(); // @401050

        void                    DecodeFingerprintKey(char* key, char* keydata); // @401450
        void                    _4010C0(const char* key); // @4010C0
        void                    _4011A0(char* key); // @4011A0
    } field_38;
};

//  NOTE: It looks like the purpose of this is just to fix raw compiled data that is inside an asset into game-ready data. Fix pointers, instantiate necessary classes etc. Could be templated.
//  TODO: might be not related to asset exactly, possible name 'MultiPartStream'. This supports multiple 'caret' positions.
struct CompiledAssetInfo
{
    struct ListAssetInfo
    {
        uint8_t**       m_AssetDataPtr;
        int32_t         m_Flags;
    };

    enum AssetType
    {
        ZERO = 0,
        ONE,
        TWO,
        THREE,
        FOUR
    }                   m_AssetType;    //  NOTE: more likely it's a 'block' number. Like 'header', 'data', ...
    int                 m_AssetSize;
    int                 field_8;
    int                 m_Alignment;
    std::vector<ListAssetInfo>    field_10;
    char               *field_20;
    char               *field_24;
    char               *field_28;
    char               *field_2C;
    int                 field_30;
    bool                field_34;
    bool                field_35;

    CompiledAssetInfo(const AssetType asstype, const char* assetinstanceinfo, const char* assetdata, const int alignment, const int a5, const int a6); //  @40CCD0
    inline ~CompiledAssetInfo() //  NOTE: always inlined.
    {
        MESSAGE_CLASS_DESTROYED(CompiledAssetInfo);
    }

    void                ParseInfo(const uint8_t** assetPtr, CompiledAssetInfo** assetBufferPtr, const size_t assetClassSize, const int32_t a4, const int32_t a5);    //  @85AE90
    void                ParseAssetData(const uint8_t** assetdataptr, int* dataptr, int flags, int a4); //  @40D0C0
    int                 GetAssetSize() const;   //  @40CB00
    void                AlignDataOrSize(unsigned int alignment, unsigned char flags, int a3);  //  @40CC10
    void                AddAssetToList(const uint8_t** dataptr, const int32_t flags);  //  @40CDA0
    char*               GetDataPtr(const int flags);    //  @4062E0
    void                _40CB90(const uint32_t dataptr, const int8_t a2, const int32_t a3);  //  @40CB90
    void                _40CBD0(const uint32_t dataptr, const int8_t a2, const int32_t a3);  //  @40CBD0
    void                OffsetToPtr(const uint8_t** dataptr, char flags) const;   //  @40CB20
    void                _85E160(uint8_t** dataptr, uint8_t** a2, uint32_t flags, uint32_t a4);    //  @85E160
};

ASSERT_CLASS_SIZE(CompiledAssetInfo, 56);

class AssetManager
{
public:
    enum PlatformId
    {
        PC = 0,
        PS2 = 1,
        XBOX = 2
    };

    enum RegionCode
    {
        REGION_NOT_SET = -1,
        REGION_EUROPE = 0,
        REGION_USA = 1,
        REGION_ASIA = 2
    };

    struct FastFindInfo
    {
        unsigned int            m_NodeNameCRC; // NOTE: CRC for Node name OR model name (if it's a model).
        unsigned int            m_Index; // NOTE: maybe?
        Node                   *m_Node;

        //  NOTE: this is necessary for 'std::find' to work.
        bool operator==(const FastFindInfo& rhs)
        {
            return m_NodeNameCRC == rhs.m_NodeNameCRC;
        }
    };

    struct AssetInfo
    {
        unsigned int            m_AssetNameCRC;
        Asset                  *m_Asset;

        //  NOTE: this is necessary for 'std::find' to work.
        bool operator==(const AssetInfo& rhs)
        {
            return m_AssetNameCRC == rhs.m_AssetNameCRC;
        }
    };

public:
    bool                        m_HasDanglingEntities;
    int                         m_ActiveBlockId;
    char                        m_FingerprintKey[256]; // NOTE: default value is 'THIS IS THE DEFAULT FINGERPRINT KEY, PLEASE CHANGE IT!". LOLZ.
    int                         field_108;

    DefragmentatorBase*         m_Defragmentator;
    std::vector<AssetInfo>      m_DefragmentatorList;

    std::vector<FastFindInfo>   m_FastFindNodeVector;
    std::vector<Entity*>        m_NodesList[6];
    std::vector<Asset*>         m_ResourcesInstancesList;
    std::vector<String>         m_LoadedAssetsNames;
    unsigned int                m_NodesInNodeList[6];
    int                         m_FragmentMainNodeId;
    bool                        m_BlocksUnloaded;
    std::map<int, int>         *m_EntityIdsMap; //  NOTE: not sure why it's here, but it's used in 
    int                         m_EngineVersionTimestamp;
    RegionCode                  m_RegionId;
    bool                        m_CheckTimestamp;
    std::vector<Asset*>         m_AssetsList;
    bool                        m_LoadBlocks;

private:
    void                        AddTypesListItemAtPos(Asset* element, unsigned int index); // @8760C0
    unsigned int                FindNodeById(uint32_t id); // @875570

public:
    AssetManager(bool loadBlocks); // @876E20
    ~AssetManager(); // @877250

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void* operator new[](size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }
    void operator delete[](void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    void                        AddLoadedAssetName(const char* assetName); // @877F40
    const char* const           GetLastLoadedAssetName() const; //  @8756B0
    unsigned int                FindFirstLoadedAsset(unsigned int startIndex) const; // @875540
    unsigned int                AddEntity(class Entity* ent); // @875FA0 // NOTE: returns index
    void                        SetRegion(RegionCode id); // @875434
    AllocatorIndex              GetAllocatorType() const; // @875360
    int                         AddAssetReference(Asset* a); // @877A90
#ifdef INCLUDE_FIXES
    void                        GetPlatformSpecificPath(char* outStr, const char* respath, const char* resext, PlatformId platform); // @8776B0
#else
    void                        GetPlatformSpecificPath(String& outStr, const char* respath, const char* resext, ResType::PlatformId platform); // @8776B0
#endif
    const char*                 GetResourcePathSceneRelative(const char* const path); // @8773A0
    void                        GetResourcePath(String& outStr, const char* path) const; // @875770
    void                        IncreaseResourceReferenceCount(Asset*); // @875320
    void                        DecreaseResourceReferenceCount(Asset*); // @875330
    const char*                 GetCurrentSceneName() const;
    void                        BuildFastFindNodeVector(); // @877DA0
    void                        DeleteFastFindNodeVector(); //  @876340
    void                        FillFastFindNodeVector(Node* baseNode, FastFindInfo* ffi); // @877B00 // NOTE: this goes through 'baseNode' children and fills vector.
    Asset*                      LoadResourceFile(const char* const respath); // @878AB0
    void*                       LoadResourceBlock(class FileBuffer*, int* resbufferptr, unsigned int* resdatasize, unsigned int resblockid); // @8759E0
    Entity*                     FindFirstEntity(); // @8755E0
    Entity*                     FindNextEntity(Entity*); // @875610
    void                        DestroySceneNodesFrameBuffers(const int);  //  @875390
    RegionCode                  GetRegion() const; // @875440
    String&                     GetDataPath(String& outstr) const; // @8764E0
    Asset*                      FindFirstLoadedAsset() const; // @879E00
    void                        DestroyTextureAsset(TextureAsset& ass); //  @875340
    Asset*                      GetNextLoadedAsset(const Asset* a) const; //  @875720
    void                        _878030();  //  @878030
    void                        MakeSpaceForAssetsList();  //  @877AE0
    bool                        LoadOriginalVersion(Asset * asset);  //  @878220
    Asset*                      FindLoadedAsset(const char* const assetname);   //  @876140
    void                        InstantiateAssetsAndClearAssetsList();  //  @875EB0
    Node*                       FindEntityById(const int id);   //  @879740
    void                        _8794B0(const char* const respath);  //  @8794B0    //  NOTE: only two references are present.
    void                        DestroyDanglingAssets(const char* const resourceBufferDataBegin, const char* const resourceBufferDataEnd);    //  @875910

    static void                 CorrectTextureResourcePath(String& outPath, const char* respath, RegionCode region, PlatformId platform); // @876500
    static RegionCode           RegionIdByName(const String& region); // @875450

    static bool                 ChecksumChecked; // @A3D7C9
    static bool                 _A3D7C0;    //  @A3D7C0
};

extern AssetManager            *g_AssetManager;

ASSERT_CLASS_SIZE(AssetManager, 500);
ASSERT_CLASS_SIZE(AssetHeaderStruct_t, 136);