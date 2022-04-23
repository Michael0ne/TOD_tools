#pragma once
#include "Sprite.h"
#include "FontAsset.h"

#define MAX_TEXT_SLOTS 10

class TextSlot;

enum eHorizontalAlign
{
    LEFT = 0,
    RIGHT = 1,
    CENTER = 2,
    JUSTIFY = 3
};

class TextBox : public Sprite
{
    struct ColorBits
    {
        union
        {
            struct
            {
                unsigned char   Red;
                unsigned char   Green;
                unsigned char   Blue;
                unsigned char   Alpha;
            };
            unsigned int        ColorDword;
        };
    };

protected:
    AssetLoader     m_FontAsset;
    String         *m_Text;
    short         **m_TextSlotsContents;
    TextSlot       *m_TextSlot;
    unsigned short *m_TextSlotIndexList;
    ColorBits       m_TextColor;
    ColorBits       m_TextColor2;
    float           m_ScaleX;
    float           m_ScaleY;
    float           m_HorizontalSpacing;
    float           m_VerticalSpacing;
    float           m_VerticalScroll;
    union
    {
        struct
        {
            eHorizontalAlign    HorizontalAlignment : 4;
            unsigned    VerticalAlignment : 4;
            unsigned    TextResMode : 1;
            unsigned    UseSlotIndex : 1;
            unsigned    _10 : 1;
            unsigned    VerticalClip : 1;
            unsigned    ViewAllSlotIndices : 1;
            unsigned    UseGlyphIgnoreColor : 1;
            unsigned    ScaleIfWidescreen : 1;
            unsigned    _15 : 1;
            unsigned    TextSlotIndex : 11;
            unsigned    _27 : 1;
            unsigned    _28 : 1;
            unsigned    _29 : 1;
            unsigned    _30 : 1;
            unsigned    _31 : 1;
        };
    }               m_Flags;

public:
    TextBox(); // @8FDAA0

    virtual ~TextBox(); // @8FDBE0
    virtual Vector4f* GetBounds(Vector4f& outBounds) override; // @8FF220

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }

    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    void            SetTextresMode(const int mode); //  @8FFD70
    void            SetTextScale(const float* args);    //  @8FD200
    Vector4f*       GetActualBoxSize(Vector4f& outSize) const; // @8FDCD0
    void            SetFont(const char* fontName); // @8FF2F0
    void            ParseFormatting(char* key, float* linescale, int* colorindex, eHorizontalAlign* alignment);  //  @8FD570
    void            ParseFormattedText(const unsigned short* text, unsigned short* outText, const size_t maxLength) const;    //  @8FD410
    const char*     GetText() const;    //  @8FD720
    void            SetText(const char* const text);    //  @8FD8F0
    const char*     GetFont() const;    //  @8FD700
    const bool      UseSlotIndex() const;   //  @8FCDC0
    void            SetUseSlotIndex(const bool use);    //  @8FCEA0
    const bool      ViewAllSlotIndicies() const;    //  @8FCDF0
    void            SetViewAllSlotIndicies(const bool view);    //  @8FCDD0
    const int       GetTextSlotIndex() const;   //  @60BF70
    void            SetTextSlotIndex(const int index);  //  @60FA50
    const int       GetAsciiOffset() const; //  @8FD310
    void            SetAsciiOffset(const int offset);   //  @8FD320
    const bool      GetShouldScaleIfWidescreen() const; //  @8FCE60
    void            SetShouldScaleIfWidescreen(const bool should);  //  @7458A0
    const bool      GetUseGlyphIgnoreColor() const; //  @8FCE50
    void            SetUseGlyphIgnoreColor(const bool ignore);  //  @8FCE30
    const char*     GetTextSlotByName() const;  //  @8FCE80
    void            GetTextColor(ColorRGB& outTextColor) const; //  @8FCF40
    void            SetTextColor(const ColorRGB& color);    //  @8FCFE0
    void            GetTextColor2(ColorRGB& outTextColor) const;    //  @8FD080
    void            SetTextColor2(const ColorRGB& color);   //  @8FD120
    void            GetTextScale(Vector4f& outTextScale) const; //  @8FD1C0
    const float     GetHorizontalSpacing() const;   //  @8FD260
    void            SetHorizontalSpacing(const float spacing);  //  @8FD270
    const float     GetVerticalSpacing() const; //  @8FD2A0
    void            SetVerticalSpacing(const float spacing);    //  @8FD2B0
    const eHorizontalAlign  GetHorizontalAlignment() const; //  @8FD330
    void            SetHorizontalAlignment(const eHorizontalAlign align);   //  @8FD340
    const int       GetVerticalAlignment() const;   //  @8FD370
    void            SetVerticalAlignment(const int align);  //  @8FD380
    const float     GetVerticalScroll() const;  //  @931700
    void            SetVerticalScroll(const float scroll);  //  @8FD2E0
    const bool      GetVerticalClip() const;    //  @8FCE20
    void            SetVerticalClip(const bool enabled);    //  @8FCE00
    const bool      GetViewAllSlotIndicies() const; //  @8FCDF0
    const bool      GetUseSlotIndex() const;    //  @8FCDC0
    const bool      GetTextresMode() const; //  @8FCDB0
    const int       GetStringWidth() const; //  @8FE040
    void            SetTextSlotParam(int* args); //  @8FD830
    void            SetTextSlotParamFromTextSlot(int* args);    //  @8FFE60
    void            SetTextSlot(int* args); //  @8FFCC0
    void            SetTextSlotIndexList(int* args);    //  @8FFE80

private:
    void            SetTextSlotParam_Impl(unsigned int slotindex, unsigned short* slotvalue);   //  @8FD740
    void            SetTextSlotParamFromTextSlot_Impl(const int paramIndex, TextSlot* textslot, int textIndex); //  @8FD870
    void            sUpdate(int* args); //  @8FCE70

public:
    static void     Register(); //  @8FF490

private:
    static TextBox* Create(AllocatorIndex); //  @8FF1E0
};

extern EntityType* tTextBox;    //  @A3E0F8

ASSERT_CLASS_SIZE(TextBox, 220);