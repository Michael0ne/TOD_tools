#pragma once
#include "Sprite.h"
#include "FontAsset.h"

class TextSlot;

class TextBox : public Sprite
{
    #define MAX_TEXT_SLOTS 10

    enum HorizontalAlign
    {
        LEFT        = 0,
        RIGHT       = 1,
        CENTER      = 2,
        JUSTIFY     = 3
    };

protected:
    FontAsset      *m_FontRes;
    int             field_A8;
    String         *m_Text;
    short         **m_TextSlotsContents;
    TextSlot       *m_TextSlot;
    int             field_B8;
    int             m_TextColor;
    int             m_TextColor2;
    float           m_ScaleX;
    float           m_ScaleY;
    float           m_HorizontalSpacing;
    float           m_VerticalSpacing;
    float           m_VerticalScroll;
    union
    {
        struct
        {
            unsigned char   HorizontalAlignment : 4;
            unsigned char   VerticalAlignment : 4;
            unsigned char   TextResMode : 1;
            unsigned char   UseSlotIndex : 1;
            unsigned char   _10 : 1;
            unsigned char   VerticalClip : 1;
            unsigned char   ViewAllSlotIndices : 1;
            unsigned char   UseGlyphIgnoreColor : 1;
            unsigned char   ScaleIfWidescreen : 1;
        };
    }               m_Flags;

public:
    TextBox(); // @8FDAA0

    virtual ~TextBox(); // @8FDBE0
    virtual Vector4f* GetBounds(Vector4f& outBounds) const override; // @8FF220

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

    void            SetTextScale(const float* args);    //  @8FD200
    Vector4f*       GetActualBoxSize(Vector4f& outSize) const; // @8FDCD0
    void            SetFont(const char* fontName); // @8FF2F0
    void            ParseFormatting(char* key, float* linescale, int* colorindex, HorizontalAlign* alignment);  //  @8FD570
    const char*     GetText() const;    //  @8FD720
    const char*     GetFont() const;    //  @8FD700
    const int       GetAsciiOffset() const; //  @8FD310
    void            SetAsciiOffset(const int offset);   //  @8FD320
    const bool      GetShouldScaleIfWidescreen() const; //  @8FCE60
    const bool      GetUseGlyphIgnoreColor() const; //  @8FCE50
    const char*     GetTextSlotByName() const;  //  @8FCE80
    void            GetTextColor(ColorRGB& outTextColor) const; //  @8FCF40
    void            GetTextColor2(ColorRGB& outTextColor) const;    //  @8FD080
    void            GetTextScale(Vector4f& outTextScale) const; //  @8FD1C0
    const float     GetHorizontalSpacing() const;   //  @8FD260
    const float     GetVerticalSpacing() const; //  @8FD2A0
    const bool      GetVerticalClip() const;    //  @8FCE20
    const bool      GetViewAllSlotIndicies() const; //  @8FCDF0
    const bool      GetUseSlotIndex() const;    //  @8FCDC0
    const bool      GetTextresMode() const; //  @8FCDB0
    void            SetTextSlotParam(unsigned int slotindex, short* slotvalue);   //  @8FD740
};

ASSERT_CLASS_SIZE(TextBox, 220);