#include "Surface.h"
#include "LogDump.h"

void Surface::SetupSurface(unsigned int width, unsigned int height)
{
	m_ImageWidth = width;
	m_ImageHeight = height;
	m_TextureWidth = 1;
	m_TextureHeight = 1;

	if (width > 1)
		do
			m_TextureWidth = m_TextureWidth * 2;
	while (m_TextureWidth < m_ImageWidth);

	if (height > 1)
		do
			m_TextureHeight = m_TextureHeight * 2;
	while (m_TextureHeight < m_ImageHeight);

	unsigned int totalColors = m_TextureWidth * m_BitsPerPixel * m_TextureHeight + 7;
	totalColors += (totalColors >> 31) & 7;
	totalColors = totalColors >> 3;

	m_SurfacePixels = new char[totalColors];
	memset(m_SurfacePixels, NULL, totalColors);
}

void Surface::CreateCanvasBuffer()
{
	m_Canvas = new char[1024];
	ZeroMemory(m_Canvas, sizeof(m_Canvas));
}

Surface::Surface(unsigned int width, unsigned int height)
{
	MESSAGE_CLASS_CREATED(Surface);

	m_SurfacePixels = nullptr;
	m_Canvas = nullptr;
	m_TextureWidth = NULL;
	m_TextureHeight = NULL;
	m_ImageWidth = NULL;
	m_ImageHeight = NULL;
	field_4 = NULL;
	m_BitsPerPixel = 32;

	SetupSurface(width, height);
}

Surface::~Surface()
{
	MESSAGE_CLASS_DESTROYED(Surface);

	delete m_Canvas;
	delete m_SurfacePixels;
}

void Surface::SetPixelColor(const unsigned int x, const unsigned int y, const ColorRGB& clr)
{
	// TODO: is this conversion correct?
	unsigned int i = 4 * (x + y * m_TextureWidth);
	m_SurfacePixels[i++] = (char)(clr.a * 255.f);
	m_SurfacePixels[i++] = (char)(clr.r * 255.f);
	m_SurfacePixels[i++] = (char)(clr.g * 255.f);
	m_SurfacePixels[i] = (char)(clr.b * 255.f);
}

void Surface::SetPixelColor(const unsigned int x, const unsigned int y, const char clr)
{
	if (m_BitsPerPixel == 8)
		m_SurfacePixels[m_TextureWidth * y + x] = clr;

	if (m_BitsPerPixel == 4)
	{
		int odd = (m_TextureWidth * y + x) & 0x80000001;

		if (odd < 0)
			odd = (((char)odd - 1) | 0xFFFFFFFE) == -1;
		else
			odd = odd == 0;

		char* const clr_current = &m_SurfacePixels[(m_TextureWidth * y + x) / 2];
		if (odd)
			*clr_current = clr | *clr_current & 240;
		else
			*clr_current = (16 * clr) | *clr_current & 15;
	}
}

#pragma message(TODO_IMPLEMENTATION)
void Surface::ConvertColorsToPS2()
{
	if (m_BitsPerPixel == 32)
	{
		LogDump::LogA("NOTE: Converting a 32bit image to PS2 colors - this might be slow!!\n");
		if (m_ImageWidth > 0)
		{
			for (unsigned int x = 0; x < m_ImageWidth; ++x)
				for (unsigned int y = 0; y < m_ImageHeight; y++)
				{
					// TODO: go through 'ColorPallette' and convert colors.
				}
		}
	}
	else
	{
		for (unsigned int j = 0; j < (unsigned int)field_4; ++j)
		{
			// TODO: go through 'Canvas' and convert colors.
		}
	}
}

unsigned int Surface::GetTotalSurfaceColors() const
{
	return (m_BitsPerPixel * m_TextureWidth * m_TextureHeight + 7) / 8;
}

#pragma message(TODO_IMPLEMENTATION)
void Surface::ScaleImage(const unsigned int width, const unsigned int height)
{
	if (width == m_TextureWidth || height == m_TextureHeight)
		return;

	if (width > m_TextureWidth || height > m_TextureHeight)
	{
		LogDump::LogA("WARNING: Trying to scale an image UP! Ignored...\n");
		return;
	}

	char* canvasbuffer = new char[width * height * 4];

	// TODO: actually do the conversion here.

	delete[] canvasbuffer;
}

ColorRGB Surface::GetPixelColor(const unsigned int x, const unsigned int y) const
{
	constexpr float flt255 = (float)(1 / 255);
	const unsigned int offset = m_BitsPerPixel == 32 ? (m_TextureWidth * y + x) * 4 : GetPixelColor_A(x, y) * 4;

	return
	{
	 (float)m_SurfacePixels[offset + 1] * flt255,
	 (float)m_SurfacePixels[offset + 2] * flt255,
	 (float)m_SurfacePixels[offset + 3] * flt255,
	 (float)m_SurfacePixels[offset]     * flt255
	};
}

unsigned char Surface::GetPixelColor_A(const unsigned int x, const unsigned int y) const
{
	if (m_BitsPerPixel == 8)
		return m_SurfacePixels[m_TextureWidth * y + x];

	if (m_BitsPerPixel == 4)
	{
		int odd = (m_TextureWidth * y + x) & 0x80000001;

		if (odd < 0)
			odd = (((char)odd - 1) | 0xFFFFFFFE) == -1;
		else
			odd = odd == 0;

		if (odd)
			return m_SurfacePixels[(m_TextureWidth * y + x) / 2] & 15;
		else
			return m_SurfacePixels[(m_TextureWidth * y + x) / 2] >> 4;
	}

	return NULL;
}

void Surface::SetSurfaceColorPalette(const unsigned int colors)
{
	if (colors == 256)
		m_BitsPerPixel = 8;
	if (colors == 16)
		m_BitsPerPixel = 4;

	CreateCanvasBuffer();

	unsigned int storagesize = m_BitsPerPixel * m_TextureWidth * m_TextureHeight + 7;
	storagesize = ((*((char*)&storagesize + 1) & 7) + storagesize) >> 3;

	delete[] m_SurfacePixels;
	m_SurfacePixels = new char[storagesize];

	memset(m_SurfacePixels, NULL, storagesize);
}

void Surface::CopyCanvas(const char* const canvas, const unsigned int canvassize, int* const a3)
{
	field_4 = a3;
	memcpy(m_Canvas, canvas, canvassize);
}