#include "MapBlockReader.h"

uint32_t Asset::Total = 0;
AssetResource* TextureAsset::ResourceInstance;
AssetResource* FontAsset::ResourceInstance;
AssetResource* TextAsset::ResourceInstance;
AssetResource* ModelAsset::ResourceInstance;
AssetResource* FragmentAsset::ResourceInstance;

void CreateAssetInstances()
{
    //  Texture
    {
        TextureAsset::ResourceInstance = new AssetResource("texture", TextureAsset::Create);
        TextureAsset::ResourceInstance->Extensions.push_back("bmp");
        TextureAsset::ResourceInstance->Alignment[1] = 16;
        TextureAsset::ResourceInstance->Alignment[2] = 128;
        TextureAsset::ResourceInstance->Alignment[0] = 0;
    }

    //  Font
    {
        FontAsset::ResourceInstance = new AssetResource("font", FontAsset::Create);
        FontAsset::ResourceInstance->Extensions.push_back("ttf");
        FontAsset::ResourceInstance->Extensions.push_back("font");
        FontAsset::ResourceInstance->Alignment[1] = 16;
        FontAsset::ResourceInstance->Alignment[2] = 128;
        FontAsset::ResourceInstance->Alignment[0] = 0;
        FontAsset::ResourceInstance->_f2C = 1;
    }

    //  Text
    {
        TextAsset::ResourceInstance = new AssetResource("text", TextAsset::Create);
        TextAsset::ResourceInstance->Extensions.push_back("txt");
        TextAsset::ResourceInstance->Alignment[1] = 16;
        TextAsset::ResourceInstance->Alignment[2] = 16;
        TextAsset::ResourceInstance->Alignment[0] = 16;
        TextAsset::ResourceInstance->_f2C = 1;
    }

    //  Model
    {
        ModelAsset::ResourceInstance = new AssetResource("modelres", ModelAsset::Create);
        ModelAsset::ResourceInstance->Extensions.push_back("model");
        ModelAsset::ResourceInstance->Alignment[1] = 16;
        ModelAsset::ResourceInstance->Alignment[2] = 16;
        ModelAsset::ResourceInstance->Alignment[0] = 16;
    }

    //  Fragment
    {
        FragmentAsset::ResourceInstance = new AssetResource("fragment", FragmentAsset::Create);
        FragmentAsset::ResourceInstance->Extensions.push_back("fragment");
        FragmentAsset::ResourceInstance->Extensions.push_back("scene");
        FragmentAsset::ResourceInstance->Alignment[1] = 16;
        FragmentAsset::ResourceInstance->Alignment[2] = 16;
        FragmentAsset::ResourceInstance->Alignment[0] = 16;
        FragmentAsset::ResourceInstance->VerifyChecksum = true;
    }

    //  Movie
    {
        /*MovieAsset::ResourceInstance = new AssetResource("movie", MovieAsset::Create);
        MovieAsset::ResourceInstance->Extensions.push_back("wmv");
        MovieAsset::ResourceInstance->Extensions.push_back("bik");
        MovieAsset::ResourceInstance->Alignment[1] = 16;
        MovieAsset::ResourceInstance->Alignment[2] = 16;
        MovieAsset::ResourceInstance->Alignment[0] = 16;*/
    }

    //  Cutscene
    {
        /*CutsceneAsset::ResourceInstance = new AssetResource("cutscene", CutsceneAsset::Create);
        CutsceneAsset::ResourceInstance->Extensions.push_back("cutscene");
        CutsceneAsset::ResourceInstance->Alignment[1] = 16;
        CutsceneAsset::ResourceInstance->Alignment[2] = 16;
        CutsceneAsset::ResourceInstance->Alignment[0] = 16;*/
    }

    //  Sound
    {
        /*SoundAsset::ResourceInstance = new AssetResource("sound", SoundAsset::Create);
        SoundAsset::ResourceInstance->Extensions.push_back("wav");
        SoundAsset::ResourceInstance->Alignment[1] = 16;
        SoundAsset::ResourceInstance->Alignment[2] = 16;
        SoundAsset::ResourceInstance->Alignment[0] = 16;
        SoundAsset::ResourceInstance->_f2C = 1;*/
    }

    //  StreamedSound
    {
        /*StreamedSoundAsset::ResourceInstance = new AssetResource("streamedsoundinfo", StreamedSoundAsset::Create);
        StreamedSoundAsset::ResourceInstance->Extensions.push_back("stream");
        StreamedSoundAsset::ResourceInstance->Extensions.push_back("ogg");
        StreamedSoundAsset::ResourceInstance->Alignment[1] = 16;
        StreamedSoundAsset::ResourceInstance->Alignment[2] = 16;
        StreamedSoundAsset::ResourceInstance->Alignment[0] = 16;
        StreamedSoundAsset::ResourceInstance->_f2C = 1;*/
    }

    //  Animation
    {
        /*AnimationAsset::ResourceInstance = new AssetResource("animation", AnimationAsset::Create);
        AnimationAsset::ResourceInstance->Extensions.push_back("animation");
        AnimationAsset::ResourceInstance->Alignment[1] = 16;
        AnimationAsset::ResourceInstance->Alignment[2] = 16;
        AnimationAsset::ResourceInstance->Alignment[0] = 16;*/
    }

    //  MeshColor
    {
        /*MeshColorAsset::ResourceInstance = new AssetResource("meshcolor", MeshColorAsset::Create);
        MeshColorAsset::ResourceInstance->Extensions.push_back("meshcolor");
        MeshColorAsset::ResourceInstance->Extensions.push_back("lighting");*/
    }
}

AssetResource::AssetResource(const char* const type, Asset* (*creator)(void))
{
    TypeName = type;
    ResourceIndex = AssetTypes.size();
    AssetTypes.emplace_back(this);

    Create = creator;
    _f2C = 0;
    VerifyChecksum = false;

    Alignment[1] = 16;
    Alignment[2] = 16;
    Alignment[0] = 16;

    Asset* assetInstance = Create();
    AssetVMT = (void*)(*(uint32_t*)assetInstance);
    Asset::Destroy(assetInstance);
}

Asset::Asset()
{
    Total++;

    ReferenceCount = 1;
}

void Asset::ParseAdditional(AssetBuilder* from, AssetBuilder** to)
{
    from->ParseData((AssetBuilder::BufferTypePtr*)&Name, (AssetBuilder::BufferTypePtr) & ((*to)->HeaderDataSize), 0, -1);

    if (from->Type == AssetBuilder::PartType::TWO)
    {
        Flags = 0;
        GlobalId = 0;
        EngineTimestamp = 0;
        *((uint32_t*)this) = GetInstance()->ResourceIndex;
    }
}

void Asset::Destroy(Asset* asset)
{
    delete asset;
}

TextureAsset::TextureAsset()
{
    TextureData = (Texture*)1;
    TextureData_1 = nullptr;
    _f24[0] = 0;
    _f24[1] = 0;
    _f24[2] = 0;
    _f24[3] = 1;

    SetReferencedStatus(true);
}

void TextureAsset::Apply(AssetBuilder& from)
{
    const auto alignment = GetInstance()->Alignment[from.AlignmentIndex];
    //  nullsub()

    uint8_t* buffer = nullptr;
    uint8_t* instancePtr = (uint8_t*)this;
    from.ParseInfo(&instancePtr, &buffer, 48, 1, -1);
}

FontAsset::FontAsset()
{
    _f20 = (uint32_t*)1;
    FontData = nullptr;

    SetReferencedStatus(true);
}

void FontAsset::Apply(AssetBuilder& from)
{

}

TextAsset::TextAsset()
{
}

void TextAsset::Apply(AssetBuilder& from)
{
}

ModelAsset::ModelAsset()
{
    SetReferencedStatus(true);
}

void ModelAsset::Apply(AssetBuilder& from)
{
}

FragmentAsset::FragmentAsset()
{
    Fragment = new FragmentData;
    *(uint32_t*)&Fragment &= 1;

    _f24 = 0;
    _f24 &= ~2;
}

void FragmentAsset::Apply(AssetBuilder& from)
{
    uint8_t* buffer = (uint8_t*)&from;
    uint8_t* instancePtr = (uint8_t*)this;
    from.ParseInfo(&instancePtr, &buffer, 40, 1, -1);

    AssetBuilder* builder = &from;
    if (from.Type == AssetBuilder::PartType::ZERO || from.Type == AssetBuilder::PartType::ONE || from.Type == AssetBuilder::PartType::THREE)
        ParseAdditional(builder, &builder);
}

//  MAP BLOCK READER

MapBlockReader::MapBlockReader(LPCSTR filename, LPCSTR ext)
{
    m_ResourceExtension = ext;
    m_ResourceName = filename;
    m_ResourceName += m_ResourceExtension;

    CreateAssetInstances();
}

MapBlockReader::~MapBlockReader()
{
}

void MapBlockReader::ReadInfo()
{
    if (!m_FilePtr ||
        m_LastErrorCode)
        return;

    fread(&SpecificHeader, sizeof(SpecificHeader), 1, m_FilePtr);
    fread(&SharedHeader, sizeof(SharedHeader), 1, m_FilePtr);

    //  Make a copy of a fingerprint and identifier before decode.
    constexpr size_t fingerprintSize = sizeof(FingerprintKey) - 1;
    memcpy(FingerprintKey, SpecificHeader.Fingerprint, fingerprintSize);
    FingerprintKey[fingerprintSize] = '\0';

    constexpr size_t identifierSize = sizeof(Identifier) - 1;
    memcpy(Identifier, SpecificHeader.Identifier, identifierSize - 1);
    Identifier[identifierSize] = '\0';

    FingerprintDecoder decoder;
    decoder.Decode("1E564E3B-D243-4ec5-AFB7", FingerprintKey);

    //  Next comes the assets headers.
    HeadersBuffer = new uint8_t[SharedHeader.ResourcesInfoSize];
    fread_s(HeadersBuffer, SharedHeader.ResourcesInfoSize, sizeof(uint8_t), SharedHeader.ResourcesInfoSize, m_FilePtr);

    //  Then the sizes table.
    SizesBuffer = new uint8_t[SharedHeader.TotalResources * 4];
    fread_s(SizesBuffer, SharedHeader.TotalResources * 4, sizeof(uint8_t), SharedHeader.TotalResources * 4, m_FilePtr);

    for (size_t i = 0; i < SharedHeader.TotalResources; ++i)
    {
        const uint32_t assetSize = *(uint32_t*)SizesBuffer;
        SizesBuffer += sizeof(assetSize);

        AssetsSizes.emplace_back(assetSize);
    }

    //  Actual assets data comes last.
    DataBuffer = new uint8_t[SharedHeader.DataSize];
    fread_s(DataBuffer, SharedHeader.DataSize, sizeof(uint8_t), SharedHeader.DataSize, m_FilePtr);

    for (size_t i = 0; i < SharedHeader.TotalResources; ++i)
    {
        if (!AssetsSizes[i])
            continue;

        if ((uint32_t)HeadersBuffer[0] >= AssetTypes.size())
            throw new std::exception("Unknown asset type!");

        AssetBuilder asset(AssetBuilder::PartType::THREE, HeadersBuffer, DataBuffer, 0, 2, -1);
        AssetBuilder::Instantiate(asset, HeadersBuffer);
    }
}

void MapBlockReader::PrintInfo() const
{
    printf("\tIdentity:\t%s\n", Identifier);
    printf("\tFingerprint:\t%s\n", SpecificHeader.Fingerprint);
    printf("\tFingerprint decoded:\t%s\n", FingerprintKey);
    printf("\tfield_124:\t%x\n", SpecificHeader._f124);
    printf("\tfield_128:\t%x\n", SpecificHeader._f128);

    printf("\tTimestamp:\t%x\n", SharedHeader.Timestamp);
    printf("\tProperty checksum:\t%x\n", SharedHeader.PropertyChecksum);
    printf("\tCommand checksum:\t%x\n", SharedHeader.CommandChecksum);
    printf("\tTotal resources:\t%d\n", SharedHeader.TotalResources);
    printf("\tResources info size:\t%d\n", SharedHeader.ResourcesInfoSize);
    printf("\tData size:\t%d\n", SharedHeader.DataSize);

    for (size_t i = 0; i < SharedHeader.TotalResources; ++i)
        printf("\tAsset %d size:\t%d bytes\n", i, AssetsSizes[i]);
}

void MapBlockReader::DumpData() const
{
}

void FingerprintDecoder::PrepareKey(const char* const publicKey)
{
    strcpy(OriginalKey, publicKey);

    _f20 = (uint8_t)publicKey[3] | (((uint8_t)publicKey[2] | (((uint8_t)publicKey[1] | (0x00 | (publicKey[0] << 8)) ) << 8)) << 8);
    if (!_f20)
        _f20 = 0x13579BDF;

    _f24 = (uint8_t)publicKey[7] | (((uint8_t)publicKey[6] | (((uint8_t)publicKey[5] | ((uint8_t)publicKey[4] << 8)) << 8)) << 8);
    if (!_f24)
        _f24 = 0x2468ACE0;

    _f28 = (uint8_t)publicKey[11] | (((uint8_t)publicKey[10] | (((uint8_t)publicKey[9] | ((uint8_t)publicKey[8] << 8)) << 8)) << 8);
    if (!_f28)
        _f28 = 0xFDB97531;
}

void FingerprintDecoder::DecodeInternal(char* key)
{
    //  NOTE: thank you, Ghidra (NSA)!
    uint32_t uVar1;
    byte bVar2;
    uint32_t uVar3;
    byte bVar4;
    byte bVar5;
    uint32_t uVar6;
    size_t round;
    byte local_9;
    uint32_t local_8;
    uint32_t local_4;

    uVar6 = _f20;
    local_8 = _f24 & 1;
    local_4 = _f28 & 1;
    local_9 = 0;
    round = 2;
    do
    {
        bVar4 = (byte)local_8;
        if ((uVar6 & 1) == 0)
        {
            uVar3 = uVar6 >> 1 & _f38;
            uVar1 = _f28;
            if ((uVar1 & 1) == 0)
            {
                _f28 = uVar1 >> 1 & _f40;
                local_4 = 0;
            }
            else
            {
                _f28 = (_f34 ^ uVar1) >> 1 | _f4C;
                local_4 = 1;
            }
        }
        else
        {
            uVar1 = _f24;
            uVar3 = (_f2C ^ uVar6) >> 1 | _f44;
            if ((uVar1 & 1) == 0) {
                bVar4 = 0;
                _f24 = uVar1 >> 1 & _f3C;
                local_8 = 0;
            }
            else
            {
                bVar4 = 1;
                _f24 = (_f30 ^ uVar1) >> 1 | _f48;
                local_8 = 1;
            }
        }
        bVar4 = bVar4 ^ (byte)local_4;
        if ((uVar3 & 1) == 0)
        {
            uVar3 = uVar3 >> 1 & _f38;
            uVar1 = _f28;
            if ((uVar1 & 1) == 0) {
                _f28 = uVar1 >> 1 & _f40;
                local_4 = 0;
            }
            else
            {
                _f28 = (_f34 ^ uVar1) >> 1 | _f4C;
                local_4 = 1;
            }
        }
        else
        {
            uVar3 = (_f2C ^ uVar3) >> 1 | _f44;
            uVar1 = _f24;
            if ((uVar1 & 1) == 0) {
                _f24 = uVar1 >> 1 & _f3C;
                local_8 = 0;
            }
            else
            {
                _f24 = (_f30 ^ uVar1) >> 1 | _f48;
                local_8 = 1;
            }
        }
        bVar2 = (byte)local_8 ^ (byte)local_4;
        if ((uVar3 & 1) == 0)
        {
            uVar3 = uVar3 >> 1 & _f38;
            uVar1 = _f28;
            if ((uVar1 & 1) == 0)
            {
                _f28 = uVar1 >> 1 & _f40;
                local_4 = 0;
            }
            else
            {
                _f28 = (_f34 ^ uVar1) >> 1 | _f4C;
                local_4 = 1;
            }
        }
        else
        {
            uVar3 = (_f2C ^ uVar3) >> 1 | _f44;
            uVar1 = _f24;
            if ((uVar1 & 1) == 0)
            {
                _f24 = uVar1 >> 1 & _f3C;
                local_8 = 0;
            }
            else
            {
                _f24 = (_f30 ^ uVar1) >> 1 | _f48;
                local_8 = 1;
            }
        }
        bVar5 = (byte)local_8 ^ (byte)local_4;
        if ((uVar3 & 1) == 0)
        {
            uVar6 = uVar3 >> 1 & _f38;
            uVar1 = _f28;
            if ((uVar1 & 1) == 0)
            {
                _f28 = uVar1 >> 1 & _f40;
                local_4 = 0;
            }
            else
            {
                _f28 = (_f34 ^ uVar1) >> 1 | _f4C;
                local_4 = 1;
            }
        }
        else
        {
            uVar6 = (_f2C ^ uVar3) >> 1 | _f44;
            uVar1 = _f24;
            if ((uVar1 & 1) == 0)
            {
                _f24 = uVar1 >> 1 & _f3C;
                local_8 = 0;
            }
            else
            {
                _f24 = (_f30 ^ uVar1) >> 1 | _f48;
                local_8 = 1;
            }
        }

        local_9 = (byte)local_8 ^ (byte)local_4 | (bVar5 | (bVar2 | (bVar4 | local_9 << 1) << 1) << 1) << 1;
    } while (--round > 0);

    _f20 = uVar6;

    *key ^= local_9;
    if (!*key)
        *key = local_9;
}

void FingerprintDecoder::Decode(const char* const privateKey, char* text)
{
    PrepareKey(privateKey);

    const size_t textSize = strlen(text);
    for (size_t i = 0; i < textSize; ++i)
    {
        char ch = text[i];
        DecodeInternal(&ch);
        text[i] = ch;
    }
}

AssetBuilder::AssetBuilder(const AssetBuilder::PartType type, BufferTypePtr headerDataBuffer, BufferTypePtr dataBuffer, const uint32_t alignmentIndex, const uint32_t a5, const uint32_t a6)
{
    Type = type;
    HeaderDataSize = 0;
    DataSize = 0;
    AlignmentIndex = alignmentIndex;

    DataPtr = dataBuffer;
    DataStartPtr = dataBuffer;

    HeaderDataPtr = headerDataBuffer;
    HeaderDataStartPtr = headerDataBuffer;

    _f30 = a6;
    _f34 = a5 & 1;
    _f35 = (a5 & 2) != 0;
}

void AssetBuilder::ParseData(BufferTypePtr* dataPtr, BufferTypePtr bufferPtr, const uint32_t flags, const int32_t a4)
{
    if (_f30 == -1 || a4 != -1 && _f30 == a4)
    {
        //  NOTE: maybe this function is actually 'ConvertPointer' where 'Type' could be 'StringPointer', 'ObjectPointer', 'DataPointer', etc.?
        switch (Type)
        {
            case PartType::ZERO:
            {
                size_t slen = 0;
                const uint32_t dataAligned = (uint32_t)*dataPtr & 0xFFFFFFFC;
                if ( dataAligned != 0 )
                    slen = strlen((const char*)dataAligned) + 1;

                if (_f30 == -1)
                {
                    if ((flags & 2) != 0)
                        DataSize += ((slen + 3) & 0xFFFFFFFC);
                    else
                        HeaderDataSize += ((slen + 3) & 0xFFFFFFFC);
                }
                break;
            }

            case PartType::ONE:
            {
                const auto dataAligned = (uint32_t)*dataPtr & 0xFFFFFFFC;
                if (!dataAligned)
                    return;

                const auto slen = strlen((const char*)dataAligned);
                *bufferPtr &= 0xFFFFFFFC;
                const auto lenAligned = (slen + 4) & 0xFFFFFFFC;
                memcpy(GetBufferPtr(flags), (const void*)dataAligned, lenAligned);
                *bufferPtr &= 0xFFFFFFFC;
                auto bufferPtrMutable = GetBufferPtr(flags);
                if (bufferPtrMutable)
                    *bufferPtr = (uint32_t)bufferPtrMutable | 1;
                *bufferPtr &= 0xFFFFFFFC;
                if (_f30 == -1)
                {
                    if ((flags & 2) != 0)
                        DataPtr += (lenAligned + 3) & 0xFFFFFFFC;
                    else
                        HeaderDataPtr += (lenAligned + 3) & 0xFFFFFFFC;
                }
                break;
            }

            case PartType::TWO:
            {
                if (((uint32_t)*dataPtr & 0xFFFFFFFC) != 0)
                    RememberBufferPosition(dataPtr, flags);
                break;
            }

            case PartType::THREE:
            {
                OffsetToPointer(dataPtr, flags);
                *dataPtr = (BufferTypePtr)((uint32_t)*dataPtr & 0xFFFFFFFC);
                break;
            }

            default:
                return;
        }
    }
}

void AssetBuilder::ParseInfo(BufferTypePtr* assetInstancePtr, BufferTypePtr* bufferPtr, const size_t instanceSize, const uint32_t flags, const uint32_t a5)
{
    if (_f30 == -1 || a5 != -1 && _f30 == a5)
    {
        if (*assetInstancePtr)
        {
            switch (Type)
            {
                case PartType::ZERO:
                {
                    IncreaseSize(instanceSize, flags, -1);
                    *bufferPtr = *assetInstancePtr;
                    break;
                }

                case PartType::ONE:
                {
                    memcpy(GetBufferPtr(flags), *assetInstancePtr, instanceSize);
                    *bufferPtr = GetBufferPtr(flags);
                    IncreaseBufferPtr(instanceSize, flags, -1);
                    break;
                }

                case PartType::TWO:
                {
                    *bufferPtr = *assetInstancePtr;
                    RememberBufferPosition(assetInstancePtr, flags);
                    break;
                }

                case PartType::THREE:
                {
                    OffsetToPointer(assetInstancePtr, flags);
                    *bufferPtr = *assetInstancePtr;
                    break;
                }

                case PartType::FOUR:
                {
                    *bufferPtr = *assetInstancePtr;
                    break;
                }

                default:
                    return;
            }
        }
        else
        {
            *bufferPtr = nullptr;
        }
    }
    else
    {
        *bufferPtr = *assetInstancePtr;
    }
}

void AssetBuilder::OffsetToPointer(BufferTypePtr* bufferPtr, const uint32_t flags)
{
    if ((flags & 1) != 0)
        return;

    if (!*bufferPtr)
        return;

    if (*bufferPtr == (BufferTypePtr)0x80000000)
        *bufferPtr = nullptr;

    auto bufferOffsetPtr = *bufferPtr + (uint32_t)bufferPtr;
    *bufferPtr = bufferOffsetPtr;

    if ((flags & 2) != 0)
    {
        if ((flags & 4) == 0)
            *bufferPtr = &bufferOffsetPtr[DataStartPtr - HeaderDataStartPtr];
    }
    else
        if ((flags & 4) != 0)
            *bufferPtr = &bufferOffsetPtr[HeaderDataStartPtr - DataStartPtr];
}

void AssetBuilder::Instantiate(AssetBuilder& builder, BufferTypePtr buffer)
{
    const uint32_t dummyReferenceIndex = (uint32_t)-1;

    if (builder.Type == AssetBuilder::PartType::THREE)
        *(uint32_t*)buffer = (uint32_t)AssetTypes[*buffer]->AssetVMT;

    builder.ParseData((BufferTypePtr*)&buffer[4], nullptr, 0, -1);

    if (builder.Type == AssetBuilder::PartType::THREE)
        *(uint32_t*)(&buffer[8]) = dummyReferenceIndex;

    ((Asset*)buffer)->Apply(builder);
}