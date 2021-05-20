#include "Surface.h"
#include "LogDump.h"

void Surface::SetupSurface(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;
	m_WidthPowerOf2 = 1;
	m_HeightPowerOf2 = 1;

	if (width > 1)
		do
			m_WidthPowerOf2 = m_WidthPowerOf2 * 2;
		while (m_WidthPowerOf2 < m_Width);

	if (height > 1)
		do
			m_HeightPowerOf2 = m_HeightPowerOf2 * 2;
		while (m_HeightPowerOf2 < m_Height);

	unsigned int totalColors = m_WidthPowerOf2 * m_BitsPerPixel * m_HeightPowerOf2 + 7;
	totalColors += (totalColors >> 31) & 7;
	totalColors = totalColors >> 3;

	m_ColorPallette = new char[totalColors];
	memset(m_ColorPallette, NULL, totalColors);
}

void Surface::CreateCanvasBuffer()
{
	m_Canvas = new char[1024];
	ZeroMemory(m_Canvas, sizeof(m_Canvas));
}

Surface::Surface(unsigned int width, unsigned int height)
{
	MESSAGE_CLASS_CREATED(Surface);

	m_ColorPallette = nullptr;
	m_Canvas = nullptr;
	m_WidthPowerOf2 = NULL;
	m_HeightPowerOf2 = NULL;
	m_Width = NULL;
	m_Height = NULL;
	field_4 = NULL;
	m_BitsPerPixel = 32;

	SetupSurface(width, height);
}

Surface::~Surface()
{
	MESSAGE_CLASS_DESTROYED(Surface);

	delete m_Canvas;
	delete m_ColorPallette;
}

void Surface::SetPixelColor(unsigned int x, unsigned int y, const ColorRGB& clr)
{
	//	TODO: is this conversion correct?
	unsigned int i = 4 * (x + y * m_WidthPowerOf2);
	m_ColorPallette[i++]	= (char)(clr.a * 255.f);
	m_ColorPallette[i++]	= (char)(clr.r * 255.f);
	m_ColorPallette[i++]	= (char)(clr.g * 255.f);
	m_ColorPallette[i]		= (char)(clr.b * 255.f);
}

void Surface::ConvertColorsToPS2()
{
	if (m_BitsPerPixel == 32)
	{
		LogDump::LogA("NOTE: Converting a 32bit image to PS2 colors - this might be slow!!\n");
		if (m_Width > 0)
		{
			for (unsigned int x = 0; x < m_Width; ++x)
				for (unsigned int y = 0; y < m_Height; y++)
				{
					//	TODO: go through 'ColorPallette' and convert colors.
				}
		}
	}
	else
	{
		for (unsigned int j = 0; j < (unsigned int)field_4; ++j)
		{
			//	TODO: go through 'Canvas' and convert colors.
		}
	}
}

unsigned int Surface::GetTotalSurfaceColors() const
{
	return (m_BitsPerPixel * m_WidthPowerOf2 * m_HeightPowerOf2 + 7) / 8;
}