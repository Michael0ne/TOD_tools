#pragma once
#include "MemoryManager.h"
#include "Types.h"

class GfxInternal_Dx9_Surface
{
public:
	unsigned int	m_BitsPerPixel;
	int*			field_4;
	unsigned int	m_Width;
	unsigned int	m_Height;
	unsigned int	m_WidthPowerOf2;
	unsigned int	m_HeightPowerOf2;
	char*			m_ColorPallette;
	char*			m_Canvas;

private:
	void			SetupSurface(unsigned int width, unsigned int height);	//	@42FE00
	void			CreateCanvasBuffer();	//	@42FD80

public:
	GfxInternal_Dx9_Surface(unsigned int width, unsigned int height);	//	@430450

	void* operator new (size_t size)
	{
		return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
	}
	void operator delete(void* ptr)
	{
		if (ptr)
			MemoryManager::ReleaseMemory(ptr, 0);
		ptr = nullptr;
	}

	void			SetPixelColor(unsigned int x, unsigned int y, const ColorRGB& clr);	//	@42F8A0
};
