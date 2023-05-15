#pragma once
#include "StringsPool.h"
#include "Texture.h"
#include <vector>
#include <map>

class FrameBuffer;

//  NOTE: actual class path 'libs/adapter/fontbuffer.h".
class Font
{
public:
    struct Glyph
    {
        uint16_t        Character;
        float_t         X;
        float_t         Y;
        float_t         Width;
        float_t         Height;
        float_t         X1;
        float_t         X2;
        uint32_t        field_1C;
    };

    struct GlyphInfo
    {
        unsigned int    m_TotalGlyphs;
        unsigned int    field_4;
        Glyph          *m_GlyphsInfoArray;
        const unsigned int   *m_FontColorsTable;
    };

    uint32_t            field_0;
    uint32_t            field_4;
    uint32_t            field_8;
    uint32_t            field_C;
    uint32_t            Color;
    float_t             HorizontalSpacing;
    float_t             VerticalSpacing;
    float_t             ScaleX;
    float_t             ScaleY;
    float_t             field_24;
    float_t             field_28;
    float_t             field_2C;
    float_t             field_30;
    float_t             field_34;
    uint16_t            field_38;
    uint16_t            field_3A;
    Texture            *FontTexture;
    std::vector<Glyph>  GlyphsList;
    std::map<uint16_t, Glyph*> *GlyphsMap;   //  NOTE: key - character index, value - pointer to a glyph information.
    uint32_t            field_54;
    float_t             GlyphsTotalWidth;
    float_t             field_5C;
    float_t             field_60;

public:
    Font(const GlyphInfo& GlyphsInfo);  //  @41EA80

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    void                CreateBakedFont(const GlyphInfo& GlyphsInfo);   //  @41E6D0
    void                FillGlyphMapInfo(); //  @41E4B0
    const float_t       GetTextWidth(const char* const text) const;   //  @41B0C0
    const float_t       GetGameTextWidth(const unsigned short* const text) const;  //  @41B170
    const Glyph*        GetGlyph(const uint16_t letter) const; //  @41AE90
    void                GetTextDimensions(Vector4f& outDimensions, const char* const text, const float_t spriteSizeX) const;   //  @41E200
    void                GetTextDimensions_A(Vector4f& outDimensions, const uint16_t* text, const float_t scaleX) const; //  @41D310
    const float         _41A9D0() const;    //  @41A9D0
    void                SetColor(const uint32_t color); //  @41A9B0
    void                DrawText(FrameBuffer* fb, const uint16_t* text, const Vector2f& a3, const Vector2f& pos, const float_t a5, const float_t a6, const uint8_t a7);  //  @41CE30

    static void         MakeCharactersMap(GlyphInfo& GlyphsInfo);   //  @938E10

    static const int    _9B38D4;   //  @9B38D4
    static const float  _9B38D8; //  @9B38D8
    static GlyphInfo    GlyphsInfo;    //  @A1B698
    static Glyph        Glyphs[138];    //  @A5D5C0
    static const unsigned int GlyphsColorsArray[8192];    //  @A13698
    static uint32_t     CharactersDrawn;    //  @A35DFC
};

extern Font* g_Font;

ASSERT_CLASS_SIZE(Font, 100);