#include "Font.h"
#include "LogDump.h"

Font* g_Font;

const int Font::_9B38D4 = 0x00008000;
const float Font::_9B38D8 = (float)0xFFFF7F7F;
Font::Glyph Font::Glyphs[138];
Font::GlyphInfo Font::GlyphsInfo =
{
	138, 18, Glyphs, nullptr
};

Font::Font(const GlyphInfo& glyphsInfo)
{
	MESSAGE_CLASS_CREATED(Font);

	field_0 = NULL;
	field_3C = NULL;
	m_GlyphsList = {};
	field_54 = NULL;
	field_50 = NULL;

	if (!g_Font)
		g_Font = this;

	field_10 = -1;
	field_14 = NULL;
	field_18 = 0.f;
	field_20 = 0.f;
	field_30 = _9B38D4;
	field_8 = NULL;
	field_C = NULL;
	field_1C = 1.f;
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

#pragma message(TODO_IMPLEMENTATION)
void Font::MakeCharactersMap(GlyphInfo& GlyphsInfo)
{
	GlyphsInfo.m_ActualGlyphsArray[0] = { 'a', 1, 4, 10.854492, 0, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[1] = { 'b', 2, 0, 10.854492, 0.070312999, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[2] = { 'c', 1, 4, 10.854492, 0.140625, 0, 0.066405997 };
	GlyphsInfo.m_ActualGlyphsArray[3] = { 'd', 1, 0, 10.854492, 0.21093801, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[4] = { 'e', 1, 4, 10.854492, 0.28125, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[5] = { 'f', 1, 0, 10.854492, 0.35156301, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[6] = { 'g', 1, 4, 10.854492, 0.421875, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[7] = { 'h', 2, 0, 10.854492, 0.49218801, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[8] = { 'i', 2, 1, 10.854492, 0.5625, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[9] = { 'j', 1, 1, 10.854492, 0.63281298, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[10] = { 'k', 2, 0, 10.854492, 0.703125, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[11] = { 'l', 2, 0, 10.854492, 0.77343798, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[12] = { 'm', 1, 4, 10.854492, 0.84375, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[13] = { 'n', 2, 4, 10.854492, 0.91406298, 0, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[14] = { 'o', 1, 4, 10.854492, 0, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[15] = { 'p', 2, 4, 10.854492, 0.070312999, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[16] = { 'q', 1, 4, 10.854492, 0.140625, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[17] = { 'r', 3, 4, 10.854492, 0.21093801, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[18] = { 's', 2, 4, 10.854492, 0.28125, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[19] = { 't', 1, 3, 10.854492, 0.35156301, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[20] = { 'u', 2, 4, 10.854492, 0.421875, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[21] = { 'v', 1, 4, 10.854492, 0.49218801, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[22] = { 'w', 0, 4, 10.854492, 0.5625, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[23] = { 'x', 1, 4, 10.854492, 0.63281298, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[24] = { 'y', 1, 4, 10.854492, 0.703125, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[25] = { 'z', 1, 4, 10.854492, 0.77343798, 0.070312999, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[26] = { ' ', 0, 14, 10.854492, 0.84375, 0.070312999, 0.066405997, 0 };

	GlyphsInfo.m_ActualGlyphsArray[27] = { 204, 2, 0, 10.854492, 0.91406298, 0.070312999, 0.066405997, 0 };

	GlyphsInfo.m_ActualGlyphsArray[28] = { 'A', 0, 3, 10.854492, 0, 0.140625, 0.066405997, 0 };
	GlyphsInfo.m_ActualGlyphsArray[29] = { 'B', 2, 3, 10.854492, 0.070312999, 0.140625, 0.066405997, 0 };
	//	TODO: ...
}