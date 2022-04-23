#pragma once
#include "BaseAsset.h"

class TextAsset : public Asset
{
    friend class TextBox;
    friend class TextSlot;
    friend class ControlSetup;
protected:
    struct Dictionary
    {
        Dictionary     *m_Next;
        Dictionary     *m_Prev;

        short           m_Contents;
    };

    int                 field_1C;
    std::vector<int>    m_List_1;
    std::vector<unsigned short> m_TextIndicies;
    std::vector<char>   m_List_3;
    Dictionary         *field_50;
    int                 field_54;

private:
    TextAsset();    //  @862380

public:
    void                GetIdentifierByIndex(String& outString, const unsigned int ind) const;  //  @8A9E70
    void                GetTextContents(String& outString, const unsigned int ind) const;   //  @6F89E0
    void                GetGameString(const unsigned short indicieslistindex, unsigned short* outString, const unsigned int maxlength, const bool contents) const;    //  @861800

    static int          GetGameStringLength(const unsigned short* str);    //  @4146F0
    static Dictionary*  GetCharacterInfo(Dictionary*);  //  @861760
    static void         EncodeGameString(short* outString, short* inString);    //  @4147D0
    static void         CopyCharArrayToGameString(unsigned short* gamestring, char* instring);   //  @414880

    static char*        _A3CE80;    //  @A3CE80
    static int          _A3CE84;    //  @A3CE84

public:
    virtual AssetInstance* GetInstancePtr() const override;

    static void       CreateInstance(); // @861CE0
    static TextAsset* Create(); // @861BD0

    static AssetInstance*   Instance; //  @A3CE7C
};

ASSERT_CLASS_SIZE(TextAsset, 88);