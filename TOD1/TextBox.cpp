#include "TextBox.h"
#include "AssetManager.h"
#include "LogDump.h"
#include "TextSlot.h"
#include "TextAsset.h"

TextBox::~TextBox()
{
    MESSAGE_CLASS_DESTROYED(TextBox);

    delete m_Text;
}

Vector4f* TextBox::GetBounds(Vector4f& outBounds) const
{
    if (!m_Flags.m_FlagsBits.VerticalAlignment)
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
    m_Flags.m_Flags = m_Flags.m_Flags & 0xFFFFFF00;
    m_HorizontalSpacing = m_VerticalSpacing = 0.0f;
    field_94 = 0;
    m_Flags.m_FlagsBits.TextResMode = false;
    m_Text = new String();
    m_Flags.m_Flags = m_Flags.m_Flags & 0xFFFFA5FF | 0x7FF2000;
    m_TextSlot = nullptr;
    field_B8 = NULL;
    m_TextSlotsContents = nullptr;
    m_VerticalScroll = 0.0f;
    m_Id = m_Id | 8;

    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
}

void TextBox::SetTextScale(const float* args)
{
    m_Id |= 8;
    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_ScaleY = args[1];

    m_Id |= 8;
    m_ScaleX = args[0];

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* TextBox::GetActualBoxSize(Vector4f& outSize) const
{
    if (!m_FontRes)
        return (outSize = Vector4f(), &outSize);

    return nullptr;
}

#pragma message(TODO_IMPLEMENTATION)
void TextBox::SetFont(const char* fontName)
{
    String fontPath;
    //AssetTemplate res;

    g_AssetManager->GetResourcePath(fontPath, fontName);

    if (!_stricmp(fontPath.m_Str, "/data/fonts/screenfont_Headlines.font") ||
        !_stricmp(fontPath.m_Str, "/data/fonts/InnerSanctumCondensed_Screenfont.font"))
    {
        fontPath = "/data/fonts/screenfont_ODserif_64all.font";
        LogDump::LogA("remapped %s to %s\n", fontName, fontPath.m_Str);
    }

    if (!_stricmp(fontPath.m_Str, "/data/fonts/screenfont_ODserif_64cond.font"))
    {
        fontPath = "/data/fonts/screenfont_ODserif_64all.font";
        LogDump::LogA("remapped %s to %s\n", fontName, fontPath.m_Str);
    }

    /*if (fontName)
    {
     res.LoadResourceFromBlock(fontPath.m_szString);
     m_FontRes->ApplyLoadedResource(res);

     if (res.m_Resource)
      g_AssetManager->DecreaseResourceReferenceCount(res.m_Resource);
    }
    else
    {
     res.m_Resource = nullptr;
     m_FontRes->ApplyLoadedResource(res);
    }*/

    m_Id |= 8;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const char* TextBox::GetText() const
{
    return m_Text ? m_Text->m_Str : nullptr;
}

const char* TextBox::GetFont() const
{
    return m_FontRes ? m_FontRes->AddResToOpenListAndReturnName() : nullptr;
}

const int TextBox::GetAsciiOffset() const
{
    return 0;
}

void TextBox::SetAsciiOffset(const int offset)
{
    m_Id |= 8;
}

const bool TextBox::GetShouldScaleIfWidescreen() const
{
    return m_Flags.m_FlagsBits.ScaleIfWidescreen;
}

const bool TextBox::GetUseGlyphIgnoreColor() const
{
    return m_Flags.m_FlagsBits.UseGlyphIgnoreColor;
}

const char* TextBox::GetTextSlotByName() const
{
    if (!m_TextSlot)
        return nullptr;

    if (!m_TextSlot->m_Name)
        return nullptr;
    else
        return m_TextSlot->m_Name;
}

void TextBox::GetTextColor(ColorRGB& outTextColor) const
{
    const int& clr = m_TextColor;
    outTextColor = {
        (float)((unsigned char)(clr >> 24) * (1 / 255)),
        (float)((unsigned char)(clr >> 16) * (1 / 255)),
        (float)((unsigned char)(clr >> 8) * (1 / 255)),
        (float)((unsigned char)(clr >> 0) * (1 / 255))
    };
}

void TextBox::GetTextColor2(ColorRGB& outTextColor) const
{
    const int& clr = m_TextColor2;
    outTextColor = {
        (float)((unsigned char)(clr >> 24) * (1 / 255)),
        (float)((unsigned char)(clr >> 16) * (1 / 255)),
        (float)((unsigned char)(clr >> 8) * (1 / 255)),
        (float)((unsigned char)(clr >> 0) * (1 / 255))
    };
}

void TextBox::GetTextScale(Vector4f& outTextScale) const
{
    outTextScale = { m_ScaleX, m_ScaleY, 1, 0 };
}

const float TextBox::GetHorizontalSpacing() const
{
    return m_HorizontalSpacing;
}

const float TextBox::GetVerticalSpacing() const
{
    return m_VerticalSpacing;
}

const bool TextBox::GetVerticalClip() const
{
    return m_Flags.m_FlagsBits.VerticalClip;
}

const bool TextBox::GetViewAllSlotIndicies() const
{
    return m_Flags.m_FlagsBits.ViewAllSlotIndices;
}

const bool TextBox::GetUseSlotIndex() const
{
    return m_Flags.m_FlagsBits.UseSlotIndex;
}

const bool TextBox::GetTextresMode() const
{
    return m_Flags.m_FlagsBits.TextResMode;
}

void TextBox::SetTextSlotParam(unsigned int slotindex, short* slotvalue)
{
    if (slotindex >= 0)
    {
        if (slotindex >= MAX_TEXT_SLOTS)
            slotindex = (MAX_TEXT_SLOTS - 1);
    }
    else
        slotindex = 0;

    if (!m_TextSlotsContents)
    {
        m_TextSlotsContents = (short**)new short[MAX_TEXT_SLOTS];

        for (unsigned int i = 0; i < MAX_TEXT_SLOTS; ++i)
            m_TextSlotsContents[i] = nullptr;
    }

    if (m_TextSlotsContents[slotindex])
        delete m_TextSlotsContents[slotindex];

    if (slotvalue)
    {
        m_TextSlotsContents[slotindex] = new short[TextAsset::GetGameStringLength(slotvalue) + 2];
        TextAsset::EncodeGameString(m_TextSlotsContents[slotindex], slotvalue);
    }
}
