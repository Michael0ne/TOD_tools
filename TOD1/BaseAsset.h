#pragma once
#include "StringsPool.h"
#include "Types.h"
#include <vector>

class Asset;

class AssetInstance
{
public:
    Asset*              (*m_Creator)(void);
    String              m_ResourceTypeName;
    void               *m_ResourceTypeMethods;
    int                 m_ResourceIndex;
    std::vector<String> m_FileExtensions;
    char                field_2C;
    char                m_VerifyChecksum;
    int                 m_Alignment[3];

public:
    AssetInstance(const char* const assetname, Asset* (*creatorptr)()); //  @852440

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    void                SetAlignment(unsigned int size, unsigned int slot); //  @852160

    static unsigned int AssetAlignment[3];  //  @A3BE1C
    static std::vector<AssetInstance*> Assets;  //  @A10F80

    static AssetInstance*   GetAssetInstanceByName(const char* const asspath);  //  @852220
};

//	NOTE: this class is actually inherited from another class, but parent doesn't seem to do anything important, so skipping it now.
#pragma pack(4)
class Asset
{
public:
    enum PlatformId
    {
        PC           = 0,
        PS2          = 1,
        XBOX         = 2
    };

    enum ResourceBlockTypeNumber
    {
        RES_NONE         = 0,
        RES_MAP          = 1,
        RES_SUBMAP       = 1,
        RES_MISSION      = 2,
        RES_CUTSCENE     = 3,
        RES_PLAYERDATA   = 4
    };

    enum BlockTypeNumber
    {
        UNKNOWN      = -1,
        NONE         = 0,
        MAP          = 1,
        SUBMAP       = 2,
        MISSION      = 3,
        CUTSCENE     = 4,
        PLAYERDATA   = 5,
        MAIN         = 6
    };

    typedef Asset* (*CREATOR)();

public:
    const char*         m_ResourcePath;
    int                 m_GlobalResourceId;	//	NOTE: this is an index for Blocks global 'ResourceTypeList'.
    int                 field_C;
    UINT64              m_ResourceTimestamp;
    union
    {
        unsigned short  m_ReferenceCount;
        struct
        {
            unsigned _0 : 8;

            unsigned _8 : 1;
            unsigned _9 : 1;
            unsigned _10 : 1;
            unsigned _11 : 1;
            unsigned _12 : 1;
            unsigned _13 : 1;
            unsigned _14 : 1;
            unsigned _15 : 1;

            unsigned NotUsed : 1;
            unsigned _17 : 1;
            unsigned _18 : 1;
            unsigned _19 : 1;
            unsigned AssetRegion : 4;

            unsigned _24 : 1;
            unsigned _25 : 1;
            unsigned _26 : 1;
            unsigned _27 : 1;
            unsigned _28 : 1;
            unsigned _29 : 1;
            unsigned _30 : 1;
            unsigned _31 : 1;
        }               m_FlagBits;
        unsigned int    m_Flags;
    }                   m_Flags;

public:
    virtual             ~Asset();	//	@8516C0
    virtual AssetInstance* GetInstancePtr() const = 0;
    virtual bool        stub3(unsigned char a1, int, int);    //  @851400
    virtual bool        stub4() const; //  @851E80
    virtual void        stub5(int);
    virtual void        GetResourcesDir(String& outDir, PlatformId platformId) const;    //  @851EC0
    virtual void        ApplyAssetData(int*);
    virtual char        SetResourcePlaceholder();  //  @42F4F0
    virtual int         stub9() const;   //  @851EA0
    virtual void        GetResourceName(String& outName, int a2);  //  @851DB0
    virtual void        LoadResource(const char* const resPath);
    virtual void        DestroyResource(); //  @851E90

    Asset(bool dontmakeglobal);	//	@851D00

    void* operator new(size_t size)
    {
        return AssetInstance::AssetAlignment[0]
            ? MemoryManager::AllocatorsList[TextureAssetAllocatorId]->AllocateAligned(size, AssetInstance::AssetAlignment[0], NULL, NULL)
            : MemoryManager::AllocatorsList[TextureAssetAllocatorId]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, AssetInstance::AssetAlignment[0] != false);
        ptr = nullptr;
    }

    const char*         AddResToOpenListAndReturnName() const;	//	@851720
    void                _851800(String& outstr, const char* inpath, bool a3, bool a4) const;	//	@851800
    void                SetReferenceCount(unsigned char count);	//	@8513E0
    void                EncodeCountryCode(const char* const countrycode);	//	@851480
    const char* const   GetResourceCountryCode() const;	//	@851CC0
    void                SetResourcePath(const char* const respath); //  @851DF0

    static AllocatorIndex AllocatorIndexByBlockType(unsigned int blocktype);   //  @851FE0
    static void         Destroy(Asset* res);	//	@851FC0
    static Asset*       CreateInstance(size_t classsize);  //  @852100
    static void         AllocateResourceForBlockLoad(const unsigned int size, int** bufaligned, int* buf, const unsigned int blockid);  //  @852070

    static const char* const BlockTypeExtension[];  //  @A11B64
    static std::vector<String> OpenResourcesList;	//	@A10F00
    static unsigned int TotalResourcesCreated;	//	@A3BE10
    static unsigned int	LastOpenResourceIndex;	//	@A3BE14
    static unsigned int TextureAssetAllocatorId;    //  @A3BE18
};


ASSERT_CLASS_SIZE(Asset, 28);