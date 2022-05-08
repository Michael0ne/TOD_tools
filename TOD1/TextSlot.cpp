#include "TextSlot.h"
#include "TextAsset.h"
#include "TextAsset.h"
#include "StringType.h"
#include "IntegerType.h"

EntityType* tTextSlot;

TextSlot::~TextSlot()
{
    MESSAGE_CLASS_DESTROYED(TextSlot);

    delete[] m_CurrentText;
}

void TextSlot::Register()
{
    tTextSlot = new EntityType("TextSlot");
    tTextSlot->InheritFrom(tNode);
    tTextSlot->SetCreator((CREATOR)Create);

    tTextSlot->RegisterProperty(tSTRING, "textres", (EntityGetterFunction)&GetTextRes, NULL, NULL, NULL, (EntitySetterFunction)&SetTextRes, NULL, NULL, NULL, "control=resource|type=*.txt", NULL, NULL, -1);
    tTextSlot->RegisterProperty(tINTEGER, "current_index", (EntityGetterFunction)&GetCurrentIndex, NULL, NULL, NULL, (EntitySetterFunction)&SetCurrentIndex, NULL, NULL, NULL, "control=slider|min=0|max=48", NULL, NULL, -1);
    tTextSlot->RegisterProperty(tSTRING, "text_string_identifier", (EntityGetterFunction)&GetTextStringIdentifier, NULL, NULL, NULL, nullptr, NULL, NULL, -1, "control=string", NULL, NULL, -1);
    tTextSlot->RegisterProperty(tINTEGER, "number_of_text_indices", (EntityGetterFunction)&GetNumberOfTextIndicies, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tTextSlot->RegisterProperty(tSTRING, "current_text_content", (EntityGetterFunction)&GetCurrentTextContent, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
 
    tTextSlot->RegisterScript("Dump", (EntityFunctionMember)&Dump, NULL, NULL, NULL, "control=button|text=dump", nullptr);

    tTextSlot->PropagateProperties();
}

const char* TextSlot::GetTextRes() const
{
    return m_TextAsset.m_AssetPtr ? m_TextAsset.m_AssetPtr->GetName() : nullptr;
}

void TextSlot::SetTextRes(const char* text)
{
    m_TextAsset = AssetLoader(text);
}

int TextSlot::GetCurrentIndex() const
{
    return m_CurrentIndex;
}

void TextSlot::SetCurrentIndex(unsigned int index)
{
    index = index < 0 ? 0 : index;

    if (m_TextAsset.m_AssetPtr && index >= ((TextAsset*)m_TextAsset.m_AssetPtr)->m_TextIndicies.size())
        index = ((TextAsset*)m_TextAsset.m_AssetPtr)->m_TextIndicies.size() - 1;

    m_CurrentIndex = index;
}

char* TextSlot::GetTextStringIdentifier()
{
    String textident;

    if (m_TextAsset.m_AssetPtr)
        ((TextAsset*)m_TextAsset.m_AssetPtr)->GetIdentifierByIndex(textident, m_CurrentIndex);

    delete[] m_CurrentText;

    m_CurrentText = textident.m_Str;
    return m_CurrentText;
}

int TextSlot::GetNumberOfTextIndicies() const
{
    return m_TextAsset.m_AssetPtr ? ((TextAsset*)m_TextAsset.m_AssetPtr)->m_TextIndicies.size() : NULL;
}

char* TextSlot::GetCurrentTextContent()
{
    String textcontent;

    if (m_TextAsset.m_AssetPtr)
        ((TextAsset*)m_TextAsset.m_AssetPtr)->GetTextContents(textcontent, m_CurrentIndex);

    delete[] m_CurrentText;

    m_CurrentText = textcontent.m_Str;
    return m_CurrentText;
}

void TextSlot::Dump(const int)
{
    if (!m_TextAsset.m_AssetPtr)
        return;

    String textident, textcont;
    for (unsigned int i = 0; i < ((TextAsset*)m_TextAsset.m_AssetPtr)->m_TextIndicies.size(); ++i)
    {
        ((TextAsset*)m_TextAsset.m_AssetPtr)->GetIdentifierByIndex(textident, i);
        LogDump::LogA("Index: %d |   StrID: '%s'\n", i, textident.m_Str);
        ((TextAsset*)m_TextAsset.m_AssetPtr)->GetTextContents(textcont, i);
        LogDump::LogA("Index: %d | Content: '%s'\n", i, textcont.m_Str);
    }
}

void TextSlot::GetCurrentLoadedText(unsigned short* outText, const unsigned int maxLength) const
{
    outText[0] = 0;
    if (!m_TextAsset)
        return;

    TextAsset* asText = ((TextAsset*)m_TextAsset.m_AssetPtr);
    if (m_CurrentIndex < asText->m_TextIndicies.size())
        asText->GetGameString(asText->m_TextIndicies[m_CurrentIndex], outText, maxLength, true);
    else
        outText[0] = 0;
}

void TextSlot::GetTextByIndex(int index, unsigned short* outText, const unsigned int maxLength) const
{
    TextAsset* asText = ((TextAsset*)m_TextAsset.m_AssetPtr);

    if (index < 0)
        index = 0;

    if ((size_t)index >= asText->m_TextIndicies.size())
        index = asText->m_TextIndicies.size() - 1;

    outText[0] = 0;
    if (!asText)
        return;

    if ((size_t)index < asText->m_TextIndicies.size())
        asText->GetGameString(asText->m_TextIndicies[index], outText, maxLength, true);
    else
        outText[0] = 0;
}

TextSlot* TextSlot::Create(AllocatorIndex allocator)
{
    return new TextSlot;
}