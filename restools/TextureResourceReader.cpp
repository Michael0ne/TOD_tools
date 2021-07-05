#include "TextureResourceReader.h"
#include <EasyBMP.h>

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

// unsigned long PackRGBA(): Helper method that packs RGBA channels into a single 4 byte pixel.
//
// unsigned char r:     red channel.
// unsigned char g:     green channel.
// unsigned char b:     blue channel.
// unsigned char a:     alpha channel.

unsigned long PackRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

// void DecompressBlockDXT1(): Decompresses one block of a DXT1 texture and stores the resulting pixels at the appropriate offset in 'image'.
//
// unsigned long x:                     x-coordinate of the first pixel in the block.
// unsigned long y:                     y-coordinate of the first pixel in the block.
// unsigned long width:                 width of the texture being decompressed.
// unsigned long height:                height of the texture being decompressed.
// const unsigned char *blockStorage:   pointer to the block to decompress.
// unsigned long *image:                pointer to image where the decompressed pixel data should be stored.

void DecompressBlockDXT1(unsigned long x, unsigned long y, unsigned long width, const unsigned char* blockStorage, unsigned long* image)
{
	unsigned short color0 = *reinterpret_cast<const unsigned short*>(blockStorage);
	unsigned short color1 = *reinterpret_cast<const unsigned short*>(blockStorage + 2);

	unsigned long temp;

	temp = (color0 >> 11) * 255 + 16;
	unsigned char r0 = (unsigned char)((temp / 32 + temp) / 32);
	temp = ((color0 & 0x07E0) >> 5) * 255 + 32;
	unsigned char g0 = (unsigned char)((temp / 64 + temp) / 64);
	temp = (color0 & 0x001F) * 255 + 16;
	unsigned char b0 = (unsigned char)((temp / 32 + temp) / 32);

	temp = (color1 >> 11) * 255 + 16;
	unsigned char r1 = (unsigned char)((temp / 32 + temp) / 32);
	temp = ((color1 & 0x07E0) >> 5) * 255 + 32;
	unsigned char g1 = (unsigned char)((temp / 64 + temp) / 64);
	temp = (color1 & 0x001F) * 255 + 16;
	unsigned char b1 = (unsigned char)((temp / 32 + temp) / 32);

	unsigned long code = *reinterpret_cast<const unsigned long*>(blockStorage + 4);

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			unsigned long finalColor = 0;
			unsigned char positionCode = (code >> 2 * (4 * j + i)) & 0x03;

			if (color0 > color1)
			{
				switch (positionCode)
				{
				case 0:
					finalColor = PackRGBA(r0, g0, b0, 255);
					break;
				case 1:
					finalColor = PackRGBA(r1, g1, b1, 255);
					break;
				case 2:
					finalColor = PackRGBA((2 * r0 + r1) / 3, (2 * g0 + g1) / 3, (2 * b0 + b1) / 3, 255);
					break;
				case 3:
					finalColor = PackRGBA((r0 + 2 * r1) / 3, (g0 + 2 * g1) / 3, (b0 + 2 * b1) / 3, 255);
					break;
				}
			}
			else
			{
				switch (positionCode)
				{
				case 0:
					finalColor = PackRGBA(r0, g0, b0, 255);
					break;
				case 1:
					finalColor = PackRGBA(r1, g1, b1, 255);
					break;
				case 2:
					finalColor = PackRGBA((r0 + r1) / 2, (g0 + g1) / 2, (b0 + b1) / 2, 255);
					break;
				case 3:
					finalColor = PackRGBA(0, 0, 0, 255);
					break;
				}
			}

			if (x + i < width)
				image[(y + j) * width + (x + i)] = finalColor;
		}
	}
}

// void BlockDecompressImageDXT1(): Decompresses all the blocks of a DXT1 compressed texture and stores the resulting pixels in 'image'.
//
// unsigned long width:                 Texture width.
// unsigned long height:                Texture height.
// const unsigned char *blockStorage:   pointer to compressed DXT1 blocks.
// unsigned long *image:                pointer to the image where the decompressed pixels will be stored.

void BlockDecompressImageDXT1(unsigned long width, unsigned long height, const unsigned char* blockStorage, unsigned long* image)
{
	unsigned long blockCountX = (width + 3) / 4;
	unsigned long blockCountY = (height + 3) / 4;
	unsigned long blockWidth = (width < 4) ? width : 4;
	unsigned long blockHeight = (height < 4) ? height : 4;

	for (unsigned long j = 0; j < blockCountY; j++)
	{
		for (unsigned long i = 0; i < blockCountX; i++) DecompressBlockDXT1(i * 4, j * 4, width, blockStorage + i * 8, image);
		blockStorage += blockCountX * 8;
	}
}

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
	const unsigned char* rawclrs_end = rawclrs + m_GfxTexture->GetSizeForLevel(1);
	const unsigned int width = m_GfxTexture->GetSize(1).w;
	const unsigned int height = m_GfxTexture->GetSize(1).h;

	BMP bmImage;
	bmImage.SetSize(width, height);
	bmImage.SetBitDepth(m_BitsPerPixel);

	for (unsigned y = 0; y < height; y += 8)
		for (unsigned x = 0; x < width; x += 8)
	{
		DXT1Texture	dxt1tex = *(DXT1Texture*)rawclrs;

		bmImage(x, y)->Alpha = NULL;

		//	Blue color
		bmImage(x, y)->Blue =		dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._00].m_ColorBits.Blue;
		bmImage(x + 1, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._01].m_ColorBits.Blue;
		bmImage(x + 2, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._02].m_ColorBits.Blue;
		bmImage(x + 3, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._03].m_ColorBits.Blue;

		bmImage(x, y)->Blue =		dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._10].m_ColorBits.Blue;
		bmImage(x, y + 1)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._11].m_ColorBits.Blue;
		bmImage(x, y + 2)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._12].m_ColorBits.Blue;
		bmImage(x, y + 3)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._13].m_ColorBits.Blue;

		bmImage(x + 4, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._20].m_ColorBits.Blue;
		bmImage(x + 5, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._21].m_ColorBits.Blue;
		bmImage(x + 6, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._22].m_ColorBits.Blue;
		bmImage(x + 7, y)->Blue =	dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._23].m_ColorBits.Blue;

		bmImage(x + 4, y + 4)->Blue = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._30].m_ColorBits.Blue;
		bmImage(x + 5, y + 5)->Blue = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._31].m_ColorBits.Blue;
		bmImage(x + 6, y + 6)->Blue = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._32].m_ColorBits.Blue;
		bmImage(x + 7, y + 7)->Blue = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._33].m_ColorBits.Blue;

		//	Green color
		bmImage(x, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._00].GetNormalGreenColor();
		bmImage(x + 1, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._01].GetNormalGreenColor();
		bmImage(x + 2, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._02].GetNormalGreenColor();
		bmImage(x + 3, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._03].GetNormalGreenColor();

		bmImage(x, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._10].GetNormalGreenColor();
		bmImage(x, y + 1)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._11].GetNormalGreenColor();
		bmImage(x, y + 2)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._12].GetNormalGreenColor();
		bmImage(x, y + 3)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._13].GetNormalGreenColor();

		bmImage(x + 4, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._20].GetNormalGreenColor();
		bmImage(x + 5, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._21].GetNormalGreenColor();
		bmImage(x + 6, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._22].GetNormalGreenColor();
		bmImage(x + 7, y)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._23].GetNormalGreenColor();

		bmImage(x + 4, y + 4)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._30].GetNormalGreenColor();
		bmImage(x + 5, y + 5)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._31].GetNormalGreenColor();
		bmImage(x + 6, y + 6)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._32].GetNormalGreenColor();
		bmImage(x + 7, y + 7)->Green = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._33].GetNormalGreenColor();

		//	Red color
		bmImage(x, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._00].m_ColorBits.Red;
		bmImage(x + 1, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._01].m_ColorBits.Red;
		bmImage(x + 2, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._02].m_ColorBits.Red;
		bmImage(x + 3, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._03].m_ColorBits.Red;

		bmImage(x, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._10].m_ColorBits.Red;
		bmImage(x, y + 1)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._11].m_ColorBits.Red;
		bmImage(x, y + 2)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._12].m_ColorBits.Red;
		bmImage(x, y + 3)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._13].m_ColorBits.Red;

		bmImage(x + 4, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._20].m_ColorBits.Red;
		bmImage(x + 5, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._21].m_ColorBits.Red;
		bmImage(x + 6, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._22].m_ColorBits.Red;
		bmImage(x + 7, y)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._23].m_ColorBits.Red;

		bmImage(x + 4, y + 4)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._30].m_ColorBits.Red;
		bmImage(x + 5, y + 5)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._31].m_ColorBits.Red;
		bmImage(x + 6, y + 6)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._32].m_ColorBits.Red;
		bmImage(x + 7, y + 7)->Red = dxt1tex.ColorArr[dxt1tex.WordArr.m_WordBits._33].m_ColorBits.Red;

		rawclrs += sizeof(DXT1Texture);
	}

	bmImage.WriteToFile(outfilename.c_str());
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