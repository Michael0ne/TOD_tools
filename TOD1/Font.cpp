#include "Font.h"
#include "LogDump.h"
#include "TextAsset.h"

Font* g_Font;

uint32_t Font::CharactersDrawn = 0;
const int Font::_9B38D4 = 0x00008000;
const float Font::_9B38D8 = (float)0xFFFF7F7F;
const unsigned int Font::GlyphsColorsArray[8192] = {};
Font::Glyph Font::Glyphs[138];
Font::GlyphInfo Font::GlyphsInfo =
{
    138,
    18,
    Glyphs,
    GlyphsColorsArray
};

Font::Font(const GlyphInfo& glyphsInfo)
{
    MESSAGE_CLASS_CREATED(Font);

    field_0 = NULL;
    FontTexture = nullptr;
    GlyphsList = {};
    field_54 = NULL;
    GlyphsMap = NULL;

    if (!g_Font)
        g_Font = this;

    Color = -1;
    HorizontalSpacing = NULL;
    VerticalSpacing = 0.f;
    ScaleY = 0.f;
    field_30 = _9B38D4;
    field_8 = NULL;
    field_C = NULL;
    ScaleX = 1.f;
    field_34 = _9B38D8;

    CreateBakedFont(glyphsInfo);

    field_38 = field_38 & 0xFFFFE09A | 0x88;
    field_5C = 1.f;
    field_60 = 1.f;
}

#pragma message(TODO_IMPLEMENTATION)
void Font::CreateBakedFont(const GlyphInfo& GlyphsInfo)
{
    LogDump::LogA("Creating baked font...\n");
}

void Font::FillGlyphMapInfo()
{
    delete GlyphsMap;

    GlyphsMap = new std::map<uint16_t, Glyph*>;

    if (GlyphsList.size())
    {
        for (unsigned int i = 0; i < GlyphsList.size(); ++i)
            GlyphsMap->insert({GlyphsList[i].Character, &GlyphsList[i]});
    }
}

const float Font::GetTextWidth(const char* const text) const
{
    float overallWidth = 0.f;
    size_t i = 0;
    const size_t textLength = strlen(text);

    while (i++ < textLength)
    {
        const Glyph* glyph = GetGlyph((short)text[i]);
        if (glyph)
            overallWidth += field_54 & 4 ? ScaleX * GlyphsTotalWidth : ScaleX * glyph->Width;

        overallWidth += HorizontalSpacing;
    }

    return overallWidth;
}

const float Font::GetGameTextWidth(const unsigned short* const text) const
{
    float overallWidth = 0.f;
    size_t i = 0;
    const size_t textLength = TextAsset::GetGameStringLength(text);

    while (i++ < textLength)
    {
        const Glyph* glyph = GetGlyph(text[i]);
        if (glyph)
            overallWidth += field_54 & 4 ? ScaleX * GlyphsTotalWidth : ScaleX * glyph->Width;

        overallWidth += HorizontalSpacing;
    }

    return overallWidth;
}

const Font::Glyph* Font::GetGlyph(const uint16_t letter) const
{
    auto it = GlyphsMap->find(letter);
    if (it != GlyphsMap->end())
        return it->second;
    else
        return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void Font::GetTextDimensions(Vector4f& outDimensions, const char* const text, const float_t spriteSizeX) const
{
}

#pragma message(TODO_IMPLEMENTATION)
void Font::GetTextDimensions_A(Vector4f& outDimensions, const uint16_t* text, const float_t scaleX) const
{
}

const float Font::_41A9D0() const
{
    return ((float_t)field_4 + VerticalSpacing) * ScaleY;
}

void Font::SetColor(const uint32_t color)
{
    Color = color | 0xFF000000;
}

#pragma message(TODO_IMPLEMENTATION)
void Font::DrawText(FrameBuffer* fb, const uint16_t* text, const Vector2f& a3, const Vector2f& pos, const float_t a5, const float_t a6, const uint8_t a7)
{
}

#pragma message(TODO_IMPLEMENTATION)
void Font::MakeCharactersMap(GlyphInfo& GlyphsInfo)
{
    Font::Glyph* glyphs = GlyphsInfo.m_GlyphsInfoArray;

    glyphs[0] = { 'a', 1, 4, 10.854492, 0, 0, 0.066405997, 0 };
    glyphs[1] = { 'b', 2, 0, 10.854492, 0.070312999, 0, 0.066405997, 0 };
    glyphs[2] = { 'c', 1, 4, 10.854492, 0.140625, 0, 0.066405997 };
    glyphs[3] = { 'd', 1, 0, 10.854492, 0.21093801, 0, 0.066405997, 0 };
    glyphs[4] = { 'e', 1, 4, 10.854492, 0.28125, 0, 0.066405997, 0 };
    glyphs[5] = { 'f', 1, 0, 10.854492, 0.35156301, 0, 0.066405997, 0 };
    glyphs[6] = { 'g', 1, 4, 10.854492, 0.421875, 0, 0.066405997, 0 };
    glyphs[7] = { 'h', 2, 0, 10.854492, 0.49218801, 0, 0.066405997, 0 };
    glyphs[8] = { 'i', 2, 1, 10.854492, 0.5625, 0, 0.066405997, 0 };
    glyphs[9] = { 'j', 1, 1, 10.854492, 0.63281298, 0, 0.066405997, 0 };
    glyphs[10] = { 'k', 2, 0, 10.854492, 0.703125, 0, 0.066405997, 0 };
    glyphs[11] = { 'l', 2, 0, 10.854492, 0.77343798, 0, 0.066405997, 0 };
    glyphs[12] = { 'm', 1, 4, 10.854492, 0.84375, 0, 0.066405997, 0 };
    glyphs[13] = { 'n', 2, 4, 10.854492, 0.91406298, 0, 0.066405997, 0 };
    glyphs[14] = { 'o', 1, 4, 10.854492, 0, 0.070312999, 0.066405997, 0 };
    glyphs[15] = { 'p', 2, 4, 10.854492, 0.070312999, 0.070312999, 0.066405997, 0 };
    glyphs[16] = { 'q', 1, 4, 10.854492, 0.140625, 0.070312999, 0.066405997, 0 };
    glyphs[17] = { 'r', 3, 4, 10.854492, 0.21093801, 0.070312999, 0.066405997, 0 };
    glyphs[18] = { 's', 2, 4, 10.854492, 0.28125, 0.070312999, 0.066405997, 0 };
    glyphs[19] = { 't', 1, 3, 10.854492, 0.35156301, 0.070312999, 0.066405997, 0 };
    glyphs[20] = { 'u', 2, 4, 10.854492, 0.421875, 0.070312999, 0.066405997, 0 };
    glyphs[21] = { 'v', 1, 4, 10.854492, 0.49218801, 0.070312999, 0.066405997, 0 };
    glyphs[22] = { 'w', 0, 4, 10.854492, 0.5625, 0.070312999, 0.066405997, 0 };
    glyphs[23] = { 'x', 1, 4, 10.854492, 0.63281298, 0.070312999, 0.066405997, 0 };
    glyphs[24] = { 'y', 1, 4, 10.854492, 0.703125, 0.070312999, 0.066405997, 0 };
    glyphs[25] = { 'z', 1, 4, 10.854492, 0.77343798, 0.070312999, 0.066405997, 0 };
    glyphs[26] = { ' ', 0, 14, 10.854492, 0.84375, 0.070312999, 0.066405997, 0 };

    glyphs[27] = { 204, 2, 0, 10.854492, 0.91406298, 0.070312999, 0.066405997, 0 };

    glyphs[28] = { 'A', 0, 3, 10.854492, 0, 0.140625, 0.066405997, 0 };
    glyphs[29] = { 'B', 2, 3, 10.854492, 0.070312999, 0.140625, 0.066405997, 0 };
    // TODO: ...
}