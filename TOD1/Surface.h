#pragma once
#include "MemoryManager.h"
#include "Types.h"

class Surface
{
public:
    unsigned char   m_BitsPerPixel;
    int            *field_4;
    unsigned int    m_ImageWidth;
    unsigned int    m_ImageHeight;
    unsigned int    m_TextureWidth;
    unsigned int    m_TextureHeight;
    char           *m_SurfacePixels;    //  NOTE: format ARGB.
    char           *m_Canvas;

private:
    void            SetupSurface(unsigned int width, unsigned int height);	//	@42FE00
    void            CreateCanvasBuffer();	//	@42FD80

public:
    Surface(unsigned int width, unsigned int height);	//	@430450
    ~Surface(); //  @42FA40

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

    void            SetPixelColor(const unsigned int x, const unsigned int y, const ColorRGB& clr);	//	@42F8A0
    void            ConvertColorsToPS2();   //  @42FBA0
    unsigned int    GetTotalSurfaceColors() const;  //  @42F820
    void            ScaleImage(const unsigned int width, const unsigned int height);    //  @42FFF0
    ColorRGB        GetPixelColor(const unsigned int x, const unsigned int y) const;  //  @42FA80
    unsigned char   GetPixelColor_A(const unsigned int x, const unsigned int y) const;    //  @42F920
    void            SetSurfaceColorPalette(const unsigned int colors);  //  @430290
    void            CopyCanvas(const char* const canvas, const unsigned int canvassize, int* const a3); //  @42FA10
    void            SetPixelColor(const unsigned int x, const unsigned int y, const char clr);  //  @42F990
};
