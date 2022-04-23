#include "TextBox.h"
#include "AssetManager.h"
#include "LogDump.h"
#include "TextSlot.h"
#include "TextAsset.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "StringType.h"

TextBox::~TextBox()
{
    MESSAGE_CLASS_DESTROYED(TextBox);

    delete m_Text;
}

Vector4f* TextBox::GetBounds(Vector4f& outBounds)
{
    if (!m_Flags.VerticalAlignment)
        return (outBounds = Vector4f(0.f, 0.f, 0.f, 10000.f), &outBounds);

    Vector4f boxSize;

    if (m_FontAsset)
        return (outBounds = Vector4f(0.f, 0.f, 0.f, (float)((GetActualBoxSize(boxSize)->x + GetActualBoxSize(boxSize)->y) * 0.041666668)), &outBounds);
    else
        return (outBounds = Vector4f(0.f, 0.f, 0.f, m_SpriteSize_Y + m_SpriteSize_X), &outBounds);
}

TextBox::TextBox() : Sprite()
{
    MESSAGE_CLASS_CREATED(TextBox);

    m_TextColor.ColorDword = -1;
    m_TextColor2.ColorDword = -1;
    m_ScaleX = 1.f;
    m_ScaleY = 1.f;
    m_Flags.HorizontalAlignment = eHorizontalAlign::LEFT;
    m_Flags.VerticalAlignment = 0;
    m_HorizontalSpacing = 0.f;
    m_VerticalSpacing = 0.0f;
    field_94 = 0;
    m_Flags.TextResMode = false;
    m_Text = new String;
    m_Flags.UseGlyphIgnoreColor = true;
    m_TextSlot = nullptr;
    m_TextSlotIndexList = nullptr;
    m_TextSlotsContents = nullptr;
    m_VerticalScroll = 0.0f;
    m_Id._3 = true;

    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x8000000;
}

void TextBox::SetTextresMode(const int mode)
{
    delete m_TextSlotIndexList;

    m_Flags.TextResMode = mode;

    if (m_Flags.TextResMode)
        delete m_Text;
    else
        if (!m_Text)
            m_Text = new String;
}

void TextBox::SetTextScale(const float* args)
{
    m_Id._3 = true;
    if (m_QuadTree)
        m_QuadTree->Refresh();

    m_ScaleY = args[1];

    m_Id._3 = true;
    m_ScaleX = args[0];

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

#pragma message(TODO_IMPLEMENTATION)
Vector4f* TextBox::GetActualBoxSize(Vector4f& outSize) const
{
    if (!m_FontAsset)
        return (outSize = Vector4f(), &outSize);

    return nullptr;
}

void TextBox::SetFont(const char* fontName)
{
    String fontPath;

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

    m_FontAsset = AssetLoader(fontPath.m_Str);

    m_Id._3 = true;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const char* TextBox::GetText() const
{
    return m_Text ? m_Text->m_Str : nullptr;
}

void TextBox::SetText(const char* const text)
{
    if (!m_Text)
        return;

    m_Id._3 = 1;
    *m_Text = text;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const char* TextBox::GetFont() const
{
    return m_FontAsset ? m_FontAsset.m_AssetPtr->GetName() : nullptr;
}

const bool TextBox::UseSlotIndex() const
{
    return m_Flags.UseSlotIndex;
}

void TextBox::SetUseSlotIndex(const bool use)
{
    delete m_TextSlotIndexList;

    m_Flags.UseSlotIndex = use;
    if (use && m_Flags.TextSlotIndex == 2048)
        m_Flags.TextSlotIndex = 0;
}

const bool TextBox::ViewAllSlotIndicies() const
{
    return m_Flags.ViewAllSlotIndices;
}

void TextBox::SetViewAllSlotIndicies(const bool view)
{
    m_Flags.ViewAllSlotIndices = view;
}

const int TextBox::GetTextSlotIndex() const
{
    return m_Flags.TextSlotIndex;
}

void TextBox::SetTextSlotIndex(const int index)
{
    delete m_TextSlotIndexList;
    m_Flags.TextSlotIndex = index;

    if (m_TextSlot && m_TextSlot->m_TextAsset)
    {
        TextAsset* txtAsset = (TextAsset*)(m_TextSlot->m_TextAsset.m_AssetPtr);
        if ((size_t)index >= txtAsset->m_TextIndicies.size())
            m_Flags.TextSlotIndex = txtAsset->m_TextIndicies.size() - 1;
    }

    m_Id._3 = 1;
    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const int TextBox::GetAsciiOffset() const
{
    return 0;
}

void TextBox::SetAsciiOffset(const int offset)
{
    m_Id._3 = true;
}

const bool TextBox::GetShouldScaleIfWidescreen() const
{
    return m_Flags.ScaleIfWidescreen;
}

void TextBox::SetShouldScaleIfWidescreen(const bool should)
{
    m_Flags.ScaleIfWidescreen = should;
}

const bool TextBox::GetUseGlyphIgnoreColor() const
{
    return m_Flags.UseGlyphIgnoreColor;
}

void TextBox::SetUseGlyphIgnoreColor(const bool ignore)
{
    m_Flags.UseGlyphIgnoreColor = ignore;
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
    outTextColor =
    {
        (float)(m_TextColor.Red * (1.f / 255.f)),
        (float)(m_TextColor.Green * (1.f / 255.f)),
        (float)(m_TextColor.Blue * (1.f / 255.f)),
        (float)(m_TextColor.Alpha * (1.f / 255.f))
    };
}

void TextBox::SetTextColor(const ColorRGB& color)
{
    m_Id._3 = 1;
    m_TextColor.ColorDword = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

void TextBox::GetTextColor2(ColorRGB& outTextColor) const
{
    outTextColor =
    {
        (float)(m_TextColor2.Red * (1.f / 255.f)),
        (float)(m_TextColor2.Green * (1.f / 255.f)),
        (float)(m_TextColor2.Blue * (1.f / 255.f)),
        (float)(m_TextColor2.Alpha * (1.f / 255.f))
    };
}

void TextBox::SetTextColor2(const ColorRGB& color)
{
    m_Id._3 = 1;
    m_TextColor2.ColorDword = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

void TextBox::GetTextScale(Vector4f& outTextScale) const
{
    outTextScale = { m_ScaleX, m_ScaleY, 1, 0 };
}

const float TextBox::GetHorizontalSpacing() const
{
    return m_HorizontalSpacing;
}

void TextBox::SetHorizontalSpacing(const float spacing)
{
    m_HorizontalSpacing = spacing;
    m_Id._3 = 1;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const float TextBox::GetVerticalSpacing() const
{
    return m_VerticalSpacing;
}

void TextBox::SetVerticalSpacing(const float spacing)
{
    m_VerticalSpacing = spacing;
    m_Id._3 = 1;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const eHorizontalAlign TextBox::GetHorizontalAlignment() const
{
    return m_Flags.HorizontalAlignment;
}

void TextBox::SetHorizontalAlignment(const eHorizontalAlign align)
{
    m_Flags.HorizontalAlignment = align;
    m_Id._3 = 1;
}

const int TextBox::GetVerticalAlignment() const
{
    return m_Flags.VerticalAlignment;
}

void TextBox::SetVerticalAlignment(const int align)
{
    m_Flags.VerticalAlignment = align;
    m_Id._3 = 1;
}

const float TextBox::GetVerticalScroll() const
{
    return m_VerticalScroll;
}

void TextBox::SetVerticalScroll(const float scroll)
{
    m_VerticalScroll = scroll;
    m_Id._3 = 1;

    if (m_QuadTree)
        m_QuadTree->Refresh();
}

const bool TextBox::GetVerticalClip() const
{
    return m_Flags.VerticalClip;
}

void TextBox::SetVerticalClip(const bool enabled)
{
    m_Flags.VerticalClip = enabled;
}

const bool TextBox::GetViewAllSlotIndicies() const
{
    return m_Flags.ViewAllSlotIndices;
}

const bool TextBox::GetUseSlotIndex() const
{
    return m_Flags.UseSlotIndex;
}

const bool TextBox::GetTextresMode() const
{
    return m_Flags.TextResMode;
}

const int TextBox::GetStringWidth() const
{
    if (!m_FontAsset)
        return 0;

    Font* asFont = ((FontAsset*)m_FontAsset.m_AssetPtr)->m_Font;
    if (m_Text)
    {
        asFont->m_ScaleX = m_ScaleX;
        asFont->m_ScaleY = m_ScaleY;
        asFont->m_HorizontalSpacing = m_HorizontalSpacing;
        asFont->m_VerticalSpacing = m_VerticalSpacing;

        return asFont->GetTextWidth(m_Text->m_Str);
    }

    if (!m_TextSlot)
        return 0;

    asFont->m_ScaleX = m_ScaleX;
    asFont->m_ScaleY = m_ScaleY;
    asFont->m_HorizontalSpacing = m_HorizontalSpacing;
    asFont->m_VerticalSpacing = m_VerticalSpacing;

    unsigned short textBuffer[1024];
    unsigned short* textBufferPtr;

    if (!m_TextSlotIndexList)
    {
        textBufferPtr = textBuffer;

        if (m_Flags.UseSlotIndex)
            m_TextSlot->GetCurrentLoadedText(textBuffer, sizeof(textBuffer));
        else
            if (m_Flags.TextSlotIndex == -1)
                textBuffer[0] = 0;
            else
                m_TextSlot->GetTextByIndex(m_Flags.TextSlotIndex, textBuffer, sizeof(textBuffer));
    }
    else
    {
        textBufferPtr = m_TextSlotIndexList;
    }

    unsigned short parsedTextBuffer[1024];
    ParseFormattedText(textBufferPtr, parsedTextBuffer, sizeof(parsedTextBuffer));
    return asFont->GetGameTextWidth(parsedTextBuffer);
}

void TextBox::SetTextSlotParam(int* args)
{
    unsigned short str[1024];
    TextAsset::CopyCharArrayToGameString(str, (char*)args[1]);
    SetTextSlotParam_Impl(args[0], str);
}

void TextBox::SetTextSlotParamFromTextSlot(int* args)
{
    SetTextSlotParamFromTextSlot_Impl(args[0], (TextSlot*)args[1], args[2]);
}

void TextBox::SetTextSlot(int* args)
{
    TextSlot* txSlot = (TextSlot*)args[0];
    EntityType* txSlotEntity = txSlot->m_ScriptEntity;

    if (txSlot && txSlotEntity)
    {
        while (tTextSlot != txSlotEntity)
        {
            txSlotEntity = txSlotEntity->m_Parent;
            if (!txSlotEntity)
                txSlot = nullptr;
        }
    }

    m_TextSlot = txSlot;

    if (txSlot)
    {
        m_Id._3 = 1;
        if (m_QuadTree)
            m_QuadTree->Refresh();
    }
    else
    {
        LogDump::LogA("TextBox::SetTextSlot() - Failed to set pTextSlot, Entity not a TextSlot.\n");
    }
}

#pragma message(TODO_IMPLEMENTATION)
void TextBox::SetTextSlotIndexList(int* args)
{
    delete m_TextSlotIndexList;

    if (!args[0])
        return;

    if (!m_TextSlot)
    {
        LogDump::LogA("Ignoring SetTextSlotIndexList because no TextSlot is set\n");
        return;
    }

    if (!m_Flags.TextResMode)
    {
        LogDump::LogA("Ignoring SetTextSlotIndexList because TextSlot mode is not enabled\n");
        return;
    }

    //  ...
}

void TextBox::SetTextSlotParam_Impl(unsigned int slotindex, unsigned short* slotvalue)
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
        TextAsset::EncodeGameString(m_TextSlotsContents[slotindex], (short*)slotvalue);
    }
}

void TextBox::SetTextSlotParamFromTextSlot_Impl(const int paramIndex, TextSlot* textslot, int textIndex)
{
    TextAsset* asText = ((TextAsset*)textslot->m_TextAsset.m_AssetPtr);
    if (textIndex < 0)
        textIndex = 0;

    if ((size_t)textIndex >= asText->m_TextIndicies.size())
        textIndex = asText->m_TextIndicies.size() - 1;

    unsigned short text[1024];

    if (asText)
    {
        if ((size_t)textIndex < asText->m_TextIndicies.size())
            asText->GetGameString(asText->m_TextIndicies[textIndex], text, sizeof(text), true);
        else
            text[0] = 0;
    }

    SetTextSlotParam_Impl(paramIndex, text);
}

void TextBox::sUpdate(int* args)
{
    Update();
}

void TextBox::Register()
{
    tTextBox = new EntityType("TextBox");
    tTextBox->InheritFrom(tSprite);
    tTextBox->SetCreator((CREATOR)Create);

    tTextBox->RegisterProperty(tTRUTH, "textres_mode", (EntityGetterFunction)&GetTextresMode, (EntitySetterFunction)&SetTextresMode, "control=truth");
    tTextBox->RegisterProperty(tSTRING, "font", (EntityGetterFunction)&GetFont, (EntitySetterFunction)&SetFont, "control=resource|type=*.ttf;*.font");
    tTextBox->RegisterProperty(tSTRING, "text", (EntityGetterFunction)&GetText, (EntitySetterFunction)&SetText, "control=advanced_string");
    tTextBox->RegisterProperty(tSTRING, "textslot_by_name", (EntityGetterFunction)&GetTextSlotByName, nullptr, "control=string");
    tTextBox->RegisterProperty(tTRUTH, "use_slot_index", (EntityGetterFunction)&UseSlotIndex, (EntitySetterFunction)&SetUseSlotIndex, "control=truth");
    tTextBox->RegisterProperty(tTRUTH, "view_all_slot_indices", (EntityGetterFunction)&ViewAllSlotIndicies, (EntitySetterFunction)&SetViewAllSlotIndicies, "control=truth");
    tTextBox->RegisterProperty(tINTEGER, "textslot_index", (EntityGetterFunction)&GetTextSlotIndex, (EntitySetterFunction)&SetTextSlotIndex, "control=slider|min=0|max=1024");
    tTextBox->RegisterProperty(tVECTOR, "text_color", (EntityGetterFunction)&GetTextColor, (EntitySetterFunction)&SetTextColor, "control=colorrgb");
    tTextBox->RegisterProperty(tVECTOR, "text_color2", (EntityGetterFunction)&GetTextColor2, (EntitySetterFunction)&SetTextColor2, "control=colorrgb");
    tTextBox->RegisterProperty(tTRUTH, "use_glyph_ignore_color", (EntityGetterFunction)&GetUseGlyphIgnoreColor, (EntitySetterFunction)&SetUseGlyphIgnoreColor, "control=truth");
    tTextBox->RegisterProperty(tTRUTH, "scale_if_widescreen", (EntityGetterFunction)&GetShouldScaleIfWidescreen, (EntitySetterFunction)&SetShouldScaleIfWidescreen, "control=truth");
    tTextBox->RegisterProperty(tVECTOR, "text_scale", (EntityGetterFunction)&GetTextScale, (EntitySetterFunction)&SetTextScale, "control=string");
    tTextBox->RegisterProperty(tNUMBER, "horizontal_spacing", (EntityGetterFunction)&GetHorizontalSpacing, (EntitySetterFunction)&SetHorizontalSpacing, "control=slider|min=-32|max=32|step=0.5");
    tTextBox->RegisterProperty(tNUMBER, "vertical_spacing", (EntityGetterFunction)&GetVerticalSpacing, (EntitySetterFunction)&SetVerticalSpacing, "control=slider|min=-32|max=32|step=0.5");
    tTextBox->RegisterProperty(tINTEGER, "asciioffset", (EntityGetterFunction)&GetAsciiOffset, (EntitySetterFunction)&SetAsciiOffset, "control=slider|min=-64|max=64|step=1");
    tTextBox->RegisterProperty(tINTEGER, "horizontal_alignment", (EntityGetterFunction)&GetHorizontalAlignment, (EntitySetterFunction)&SetHorizontalAlignment, "control=dropdown|left=0|right=1|center=2|justify=3");
    tTextBox->RegisterProperty(tINTEGER, "vertical_alignment", (EntityGetterFunction)&GetVerticalAlignment, (EntitySetterFunction)&SetVerticalAlignment, nullptr);
    tTextBox->RegisterProperty(tVECTOR, "actualboxsize", (EntityGetterFunction)&GetActualBoxSize, nullptr, nullptr);
    tTextBox->RegisterProperty(tINTEGER, "stringwidth", (EntityGetterFunction)&GetStringWidth, nullptr, nullptr);
    tTextBox->RegisterProperty(tNUMBER, "vertical_scroll", (EntityGetterFunction)&GetVerticalScroll, (EntitySetterFunction)&SetVerticalScroll, "control=slider|min=-100|max=100|step=1.0");
    tTextBox->RegisterProperty(tTRUTH, "vertical_clip", (EntityGetterFunction)&GetVerticalClip, (EntitySetterFunction)&SetVerticalClip, "control=truth");

    tTextBox->RegisterScript("settextslotparam(integer,string)", (EntityFunctionMember)&SetTextSlotParam);
    tTextBox->RegisterScript("settextslotparamfromtextslot(integer,entity,integer)", (EntityFunctionMember)&SetTextSlotParamFromTextSlot);
    tTextBox->RegisterScript("settextslot(entity)", (EntityFunctionMember)&SetTextSlot);
    tTextBox->RegisterScript("set_textslot_index_list(list(integer))", (EntityFunctionMember)&SetTextSlotIndexList);
    tTextBox->RegisterScript("update", (EntityFunctionMember)&sUpdate, 0, 0, 0, "control=button|text=update");

    tTextBox->PropagateProperties();
}

TextBox* TextBox::Create(AllocatorIndex)
{
    return new TextBox;
}