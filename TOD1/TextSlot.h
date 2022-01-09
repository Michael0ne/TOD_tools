#pragma once
#include "Node.h"

class TextAsset;

class TextSlot : public Node
{
protected:
    char       *m_CurrentText;
    int         m_CurrentIndex;
    TextAsset  *m_TextRes;
    int        *field_5C;

public:
    TextSlot() : Node(NODE_MASK_EMPTY) // NOTE: no constructor.
    {
        MESSAGE_CLASS_CREATED(TextSlot);

        m_TextRes = nullptr;
        field_5C = (int*)1;
        m_CurrentText = 0;
        m_CurrentIndex = 0;
    };
    ~TextSlot();    //  @8A9B90

    static void     Register(); //  @8A9C60

private:
    const char*     GetTextRes() const; //  @8A9B30
    void            SetTextRes(const char* text);   //  @8A9C00
    int             GetCurrentIndex() const;    //  @8A9B10
    void            SetCurrentIndex(unsigned int index);   //  @6F89B0
    char*           GetTextStringIdentifier();  //  @8A9FA0
    int             GetNumberOfTextIndicies() const;    //  @8A9B20
    char*           GetCurrentTextContent();    //  @6F8B10
    void            Dump(const int);    //  @8AA090

    static TextSlot*    Create(AllocatorIndex allocator);   //  @8AA150
};

extern EntityType* tTextSlot;   //  @A3DE00

ASSERT_CLASS_SIZE(TextSlot, 96);