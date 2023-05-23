#include "MapBlockReader.h"

MapBlockReader::MapBlockReader(LPCSTR filename, LPCSTR ext)
{
    m_ResourceExtension = ext;
    m_ResourceName = filename;
    m_ResourceName += m_ResourceExtension;
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
    MultiPartReader readerAssetsHeaders(m_FilePtr, SharedHeader.ResourcesInfoSize, false);
    MultiPartReader readerAssetsSizeTable(m_FilePtr, SharedHeader.TotalResources * 4, false);

    for (size_t i = 0; i < SharedHeader.TotalResources; ++i)
    {
        uint32_t assetSize;
        readerAssetsSizeTable.Read(assetSize);

        AssetsSizes.emplace_back(assetSize);
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