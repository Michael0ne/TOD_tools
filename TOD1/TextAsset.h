#pragma once
#include "BaseAsset.h"

class TextAsset : public Asset
{
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
    TextAsset();

protected:
    void                GetStringById(String& outString, const unsigned int ind) const;  //  @8A9E70
    void                GetGameString(const unsigned short indicieslistindex, short* outString, const unsigned int maxlength, const bool a4) const;    //  @861800

    static int          GetGameStringLength(short* str);    //  @4146F0
    static Dictionary*  GetCharacterInfo(Dictionary*);  //  @861760

    static char*        _A3CE80;    //  @A3CE80
    static int          _A3CE84;    //  @A3CE84

public:
    virtual AssetInstance* GetInstancePtr() const override;

    static void       CreateInstance();	//	@861CE0
    static TextAsset* Create();	//	@861BD0
};

ASSERT_CLASS_SIZE(TextAsset, 88);