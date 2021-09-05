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

void TextAsset::GetStringById(String& outString, const unsigned int ind) const
{
 if (ind >= m_TextIndicies.size())
  return;

 short gamestring[1024] = {};
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

void TextAsset::GetGameString(const unsigned short indicieslistindex, short* outString, const unsigned int maxlength, const bool a4) const
{
 _A3CE80 = (char*)&m_List_3[indicieslistindex];
 _A3CE84 = 0;

 unsigned int i = 0;
 for (Dictionary* charinfo = GetCharacterInfo(field_50); (char)charinfo->m_Contents; charinfo = GetCharacterInfo(field_50))
  if (!a4)
   outString[i++] = charinfo->m_Contents;

 if (a4)
  for (Dictionary* charinfo = GetCharacterInfo(field_50); (char)charinfo->m_Contents; charinfo = GetCharacterInfo(field_50))
   outString[i++] = charinfo->m_Contents;
}

int TextAsset::GetGameStringLength(short* str)
{
 int len = 0;

 while (*str)
  len++;

 return len;
}

TextAsset::Dictionary* TextAsset::GetCharacterInfo(Dictionary* dict)
{
 if (dict->m_Contents == 0xA74)
 {
  for (; dict->m_Contents == 0xA74;)
  {
   if (_A3CE84 >= 8)
   {
    _A3CE80++;
    _A3CE84 -= 8;
   }

   if ( (*_A3CE80 >> _A3CE84++) != 0 )
    dict = dict->m_Prev;
   else
    dict = dict->m_Next;
  }
 }

 return dict;
}