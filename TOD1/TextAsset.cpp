#include "TextAsset.h"

char* TextAsset::_A3CE80;
int TextAsset::_A3CE84;
AssetInstance* TextAsset::Instance;

AssetInstance* TextAsset::GetInstancePtr() const
{
    return Instance;
}

void TextAsset::CreateInstance()
{
    Instance = new AssetInstance("textres", (CREATOR)Create);

    Instance->m_FileExtensions.push_back("txt");
    Instance->SetAlignment(16, 1);
    Instance->SetAlignment(16, 2);
    Instance->SetAlignment(16, 0);
    Instance->field_2C = true;
}

TextAsset* TextAsset::Create()
{
    return new TextAsset;
}

TextAsset::TextAsset() : Asset(false)
{
    MESSAGE_CLASS_CREATED(TextAsset);
}

void TextAsset::GetIdentifierByIndex(String& outString, const unsigned int ind) const
{
    if (ind >= m_TextIndicies.size())
        return;

    unsigned short gamestring[1024] = {};
    GetGameString(m_TextIndicies[ind], gamestring, 1024, 0);
    const int slen = GetGameStringLength(gamestring);

    if (slen)
    {
        char convstring[1024] = {};
        int i = 0;
        for (; i < slen; ++i)
            convstring[i] = (char)gamestring[i];
        convstring[i] = NULL;

        outString = convstring;
    }
}

void TextAsset::GetTextContents(String& outString, const unsigned int ind) const
{
    if (ind >= m_TextIndicies.size())
        return;

    unsigned short gamestring[1024] = {};
    GetGameString(m_TextIndicies[ind], gamestring, 1024, true);
    const int slen = GetGameStringLength(gamestring);

    if (slen)
    {
        char convstring[1024] = {};
        int i = 0;
        for (; i < slen; ++i)
            convstring[i] = (char)gamestring[i];
        convstring[i] = NULL;

        outString = convstring;
    }
}

void TextAsset::GetGameString(const unsigned short indicieslistindex, unsigned short* outString, const unsigned int maxlength, const bool contents) const
{
    //  NOTE: argument 'contents' decides what information should be in the output - only text identifier OR actual contents.
    _A3CE80 = (char*)&m_List_3[indicieslistindex];
    _A3CE84 = 0;

    unsigned int i = 0;
    for (Dictionary* charinfo = GetCharacterInfo(field_50); (char)charinfo->m_Contents; charinfo = GetCharacterInfo(field_50))
        if (!contents)
            outString[i++] = charinfo->m_Contents;

    if (contents)
        for (Dictionary* charinfo = GetCharacterInfo(field_50); (char)charinfo->m_Contents; charinfo = GetCharacterInfo(field_50))
            outString[i++] = charinfo->m_Contents;
}

int TextAsset::GetGameStringLength(const unsigned short* str)
{
    int len = 0;
    unsigned short* strLocal = (unsigned short*)str;

    while (*strLocal)
        len++;

    return len;
}

TextAsset::Dictionary* TextAsset::GetCharacterInfo(Dictionary* dict)
{
    if (dict->m_Contents != 0xA74)
        return dict;

    do
    {
        if (_A3CE84 >= 8)
        {
            _A3CE80++;
            _A3CE84 -= 8;
        }

        const unsigned char currchar = *_A3CE80 >> _A3CE84++;
        if ( (currchar & 1) != 0)
            dict = (Dictionary*)ALIGN_4BYTES(dict->m_Prev);
        else
            dict = (Dictionary*)ALIGN_4BYTES(dict->m_Next);
    } while (dict->m_Contents == 0xA74);

    return dict;
}

void TextAsset::EncodeGameString(short* outString, short* inString)
{
    unsigned int inputStringLength = 0;
    for (short* i = inString; *i; ++inputStringLength)
        ++i;

    unsigned int inputStringLengthPastEnd = inputStringLength + 1;
    if (inputStringLengthPastEnd > 0)
    {
        for (short* outStringPtr = outString; inputStringLengthPastEnd; --inputStringLengthPastEnd, ++outStringPtr)
            *outStringPtr = *(short*)(*(char*)&outStringPtr + *(char*)&inString - *(char*)&outString);
    }
}

void TextAsset::CopyCharArrayToGameString(unsigned short* gamestring, char* instring)
{
    for (unsigned int i = 0; i < strlen(instring) + 1; ++i)
    {
        char ch[2] = { 0, instring[i] };

        if (ch[1] == 255)
            *(short*)&ch = -1;
        else
            if (((short)ch & 128) != 0)
                *(short*)&ch = -1;

        gamestring[i] = (short)ch;
    }
}