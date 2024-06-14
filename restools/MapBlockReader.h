#pragma once
#include "GenericResourceReader.h"

class FingerprintDecoder
{
    char        OriginalKey[32] = {};
    uint32_t    _f20 = 0x13579BDF;
    uint32_t    _f24 = 0x2468ACE0;
    uint32_t    _f28 = 0xFDB97531;
    uint32_t    _f2C = 0x80000062;
    uint32_t    _f30 = 0x40000020;
    uint32_t    _f34 = 0x10000002;
    uint32_t    _f38 = 0x7FFFFFFF;
    uint32_t    _f3C = 0x3FFFFFFF;
    uint32_t    _f40 = 0xFFFFFFF;
    uint32_t    _f44 = 0x80000000;
    uint32_t    _f48 = 0xC0000000;
    uint32_t    _f4C = 0xF0000000;

    void        PrepareKey(const char* const publicKey);
    void        DecodeInternal(char* keyMutable);

public:
    void        Decode(const char* const privateKey, char* text);
};

//  Assets.
class AssetResource;
class AssetBuilder;

enum class AssetType
{
    Texture = 0,
    Font,
    Text,
    Model,
    Fragment,
    Movie,
    Cutscene,
    Sound,
    StreamedSoundInfo,
    Animation,
    MeshColor,
    Undefined
};

class Asset
{
protected:
    AssetType   Type = AssetType::Undefined;
    char*       Name = nullptr;
    uint32_t    GlobalId = 0;
    uint32_t    _fC = 0;
    UINT64      EngineTimestamp = 0;
    uint16_t    Flags = 0;
    union
    {
        struct
        {
            unsigned    ReferenceCount : 4;
        };
    };

    static uint32_t Total;

public:
    void operator delete(void* ptr)
    {
        ((Asset*)ptr)->~Asset();

        if (!((Asset*)ptr)->ReferenceCount)
            delete ptr;
    }

    Asset();

    virtual ~Asset() { delete[] Name; };
    virtual AssetResource*  GetInstance() = 0;
    virtual uint8_t         stub3(bool, uint32_t, uint32_t) { return 0; };
    virtual uint32_t        stub4() { return -1; };
    virtual void            stub5(uint32_t) {};
    virtual void            GetResourceDirectory(std::string& dir, uint32_t platform) const = 0;
    virtual void            Apply(AssetBuilder& from) = 0;
    virtual bool            SetPlaceholder() = 0;
    virtual uint32_t        stub9() { return 0; };
    virtual void            GetName(std::string& name, uint32_t) const = 0;
    virtual uint32_t        Load(const char* const) = 0;
    virtual void            Delete() = 0;

    inline void             SetReferencedStatus(const bool referenced)
    {
        ReferenceCount = referenced;
    }

    void                    ParseAdditional(AssetBuilder* from, AssetBuilder** to);

    static void             Destroy(Asset* asset);
};

class AssetResource
{
public:
    Asset* (*Create)(void) = nullptr;
    std::string TypeName = {};
    void* AssetVMT = nullptr;
    uint32_t    ResourceIndex = 0;
    std::vector<std::string>    Extensions = {};
    uint8_t     _f2C = 0;
    bool        VerifyChecksum = false;
    uint32_t    Alignment[3] = {};

public:
    AssetResource(const char* const type, Asset* (*creator)(void));
};

static std::vector<AssetResource*> AssetTypes;
static void CreateAssetInstances();

//  Asset types.
class TextureAsset : public Asset
{
    struct Texture
    {

    };

    struct Texture_1
    {

    };

protected:
    uint32_t    _f1C = 0;
    Texture*    TextureData = nullptr;
    uint8_t     _f24[4] = {};
    Texture_1*  TextureData_1 = nullptr;
    uint32_t    _f2C = 0;

public:
    TextureAsset();
    virtual ~TextureAsset()
    {
        if (TextureData_1 && ((uint32_t)TextureData_1 & 1) == 0)
            delete TextureData_1;
        if (TextureData && ((uint32_t)TextureData & 1) == 0)
            delete TextureData;
    }

    virtual AssetResource*  GetInstance() override
    {
        return ResourceInstance;
    }
    virtual void            GetResourceDirectory(std::string& dir, uint32_t platform) const override
    {
        switch (platform)
        {
        case 1:
            dir = "texture_ps2";
            break;
        case 2:
            dir = "texture_x";
            break;
        case 0:
            dir = "texture_pc";
            break;
        default:
            break;
        }
    }
    virtual void            Apply(AssetBuilder& from) override;
    virtual bool            SetPlaceholder() override
    {
        const auto isPlaceholderTexture = strstr(Name, "pinkyellowcheckers.bmp") == 0;
        //  TODO: then, get this texture pointer from GfxInternal 'CheckerboardTextures' list and set it to 'Texture_1' member.
        return true;
    }
    virtual void            GetName(std::string& name, uint32_t) const override
    {
        //  TODO: lots of stuff here. Don't care right now.
    }
    virtual uint32_t        Load(const char* const) override { return 0; };
    virtual void            Delete() override
    {
        if (TextureData_1 && ((uint32_t)TextureData_1 & 1) == 0)
            delete TextureData_1;
        EngineTimestamp = 0;
    }

    static inline Asset*    Create()
    {
        return (Asset*)(new TextureAsset);
    }
    static AssetResource* ResourceInstance;
};

class FontAsset : public Asset
{
    struct Font
    {

    };

private:
    uint32_t    _f1C = 0;
    uint32_t*   _f20;
    Font*       FontData;

public:
    FontAsset();
    virtual ~FontAsset()
    {
        if (FontData && ((uint32_t)FontData & 1) == 0)
            delete FontData;
    }

    virtual AssetResource*  GetInstance() override
    {
        return ResourceInstance;
    }
    virtual void            GetResourceDirectory(std::string& dir, uint32_t platform) const override
    {
        switch (platform)
        {
        case 1:
            dir = "font_ps2";
            break;
        case 2:
            dir = "font_xb";
            break;
        case 0:
            dir = "font_pc";
            break;
        default:
            break;
        }
    }
    virtual void            Apply(AssetBuilder& from) override;
    virtual bool            SetPlaceholder() override
    {
        return true;
    }
    virtual void            GetName(std::string& name, uint32_t) const override
    {
        //  TODO: lots of stuff here. Don't care right now.
    }
    virtual uint32_t        Load(const char* const) override { return 0; };
    virtual void            Delete() override
    {
        if (FontData && ((uint32_t)FontData & 1) == 0)
            delete FontData;
        EngineTimestamp = 0;
    }

    static inline Asset* Create()
    {
        return (Asset*)(new FontAsset);
    }
    static AssetResource* ResourceInstance;
};

class TextAsset : public Asset
{
private:
    uint32_t    _f1C = 0;
    std::vector<uint32_t>   _f20 = {};
    std::vector<uint32_t>   TextIndicies = {};
    std::vector<uint32_t>   _f40 = {};
    std::map<uint16_t, uint16_t>*   Characters = nullptr;
    uint32_t    _54;

public:
    TextAsset();
    virtual ~TextAsset()
    {
        if (Characters && ((uint32_t)Characters & 1) == 0)
            delete Characters;
    }

    virtual AssetResource* GetInstance() override
    {
        return ResourceInstance;
    }
    virtual void            GetResourceDirectory(std::string& dir, uint32_t platform) const override
    {
        switch (platform)
        {
        case 1:
            dir = "textres_ps2";
            break;
        case 2:
            dir = "textres_xb";
            break;
        case 0:
            dir = "textres_pc";
            break;
        default:
            break;
        }
    }
    virtual void            Apply(AssetBuilder& from) override;
    virtual bool            SetPlaceholder() override
    {
        return true;
    }
    virtual void            GetName(std::string& name, uint32_t) const override
    {
        //  TODO: lots of stuff here. Don't care right now.
    }
    virtual uint32_t        Load(const char* const) override { return 0; };
    virtual void            Delete() override
    {
        if (Characters && ((uint32_t)Characters & 1) == 0)
            delete Characters;
        EngineTimestamp = 0;
    }

    static inline Asset* Create()
    {
        return (Asset*)(new TextAsset);
    }
    static AssetResource* ResourceInstance;
};

class ModelAsset : public Asset
{
    struct TextureSet
    {
    };

    struct Pivot
    {
    };

    struct PhysAttachment
    {
    };
private:
    uint32_t    _f1C = 0;
    std::vector<TextureSet> TextureResources = {};
    std::vector<Pivot>      PivotsList = {};
    std::vector<PhysAttachment>*    PhysAttachmentsList = nullptr;
    float_t     BoundingRadius[4] = {};
    uint32_t    _f54 = 0;
    uint32_t*   _f58 = (uint32_t*)1;
    uint8_t     _f5C = 0x20;

public:
    ModelAsset();
    virtual ~ModelAsset()
    {
        if (PhysAttachmentsList && ((uint32_t)PhysAttachmentsList & 1) == 0)
            delete PhysAttachmentsList;

        if (_f58 && ((uint32_t)_f58 & 1) == 0)
            delete _f58;
    }

    virtual AssetResource* GetInstance() override
    {
        return ResourceInstance;
    }
    virtual void            GetResourceDirectory(std::string& dir, uint32_t platform) const override
    {
        switch (platform)
        {
        case 1:
            dir = "modelres_ps2";
            break;
        case 2:
            dir = "modelres_x";
            break;
        case 0:
            dir = "modelres_pc";
            break;
        default:
            break;
        }
    }
    virtual void            Apply(AssetBuilder& from) override;
    virtual bool            SetPlaceholder() override
    {
        return true;
    }
    virtual void            GetName(std::string& name, uint32_t) const override
    {
        //  TODO: lots of stuff here. Don't care right now.
    }
    virtual uint32_t        Load(const char* const) override { return 0; };
    virtual void            Delete() override
    {
        if (PhysAttachmentsList && ((uint32_t)PhysAttachmentsList & 1) == 0)
            delete PhysAttachmentsList;

        if (_f58 && ((uint32_t)_f58 & 1) == 0)
            delete _f58;
        EngineTimestamp = 0;
    }

    static inline Asset* Create()
    {
        return (Asset*)(new ModelAsset);
    }
    static AssetResource* ResourceInstance;
};

class FragmentAsset : public Asset
{
    struct FragmentData
    {
    };
private:
    uint32_t        _f1C = 0;
    FragmentData*   Fragment;
    uint32_t        _f24;

public:
    FragmentAsset();
    virtual ~FragmentAsset()
    {
        if (Fragment && ((uint32_t)Fragment & 1) == 0)
            delete Fragment;
    }

    virtual AssetResource* GetInstance() override
    {
        return ResourceInstance;
    }
    virtual void            GetResourceDirectory(std::string& dir, uint32_t platform) const override
    {
        switch (platform)
        {
        case 1:
            dir = "fragment_ps2";
            break;
        case 2:
            dir = "fragment_x";
            break;
        case 0:
            dir = "fragment_pc";
            break;
        default:
            break;
        }
    }
    virtual void            Apply(AssetBuilder& from) override;
    virtual bool            SetPlaceholder() override
    {
        return true;
    }
    virtual void            GetName(std::string& name, uint32_t) const override
    {
        //  TODO: lots of stuff here. Don't care right now.
    }
    virtual uint32_t        Load(const char* const) override { return 0; };
    virtual void            Delete() override
    {
        if (Fragment && ((uint32_t)Fragment & 1) == 0)
            delete Fragment;
        EngineTimestamp = 0;
    }

    static inline Asset* Create()
    {
        return (Asset*)(new FragmentAsset);
    }
    static AssetResource* ResourceInstance;
};

class AssetBuilder
{
    friend class Asset;
    friend class TextureAsset;

public:
    enum class PartType
    {
        ZERO = 0,
        ONE,
        TWO,
        THREE,
        FOUR
    };

public:
#ifdef USING_READER
    typedef MultiPartReader BufferType;
    typedef BufferType* BufferTypePtr;
#else
    typedef uint8_t     BufferType;
    typedef BufferType* BufferTypePtr;
#endif

    struct BufferPositionData
    {
        BufferTypePtr*  DataPtr;
        uint32_t        Flags;

        BufferPositionData(BufferTypePtr* dataPtr, const uint32_t flags)
            :DataPtr(dataPtr), Flags(flags)
        {}
    };

    PartType            Type;
    uint32_t            HeaderDataSize;
    uint32_t            DataSize;
    uint32_t            AlignmentIndex;
    std::vector<BufferPositionData>    _f10;
    BufferTypePtr       HeaderDataPtr;  //  NOTE: header part.
    BufferTypePtr       DataPtr;        //  NOTE: actual data part.
    BufferTypePtr       HeaderDataStartPtr;
    BufferTypePtr       DataStartPtr;
    uint32_t            _f30;
    bool                _f34;
    bool                _f35;

public:
    AssetBuilder(const PartType type, BufferTypePtr headerDataReader, BufferTypePtr dataReader, const uint32_t alignmentIndex, const uint32_t a5, const uint32_t a6);
    void                ParseData(BufferTypePtr* dataPtr, BufferTypePtr bufferPtr, const uint32_t flags, const int32_t a4);
    void                ParseInfo(BufferTypePtr* assetInstancePtr, BufferTypePtr* bufferPtr, const size_t instanceSize, const uint32_t flags, const uint32_t a5);
    void                OffsetToPointer(BufferTypePtr* bufferPtr, const uint32_t flags);

    inline void         IncreaseBufferPtr(const int32_t amount, const char what, const uint32_t flags)
    {
        if (_f30 == -1 || flags != -1 && _f30 == flags)
        {
            if ((what & 2) != 0)
                DataPtr += ((amount + 3) & 0xFFFFFFFC);
            else
                HeaderDataPtr += ((amount + 3) & 0xFFFFFFFC);
        }
    }
    inline void         IncreaseSize(const int32_t amount, const char what, const uint32_t flags)
    {
        if (_f30 == -1 || flags != -1 && _f30 == flags)
        {
            if ((what & 2) != 0)
                DataSize += ((amount + 3) & 0xFFFFFFFC);
            else
                HeaderDataSize += ((amount + 3) & 0xFFFFFFFC);
        }
    }
    inline void         RememberBufferPosition(BufferTypePtr* bufferPtr, const uint32_t flags)
    {
        if ((flags & 1) != 0)
            return;

        _f10.push_back({ bufferPtr, flags });
    }
    inline BufferTypePtr    GetBufferPtr(const char what) const
    {
        return what & 2 ? DataPtr : HeaderDataPtr;
    }

    static void         Instantiate(AssetBuilder& builder, BufferTypePtr buffer);
};

class MapBlockReader : public GenericResourceReader
{
private:
    #pragma pack(1)
    struct MapAssetSharedHeader
    {
        char        Identifier[36];
        char        Fingerprint[255];
        uint32_t    _f124;
        uint32_t    _f128;
    };

    struct AssetSharedHeader
    {
        uint32_t    Timestamp;
        uint32_t    PropertyChecksum;
        uint32_t    CommandChecksum;
        uint32_t    TotalResources;
        uint32_t    ResourcesInfoSize;
        uint32_t    DataSize;
    };

    //  Headers.
    MapAssetSharedHeader    SpecificHeader;
    AssetSharedHeader       SharedHeader;
    char                    FingerprintKey[256] = {};
    char                    Identifier[37] = {};
    uint8_t*                HeadersBuffer = nullptr;
    uint8_t*                DataBuffer = nullptr;
    uint8_t*                SizesBuffer = nullptr;

    //  Assets.
    std::vector<Asset*> Assets;
    std::vector<uint32_t> AssetsSizes;

public:
    MapBlockReader(LPCSTR filename, LPCSTR ext = ".map");
    ~MapBlockReader();

    virtual void    ReadInfo() override;
    virtual void    PrintInfo() const override;
    virtual void    DumpData() const override;
};