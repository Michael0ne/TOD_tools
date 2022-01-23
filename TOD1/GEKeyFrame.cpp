#include "GEKeyFrame.h"
#include "NumberType.h"

EntityType* tGEKeyFrame;

const float GEKeyFrame::GetKeyTime() const
{
    return m_KeyTime;
}

void GEKeyFrame::SetKeyTime(const float keytime)
{
    m_KeyTime = keytime;
}

const float GEKeyFrame::GetKeyTranslationX() const
{
    return m_KeyTranslationX;
}

void GEKeyFrame::SetKeyTranslationX(const float translationx)
{
    m_KeyTranslationX = translationx;
}

const float GEKeyFrame::GetKeyTranslationY() const
{
    return m_KeyTranslationY;
}

void GEKeyFrame::SetKeyTranslationY(const float translationy)
{
    m_KeyTranslationY = translationy;
}

const float GEKeyFrame::GetKeyTranslationZ() const
{
    return m_KeyTranslationZ;
}

void GEKeyFrame::SetKeyTranslationZ(const float translationz)
{
    m_KeyTranslationZ = translationz;
}

const float GEKeyFrame::GetKeyScaleX() const
{
    return m_KeyScaleX;
}

void GEKeyFrame::SetKeyScaleX(const float scalex)
{
    m_KeyScaleX = scalex;
}

const float GEKeyFrame::GetKeyScaleY() const
{
    return m_KeyScaleY;
}

void GEKeyFrame::SetKeyScaleY(const float scaley)
{
    m_KeyScaleY = scaley;
}

const float GEKeyFrame::GetKeyScaleZ() const
{
    return m_KeyScaleZ;
}

void GEKeyFrame::SetKeyScaleZ(const float scalez)
{
    m_KeyScaleZ = scalez;
}

const float GEKeyFrame::GetKeyRotationX() const
{
    return m_KeyRotationX;
}

void GEKeyFrame::SetKeyRotationX(const float rotationx)
{
    m_KeyRotationX = rotationx;
}

const float GEKeyFrame::GetKeyRotationY() const
{
    return m_KeyRotationY;
}

void GEKeyFrame::SetKeyRotationY(const float rotationy)
{
    m_KeyRotationY = rotationy;
}

const float GEKeyFrame::GetKeyRotationZ() const
{
    return m_KeyRotationZ;
}

void GEKeyFrame::SetKeyRotationZ(const float rotationz)
{
    m_KeyRotationZ = rotationz;
}

const float GEKeyFrame::GetKeyOpacity() const
{
    return m_KeyOpacity;
}

void GEKeyFrame::SetKeyOpacity(const float opacity)
{
    m_KeyOpacity = opacity;
}

const float GEKeyFrame::GetKeyTextureScroll() const
{
    return m_KeyTextureScroll;
}

void GEKeyFrame::SetKeyTextureScroll(const float scroll)
{
    m_KeyTextureScroll = scroll;
}

void GEKeyFrame::Register()
{
    tGEKeyFrame = new EntityType("GEKeyFrame");
    tGEKeyFrame->InheritFrom(tNode);
    tGEKeyFrame->SetCreator((EntityType::CREATOR)Create);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyTime", &GetKeyTime, NULL, NULL, NULL, &SetKeyTime, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);
    
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyTranslationX", &GetKeyTranslationX, NULL, NULL, NULL, &SetKeyTranslationX, NULL, NULL, NULL, "control=slider|min=-2|max=2", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyTranslationY", &GetKeyTranslationY, NULL, NULL, NULL, &SetKeyTranslationY, NULL, NULL, NULL, "control=slider|min=-2|max=2", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyTranslationZ", &GetKeyTranslationZ, NULL, NULL, NULL, &SetKeyTranslationZ, NULL, NULL, NULL, "control=slider|min=-2|max=2", NULL, NULL, -1);

    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyScaleX", &GetKeyScaleX, NULL, NULL, NULL, &SetKeyScaleX, NULL, NULL, NULL, "control=slider|min=0|max=2", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyScaleY", &GetKeyScaleY, NULL, NULL, NULL, &SetKeyScaleY, NULL, NULL, NULL, "control=slider|min=0|max=2", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyScaleZ", &GetKeyScaleZ, NULL, NULL, NULL, &SetKeyScaleZ, NULL, NULL, NULL, "control=slider|min=0|max=2", NULL, NULL, -1);

    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyRotationX", &GetKeyRotationX, NULL, NULL, NULL, &SetKeyRotationX, NULL, NULL, NULL, "control=slider|min=-3|max=3", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyRotationY", &GetKeyRotationY, NULL, NULL, NULL, &SetKeyRotationY, NULL, NULL, NULL, "control=slider|min=-3|max=3", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyRotationZ", &GetKeyRotationZ, NULL, NULL, NULL, &SetKeyRotationZ, NULL, NULL, NULL, "control=slider|min=-3|max=3", NULL, NULL, -1);

    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyOpacity", &GetKeyOpacity, NULL, NULL, NULL, &SetKeyOpacity, NULL, NULL, NULL, "control=slider|min=0|max=1", NULL, NULL, -1);
    tGEKeyFrame->RegisterProperty(tNUMBER, "KeyTextureScroll", &GetKeyTextureScroll, NULL, NULL, NULL, &SetKeyTextureScroll, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);

    tGEKeyFrame->PropagateProperties();
}

GEKeyFrame* GEKeyFrame::Create(AllocatorIndex)
{
    return new GEKeyFrame;
}