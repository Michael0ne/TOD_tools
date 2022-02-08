#include "GuideBox.h"
#include "NumberType.h"

EntityType* tGuideBox;

const float GuideBox::GetLeft() const
{
    return m_Left;
}

void GuideBox::SetLeft(const float left)
{
    m_Left = left;
}

const float GuideBox::GetRight() const
{
    return m_Right;
}

void GuideBox::SetRight(const float right)
{
    m_Right = right;
}

const float GuideBox::GetFront() const
{
    return m_Front;
}

void GuideBox::SetFront(const float front)
{
    m_Front = front;
}

const float GuideBox::GetBack() const
{
    return m_Back;
}

void GuideBox::SetBack(const float back)
{
    m_Back = back;
}

const float GuideBox::GetHeight() const
{
    return m_Height;
}

void GuideBox::SetHeight(const float height)
{
    m_Height = height;
}

void GuideBox::Register()
{
    tGuideBox = new EntityType("GuideBox");
    tGuideBox->InheritFrom(tNode);
    tGuideBox->SetCreator((CREATOR)Create);

    tGuideBox->RegisterProperty(tNUMBER, "left", (EntityGetterFunction)&GetLeft, NULL, NULL, NULL, (EntitySetterFunction)&SetLeft, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "right", (EntityGetterFunction)&GetRight, NULL, NULL, NULL, (EntitySetterFunction)&SetRight, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "front", (EntityGetterFunction)&GetFront, NULL, NULL, NULL, (EntitySetterFunction)&SetFront, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "back", (EntityGetterFunction)&GetBack, NULL, NULL, NULL, (EntitySetterFunction)&SetBack, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "height", (EntityGetterFunction)&GetHeight, NULL, NULL, NULL, (EntitySetterFunction)&SetHeight, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);

    tGuideBox->PropagateProperties();
}

GuideBox* GuideBox::Create(AllocatorIndex)
{
    return new GuideBox;
}