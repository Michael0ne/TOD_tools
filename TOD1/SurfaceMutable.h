#pragma once
#include "MemoryManager.h"
#include "Surface.h"
#include <vector>

struct SurfaceMutable
{
    unsigned char               field_0;
    std::vector<Surface*>       m_Surfaces;

    SurfaceMutable(Surface* surf);  //  @424470
    SurfaceMutable(const SurfaceMutable& rhs);  //  @4243D0
    SurfaceMutable();  //  @424110  //  NOTE: unused.
    ~SurfaceMutable(); //  @424140

    void						EmplaceSurfaceAtIndex(Surface* surf, int ind);   //  @4242D0
    Surface*    GetSurfaceByIndex(const int ind) const; //  @423D80
    ScreenResolution&           GetSurfaceResolution(ScreenResolution& resolution, const int ind) const;    //  @423D60
    ScreenResolution&           GetSurfaceResolutionNormalised(ScreenResolution& resolution, const int ind) const;    //  @423D40
    void                        ReduceColors(const int colors); //  @423DA0
    void                        ConvertSurfacesToPS2Colors();   //  @423D10
    char*                       GetColorPalletteForSurfaceByIndex(const unsigned int ind) const;    //  @4240B0
    const unsigned int          GetTotalColorsForSurfaceByIndex(const unsigned int ind) const;  //  @4240E0
};

ASSERT_CLASS_SIZE(SurfaceMutable, 20);