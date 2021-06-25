#include "AssetBlockReader.h"
#include <stdlib.h>

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

unsigned int AssetBlockReader::AssetTypeAlignment[][3] =
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
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"DXT1",
	"DXT2",
	"DXT3",
	"DXT4",
	"DXT5"
};

AssetBlockReader::AssetBlockReader(LPCSTR filename)
{
	m_ResourceExtension = ".main";
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
		const AssetType assettype = *(AssetType*)infobuffer;

		switch (assettype)
		{
		case TEXTURE:
			asset = new CompiledTextureAsset(&infobuffer);
			break;
		case FONT:
			asset = new CompiledFontAsset(&infobuffer);
			break;
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
			//asset = new CompiledAnimationAsset(&infobuffer);
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

		m_AssetsNames[i] = new char[strlen(asset->m_AssetName) + 1];
		strcpy(m_AssetsNames[i], asset->m_AssetName);

		delete asset;
	}
}

void AssetBlockReader::DumpData() const
{
	if (m_SharedHeader.m_ResourcesTotal <= 0)
	{
		printf("\tSorry, but this asset block has no assets in it!\n");
		return;
	}

	for (int i = 0; i < m_SharedHeader.m_ResourcesTotal; ++i)
	{
		char assname[512] = {};
#ifdef KEEP_STRUCTURE
		strcpy(assname, m_AssetsNames[i]);
#else
		strcpy(assname, strrchr(m_AssetsNames[i], '/') + 1);
#endif
		strcat(assname, ".dump");

		//	TODO: restore asset header so it can be open as normal file.
		FILE* ofile = fopen(assname, "wb");
		fwrite(m_AssetsDataBuffer[i], m_AssetsSizes[i], 1, ofile);
		fclose(ofile);

		printf("Saved asset dump: %s\n", assname);
	}
}

AssetBlockReader::CompiledAsset::CompiledAsset(unsigned char** infobuffer)
{
	m_AssetType = **(AssetType**)infobuffer;
	*infobuffer += sizeof(AssetType);
	
	m_AssetName = (char*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(unsigned int);
	
	m_AssetGlobalId = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);
	
	field_C = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);
	
	m_EngineTimestamp = **(UINT64**)infobuffer;
	*infobuffer += sizeof(UINT64);

	m_Flags = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);
}

void AssetBlockReader::CompiledAsset::PrintInfo() const
{
	printf("\tType:\t%s\n", AssetTypeIndexString[m_AssetType]);
	printf("\tPath:\t\"%s\"\n", m_AssetName);
	printf("\tGlobal id:\t%d\n", m_AssetGlobalId);
	printf("\tfield_C:\t%x\n", field_C);
	printf("\tEngine timestamp:\t%llu\n", m_EngineTimestamp);
	printf("\tFlags:\t%X\n", m_Flags);
}

void AssetBlockReader::CompiledAsset::SkipNameRead(unsigned char** infobuffer)
{
	*infobuffer += strlen(m_AssetName) + 1;

	if (**infobuffer != 0xAB)
		return;

	while (**infobuffer == 0xAB)
		*infobuffer += (char)1;
}

void AssetBlockReader::CompiledAsset::SkipAlignment(unsigned char** infobuffer)
{
	if (**infobuffer == NULL)
		return;

	while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D)
		*infobuffer += (char)1;
}

AssetBlockReader::CompiledTextureAsset::CompiledTextureAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
	m_BitsPerPixel = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	m_TextureInfo = (TextureInfo*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(TextureInfo*);

	field_24 = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	m_GfxTexture = (GfxTexture*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(GfxTexture*);

	field_2C = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	SkipNameRead(infobuffer);
	SkipAlignment(infobuffer);
	SkipSpecificData(infobuffer);
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

AssetBlockReader::CompiledFragmentAsset::CompiledFragmentAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
	field_1C = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_20 = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_24 = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	SkipNameRead(infobuffer);
	SkipAlignment(infobuffer);
}

void AssetBlockReader::CompiledFragmentAsset::PrintInfo() const
{
	CompiledAsset::PrintInfo();

	printf("\tfield_1C:\t%d\n", field_1C);
	printf("\tfield_20:\t%d\n", field_20);
	printf("\tfield_24:\t%d\n", field_24);
}

void AssetBlockReader::CompiledFragmentAsset::SkipSpecificData(unsigned char** infobuffer)
{
	return;
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
	SkipAlignment(infobuffer);
	SkipSpecificData(infobuffer);
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

	CompiledAsset::SkipAlignment(infobuffer);
}

void AssetBlockReader::CompiledStreamedSoundInfoAsset::SkipAlignment(unsigned char** infobuffer)
{
	while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D)
		if (*infobuffer == (unsigned char*)m_SoundFile)
			break;
		else
			*infobuffer += (char)1;
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
	SkipAlignment(infobuffer);
	SkipSpecificData(infobuffer);
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
}

void AssetBlockReader::CompiledFontAsset::SkipAlignment(unsigned char** infobuffer)
{
	while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D)
		if (*infobuffer == (unsigned char*)m_FontInfo)
			break;
		else
			*infobuffer += (char)1;
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

	SkipAlignment(infobuffer);
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

	m_TextIndicies_Elements = (unsigned int*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(int);
	m_TextIndicies_Size = **(int**)infobuffer;
	*infobuffer += sizeof(int);
	field_38[0] = **(int**)infobuffer;
	*infobuffer += sizeof(int);
	field_38[1] = **(int**)infobuffer;
	*infobuffer += sizeof(int);

	m_List_3_Elements = (unsigned int*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(int);
	m_List_3_Size = **(int**)infobuffer;
	*infobuffer += sizeof(int);
	field_48[0] = **(int**)infobuffer;
	*infobuffer += sizeof(int);
	field_48[1] = **(int**)infobuffer;
	*infobuffer += sizeof(int);

	field_50 = (Dictionary*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(unsigned int);

	field_54 = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	SkipNameRead(infobuffer);
	SkipAlignment(infobuffer);
	SkipSpecificData(infobuffer);
}

void AssetBlockReader::CompiledTextAsset::PrintInfo() const
{
	CompiledAsset::PrintInfo();

	printf("\tfield_1C:\t%d\n", field_1C);
	printf("\tList 1 size:\t%d\n", m_List_1_Size);
	printf("\tText indicies list size:\t%d\n", m_TextIndicies_Size);
	printf("\tText 3 size:\t%d\n", m_List_3_Size);
	printf("\tfield_50:\t%p\n", field_50);
	printf("\tfield_54:\t%d\n", field_54);
}

void AssetBlockReader::CompiledTextAsset::SkipAlignment(unsigned char** infobuffer)
{
	while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D)
		if (*infobuffer == (unsigned char*)field_50)
			break;
		else
			*infobuffer += (char)1;
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

	SkipAlignment(infobuffer);
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
	SkipAlignment(infobuffer);
	SkipSpecificData(infobuffer);
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

	SkipAlignment(infobuffer);
}

void AssetBlockReader::CompiledSoundAsset::SkipAlignment(unsigned char** infobuffer)
{
	while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D)
		if (*infobuffer == (unsigned char*)m_StreamBuffer)
			break;
		else
			*infobuffer += (char)1;
}

AssetBlockReader::CompiledModelAsset::CompiledModelAsset(unsigned char** infobuffer) : CompiledAsset(infobuffer)
{
	field_1C = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	m_TextureResources_Elements = (TextureReference*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(TextureReference*);

	m_TextureResources_Size = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	for (unsigned int i = 0; i < m_TextureResources_Size; ++i)
		m_TextureResources_Elements[i].m_TextureName = (char*)((unsigned int)&m_TextureResources_Elements[i] + offsetof(TextureReference, m_TextureName) + (unsigned int)m_TextureResources_Elements[i].m_TextureName);

	field_28[0] = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_28[1] = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	m_MeshList_Elements = (Mesh*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(Mesh*);

	m_MeshList_Size = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_38[0] = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_38[1] = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_40 = (unsigned int*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(unsigned int);

	m_BoundingRadius = **(vec4**)infobuffer;
	*infobuffer += sizeof(vec4);

	field_54 = **(unsigned int**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	field_58 = (unsigned int*)(*infobuffer + **(unsigned int**)infobuffer);
	*infobuffer += sizeof(unsigned int);

	field_5C = **(unsigned char**)infobuffer;
	*infobuffer += sizeof(unsigned int);

	SkipNameRead(infobuffer);
	SkipAlignment(infobuffer);
	SkipSpecificData(infobuffer);
}

void AssetBlockReader::CompiledModelAsset::PrintInfo() const
{
	CompiledAsset::PrintInfo();
}

void AssetBlockReader::CompiledModelAsset::SkipAlignment(unsigned char** infobuffer)
{
	while (**infobuffer == 0xBA || **infobuffer == 0xAD || **infobuffer == 0xF0 || **infobuffer == 0x0D)
		if (*infobuffer == (unsigned char*)m_TextureResources_Elements)
			break;
		else
			*infobuffer += (char)1;
}

void AssetBlockReader::CompiledModelAsset::SkipSpecificData(unsigned char** infobuffer)
{
	unsigned int maxstructaddr = max((unsigned int)m_TextureResources_Elements, (unsigned int)m_MeshList_Elements);
	maxstructaddr = max((unsigned int)field_40, maxstructaddr);
	maxstructaddr = max((unsigned int)field_58, maxstructaddr);

	*infobuffer = (unsigned char*)maxstructaddr;
}