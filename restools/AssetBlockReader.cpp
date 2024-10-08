#include "AssetBlockReader.h"
#include <stdlib.h>
#include <cassert>

const unsigned int AssetBlockReader::CompiledTextureAsset::DDS_HEADER::magick = 0x20534444;
int AssetBlockReader::CompiledTextAsset::Dictionary::Offset;
unsigned char* AssetBlockReader::CompiledTextAsset::Dictionary::Indicy;

const char* AssetBlockReader::AssetTypeIndexString[] =
{
    "TEXTURE",
    "FONT",
    "TEXT",
    "MODEL",
    "FRAGMENT",
    "MOVIE",
    "CUTSCENE",
    "SOUND",
    "STREAMEDSOUNDINFO",
    "ANIMATION",
    "MESHCOLOR"
};

unsigned int AssetBlockReader::AssetInstanceSize[] =
{
    48,	//	texture
    40,	//	font
    88,	//	text
    96,	//	model
    40,	//	fragment
    40,	//	movie
    96,	//	cutscene
    40,	//	sound
    40,	//	streamedsoundinfo
    104,//	animation
    80	//	meshcolor
};

uint32_t AssetBlockReader::AssetTypeAlignment[][3] =
{
    { 16, 16, 128}, //	texture
    { 16, 16, 128},	//	font
    { 16, 16, 16 },	//	text
    { 16, 16, 16 },	//	model
    { 16, 16, 16 },	//	fragment
    { 16, 16, 16 },	//	movie
    { 16, 16, 16 },	//	cutscene
    { 16, 16, 16 },	//	sound
    { 16, 16, 16 },	//	streamedsoundinfo
    { 16, 16, 16 },	//	animation
    { 16, 16, 16}	//	meshcolor
};

const char* const AssetBlockReader::CompiledTextureAsset::TextureFormatString[] =
{
    "R8G8B8",
    "A8R8G8B8",
    "R5G6B5",
    "A1R5G5B5",
    "A4R4G4B4",
    "P8",
    "UNKNOWN",
    "DXT1",
    "DXT2",
    "DXT3",
    "DXT4",
    "DXT5"
};

AssetBlockReader::AssetBlockReader(LPCSTR filename, LPCSTR ext)
{
    m_ResourceExtension = ext;
    m_ResourceName = filename;
    m_ResourceName += m_ResourceExtension;
}

AssetBlockReader::~AssetBlockReader()
{
    delete[] m_AssetsInfoBuffer;
    for (int i = 0; i < m_SharedHeader.ResourcesTotal; ++i)
        delete[] m_AssetsDataBuffer[i];
    delete[] m_AssetsSizes;
    for (int i = 0; i < m_SharedHeader.ResourcesTotal; ++i)
        delete[] m_AssetsNames[i];
    for (int i = 0; i < m_SharedHeader.ResourcesTotal; ++i)
        if (m_AssetsList[i])
            delete m_AssetsList[i];
    m_AssetsList.clear();
}

void AssetBlockReader::ReadInfo()
{
    if (!m_FilePtr ||
        m_LastErrorCode)
        return;

    fread(&m_SharedHeader, sizeof(m_SharedHeader), 1, m_FilePtr);

    m_AssetsInfoBuffer = new char[m_SharedHeader.AssetsHeaderSize];
    fread(m_AssetsInfoBuffer, m_SharedHeader.AssetsHeaderSize, 1, m_FilePtr);

    m_AssetsSizes = new int[m_SharedHeader.ResourcesTotal];
    fread(m_AssetsSizes, sizeof(m_AssetsSizes), m_SharedHeader.ResourcesTotal, m_FilePtr);

    m_AssetsDataBuffer = (char**)new char[m_SharedHeader.ResourcesTotal * 4];
    m_AssetsNames = (char**)new char[m_SharedHeader.ResourcesTotal * 4];
    for (int i = 0; i < m_SharedHeader.ResourcesTotal; ++i)
    {
        m_AssetsDataBuffer[i] = new char[m_AssetsSizes[i]];
        fread(m_AssetsDataBuffer[i], m_AssetsSizes[i], 1, m_FilePtr);
    }
}

void AssetBlockReader::PrintInfo() const
{
    printf("\tEngine timestamp:\t%X\n", m_SharedHeader.EngineTimestamp);
    printf("\tProperties checksum:\t%X\n", m_SharedHeader.PropertyChecksum);
    printf("\tCommands checksum:\t%X\n", m_SharedHeader.CommandsChecksum);
    printf("\tResources in block:\t%d\n", m_SharedHeader.ResourcesTotal);
    printf("\tResources info header size:\t%d\n", m_SharedHeader.AssetsHeaderSize);
    printf("\tResources data buffer size:\t%d\n", m_SharedHeader.MaxBufferSize);

    if (m_SharedHeader.ResourcesTotal == -1)
        return;

    unsigned char* infobuffer = (unsigned char*)m_AssetsInfoBuffer;
    uint32_t offsetInFile = sizeof(m_SharedHeader);
    for (int i = 0; i < m_SharedHeader.ResourcesTotal; ++i)
    {
        printf("\n\tFile offset:\t0x%x\n", offsetInFile);
        const uint8_t* infobufferStart = infobuffer;

        CompiledAsset* asset = nullptr;
        const tAssetType assettype = *(tAssetType*)infobuffer;

        switch (assettype)
        {
        case TEXTURE:
        {
            asset = new CompiledTextureAsset(&infobuffer);
            ((CompiledTextureAsset*)asset)->m_FileSize = m_AssetsSizes[i];
            ((CompiledTextureAsset*)asset)->m_DataBuffer = m_AssetsDataBuffer[i];
            break;
        }
        case FONT:
        {
            asset = new CompiledFontAsset(&infobuffer);
            ((CompiledFontAsset*)asset)->m_FileSize = m_AssetsSizes[i];
            ((CompiledFontAsset*)asset)->m_DataBuffer = m_AssetsDataBuffer[i];
            break;
        }
        case TEXT:
            asset = new CompiledTextAsset(&infobuffer);
            break;
        case MODEL:
            asset = new CompiledModelAsset(&infobuffer);
            break;
        case FRAGMENT:
        {
            asset = new CompiledFragmentAsset(&infobuffer);
            ((CompiledFragmentAsset*)asset)->FileSize = m_AssetsSizes[i];
            ((CompiledFragmentAsset*)asset)->DataBuffer = (uint8_t*)m_AssetsDataBuffer[i];
            break;
        }
        case MOVIE:
            //asset = new CompiledMovieAsset(&infobuffer);
            break;
        case CUTSCENE:
            //asset = new CompiledCutsceneAsset(&infobuffer);
            break;
        case SOUND:
            asset = new CompiledSoundAsset(&infobuffer);
            break;
        case STREAMEDSOUNDINFO:
            asset = new CompiledStreamedSoundInfoAsset(&infobuffer);
            break;
        case ANIMATION:
            asset = new CompiledAnimationAsset(&infobuffer);
            break;
        case MESHCOLOR:
            //asset = new CompiledMeshColorAsset(&infobuffer);
            break;
        }

        if (asset == nullptr)
        {
            //  If we got here, then no need to proceed reading the file.
            printf("\tFATAL ERROR: unsupported asset type: 0x%x!\n", assettype);
            throw std::bad_typeid();
        }

        asset->PrintInfo();

        m_AssetsNames[i] = new char[strlen(asset->Name) + 1];
        strcpy(m_AssetsNames[i], asset->Name);

        m_AssetsList.push_back(asset);

        offsetInFile += infobuffer - infobufferStart;

        //  Dump this data right away if flag was set.
        if (m_ShouldDumpData)
            asset->DumpData(this);
    }
}

void AssetBlockReader::DumpData() const
{
    if (m_SharedHeader.ResourcesTotal <= 0)
    {
        printf("\tSorry, but this asset block has no assets in it!\n");
        return;
    }

    printf("\tDumping \"%s\" (%d assets):\n", m_ResourceName.c_str(), m_SharedHeader.ResourcesTotal);

    for (int i = 0; i < m_SharedHeader.ResourcesTotal; ++i)
    {
        char assname[512] = {};
        strcpy(assname, strrchr(m_AssetsNames[i], '/') + 1);
        const char* const assetext = _strlwr(strrchr(assname, '.') + 1);

        printf("\tDumping \"%s\"...\n", assname);

        m_AssetsList[i]->DumpData(this);
    }
}

AssetBlockReader::CompiledAsset::CompiledAsset(unsigned char** infobuffer)
{
    READ_FIELD_VALUE(AssetType, tAssetType, infobuffer);
    READ_FIELD_VALUE_POINTER(Name, char, infobuffer);
    READ_FIELD_VALUE(GlobalId, uint32_t, infobuffer);
    READ_FIELD_VALUE(field_C, uint32_t, infobuffer);
    READ_FIELD_VALUE(EngineTimestamp, uint64_t, infobuffer);
    READ_FIELD_VALUE(Flags, uint32_t, infobuffer);
}

void AssetBlockReader::CompiledAsset::PrintInfo() const
{
    printf("\tType:\t%s\n", AssetTypeIndexString[AssetType]);
    printf("\tPath:\t\"%s\"\n", Name);
    printf("\tGlobal id:\t%d\n", GlobalId);
    printf("\tfield_C:\t%x\n", field_C);
    printf("\tEngine timestamp:\t%llu\n", EngineTimestamp);
    printf("\tFlags:\t%X\n", Flags);
}

void AssetBlockReader::CompiledAsset::SkipNameRead(unsigned char** infobuffer)
{
    const size_t nameLength = strlen(Name) + 1;
    *infobuffer += ALIGN_4BYTESUP(nameLength);
}

void AssetBlockReader::CompiledAsset::SkipEndAlignment(uint8_t** infobuffer)
{
    const uint32_t alignment = ~(AssetTypeAlignment[AssetType][0] - 1) & ((uint32_t)*infobuffer + AssetTypeAlignment[AssetType][0]);
    *infobuffer = (uint8_t*)alignment;
}

const bool AssetBlockReader::CompiledAsset::IsAligned(uint8_t** infobuffer) const
{
    const uint32_t alignment = ~(AssetTypeAlignment[AssetType][0] - 1) & ((uint32_t)*infobuffer + AssetTypeAlignment[AssetType][0]);
    return (uint32_t)*infobuffer == alignment;
}

AssetBlockReader::CompiledTextureAsset::CompiledTextureAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(m_BitsPerPixel, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(m_TextureInfo, TextureInfo, infobuffer);
    READ_FIELD_VALUE(field_24, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(m_GfxTexture, GfxTexture, infobuffer);
    READ_FIELD_VALUE(field_2C, uint32_t, infobuffer);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);

    if (((uint32_t)*infobuffer & 0xFFFFFFFC) != (uint32_t)*infobuffer)
        SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledTextureAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tBits per pixel:\t%d\n", m_BitsPerPixel);
    printf("\tGFX texture:\n");
    printf("\tResolution:\t%dx%d\n", m_GfxTexture->m_Resolution[0], m_GfxTexture->m_Resolution[1]);
    printf("\tSurface res.:\t%dx%d\n", m_GfxTexture->m_SurfaceSize[0], m_GfxTexture->m_SurfaceSize[1]);
    printf("\tFormat:\t%s\n", TextureFormatString[m_GfxTexture->m_Format]);
    printf("\tfield_24:\t%x\n", field_24);
    printf("\tfield_2C:\t%x\n", field_2C);
}

void AssetBlockReader::CompiledTextureAsset::SkipSpecificData(unsigned char** infobuffer)
{
    if ((uint32_t)m_TextureInfo > (uint32_t)m_GfxTexture)
        *infobuffer = (uint8_t*)m_TextureInfo + sizeof(TextureInfo);
    else
        *infobuffer = (uint8_t*)m_GfxTexture + sizeof(GfxTexture);
}

void AssetBlockReader::CompiledTextureAsset::DumpData(const AssetBlockReader* reader)
{
    CompiledTextureAsset::DDS_HEADER ddsheader;

    ddsheader.size = sizeof(CompiledTextureAsset::DDS_HEADER);
    ddsheader.flags = (CompiledTextureAsset::DDS_FLAGS)(
        CompiledTextureAsset::DDS_FLAGS::DDSD_CAPS |
        CompiledTextureAsset::DDS_FLAGS::DDSD_HEIGHT |
        CompiledTextureAsset::DDS_FLAGS::DDSD_WIDTH |
        CompiledTextureAsset::DDS_FLAGS::DDSD_PIXELFORMAT |
        (m_GfxTexture->m_Format == 1 ? CompiledTextureAsset::DDS_FLAGS::DDSD_PITCH : CompiledTextureAsset::DDS_FLAGS::DDSD_LINEARSIZE));
    ddsheader.height = m_GfxTexture->m_SurfaceSize[1];
    ddsheader.width = m_GfxTexture->m_SurfaceSize[0];
    ddsheader.pitchOrLinearSize = m_GfxTexture->m_Format == 1 ? 4 : m_FileSize;
    ddsheader.depth = NULL;
    ddsheader.mipMapCount = m_GfxTexture->m_MipMapLevels;

    ddsheader.ddspf.size = sizeof(CompiledTextureAsset::DDS_HEADER::DDS_PIXELFORMAT);
    ddsheader.ddspf.flags = m_GfxTexture->m_Format == 1 ? (CompiledTextureAsset::DDS_PIXELFORMAT::DDPF_RGB | CompiledTextureAsset::DDS_PIXELFORMAT::DDPF_ALPHAPIXELS) : CompiledTextureAsset::DDS_PIXELFORMAT::DDPF_FOURCC;
    if (m_GfxTexture->m_Format != 1)
        strcpy(ddsheader.ddspf.fourcc, CompiledTextureAsset::TextureFormatString[m_GfxTexture->m_Format]);
    else
        *(int*)&ddsheader.ddspf.fourcc = 0;
    ddsheader.ddspf.RGBBitCount = m_GfxTexture->m_Format == 1 ? 32 : NULL;
    ddsheader.ddspf.RBitMask = m_GfxTexture->m_Format == 1 ? 0x00ff0000 : NULL;
    ddsheader.ddspf.GBitMask = m_GfxTexture->m_Format == 1 ? 0x0000ff00 : NULL;
    ddsheader.ddspf.BBitMask = m_GfxTexture->m_Format == 1 ? 0x000000ff : NULL;
    ddsheader.ddspf.ABitMask = m_GfxTexture->m_Format == 1 ? 0xFF000000 : NULL;

    ddsheader.caps = CompiledTextureAsset::DDS_HEADER::DDSCAPS_TEXTURE;
    ddsheader.caps2 = NULL;
    ddsheader.caps3 = NULL;
    ddsheader.caps4 = NULL;
    ddsheader.reserved2 = NULL;

    FILE* f = nullptr;
    char* assetname = strrchr(Name, '.');
    strcat(assetname, ".dds");
    char* assetfilename = strrchr(Name, '/') + 1;
    errno_t err = fopen_s(&f, assetfilename, "wb");
    //  TODO: generate missing folders or give choice to user.
    if (!f)
    {
        printf("Failed to create texture file '%s'! fopen returned error %d\n", assetfilename, err);
        return;
    }
    fwrite((const void*)&CompiledTextureAsset::DDS_HEADER::magick, sizeof(CompiledTextureAsset::DDS_HEADER::magick), 1, f);
    fwrite((const void*)&ddsheader, sizeof(CompiledTextureAsset::DDS_HEADER), 1, f);
    fwrite(m_DataBuffer, m_FileSize, 1, f);

    fclose(f);

    printf("\tTexture asset dump done!\n");
}

AssetBlockReader::CompiledFragmentAsset::CompiledFragmentAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(FragmentInfo, FragmentData, infobuffer);
    READ_FIELD_VALUE(field_24, uint32_t, infobuffer);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);

    if (((uint32_t)*infobuffer & 0xFFFFFFFC) != (uint32_t)*infobuffer)
        SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledFragmentAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tFragment Info:\t%p\n", FragmentInfo);
    printf("\tfield_24:\t%d\n", field_24);
}

void AssetBlockReader::CompiledFragmentAsset::SkipSpecificData(unsigned char** infobuffer)
{
    *infobuffer += sizeof(FragmentData);
}

void AssetBlockReader::CompiledFragmentAsset::DumpData(const AssetBlockReader* reader)
{
    FILE* filePtr = nullptr;

    char* fragmentName = strrchr(Name, '/') + 1;
    errno_t openStatus = fopen_s(&filePtr, fragmentName, "wb");
    if (!filePtr)
    {
        printf("\tFailed to create output file! Error %d\n", openStatus);
        return;
    }

    fwrite((const void*)DataBuffer, FileSize, 1, filePtr);

    fclose(filePtr);

    printf("\tFragment asset dump done!\n");
}

AssetBlockReader::CompiledStreamedSoundInfoAsset::CompiledStreamedSoundInfoAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(m_SoundFile, SoundFile, infobuffer);
    READ_FIELD_VALUE(field_24, uint32_t, infobuffer);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);

    //  TODO: alignment?
    if (!IsAligned(infobuffer))
        SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledStreamedSoundInfoAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%x\n", field_1C);
    printf("\tSound file info:\n");
    printf("\t\tWAV chunk size:\t%d\n", m_SoundFile->m_WavChunkSize);
    printf("\t\tfield_4:\t%x\n", m_SoundFile->field_4);
    printf("\t\tSamples:\t%d\n", m_SoundFile->m_Samples);
    printf("\t\tBytes per sample:\t%f\n", m_SoundFile->m_BytesPerSample);
    printf("\t\tAverage bytes per sec.:\t%d\n", m_SoundFile->m_AverageBytesPerSec);
    printf("\t\tBlock align:\t%d\n", m_SoundFile->m_BlockAlign);
    printf("\t\tFrequency:\t%d\n", m_SoundFile->m_Frequency);
    printf("\t\tChunk size:\t%d\n", m_SoundFile->m_ChunkSize);
    printf("\t\tSound format:\t%d\n", m_SoundFile->m_SoundFormat);
    printf("\t\tfield_24:\t%d\n", m_SoundFile->field_24);
    printf("\t\tFilename:\t\"%s\"\n", (const char*)((unsigned int)m_SoundFile + offsetof(SoundFile, m_FileName) + offsetof(String, m_String)) + (unsigned int)m_SoundFile->m_FileName.m_String);
    printf("\tfield_24:\t%x\n", field_24);
}

void AssetBlockReader::CompiledStreamedSoundInfoAsset::SkipSpecificData(unsigned char** infobuffer)
{
    if (*infobuffer != (uint8_t*)m_SoundFile)
        SkipEndAlignment(infobuffer);

    *infobuffer += sizeof(SoundFile);
    *infobuffer += m_SoundFile->m_FileName.m_Length;
}

void AssetBlockReader::CompiledStreamedSoundInfoAsset::DumpData(const AssetBlockReader* reader)
{
    if (!m_SoundFile->m_StreamedWAV)
    {
        printf("\tSound file is not in this archive! Check 'sounds'.\n");
        return;
    }
    else
        printf("\tNOT IMPLEMENTED!\n");
}

AssetBlockReader::CompiledFontAsset::CompiledFontAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(field_20, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(m_FontInfo, Font, infobuffer);

    m_FontInfo->m_FontTexture = (CompiledTextureAsset::GfxTexture*)((uint32_t)m_FontInfo + offsetof(Font, m_FontTexture) + (uint32_t)m_FontInfo->m_FontTexture);
    m_FontInfo->m_GlyphsList = (Glyph*)((uint32_t)m_FontInfo + offsetof(Font, m_GlyphsList) + (uint32_t)m_FontInfo->m_GlyphsList);
    m_FontInfo->m_GlyphsMap = (int*)((uint32_t)m_FontInfo + offsetof(Font, m_GlyphsMap) + (uint32_t)m_FontInfo->m_GlyphsMap);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);
    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledFontAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tfield_20:\t%p\n", field_20);
    printf("\tFont info:\n");
    printf("\t\tGlyphs total:\t%d\n", m_FontInfo->m_GlyphsInList);
    printf("\t\tHorizontal spacing:\t%f\n", m_FontInfo->m_HorizontalSpacing);
    printf("\t\tVertical spacing:\t%f\n", m_FontInfo->m_VerticalSpacing);
    printf("\t\tScale X:\t%f\n", m_FontInfo->m_ScaleX);
    printf("\t\tScale Y:\t%f\n", m_FontInfo->m_ScaleY);
    printf("\t\tTexture:\t%p\n", m_FontInfo->m_FontTexture);
    printf("\t\tTexture size:\t%d x %d\n", m_FontInfo->m_FontTexture->m_Resolution[0], m_FontInfo->m_FontTexture->m_Resolution[1]);
}

void AssetBlockReader::CompiledFontAsset::SkipSpecificData(unsigned char** infobuffer)
{
    *infobuffer += sizeof(Font);
    *infobuffer += sizeof(CompiledTextureAsset::GfxTexture);
    *infobuffer += sizeof(Glyph) * m_FontInfo->m_GlyphsInList;
    *infobuffer += 20;
}

//  TODO: this is exactly same as Texture asset, maybe remove redundancy?
void AssetBlockReader::CompiledFontAsset::DumpData(const AssetBlockReader* reader)
{
    CompiledTextureAsset::DDS_HEADER ddsheader;

    ddsheader.size = sizeof(CompiledTextureAsset::DDS_HEADER);
    ddsheader.flags = (CompiledTextureAsset::DDS_FLAGS)(
        CompiledTextureAsset::DDS_FLAGS::DDSD_CAPS |
        CompiledTextureAsset::DDS_FLAGS::DDSD_HEIGHT |
        CompiledTextureAsset::DDS_FLAGS::DDSD_WIDTH |
        CompiledTextureAsset::DDS_FLAGS::DDSD_PIXELFORMAT |
        ( m_FontInfo->m_FontTexture->m_Format == 1 ? CompiledTextureAsset::DDS_FLAGS::DDSD_PITCH : CompiledTextureAsset::DDS_FLAGS::DDSD_LINEARSIZE) );
    ddsheader.height = m_FontInfo->m_FontTexture->m_Resolution[1];
    ddsheader.width = m_FontInfo->m_FontTexture->m_Resolution[0];
    ddsheader.pitchOrLinearSize = m_FontInfo->m_FontTexture->m_Format == 1 ? 4 : m_FileSize;
    ddsheader.depth = NULL;
    ddsheader.mipMapCount = m_FontInfo->m_FontTexture->m_MipMapLevels;

    ddsheader.ddspf.size = sizeof(CompiledTextureAsset::DDS_HEADER::DDS_PIXELFORMAT);
    ddsheader.ddspf.flags = m_FontInfo->m_FontTexture->m_Format == 1 ? (CompiledTextureAsset::DDS_PIXELFORMAT::DDPF_RGB | CompiledTextureAsset::DDS_PIXELFORMAT::DDPF_ALPHAPIXELS) : CompiledTextureAsset::DDS_PIXELFORMAT::DDPF_FOURCC;
    if (m_FontInfo->m_FontTexture->m_Format != 1)
        strcpy(ddsheader.ddspf.fourcc, CompiledTextureAsset::TextureFormatString[m_FontInfo->m_FontTexture->m_Format]);
    else
        *(int*)&ddsheader.ddspf.fourcc = 0;
    ddsheader.ddspf.RGBBitCount = m_FontInfo->m_FontTexture->m_Format == 1 ? 32 : NULL;
    ddsheader.ddspf.RBitMask = m_FontInfo->m_FontTexture->m_Format == 1 ? 0x00ff0000 : NULL;
    ddsheader.ddspf.GBitMask = m_FontInfo->m_FontTexture->m_Format == 1 ? 0x0000ff00 : NULL;
    ddsheader.ddspf.BBitMask = m_FontInfo->m_FontTexture->m_Format == 1 ? 0x000000ff : NULL;
    ddsheader.ddspf.ABitMask = m_FontInfo->m_FontTexture->m_Format == 1 ? 0xFF000000 : NULL;

    ddsheader.caps = CompiledTextureAsset::DDS_HEADER::DDSCAPS_TEXTURE;
    ddsheader.caps2 = NULL;
    ddsheader.caps3 = NULL;
    ddsheader.caps4 = NULL;
    ddsheader.reserved2 = NULL;

    FILE* f = nullptr;
    char* assetname = strrchr(Name, '.');
    strcat(assetname, ".dds");
    char* assetfilename = strrchr(Name, '/') + 1;
    errno_t err = fopen_s(&f, assetfilename, "wb");
    //  TODO: generate missing folders or give choice to user.
    if (!f)
    {
        printf("Failed to create font texture file '%s'! fopen returned error %d\n", assetfilename, err);
        return;
    }
    fwrite((const void*)&CompiledTextureAsset::DDS_HEADER::magick, sizeof(CompiledTextureAsset::DDS_HEADER::magick), 1, f);
    fwrite((const void*)&ddsheader, sizeof(CompiledTextureAsset::DDS_HEADER), 1, f);
    fwrite(m_DataBuffer, m_FileSize, 1, f);

    fclose(f);

    printf("\tFont asset dump done!\n");
}

AssetBlockReader::CompiledTextAsset::CompiledTextAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_LIST_VALUE(&m_List_1_Elements, infobuffer);
    READ_LIST_VALUE(&m_TextIndicies_Elements, infobuffer);
    READ_LIST_VALUE(&m_List_3_Elements, infobuffer);
    READ_FIELD_VALUE_POINTER(m_CharactersMap, Dictionary, infobuffer);
    READ_FIELD_VALUE(field_54, uint32_t, infobuffer);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);

    //  TODO: replace this absurd.
    if ((uint32_t)*infobuffer % 16 != 0)
        SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledTextAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tList 1 size:\t%d\n", m_List_1_Size);
    printf("\tText indicies list size:\t%d\n", m_TextIndicies_Size);
    printf("\tList 3 size:\t%d\n", m_List_3_Size);
    printf("\tCharacters map:\t%p\n", m_CharactersMap);
    printf("\tfield_54:\t%x\n", field_54);
}

void AssetBlockReader::CompiledTextAsset::SkipSpecificData(unsigned char** infobuffer)
{
    //	NOTE: there are 3 structs and their position could be in any order. Figure out biggest address out of 3, and skip there.
    unsigned int maxstructaddr = max((unsigned int)m_List_1_Elements, (unsigned int)m_TextIndicies_Elements);
    maxstructaddr = max((unsigned int)m_List_3_Elements, maxstructaddr);

    while ((unsigned int)*infobuffer != maxstructaddr)
        *infobuffer += (char)1;

    //	NOTE: once biggest address is reached - figure out what structure it is exactly and skip it.
    bool readfinished = false;
    if ((unsigned int)*infobuffer == (unsigned int)m_List_1_Elements && m_List_1_Size > 0)
    {
        *infobuffer += sizeof(unsigned int) * m_List_1_Size;
        readfinished = true;
    }

    if ((unsigned int)*infobuffer == (unsigned int)m_TextIndicies_Elements && m_TextIndicies_Size > 0 && readfinished == false)
    {
        *infobuffer += sizeof(unsigned short) * m_TextIndicies_Size;
        readfinished = true;
    }

    if ((unsigned int)*infobuffer == (unsigned int)m_List_3_Elements && m_List_3_Size > 0 && readfinished == false)
    {
        *infobuffer += sizeof(unsigned int) * m_List_3_Size;
        readfinished = true;
    }

    //	NOTE: none of the 3 structs are at that address - then algorithm is bugged!
    if (readfinished == false)
    {
        printf("\tERROR: none of the structs are at that address!\n");
        throw;
    }
}

void AssetBlockReader::CompiledTextAsset::DumpData(const AssetBlockReader* reader)
{
    unsigned char str[2048] = {};
    unsigned int slen = NULL;
    FILE* textFile = nullptr;

    char* textName = strrchr(Name, '/') + 1;
    fopen_s(&textFile, textName, "wb");
    if (!textFile)
    {
        printf("\tFailed to create output file!\n");
        return;
    }

    for (unsigned int i = 0; i < m_TextIndicies_Size; ++i)
    {
        GetGameString(m_TextIndicies_Elements[i], str, &slen, false);
        if (*str == NULL)
        {
            printf("\tSkipped empty text slot %d\n", i);
            continue;
        }

        fwrite(str, slen - 1, 1, textFile);
        fwrite("=", 1, 1, textFile);
        memset(str, NULL, slen);
        GetGameString(m_TextIndicies_Elements[i], str, &slen, true);
        fwrite(str, slen - 1, 1, textFile);
        fwrite("\n", 1, 1, textFile);
    }

    fclose(textFile);

    printf("\tText asset dump done!\n");
}

AssetBlockReader::CompiledSoundAsset::CompiledSoundAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(m_StreamBuffer, StreamBuffer, infobuffer);

    m_StreamBuffer->SampledData = (uint8_t*)((uint32_t)m_StreamBuffer + offsetof(StreamBuffer, SampledData) + (uint32_t)m_StreamBuffer->SampledData);
    m_StreamBuffer->SoundName = (String*)((uint32_t)m_StreamBuffer + offsetof(StreamBuffer, SoundName) + (uint32_t)m_StreamBuffer->SoundName);
    m_StreamBuffer->SoundName->m_String = (char*)((uint32_t)m_StreamBuffer->SoundName + offsetof(String, m_String) + (uint32_t)m_StreamBuffer->SoundName->m_String);

    READ_FIELD_VALUE(field_24, uint32_t, infobuffer);

    RIFFData.NumChannels = m_StreamBuffer->Channels;
    RIFFData.SampleRate = m_StreamBuffer->SamplesPerSec;
    RIFFData.ByteRate = (uint32_t)m_StreamBuffer->BytesPerSample;
    RIFFData.BlockAlign = (uint16_t)m_StreamBuffer->BytesPerSample;
    RIFFData.BitsPerSample = (uint32_t)m_StreamBuffer->BytesPerSample * 8;
    RIFFData.Subchunk2Size = m_StreamBuffer->SampledDataSize;
    RIFFData.ChunkSize = RIFFData.Subchunk2Size - 44 - 8;

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);

    if (!IsAligned(infobuffer))
        SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledSoundAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tStreamBuffer:\n");
    printf("\t\tSamples per sec.:\t%d\n", m_StreamBuffer->SamplesPerSec);
    printf("\t\tChannels:\t%d\n", m_StreamBuffer->Channels);
    printf("\t\tTotal chunks:\t%d\n", m_StreamBuffer->TotalChunks);
    printf("\t\tSampled data size:\t%d\n", m_StreamBuffer->SampledDataSize);
    printf("\t\tBytes per sample.:\t%f\n", m_StreamBuffer->BytesPerSample);
    printf("\t\tSound name:\t\"%s\"\n", m_StreamBuffer->SoundName->m_String);
    printf("\t\tAuxMonoStream:\t%p\n", m_StreamBuffer->AuxMonoStream);
}

void AssetBlockReader::CompiledSoundAsset::SkipSpecificData(unsigned char** infobuffer)
{
    *infobuffer = (uint8_t*)(m_StreamBuffer->SoundName);
    *infobuffer += sizeof(String);
    *infobuffer += m_StreamBuffer->SoundName->m_Length;
}

void AssetBlockReader::CompiledSoundAsset::DumpData(const AssetBlockReader* reader)
{
    FILE* f = nullptr;

    std::string assetFileName(strrchr(m_StreamBuffer->SoundName->m_String, '/') + 1);
    errno_t err = fopen_s(&f, assetFileName.c_str(), "wb");
    //  TODO: generate missing folders or give choice to user.
    if (!f)
    {
        printf("Failed to create sound file '%s'! fopen returned error %d\n", assetFileName.c_str(), err);
        return;
    }

    fwrite((const void*)&RIFFData, sizeof(RIFFData), 1, f);
    fwrite((const void*)m_StreamBuffer->SampledData, m_StreamBuffer->SampledDataSize, 1, f);

    fclose(f);

    printf("\tSound asset dump done!\n");
}

AssetBlockReader::CompiledModelAsset::CompiledModelAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_LIST_VALUE(&TextureReferencesList, infobuffer);
    READ_LIST_VALUE(&MeshList, infobuffer);
    READ_FIELD_VALUE_POINTER(PhysAttachmentsList, uint32_t, infobuffer);
    READ_FIELD_VALUE(BoundingRadius, vec4, infobuffer);
    READ_FIELD_VALUE_POINTER(field_54, uint32_t, infobuffer);
    READ_FIELD_VALUE_POINTER(field_58, uint32_t, infobuffer);
    READ_FIELD_VALUE(field_5C, uint32_t, infobuffer);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);
    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledModelAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();
}

void AssetBlockReader::CompiledModelAsset::SkipSpecificData(unsigned char** infobuffer)
{
    //  Experiment: don't really need to read lists right away, since we have furthest pointer available.
    if ((uint32_t)field_58 > (uint32_t)field_54)
        *infobuffer = (uint8_t*)((uint32_t)field_58 + *(uint32_t*)field_58);
    else
        *infobuffer = (uint8_t*)((uint32_t)field_54);
}

void AssetBlockReader::CompiledModelAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}

AssetBlockReader::CompiledAnimationAsset::CompiledAnimationAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    READ_FIELD_VALUE(field_1C, uint32_t, infobuffer);
    READ_FIELD_VALUE(field_20, uint32_t, infobuffer);
    READ_FIELD_VALUE(LoopMode, uint32_t, infobuffer);
    READ_FIELD_VALUE(field_28, uint32_t, infobuffer);

    READ_LIST_VALUE(&List_1, infobuffer);
    READ_LIST_VALUE(&List_2, infobuffer);
    READ_LIST_VALUE(&List_3, infobuffer);

    READ_FIELD_VALUE_POINTER(field_5C, AnimationPivotData, infobuffer);

    READ_FIELD_VALUE(field_60, uint32_t, infobuffer);
    READ_FIELD_VALUE(field_64, uint16_t, infobuffer);
    READ_FIELD_VALUE(field_66, uint16_t, infobuffer);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);
    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledAnimationAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();
}

void AssetBlockReader::CompiledAnimationAsset::SkipSpecificData(unsigned char** infobuffer)
{
    *infobuffer = (uint8_t*)((uint32_t)field_5C + (4 * field_60));
}

void AssetBlockReader::CompiledAnimationAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}

void AssetBlockReader::CompiledTextAsset::GetGameString(const unsigned short indicieslistindex, unsigned char* outString, unsigned int* outStringLength, const bool contents) const
{
    //  NOTE: argument 'contents' decides what information should be in the output - only text identifier OR actual contents.
    Dictionary::Indicy = (unsigned char*)&m_List_3_Elements[indicieslistindex];
    Dictionary::Offset = 0;

    unsigned int ind = 0;
    Dictionary* charinfo = nullptr;
    do
    {
        charinfo = Dictionary::GetCharacterInfo(m_CharactersMap);
        if (!contents)
            outString[ind++] = (unsigned char)charinfo->m_Contents;
    } while ((unsigned char)charinfo->m_Contents);
    *outStringLength = ind;

    if (!contents)
        return;

    charinfo = nullptr;
    do
    {
        charinfo = Dictionary::GetCharacterInfo(m_CharactersMap);
        outString[ind++] = (unsigned char)charinfo->m_Contents;
    } while ((unsigned char)charinfo->m_Contents);
    *outStringLength = ind;
}

AssetBlockReader::CompiledTextAsset::Dictionary* AssetBlockReader::CompiledTextAsset::Dictionary::GetCharacterInfo(Dictionary* dic)
{
    if (dic->m_Contents != 0xA74)
        return dic;

    do
    {
        if (Offset >= 8)
        {
            Indicy++;
            Offset -= 8;
        }

        //  NOTE: not part of the original code, but needs to be done, since 'pointers' are not adjusted right.
        if ((unsigned int)dic->m_Next <= 0xFFFF)
            RESTORE_POINTER(Dictionary, dic, m_Next);

        if ((unsigned int)dic->m_Previous <= 0xFFFF)
            RESTORE_POINTER(Dictionary, dic, m_Previous);

        const unsigned char currchar = *Indicy >> Offset++;
        if ((currchar & 1) != 0)
            dic = (Dictionary*)((int)dic->m_Previous & 0xFFFFFFFC);
        else
            dic = (Dictionary*)((int)dic->m_Next & 0xFFFFFFFC);
    } while (dic->m_Contents == 0xA74);

    return dic;
}

void AssetBlockReader::CompiledMovieAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}

void AssetBlockReader::CompiledCutsceneAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}

void AssetBlockReader::CompiledMeshColorAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}