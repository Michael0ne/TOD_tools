#pragma once
#include "Node.h"

class Fog : public Node
{
protected:
    int         m_FogType;
    float       m_FogStart;
    float       m_FogEnd;
    float       m_Density;
    ColorRGB    m_FogColor;

public:
    Fog();
    virtual ~Fog();

    static void Register(); // @8CCFB0

private:
    static Fog* Create(AllocatorIndex); // @8CD1D0

    const int   GetFogType() const; // @8F92C0
    void        SetFogType(const int fogtype); // @8CC950

    const float GetStart() const; // @89A890
    void        SetStart(const float fogstart); // @681460

    const float GetEnd() const; // @89A8A0
    void        SetEnd(float fogend); // @681480

    const float GetDensity() const; // @905800
    void        SetDensity(const float fogdensity); // @8E3730

    const ColorRGB& GetColorRGB() const; // @91CD80
    void        SetColorRGB(const ColorRGB& fogcolor); // @6814A0
};

extern EntityType* tFog; // @A3DFC4

ASSERT_CLASS_SIZE(Fog, 112);