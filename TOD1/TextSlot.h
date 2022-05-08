#pragma once
#include "Node.h"

class TextAsset;

class TextSlot : public Node
{
public:
    char       *m_CurrentText;
    unsigned int    m_CurrentIndex;
    AssetLoader m_TextAsset;

public:
    TextSlot() : Node(NODE_MASK_EMPTY) // NOTE: no constructor.
    {
        MESSAGE_CLASS_CREATED(TextSlot);

        m_CurrentText = 0;
        m_CurrentIndex = 0;
    };

    virtual ~TextSlot();    //  @8A9B90

    static void     Register(); //  @8A9C60

    const char*     GetTextRes() const; //  @8A9B30
    void            SetTextRes(const char* text);   //  @8A9C00
    int             GetCurrentIndex() const;    //  @8A9B10
    void            SetCurrentIndex(unsigned int index);   //  @6F89B0
    char*           GetTextStringIdentifier();  //  @8A9FA0
    int             GetNumberOfTextIndicies() const;    //  @8A9B20
    char*           GetCurrentTextContent();    //  @6F8B10
    void            Dump(const int);    //  @8AA090
    void            GetCurrentLoadedText(unsigned short* outText, const unsigned int maxLength) const; //  @8FFCD0
    void            GetTextByIndex(int index, unsigned short* outText, const unsigned int maxLength) const;  //  @8FFD10

    static TextSlot*    Create(AllocatorIndex allocator);   //  @8AA150
};

extern EntityType* tTextSlot;   //  @A3DE00

ASSERT_CLASS_SIZE(TextSlot, 96);