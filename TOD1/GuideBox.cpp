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
    tGuideBox->SetCreator((EntityType::CREATOR)Create);

    tGuideBox->RegisterProperty(tNUMBER, "left", &GetLeft, NULL, NULL, NULL, &SetLeft, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "right", &GetRight, NULL, NULL, NULL, &SetRight, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "front", &GetFront, NULL, NULL, NULL, &SetFront, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "back", &GetBack, NULL, NULL, NULL, &SetBack, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);
    tGuideBox->RegisterProperty(tNUMBER, "height", &GetHeight, NULL, NULL, NULL, &SetHeight, NULL, NULL, NULL, "control=slider|min=-20|max=20", NULL, NULL, -1);

    tGuideBox->PropagateProperties();
}

GuideBox* GuideBox::Create(AllocatorIndex)
{
    return new GuideBox;
}