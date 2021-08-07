#pragma once
#include "MemoryManager.h"
#include "Surface.h"
#include <vector>

struct MutableTextureBuffer
{
    unsigned char               field_0;
    std::vector<Surface*>       m_Surfaces;

    MutableTextureBuffer(Surface* surf);  //  @424470
    MutableTextureBuffer(const MutableTextureBuffer& rhs);  //  @4243D0
    MutableTextureBuffer();  //  @424110  //  NOTE: unused.
    ~MutableTextureBuffer(); //  @424140

    void						EmplaceSurfaceAtIndex(Surface* surf, const unsigned int ind);   //  @4242D0
    Surface*                    GetSurfaceByIndex(const unsigned int ind) const; //  @423D80
    ScreenResolution&           GetImageResolution(ScreenResolution& resolution, const int surfaceindex) const;    //  @423D60
    ScreenResolution&           GetTextureResolution(ScreenResolution& resolution, const int surfaceindex) const;    //  @423D40
    void                        ReduceColors(const int colors); //  @423DA0
    void                        ConvertSurfacesToPS2Colors();   //  @423D10
    char*                       GetColorPalletteForSurfaceByIndex(const unsigned int ind) const;    //  @4240B0
    const unsigned int          GetTotalColorsForSurfaceByIndex(const unsigned int ind) const;  //  @4240E0
    void                        CalculateMipMaps(const unsigned int surfaceindex);  //  @4244B0
};

ASSERT_CLASS_SIZE(MutableTextureBuffer, 20);