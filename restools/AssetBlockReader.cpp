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
    104,	//	animation
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
    for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
        delete[] m_AssetsDataBuffer[i];
    delete[] m_AssetsSizes;
    for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
        delete[] m_AssetsNames[i];
    for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
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

    m_AssetsInfoBuffer = new char[m_SharedHeader.m_AssetsHeaderSize];
    fread(m_AssetsInfoBuffer, m_SharedHeader.m_AssetsHeaderSize, 1, m_FilePtr);

    m_AssetsSizes = new int[m_SharedHeader.m_ResourcesTotal];
    fread(m_AssetsSizes, sizeof(m_AssetsSizes), m_SharedHeader.m_ResourcesTotal, m_FilePtr);

    m_AssetsDataBuffer = (char**)new char[m_SharedHeader.m_ResourcesTotal * 4];
    m_AssetsNames = (char**)new char[m_SharedHeader.m_ResourcesTotal * 4];
    for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
    {
        m_AssetsDataBuffer[i] = new char[m_AssetsSizes[i]];
        fread(m_AssetsDataBuffer[i], m_AssetsSizes[i], 1, m_FilePtr);
    }
}

void AssetBlockReader::PrintInfo() const
{
    printf("\tEngine timestamp:\t%X\n", m_SharedHeader.m_EngineTimestamp);
    printf("\tProperties checksum:\t%X\n", m_SharedHeader.m_PropertyChecksum);
    printf("\tCommands checksum:\t%X\n", m_SharedHeader.m_CommandsChecksum);
    printf("\tResources in block:\t%d\n", m_SharedHeader.m_ResourcesTotal);
    printf("\tResources info header size:\t%d\n", m_SharedHeader.m_AssetsHeaderSize);
    printf("\tResources data buffer size:\t%d\n", m_SharedHeader.m_MaxBufferSize);

    if (m_SharedHeader.m_ResourcesTotal == -1)
        return;

    unsigned char* infobuffer = (unsigned char*)m_AssetsInfoBuffer;
    for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
    {
        printf("\t\t#%i\n", i);

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
            asset = new CompiledFragmentAsset(&infobuffer);
            break;
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
            printf("\tERROR: unsupported asset type: %d!\n", assettype);
            throw;
        }

        asset->PrintInfo();

        m_AssetsNames[i] = new char[strlen(asset->Name) + 1];
        strcpy(m_AssetsNames[i], asset->Name);

        m_AssetsList.push_back(asset);
    }
}

void AssetBlockReader::DumpData() const
{
    if (m_SharedHeader.m_ResourcesTotal <= 0)
    {
        printf("\tSorry, but this asset block has no assets in it!\n");
        return;
    }

    printf("\tDumping \"%s\" (%d assets):\n", m_ResourceName.c_str(), m_SharedHeader.m_ResourcesTotal);

    for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
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
    *infobuffer += strlen(Name) + 1;

    if (**infobuffer != 0xAB)
        return;

    while (**infobuffer == 0xAB)
        *infobuffer += (char)1;
}

void AssetBlockReader::CompiledAsset::SkipEndAlignment(uint8_t** infobuffer)
{
    const uint32_t alignment = ~(AssetTypeAlignment[AssetType][0] - 1) & ((uint32_t)*infobuffer + AssetTypeAlignment[AssetType][0]);
    *infobuffer = (uint8_t*)alignment;
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
    *infobuffer += sizeof(TextureInfo) + sizeof(GfxTexture);
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

    printf("\tDone!\n");
}

AssetBlockReader::CompiledFragmentAsset::CompiledFragmentAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    field_1C = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    field_20 = (unsigned int*)(*(unsigned int*)infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(unsigned int);

    field_24 = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    SkipNameRead(infobuffer);
    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledFragmentAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tfield_20:\t%p\n", field_20);
    printf("\tfield_24:\t%d\n", field_24);
}

void AssetBlockReader::CompiledFragmentAsset::SkipSpecificData(unsigned char** infobuffer)
{
    while (**infobuffer == NULL)
        *infobuffer += 1;
}

void AssetBlockReader::CompiledFragmentAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}

AssetBlockReader::CompiledStreamedSoundInfoAsset::CompiledStreamedSoundInfoAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    field_1C = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    m_SoundFile = (SoundFile*)(*infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(unsigned int);

    field_24 = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);
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
    *infobuffer += sizeof(SoundFile);
    *infobuffer += m_SoundFile->m_FileName.m_Length + 1;

    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledStreamedSoundInfoAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
}

AssetBlockReader::CompiledFontAsset::CompiledFontAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    field_1C = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    field_20 = (unsigned int*)(*infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(unsigned int);

    m_FontInfo = (Font*)(*infobuffer + **infobuffer);
    m_FontInfo->m_FontTexture = (CompiledTextureAsset::GfxTexture*)((unsigned int)m_FontInfo + offsetof(Font, m_FontTexture) + (unsigned int)m_FontInfo->m_FontTexture);
    m_FontInfo->m_GlyphsList = (Glyph*)((unsigned int)m_FontInfo + offsetof(Font, m_GlyphsList) + (unsigned int)m_FontInfo->m_GlyphsList);
    *infobuffer += sizeof(Font*);

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

    do
    {
        if (*infobuffer == (unsigned char*)m_FontInfo->m_FontTexture)
            break;

        *infobuffer += (char)1;
    } while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D);

    *infobuffer += sizeof(CompiledTextureAsset::GfxTexture);
    *infobuffer += sizeof(Glyph) * m_FontInfo->m_GlyphsInList;
    *infobuffer += 12;	//	TODO: what is this?
}

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

    printf("\tDone!\n");
}

AssetBlockReader::CompiledTextAsset::CompiledTextAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    field_1C = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    m_List_1_Elements = (unsigned int*)(*infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(int);
    m_List_1_Size = **(int**)infobuffer;
    *infobuffer += sizeof(int);
    field_28[0] = **(int**)infobuffer;
    *infobuffer += sizeof(int);
    field_28[1] = **(int**)infobuffer;
    *infobuffer += sizeof(int);

    m_TextIndicies_Elements = (unsigned short*)(*infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(int);
    m_TextIndicies_Size = **(int**)infobuffer;
    *infobuffer += sizeof(int);
    field_38[0] = **(int**)infobuffer;
    *infobuffer += sizeof(int);
    field_38[1] = **(int**)infobuffer;
    *infobuffer += sizeof(int);

    m_List_3_Elements = (unsigned char*)(*infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(int);
    m_List_3_Size = **(int**)infobuffer;
    *infobuffer += sizeof(int);
    field_48[0] = **(int**)infobuffer;
    *infobuffer += sizeof(int);
    field_48[1] = **(int**)infobuffer;
    *infobuffer += sizeof(int);

    m_CharactersMap = (Dictionary*)(*infobuffer + **(unsigned int**)infobuffer);
    *infobuffer += sizeof(unsigned int);

    field_54 = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);
    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledTextAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tList 1 size:\t%d\n", m_List_1_Size);
    printf("\tText indicies list size:\t%d\n", m_TextIndicies_Size);
    printf("\tText 3 size:\t%d\n", m_List_3_Size);
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

    printf("\tDone!\n");
}

AssetBlockReader::CompiledSoundAsset::CompiledSoundAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
    field_1C = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    m_StreamBuffer = (StreamBuffer*)(*infobuffer + **(unsigned int**)infobuffer);
    m_StreamBuffer->m_SampledData = (char*)((unsigned int)m_StreamBuffer + offsetof(StreamBuffer, m_SampledData) + (unsigned int)m_StreamBuffer->m_SampledData);
    m_StreamBuffer->m_SoundName = (String*)((unsigned int)m_StreamBuffer + offsetof(StreamBuffer, m_SoundName) + (unsigned int)m_StreamBuffer->m_SoundName);
    m_StreamBuffer->m_SoundName->m_String = (char*)((unsigned int)m_StreamBuffer->m_SoundName + offsetof(String, m_String) + (unsigned int)m_StreamBuffer->m_SoundName->m_String);
    *infobuffer += sizeof(StreamBuffer*);

    field_24 = **(unsigned int**)infobuffer;
    *infobuffer += sizeof(unsigned int);

    SkipNameRead(infobuffer);
    SkipSpecificData(infobuffer);
    SkipEndAlignment(infobuffer);
}

void AssetBlockReader::CompiledSoundAsset::PrintInfo() const
{
    CompiledAsset::PrintInfo();

    printf("\tfield_1C:\t%d\n", field_1C);
    printf("\tStreamBuffer:\n");
    printf("\t\tSamples per sec.:\t%d\n", m_StreamBuffer->m_SamplesPerSec);
    printf("\t\tChannels:\t%d\n", m_StreamBuffer->m_Channels);
    printf("\t\tTotal chunks:\t%d\n", m_StreamBuffer->m_TotalChunks);
    printf("\t\tSampled data size:\t%d\n", m_StreamBuffer->m_SampledDataSize);
    printf("\t\tBytes per sec.:\t%f\n", m_StreamBuffer->m_BytesPerSec);
    printf("\t\tSound name:\t\"%s\"\n", m_StreamBuffer->m_SoundName->m_String);
    printf("\t\tAuxMonoStream:\t%p\n", m_StreamBuffer->m_AuxMonoStream);
}

void AssetBlockReader::CompiledSoundAsset::SkipSpecificData(unsigned char** infobuffer)
{
    *infobuffer += sizeof(StreamBuffer);
    *infobuffer += (char)4;
    *infobuffer += m_StreamBuffer->m_SampledDataSize;
    *infobuffer += sizeof(String);
    *infobuffer += m_StreamBuffer->m_SoundName->m_Length + 1;
}

void AssetBlockReader::CompiledSoundAsset::DumpData(const AssetBlockReader* reader)
{
    printf("\tNOT IMPLEMENTED!\n");
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

    *infobuffer = (uint8_t*)ALIGN_4BYTES(*infobuffer);

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