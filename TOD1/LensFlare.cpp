#include "LensFlare.h"
#include "StringType.h"
#include "IntegerType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "TruthType.h"

EntityType* tLensFlare;

LensFlare::LensFlare() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(LensFlare);

    m_Offset = 0.f;
    m_Size = 100.f;
    m_BlendMode = ADD;
    m_FadeCameraAngleDeg1 = 20.f;
    m_FadeCameraAngleCos1 = cosf(DEG2RAD(20.f));
    m_FadeCameraAngleDeg2 = 40.f;
    m_FadeCameraAngleCos2 = cosf(DEG2RAD(40.f));
    m_FadeCamBlindDiming = 0.8f;
    m_FadeFlareAngle = 180.f;
    m_FadeFlareAngleCos = cosf(DEG2RAD(180.f));
    m_Color = {1, 1, 1, 1};
    m_InverseFlareAngle = 0;
    m_Opacity = 1.f;
}

LensFlare::~LensFlare()
{
    MESSAGE_CLASS_DESTROYED(LensFlare);
}

const char* const LensFlare::GetTexture() const
{
    return m_Texture ? m_Texture.m_AssetPtr->GetName() : nullptr;
}

void LensFlare::SetTexture(const char* const texturepath)
{
    m_Texture = AssetLoader(texturepath);
    m_Id._3 = 1;
}

const LensFlare::eBlendMode LensFlare::GetBlendMode() const
{
    return m_BlendMode;
}

void LensFlare::SetBlendMode(const eBlendMode blendmode)
{
    m_BlendMode = blendmode;
}

const float LensFlare::GetOpacity() const
{
    return m_Opacity;
}

void LensFlare::SetOpacity(const float opacity)
{
    m_Opacity = opacity;
}

const float LensFlare::GetOffset() const
{
    return m_Offset;
}

void LensFlare::SetOffset(const float offset)
{
    m_Offset = offset;
}

void LensFlare::GetColorRGB(ColorRGB& outColor) const
{
    outColor = m_Color;
}

void LensFlare::SetColorRGB(const ColorRGB& color)
{
    m_Color = color;
}

const float LensFlare::GetSize() const
{
    return m_Size;
}

void LensFlare::SetSize(const float size)
{
    m_Size = size;
}

const float LensFlare::GetFadeCameraAngle1() const
{
    return m_FadeCameraAngleDeg1;
}

void LensFlare::SetFadeCameraAngle1(const float angle)
{
    m_FadeCameraAngleDeg1 = angle;
    m_FadeCameraAngleCos1 = cosf(DEG2RAD(angle));
}

const float LensFlare::GetFadeCameraAngle2() const
{
    return m_FadeCameraAngleDeg2;
}

void LensFlare::SetFadeCameraAngle2(const float angle)
{
    m_FadeCameraAngleDeg2 = angle;
    m_FadeCameraAngleCos2 = cosf(DEG2RAD(angle));
}

const float LensFlare::GetFadeCamBlindDiming() const
{
    return m_FadeCamBlindDiming;
}

void LensFlare::SetFadeCamBlindDiming(const float diming)
{
    m_FadeCamBlindDiming = diming;
}

const float LensFlare::GetFadeFlareAngle() const
{
    return m_FadeFlareAngle;
}

void LensFlare::SetFadeFlareAngle(const float angle)
{
    m_FadeFlareAngle = angle;
    m_FadeFlareAngleCos = cosf(DEG2RAD(angle));
}

const bool LensFlare::InverseFlareAngle() const
{
    return m_InverseFlareAngle;
}

void LensFlare::SetInverseFlareAngle(const bool inverse)
{
    m_InverseFlareAngle = inverse;
}

#pragma message(TODO_IMPLEMENTATION)
void LensFlare::Render(FrameBuffer* fb, const Vector2f& leftTop, const Vector2f& bottomRight, const LensFlareManager& manager) const
{
    const auto v21 = leftTop.x - ((bottomRight.x - leftTop.x) * m_Offset);
    const auto v22 = leftTop.y - (bottomRight.y - leftTop.y) * m_Offset;
    auto v10 = 1.0;
    float v16 = 0.f;

    if (m_FadeFlareAngle < 170.f)
    {
        if (m_InverseFlareAngle)
        {
            v10 = 0.0;
            auto v13 = 1.f - fabs(manager.field_94) / (1.f - m_FadeFlareAngleCos);
            if (v13 >= 0.0)
                v10 = v13;
        }
        else
        {
            const auto v11 = manager.field_94 < 0.f ? 0.f : manager.field_94;
            auto v12 = (1.f - v11) / (1.f - m_FadeFlareAngleCos);
            if ((1.f - v12) >= 0.0)
                v10 = 1.f - v12;
            else
                v10 = 0.f;
        }
    }

    if (m_FadeCameraAngleCos1 > 0.99000001 || m_FadeCameraAngleCos1 >= manager.field_90)
    {
        const float v17 = manager.field_90 < 0.f ? 0.f : manager.field_90;

        const float v18 = 1.0 / (float)(this->m_FadeCameraAngleCos1 - this->m_FadeCameraAngleCos2);
        auto v16 = ((1.f - m_FadeCameraAngleCos2) * v18) - ((1.f - v17) * v18);
        v16 = clamp(v16, 0.f, 1.f);
    }
    else
    {
        auto v15 = 0.0;
        if ((1.f - manager.field_90) >= 0.0)
            v15 = 1.f - manager.field_90;
        v16 = (((1.f - m_FadeCamBlindDiming) / (1.f - m_FadeCameraAngleCos1)) * v15) + m_FadeCamBlindDiming;
    }

    if ((v16 * v10) != 0.0)
    {
        fb->SubmitSetOpacityCommand((manager.field_A4 * m_Opacity) * v16 * v10, 0);
        if (m_BlendMode != 1)
            fb->SubmitSetBlendModeCommand(m_BlendMode);

        auto texture = m_Texture.GetAsset<TextureAsset>();
        Vector2f a4 = { manager.field_8C * m_Size, manager.field_8C * m_Size };
        const Vector2f a3 = { v21 - (a4.x * 0.5f), v22 - (a4.y * 0.5f) };
        fb->SubmitRenderTexturedQuad2D_1Command((Texture*)ALIGN_4BYTES(texture->m_Texture_1), a3, a4, m_Color);

        if (m_BlendMode != 1)
            fb->SubmitSetBlendModeCommand(1);
    }
}

void LensFlare::Register()
{
    tLensFlare = new EntityType("LensFlare");
    tLensFlare->InheritFrom(tNode);
    tLensFlare->SetCreator((CREATOR)Create);

    tLensFlare->RegisterProperty(tSTRING, "Texture", (EntityGetterFunction)&GetTexture, (EntitySetterFunction)&SetTexture, "control=resource|type=*.bmp");
    tLensFlare->RegisterProperty(tINTEGER, "BlendMode", (EntityGetterFunction)&GetBlendMode, (EntitySetterFunction)&SetBlendMode, "control=dropdown|Normal=0|Add=1");
    tLensFlare->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1");
    tLensFlare->RegisterProperty(tNUMBER, "Offset", (EntityGetterFunction)&GetOffset, (EntitySetterFunction)&SetOffset, "control=slider|min=-1|max=1");
    tLensFlare->RegisterProperty(tVECTOR, "color_rgb", (EntityGetterFunction)&GetColorRGB, (EntitySetterFunction)&SetColorRGB, "control=colorrgb");
    tLensFlare->RegisterProperty(tNUMBER, "Size", (EntityGetterFunction)&GetSize, (EntitySetterFunction)&SetSize, "control=slider|min=10|max=500");
    tLensFlare->RegisterProperty(tNUMBER, "FadeCameraAngle1", (EntityGetterFunction)&GetFadeCameraAngle1, (EntitySetterFunction)&SetFadeCameraAngle1, "control=slider|min=0|max=90");
    tLensFlare->RegisterProperty(tNUMBER, "FadeCameraAngle2", (EntityGetterFunction)&GetFadeCameraAngle2, (EntitySetterFunction)&SetFadeCameraAngle2, "control=slider|min=0|max=90");
    tLensFlare->RegisterProperty(tNUMBER, "FadeCamBlindDiming", (EntityGetterFunction)&GetFadeCamBlindDiming, (EntitySetterFunction)&SetFadeCamBlindDiming, "control=slider|min=0|max=1");
    tLensFlare->RegisterProperty(tNUMBER, "FadeFlareAngle", (EntityGetterFunction)&GetFadeFlareAngle, (EntitySetterFunction)&SetFadeFlareAngle, "control=slider|min=0|max=90");
    tLensFlare->RegisterProperty(tTRUTH, "InverseFlareAngle", (EntityGetterFunction)&InverseFlareAngle, (EntitySetterFunction)&SetInverseFlareAngle, "control=truth|name=");

    tLensFlare->PropagateProperties();
}

LensFlare* LensFlare::Create(AllocatorIndex)
{
    return new LensFlare;
}