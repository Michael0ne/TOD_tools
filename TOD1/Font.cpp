#include "Font.h"
#include "LogDump.h"

Font* g_Font;

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
    m_FontTexture = nullptr;
    m_GlyphsList = {};
    field_54 = NULL;
    m_GlyphsMap = NULL;

    if (!g_Font)
        g_Font = this;

    field_10 = -1;
    m_HorizontalSpacing = NULL;
    m_VerticalSpacing = 0.f;
    m_ScaleY = 0.f;
    field_30 = _9B38D4;
    field_8 = NULL;
    field_C = NULL;
    m_ScaleX = 1.f;
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
    delete m_GlyphsMap;

    m_GlyphsMap = new std::map<short, Glyph*>;

    if (m_GlyphsList.size())
    {
        for (unsigned int i = 0; i < m_GlyphsList.size(); ++i)
            m_GlyphsMap->insert({m_GlyphsList[i].m_Character, &m_GlyphsList[i]});
    }
}

#pragma message(TODO_IMPLEMENTATION)
void Font::MakeCharactersMap(GlyphInfo& GlyphsInfo)
{
    GlyphsInfo.m_GlyphsInfoArray[0] = { 'a', 1, 4, 10.854492, 0, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[1] = { 'b', 2, 0, 10.854492, 0.070312999, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[2] = { 'c', 1, 4, 10.854492, 0.140625, 0, 0.066405997 };
    GlyphsInfo.m_GlyphsInfoArray[3] = { 'd', 1, 0, 10.854492, 0.21093801, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[4] = { 'e', 1, 4, 10.854492, 0.28125, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[5] = { 'f', 1, 0, 10.854492, 0.35156301, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[6] = { 'g', 1, 4, 10.854492, 0.421875, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[7] = { 'h', 2, 0, 10.854492, 0.49218801, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[8] = { 'i', 2, 1, 10.854492, 0.5625, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[9] = { 'j', 1, 1, 10.854492, 0.63281298, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[10] = { 'k', 2, 0, 10.854492, 0.703125, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[11] = { 'l', 2, 0, 10.854492, 0.77343798, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[12] = { 'm', 1, 4, 10.854492, 0.84375, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[13] = { 'n', 2, 4, 10.854492, 0.91406298, 0, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[14] = { 'o', 1, 4, 10.854492, 0, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[15] = { 'p', 2, 4, 10.854492, 0.070312999, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[16] = { 'q', 1, 4, 10.854492, 0.140625, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[17] = { 'r', 3, 4, 10.854492, 0.21093801, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[18] = { 's', 2, 4, 10.854492, 0.28125, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[19] = { 't', 1, 3, 10.854492, 0.35156301, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[20] = { 'u', 2, 4, 10.854492, 0.421875, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[21] = { 'v', 1, 4, 10.854492, 0.49218801, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[22] = { 'w', 0, 4, 10.854492, 0.5625, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[23] = { 'x', 1, 4, 10.854492, 0.63281298, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[24] = { 'y', 1, 4, 10.854492, 0.703125, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[25] = { 'z', 1, 4, 10.854492, 0.77343798, 0.070312999, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[26] = { ' ', 0, 14, 10.854492, 0.84375, 0.070312999, 0.066405997, 0 };

    GlyphsInfo.m_GlyphsInfoArray[27] = { 204, 2, 0, 10.854492, 0.91406298, 0.070312999, 0.066405997, 0 };

    GlyphsInfo.m_GlyphsInfoArray[28] = { 'A', 0, 3, 10.854492, 0, 0.140625, 0.066405997, 0 };
    GlyphsInfo.m_GlyphsInfoArray[29] = { 'B', 2, 3, 10.854492, 0.070312999, 0.140625, 0.066405997, 0 };
    // TODO: ...
}