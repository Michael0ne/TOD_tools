#pragma once
#include "Node.h"
#include "FrameBuffer.h"

class Fog : public Node
{
protected:
    int         m_FogType;
    float       m_FogStart;
    float       m_FogEnd;
    float       m_Density;
    ColorRGB    m_FogColor;

public:
    inline Fog::Fog() : Node(NODE_MASK_QUADTREE)
    {
        MESSAGE_CLASS_CREATED(Fog);

        m_FogStart = 100.0f;
        m_FogEnd = 1000.0f;
        m_FogType = 1;
        m_Density = 0.02f;
        m_FogColor = BuiltinType::ColorWhite;

        m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
    };

    virtual         ~Fog();
    virtual void    Render();   //  @8CCA80

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

    inline void SetPropertiesForFrameBuffer(FrameBuffer* fb, const bool blackColor)  //  @8CC9F0
    {
        switch (m_FogType)
        {
            case 1:
                fb->SubmitSetFogPropertiesCommand(0, blackColor ? BuiltinType::ColorBlack : m_FogColor, m_FogStart, m_FogEnd, m_Density);
                fb->SubmitEnableFogCommand(true);
                break;
            case 2:
                fb->SubmitSetFogPropertiesCommand(1, blackColor ? BuiltinType::ColorBlack : m_FogColor, m_FogStart, m_FogEnd, m_Density);
                fb->SubmitEnableFogCommand(true);
                break;
            case 3:
                fb->SubmitSetFogPropertiesCommand(2, blackColor ? BuiltinType::ColorBlack : m_FogColor, m_FogStart, m_FogEnd, m_Density);
                fb->SubmitEnableFogCommand(true);
                break;
            default:
                return;
        }
    }
};

extern EntityType* tFog; // @A3DFC4

ASSERT_CLASS_SIZE(Fog, 112);