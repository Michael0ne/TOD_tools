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

    if (m_TextRes)
        g_AssetManager->DecreaseResourceReferenceCount(m_TextRes);

    delete field_5C;
}

void TextSlot::Register()
{
    tTextSlot = new EntityType("TextSlot");
    tTextSlot->InheritFrom(tNode);
    tTextSlot->SetCreator((EntityType::CREATOR)Create);

    tTextSlot->RegisterProperty(tSTRING, "textres", &GetTextRes, NULL, NULL, NULL, &SetTextRes, NULL, NULL, NULL, "control=resource|type=*.txt", NULL, NULL, -1);
    tTextSlot->RegisterProperty(tINTEGER, "current_index", &GetCurrentIndex, NULL, NULL, NULL, &SetCurrentIndex, NULL, NULL, NULL, "control=slider|min=0|max=48", NULL, NULL, -1);
    tTextSlot->RegisterProperty(tSTRING, "text_string_identifier", &GetTextStringIdentifier, NULL, NULL, NULL, nullptr, NULL, NULL, -1, "control=string", NULL, NULL, -1);
    tTextSlot->RegisterProperty(tINTEGER, "number_of_text_indices", &GetNumberOfTextIndicies, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tTextSlot->RegisterProperty(tSTRING, "current_text_content", &GetCurrentTextContent, NULL, NULL, NULL, nullptr, NULL, NULL, -1, nullptr, NULL, NULL, -1);
    tTextSlot->RegisterScript("Dump", &Dump, NULL, NULL, NULL, "control=button|text=dump", nullptr);

    tTextSlot->PropagateProperties();
}

const char* TextSlot::GetTextRes() const
{
    return m_TextRes ? m_TextRes->AddResToOpenListAndReturnName() : nullptr;
}

void TextSlot::SetTextRes(const char* text)
{
    AssetLoader assload(text);
    m_TextRes = (TextAsset*)assload.m_AssetPtr;
}

int TextSlot::GetCurrentIndex() const
{
    return m_CurrentIndex;
}

void TextSlot::SetCurrentIndex(unsigned int index)
{
    index = index < 0 ? 0 : index;

    if (m_TextRes && index >= m_TextRes->m_TextIndicies.size())
        index = m_TextRes->m_TextIndicies.size() - 1;

    m_CurrentIndex = index;
}

char* TextSlot::GetTextStringIdentifier()
{
    String textident;

    if (m_TextRes)
        m_TextRes->GetIdentifierByIndex(textident, m_CurrentIndex);

    delete[] m_CurrentText;

    m_CurrentText = textident.m_Str;
    return m_CurrentText;
}

int TextSlot::GetNumberOfTextIndicies() const
{
    return m_TextRes ? m_TextRes->m_TextIndicies.size() : NULL;
}

char* TextSlot::GetCurrentTextContent()
{
    String textcontent;

    if (m_TextRes)
        m_TextRes->GetTextContents(textcontent, m_CurrentIndex);

    delete[] m_CurrentText;

    m_CurrentText = textcontent.m_Str;
    return m_CurrentText;
}

void TextSlot::Dump(const int)
{
    if (!m_TextRes)
        return;

    String textident, textcont;
    for (unsigned int i = 0; i < m_TextRes->m_TextIndicies.size(); ++i)
    {
        m_TextRes->GetIdentifierByIndex(textident, i);
        LogDump::LogA("Index: %d |   StrID: '%s'\n", i, textident.m_Str);
        m_TextRes->GetTextContents(textcont, i);
        LogDump::LogA("Index: %d | Content: '%s'\n", i, textcont.m_Str);
    }
}

TextSlot* TextSlot::Create(AllocatorIndex allocator)
{
    return new TextSlot;
}