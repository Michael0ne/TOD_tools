#include "Font.h"
#include "LogDump.h"

Font* g_Font;

const int Font::_9B38D4 = 0x00008000;
const float Font::_9B38D8 = (float)0xFFFF7F7F;
GlyphInfo Font::GlyphsInfo =
{
	138, 18, nullptr, nullptr
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
}