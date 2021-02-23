#include "TextBox.h"
#include "Blocks.h"
#include "LogDump.h"

TextBox::~TextBox()
{
	MESSAGE_CLASS_DESTROYED(TextBox);

	delete m_Text;
}

Vector4f* TextBox::GetBounds(Vector4f& outBounds) const
{
	if (!(m_Flags & 0x10))
		return (outBounds = Vector4f(0.f, 0.f, 0.f, 10000.f), &outBounds);

	Vector4f boxSize;

	if (m_FontRes)
		return (outBounds = Vector4f(0.f, 0.f, 0.f, (float)((GetActualBoxSize(boxSize)->x + GetActualBoxSize(boxSize)->y) * 0.041666668)), &outBounds);
	else
		return (outBounds = Vector4f(0.f, 0.f, 0.f, m_SpriteSize_Y + m_SpriteSize_X), &outBounds);
}

TextBox::TextBox() : Sprite()
{
	MESSAGE_CLASS_CREATED(TextBox);

	m_FontRes = nullptr;
	field_A8 = 1;
	m_TextColor = m_TextColor2 = -1;
	m_ScaleX = 1.0f;
	m_ScaleY = 1.0f;
	m_Flags = m_Flags & 0xFFFFFF00;
	m_HorizontalSpacing = m_VerticalSpacing = 0.0f;
	field_94 = 0;
	m_Flags = m_Flags & 0xFFFFFEFF;
	m_Text = new String();
	m_Flags = m_Flags & 0xFFFFA5FF | 0x7FF2000;
	m_TextSlot = nullptr;
	field_B8 = field_B0 = 0;
	m_VerticalScroll = 0.0f;
	m_Id = m_Id | 8;

	m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* TextBox::GetActualBoxSize(Vector4f& outSize) const
{
	if (!m_FontRes)
		return (outSize = Vector4f(), &outSize);

	return nullptr;
}

void TextBox::SetFont(const char* fontName)
{
	String fontPath;
	ResType::ResourceHolder res;

	g_Blocks->GetResourcePath(fontPath, fontName);

	if (!_stricmp(fontPath.m_szString, "/data/fonts/screenfont_Headlines.font") ||
		!_stricmp(fontPath.m_szString, "/data/fonts/InnerSanctumCondensed_Screenfont.font"))
	{
		fontPath = "/data/fonts/screenfont_ODserif_64all.font";
		LogDump::LogA("remapped %s to %s\n", fontName, fontPath.m_szString);
	}

	if (!_stricmp(fontPath.m_szString, "/data/fonts/screenfont_ODserif_64cond.font"))
	{
		fontPath = "/data/fonts/screenfont_ODserif_64all.font";
		LogDump::LogA("remapped %s to %s\n", fontName, fontPath.m_szString);
	}

	if (fontName)
	{
		res.LoadResourceFromBlock(fontPath.m_szString);
		m_FontRes->ApplyLoadedResource(res);

		if (res.m_Resource)
			g_Blocks->DecreaseResourceReferenceCount(res.m_Resource);
	}
	else
	{
		res.m_Resource = nullptr;
		m_FontRes->ApplyLoadedResource(res);
	}

	m_Id |= 8;

	if (m_QuadTree)
		m_QuadTree->Refresh();
}