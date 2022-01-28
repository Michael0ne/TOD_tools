#pragma once

#include "Node.h"

#define GUIDEBOX_CLASS_SIZE 100

class GuideBox : public Node
{
protected:
    float m_Left;
    float m_Right;
    float m_Front;
    float m_Back;
    float m_Height;

public:
    inline GuideBox() : Node(NODE_MASK_POSITION)
    {
        MESSAGE_CLASS_CREATED(GuideBox);

        m_Left = m_Front = -1.5f;
        m_Right = m_Back = 1.5;
        m_Height = 3.0f;
    }

    const float     GetLeft() const;    //  @89A880
    void            SetLeft(const float left);  //  @8A6C90

    const float     GetRight() const;   //  @89A890
    void            SetRight(const float right);    //  @8A6CA0

    const float     GetFront() const;   //  @89A8A0
    void            SetFront(const float front);    //  @8E3720

    const float     GetBack() const;    //  @905800
    void            SetBack(const float back);  //  @8E3730

    const float     GetHeight() const;  //  @8A6D40
    void            SetHeight(const float height);  //  @8DDA50

    static void     Register(); //  @8CD7A0
    static GuideBox*    Create(AllocatorIndex); //  @8CD990
};

extern EntityType* tGuideBox;   //  @A3DFCC

ASSERT_CLASS_SIZE(GuideBox, 100);