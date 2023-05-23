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

struct BaseAsset
{
    AssetType   Type = AssetType::Undefined;
    char*       Name = nullptr;
    uint32_t    GlobalId = 0;
    uint32_t    _fC = 0;
    UINT64      EngineTimestamp = 0;
    uint32_t    Flags = 0;
};

class MultiPartReader
{
private:
    uint8_t*        PosCurrent = nullptr;
    uint8_t*        PosStart = nullptr;
    uint8_t*        Buffer = nullptr;
    size_t          BufferSize = 0;

public:
    inline MultiPartReader(FILE* filePtr, const size_t bufferSize, const bool rememberStart = true)
    {
        if (!filePtr || !bufferSize)
            return;

        Buffer = new uint8_t[bufferSize];
        const auto fileOffsetCurrent = rememberStart ? ftell(filePtr) : -1;
        const size_t bytesRead = fread(Buffer, sizeof(uint8_t), bufferSize, filePtr);
        BufferSize = bufferSize;

        if (bytesRead != bufferSize)
            throw new std::exception("Bytes read from file buffer differs from the requested size!");

        if (fileOffsetCurrent > 0)
            fseek(filePtr, fileOffsetCurrent, SEEK_SET);

        PosCurrent = Buffer;
        PosStart = PosCurrent;
    }

    inline ~MultiPartReader()
    {
        delete[] Buffer;
    }

    template <typename T>
    inline void Read(T& dest)
    {
        dest = *((T*)PosCurrent);
        PosCurrent += sizeof(T);
    }

    template <typename T>
    inline void ReadNoAdvance(T& dest)
    {
        dest = *((T*)PosCurrent);
    }

    inline void Skip(int32_t size)
    {
        PosCurrent += size;
    }
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

    //  Assets.
    std::vector<BaseAsset*> Assets;
    std::vector<uint32_t> AssetsSizes;

public:
    MapBlockReader(LPCSTR filename, LPCSTR ext = ".map");
    ~MapBlockReader();

    virtual void    ReadInfo() override;
    virtual void    PrintInfo() const override;
    virtual void    DumpData() const override;
};