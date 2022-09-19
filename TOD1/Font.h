#pragma once
#include "StringsPool.h"
#include "Texture.h"
#include <vector>
#include <map>

//  NOTE: actual class path 'libs/adapter/fontbuffer.h".
class Font
{
public:
    struct Glyph
    {
        unsigned short  m_Character;
        float           m_X;
        float           m_Y;
        float           m_Width;
        float           m_Height;
        float           m_X1;
        float           m_X2;
        int             field_1C;
    };

    struct GlyphInfo
    {
        unsigned int    m_TotalGlyphs;
        unsigned int    field_4;
        Glyph          *m_GlyphsInfoArray;
        const unsigned int   *m_FontColorsTable;
    };

    int                 field_0;
    int                 field_4;
    int                 field_8;
    int                 field_C;
    int                 field_10;
    float               m_HorizontalSpacing;
    float               m_VerticalSpacing;
    float               m_ScaleX;
    float               m_ScaleY;
    float               field_24;
    float               field_28;
    float               field_2C;
    int                 field_30;
    float               field_34;
    short               field_38;
    short               field_3A;
    Texture            *m_FontTexture;
    std::vector<Glyph>  m_GlyphsList;
    std::map<short, Glyph*> *m_GlyphsMap;   //  NOTE: key - character index, value - pointer to a glyph information.
    int                 field_54;
    float               m_GlyphsTotalWidth;
    float               field_5C;
    float               field_60;

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
    const float         GetTextWidth(const char* const text) const;   //  @41B0C0
    const float         GetGameTextWidth(const unsigned short* const text) const;  //  @41B170
    const Glyph*        GetGlyph(const short letter) const; //  @41AE90
    void                GetTextDimensions(Vector4f& outDimensions, const char* const text, const float spriteSizeX) const;   //  @41E200

    static void         MakeCharactersMap(GlyphInfo& GlyphsInfo);   //  @938E10

    static const int    _9B38D4;   //  @9B38D4
    static const float  _9B38D8; //  @9B38D8
    static GlyphInfo    GlyphsInfo;    //  @A1B698
    static Glyph        Glyphs[138];    //  @A5D5C0
    static const unsigned int GlyphsColorsArray[8192];    //  @A13698
};

extern Font* g_Font;

ASSERT_CLASS_SIZE(Font, 100);