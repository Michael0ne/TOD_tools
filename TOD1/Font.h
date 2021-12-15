#pragma once
#include "StringsPool.h"
#include "Texture.h"
#include <vector>

class Font
{
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
        Glyph          *m_ActualGlyphsArray;
        void           *field_C;
    };

    struct Fld50
    {
        int             field_0;
        int            *field_4;
        int            *field_8;
        int            *field_C;
    };

private:
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
    int                 field_38;
    Texture            *m_FontTexture;
    std::vector<Glyph>  m_GlyphsList;
    Fld50              *field_50;   //  NOTE: this is a pointer to some sort of list.
    int                 field_54;
    float               field_58;
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

    static void         MakeCharactersMap(GlyphInfo& GlyphsInfo);   //  @938E10

    static const int    _9B38D4;   //  @9B38D4
    static const float  _9B38D8; //  @9B38D8
    static GlyphInfo    GlyphsInfo;    //  @A1B698
    static Glyph        Glyphs[138];    //  @A5D5C0
};

extern Font* g_Font;

ASSERT_CLASS_SIZE(Font, 100);