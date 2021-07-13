#include "TextureResourceReader.h"
#include <bitmap.h>

const std::string TextureResourceReader::PlatformExtension[] =
{
	"",
	".texture_pc",
	".texture_ps2",
	".texture_xbox"
};

const char* const TextureResourceReader::AssetTypeString[] =
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

const char* const TextureResourceReader::TextureFormatString[] =
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

void TextureResourceReader::ReadInfo()
{
	if (!m_FilePtr ||
		m_LastErrorCode)
		return;

	fread(&m_Header.m_Timestamp, sizeof(m_Header.m_Timestamp), 1, m_FilePtr);
	fread(&m_Header.m_AssetStructSize, sizeof(m_Header.m_AssetStructSize), 1, m_FilePtr);
	fread(&m_Header.m_AssetDataSize, sizeof(m_Header.m_AssetDataSize), 1, m_FilePtr);
	fread(&m_Header.m_IsLocalised, 4, 1, m_FilePtr);
	fread(&m_Header.m_LocalisationStringLength, sizeof(m_Header.m_LocalisationStringLength), 1, m_FilePtr);
	
	if (m_Header.m_LocalisationStringLength)
	{
		m_Header.m_LocalisationString = new char[m_Header.m_LocalisationStringLength];
		fread(m_Header.m_LocalisationString, m_Header.m_LocalisationStringLength, 1, m_FilePtr);
	}

	m_AssetStruct = new char[m_Header.m_AssetStructSize];
	fread(m_AssetStruct, m_Header.m_AssetStructSize, 1, m_FilePtr);

	if (m_Header.m_AssetDataSize)
	{
		m_AssetData = new char[m_Header.m_AssetDataSize];
		fread(m_AssetData, m_Header.m_AssetDataSize, 1, m_FilePtr);
	}
}

void TextureResourceReader::PrintInfo() const
{
	printf("\tAsset data size:\t%d\n", m_Header.m_AssetDataSize);

	char* info = m_AssetStruct;
	printf("\tAsset type:\t%s (%d)\n", AssetTypeString[*(int*)info], *(int*)info);
	info += 4;
	printf("\tResource path:\t\"%s\"\n", (const char*)((int)info + *(int*)info) );
	info += 4;
	printf("\tGlobal ID:\t%d\n", *(int*)info);
	info += 4;
	printf("\tfield_C:\t%d\n", *(int*)info);
	info += 4;
	printf("\tEngine timestamp:\t%lld\n", *(UINT64*)info);
	info += sizeof(UINT64);
	printf("\tFlags:\t0x%x\n", *(int*)info);
	info += 4;
	printf("\tBits per pixel:\t%d\n", *(int*)info);
	m_BitsPerPixel = *(unsigned int*)info;
	info += 4;
	m_TextureInfo = (TextureInfo*)((int)info + *(int*)info);
	info += 4;
	printf("\tfield_24:\t%d\n", *(char*)info);
	info += 4;
	m_GfxTexture = (GfxTexture*)((int)info + *(int*)info);
	info += 4;
	printf("\tfield_2C:\t0x%X\n", *(int*)info);

	if ((int)m_GfxTexture->m_Mutable == NULL)
		m_GfxTexture->m_Mutable = nullptr;
	else
		m_GfxTexture->m_Mutable = (SurfaceMutable*)((int)m_GfxTexture + offsetof(GfxTexture, m_Mutable) + (int)m_GfxTexture->m_Mutable);

	if ((int)m_GfxTexture->m_Texture == NULL)
		m_GfxTexture->m_Texture = nullptr;
	else
		m_GfxTexture->m_Texture = (IDirect3DTexture9*)((int)m_GfxTexture + offsetof(GfxTexture, m_Texture) + (int)m_GfxTexture->m_Texture);

	if ((int)m_GfxTexture->m_TextureSurfaceBits == NULL)
		m_GfxTexture->m_TextureSurfaceBits = nullptr;
	else
		m_GfxTexture->m_TextureSurfaceBits = (char*)((int)m_GfxTexture + offsetof(GfxTexture, m_TextureSurfaceBits) + (int)m_GfxTexture->m_TextureSurfaceBits);

	printf("\tMutable ptr:\t0x%X\n", m_GfxTexture->m_Mutable);
	printf("\tTexture ptr:\t0x%X\n", m_GfxTexture->m_Texture);
	printf("\tSurface bits:\t0x%X\n", m_GfxTexture->m_TextureSurfaceBits);
	printf("\tResolution:\t%dx%d\n", m_GfxTexture->m_Resolution[0], m_GfxTexture->m_Resolution[1]);
	printf("\tSurface size:\t%dx%d\n", m_GfxTexture->m_SurfaceSize[0], m_GfxTexture->m_SurfaceSize[1]);
	printf("\tFormat:\t%s\n", TextureFormatString[m_GfxTexture->m_Format]);
	printf("\tfield_20:\t0x%x\n", m_GfxTexture->field_20);
	printf("\tfield_21:\t0x%x\n", m_GfxTexture->field_21);
	printf("\tBit depth:\t%d per pixel (%d bytes)\n", m_GfxTexture->m_BitsPerPixel, m_GfxTexture->m_BitsPerPixel / 8);
	printf("\tfield_23:\t0x%x\n", m_GfxTexture->field_23);
	printf("\tLevels:\t%d\n", m_GfxTexture->m_Levels & 510);

	unsigned char level = 0;
	do
	{
		printf("\tTexture size for level %d:\t%d bytes\n", level, m_GfxTexture->GetSizeForLevel(level));
		level++;
	} while (level < (unsigned char)(m_GfxTexture->m_Levels >> 1));

	printf("\tMip map levels:\t%d\n", m_GfxTexture->m_MipMapLevels);
}

void TextureResourceReader::DumpData() const
{
	//	TODO: filename is fucked up, maybe change it sometime so this is not necessary.
	std::string outfilename = m_ResourceName;
	outfilename = outfilename.substr(0, outfilename.find('.', 0));
	outfilename += ".bmp";

	unsigned char* rawclrs = (unsigned char*)m_AssetData;
	unsigned char* convclrs = new unsigned char[m_GfxTexture->GetSizeForLevel(0)];

	const unsigned int width = m_GfxTexture->GetSize(0).w;
	const unsigned int height = m_GfxTexture->GetSize(0).h;

	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth = width;
	bmpinfo.bmiHeader.biHeight = height;
	bmpinfo.bmiHeader.biPlanes = 0;
	bmpinfo.bmiHeader.biBitCount = m_GfxTexture->m_BitsPerPixel;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = m_GfxTexture->GetSizeForLevel(0);
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biClrImportant = 0;

	SaveDIBitmap(outfilename.c_str(), &bmpinfo, convclrs);

	delete[] convclrs;
	printf("Saved dump: %s -> %s\n", m_ResourceName.c_str(), outfilename.c_str());
}

TextureResourceReader::TextureResourceReader(const char* const filename, PlatformDefinition platform)
{
	m_ResourceExtension = PlatformExtension[platform];
	m_ResourceName = filename;
	m_ResourceName += m_ResourceExtension;
}

TextureResourceReader::~TextureResourceReader()
{
	delete[] m_Header.m_LocalisationString;
	delete[] m_AssetStruct;
	delete[] m_AssetData;
}

unsigned int TextureResourceReader::GfxTexture::GetSizeForLevel(const unsigned char level) const
{
	if (!(m_Levels & 1))
		return (GetSize(level).w * GetSize(level).h * (m_BitsPerPixel & 63)) >> 3;
	else
		return 8 * GetSize(level).w * GetSize(level).h;
}

const TextureResourceReader::vec2 TextureResourceReader::GfxTexture::GetSize(const unsigned char level) const
{
	if (!(m_Levels & 1))
		return { m_SurfaceSize[0] >> level, m_SurfaceSize[1] >> level };
	else
		return { (m_SurfaceSize[0] >> level) / 4, (m_SurfaceSize[1] >> level) / 4 };
}