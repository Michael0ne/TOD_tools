#include "GfxInternal_Dx9_Surface.h"

void GfxInternal_Dx9_Surface::SetupSurface(unsigned int width, unsigned int height)
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

void GfxInternal_Dx9_Surface::CreateCanvasBuffer()
{
	m_Canvas = new char[1024];
	ZeroMemory(m_Canvas, sizeof(m_Canvas));
}

GfxInternal_Dx9_Surface::GfxInternal_Dx9_Surface(unsigned int width, unsigned int height)
{
	MESSAGE_CLASS_CREATED(GfxInternal_Dx9_Surface);

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

void GfxInternal_Dx9_Surface::SetPixelColor(unsigned int x, unsigned int y, const ColorRGB& clr)
{
	//	TODO: is this conversion correct?
	unsigned int i = 4 * (x + y * m_WidthPowerOf2);
	m_ColorPallette[i++]	= (char)(clr.a * 255.f);
	m_ColorPallette[i++]	= (char)(clr.r * 255.f);
	m_ColorPallette[i++]	= (char)(clr.g * 255.f);
	m_ColorPallette[i]		= (char)(clr.b * 255.f);
}