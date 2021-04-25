#pragma once
#include "StringsPool.h"
#include "Types.h"
#include <vector>

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
        NONE         = 0,
        MAP          = 1,
        SUBMAP       = 1,
        MISSION      = 2,
        CUTSCENE     = 3,
        PLAYERDATA   = 4
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

public:
    const char*      m_ResourcePath;
    int              m_GlobalResourceId;	//	NOTE: this is an index for Blocks global 'ResourceTypeList'.
    int              field_C;
    UINT64           m_ResourceTimestamp;
    union
    {
        struct
        {
            unsigned _0 : 1;
            unsigned _1 : 1;
            unsigned _2 : 1;
            unsigned _3 : 1;
            unsigned _4 : 1;
            unsigned _5 : 1;
            unsigned _6 : 1;
            unsigned _7 : 1;
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
        }            m_FlagBits;
        unsigned int m_Flags;
    }                m_Flags;

public:
    virtual          ~Asset();	//	@8516C0
    virtual Asset*   GetInstancePtr() const = 0;
    virtual bool     stub3(unsigned char a1, int, int);    //  @851400
    virtual bool     stub4() const; //  @851E80
    virtual void     stub5(int);
    virtual void     GetResourcesDir(String& outDir, PlatformId platformId);    //  @851EC0
    virtual void     ApplyAssetData(int*);
    virtual char     SetResourcePlaceholder();  //  @42F4F0
    virtual int      stub9() const;   //  @851EA0
    virtual void     GetResourceName(String& outName, int a2);  //  @851DB0
    virtual void     LoadResource(const char* const resPath);
    virtual void     DestroyResource(); //  @851E90

    Asset(bool dontmakeglobal);	//	@851D00

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

    const char*      AddResToOpenListAndReturnName() const;	//	@851720
    void             _851800(String& outstr, const char* inpath, bool a3, bool a4) const;	//	@851800
    void             _8513E0(unsigned char);	//	@8513E0
    void             EncodeCountryCode(const char* const countrycode);	//	@851480
    const char* const GetResourceCountryCode() const;	//	@851CC0

    static void      Destroy(Asset* res);	//	@851FC0

    static std::vector<String> OpenResourcesList;	//	@A10F00
    static unsigned int TotalResourcesCreated;	//	@A3BE10
    static unsigned int	LastOpenResourceIndex;	//	@A3BE14
};


ASSERT_CLASS_SIZE(Asset, 28);