#include "LensFlare.h"

LensFlare::LensFlare() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(LensFlare);

    m_TextureRes = nullptr;
    field_54 = 1;
    m_Offset = 0.f;
    m_Size = 100.f;
    m_BlendMode = ADD;
    m_FadeCameraAngle1 = 20.f;
    field_68 = cosf(DEG2RAD(20.f));
    m_FadeCameraAngle2 = 40.f;
    field_70 = cosf(DEG2RAD(40.f));
    m_FadeCamBlindDiming = 0.8f;
    m_FadeFlareAngle = 180.f;
    field_90 = cosf(DEG2RAD(180.f));
    m_Color = {1, 1, 1, 1};
    m_InverseFlareAngle = 0;
    m_Opacity = 1.f;
}

LensFlare::~LensFlare()
{
    MESSAGE_CLASS_DESTROYED(LensFlare);
}

void LensFlare::SetColor(const float* args)
{
    m_Color = { args[0], args[1], args[2], 1 };
}

LensFlare* LensFlare::Create(AllocatorIndex)
{
    return new LensFlare;
}