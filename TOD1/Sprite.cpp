#include "Sprite.h"
#include "GfxInternal.h"
#include "GfxInternal_Dx9.h"
#include "InputMouse.h"
#include "VectorType.h"
#include "StringType.h"
#include "NumberType.h"
#include "IntegerType.h"
#include "TruthType.h"
#include "FrameBuffer.h"

std::vector<Sprite*>* Sprite::SpritesList;
int Sprite::OnMouseEnterCommand = -1;
int Sprite::OnMouseExitCommand = -1;
int Sprite::OnMouseOverCommand = -1;
short Sprite::SpritesTotal;

EntityType* tSprite;

Sprite::Sprite() : Node(NODE_MASK_POSITION | NODE_MASK_QUADTREE)
{
    MESSAGE_CLASS_CREATED(Sprite);

    m_SpriteSize_X = 64.0f;
    m_SpriteFlags.ColorsAllSame = true;
    m_FrameBuffer = nullptr;
    m_SpriteSize_Y = 64.0f;
    m_SpriteFlags.SpriteIndex = SpritesTotal++;
    m_Opacity = -1;
    m_ConstSizeNear = 10;
    m_ConstSizeFar = 10;
    m_ColorRGB_1.ColorDword = -1;
    m_ColorRGB_2.ColorDword = -1;
    m_ColorRGB_3.ColorDword = -1;
    m_ColorRGB_4.ColorDword = -1;
    m_SpriteFlags.AxisAlign3D = eAxisAlign::XY_PLANE;
    m_Angle = 0.0f;
    m_ConstSizeFarShrink = 25;
    m_SpriteState.MouseEventsEnabled = true;
    m_SpriteState.MouseOver = false;
    m_SpriteState.c = m_SpriteState.d = false;
    m_v1u = 0.f;
    m_v1v = 0.f;
    m_v2u = 0.0f;
    m_v2v = 1.0f;
    m_v3u = 1.f;
    m_v3v = 1.0f;
    m_v4u = 1.0f;
    m_v4v = 0.0f;

    m_QuadTree->m_UserType = m_QuadTree->m_UserType & 0xFFFFFF | m_QuadTree->m_UserType & 0xFF000000 | 0x80000000;

    SpritesList->push_back(this);
}

Sprite::~Sprite()
{
    MESSAGE_CLASS_DESTROYED(Sprite);

    auto spriteListPos = std::find(SpritesList->begin(), SpritesList->end(), this);
    SpritesList->erase(spriteListPos);

    delete m_FrameBuffer;
}

const char* const Sprite::GetTexture() const
{
    return m_Texture ? m_Texture.m_AssetPtr->GetName() : nullptr;
}

void Sprite::SetTexture(const char* const texture)
{
    m_Texture = AssetLoader(texture);
    m_Id._3 = 1;
}

void Sprite::GetSpriteSize(Vector4f& outSize) const
{
    outSize = { m_SpriteSize_X, m_SpriteSize_Y, 1.f, 0.f };
}

void Sprite::SetSpriteSize(const Vector4f& size)
{
    const Vector4f currentSpriteSize(m_SpriteSize_X, m_SpriteSize_Y, 0.f, 0.f);
    StoreProperty(11, &currentSpriteSize, tVECTOR);

    m_SpriteSize_X = size.x;
    m_SpriteSize_Y = size.y;

    m_QuadTree->Refresh();
    m_Id._3 = 1;
}

const eBlendMode Sprite::GetBlendMode() const
{
    return m_SpriteFlags.BlendMode;
}

void Sprite::SetBlendMode(const eBlendMode blendmode)
{
    const eBlendMode currentBlendMode = m_SpriteFlags.BlendMode;
    StoreProperty(12, &currentBlendMode, tINTEGER);

    m_SpriteFlags.BlendMode = blendmode;
    if (currentBlendMode == eBlendMode::ADD)
        SetPriorityLayer(ePlane::PLANE_4);

    m_Id._3 = 1;
}

const float Sprite::GetOpacity() const
{
    return (float)m_Opacity * (1.f / 255.f);
}

void Sprite::SetOpacity(const float opacity)
{
    const float currentOpacity = (float)m_Opacity * (1.f / 255.f);
    StoreProperty(13, &currentOpacity, tNUMBER);

    m_Opacity = (char)(opacity * 255.f);
    m_Id._3 = 1;
}

const bool Sprite::Is3D() const
{
    return m_SpriteFlags.Is3D;
}

void Sprite::SetIs3D(const bool is3d)
{
    const bool current3DStatus = m_SpriteFlags.Is3D;
    StoreProperty(14, &current3DStatus, tTRUTH);

    m_SpriteFlags.Is3D = is3d;
    m_QuadTree->Refresh();
    m_Id._3 = 1;
}

const bool Sprite::IsScreenRelative() const
{
    return m_SpriteFlags.IsScreenRelative;
}

void Sprite::SetIsScreenRelative(const bool relative)
{
    m_SpriteFlags.IsScreenRelative = relative;
    m_QuadTree->Refresh();
    m_Id._3 = 1;
}

const bool Sprite::UseVirtualHud() const
{
    return m_SpriteFlags.UseVirtualHud;
}

void Sprite::SetUseVirtualHud(const bool use)
{
    m_SpriteFlags.UseVirtualHud = use;

    m_QuadTree->Refresh();
    m_Id._3 = 1;
}

const eCullMode Sprite::GetCullMode() const
{
    return m_SpriteFlags.CullMode;
}

void Sprite::SetCullMode(const eCullMode cullmode)
{
    const eCullMode currentCullMode = m_SpriteFlags.CullMode;
    StoreProperty(15, &currentCullMode, tINTEGER);

    m_SpriteFlags.CullMode = cullmode;
    m_Id._3 = 1;
}

const ePlane Sprite::GetPriorityLayer() const
{
    return m_SpriteFlags.PriorityLayer;
}

void Sprite::SetPriorityLayer(const ePlane layer)
{
    const ePlane currentPriorityLayer = m_SpriteFlags.PriorityLayer;
    StoreProperty(16, &currentPriorityLayer, tINTEGER);

    m_SpriteFlags.PriorityLayer = layer;
    m_Id._3 = 1;
}

const bool Sprite::IsInfiniteDistance() const
{
    return m_SpriteFlags.InfiniteDistance;
}

void Sprite::SetIsInfiniteDistance(const bool inf)
{
    const bool currentInfiniteDistance = m_SpriteFlags.InfiniteDistance;
    StoreProperty(17, &currentInfiniteDistance, tTRUTH);

    m_SpriteFlags.InfiniteDistance = inf;
    m_QuadTree->Refresh();
    m_Id._3 = 1;
}

const eSizeMode Sprite::GetConstantSizeMode() const
{
    return m_SpriteFlags.ConstantSizeMode;
}

void Sprite::SetConstantSizeMode(const eSizeMode sizemode)
{
    const eSizeMode currentSizeMode = m_SpriteFlags.ConstantSizeMode;
    StoreProperty(18, &currentSizeMode, tINTEGER);

    m_SpriteFlags.ConstantSizeMode = sizemode;
    m_QuadTree->Refresh();
    m_Id._3 = 1;
}

const float Sprite::GetConstSizeNear() const
{
    return m_ConstSizeNear;
}

void Sprite::SetConstSizeNear(const float sizenear)
{
    StoreProperty(19, &m_ConstSizeNear, tNUMBER);

    m_ConstSizeNear = (char)sizenear;
}

const float Sprite::GetConstSizeFar() const
{
    return m_ConstSizeFar * 10.f;
}

void Sprite::SetConstSizeFar(const float sizefar)
{
    StoreProperty(20, &m_ConstSizeFar, tNUMBER);
    m_ConstSizeFar = (char)(sizefar * 0.1f);
}

const float Sprite::GetConstSizeFarShrink() const
{
    return m_ConstSizeFarShrink;
}

void Sprite::SetConstSizeFarShrink(const float farshrink)
{
    StoreProperty(21, &m_ConstSizeFarShrink, tNUMBER);

    m_ConstSizeFarShrink = (char)farshrink;
}

const eAxisAlign Sprite::GetAxisAlign3D() const
{
    return m_SpriteFlags.AxisAlign3D;
}

void Sprite::SetAxisAlign3D(const eAxisAlign axisalign)
{
    const eAxisAlign currentAxisAlign = m_SpriteFlags.AxisAlign3D;
    StoreProperty(22, &currentAxisAlign, tINTEGER);

    m_SpriteFlags.AxisAlign3D = axisalign;
    m_Id._3 = 1;
}

const float Sprite::GetAngle() const
{
    return m_Angle;
}

void Sprite::SetAngle(const float angle)
{
    StoreProperty(23, &m_Angle, tNUMBER);

    m_Angle = angle;
    m_Id._3 = 1;
}

const float Sprite::GetV1U() const
{
    return m_v1u;
}

void Sprite::SetV1U(const float u)
{
    StoreProperty(24, &m_v1u, tNUMBER);

    m_v1u = u;
    m_Id._3 = 1;
}

const float Sprite::GetV1V() const
{
    return m_v1v;
}

void Sprite::SetV1V(const float v)
{
    StoreProperty(25, &m_v1v, tNUMBER);

    m_v1v = v;
    m_Id._3 = 1;
}

const float Sprite::GetV2U() const
{
    return m_v2u;
}

void Sprite::SetV2U(const float u)
{
    StoreProperty(26, &m_v2u, tNUMBER);

    m_v2u = u;
    m_Id._3 = 1;
}

const float Sprite::GetV2V() const
{
    return m_v2v;
}

void Sprite::SetV2V(const float v)
{
    StoreProperty(27, &m_v2v, tNUMBER);

    m_v2v = v;
    m_Id._3 = 1;
}

const float Sprite::GetV3U() const
{
    return m_v3u;
}

void Sprite::SetV3U(const float u)
{
    StoreProperty(28, &m_v3u, tNUMBER);

    m_v3u = u;
    m_Id._3 = 1;
}

const float Sprite::GetV3V() const
{
    return m_v3v;
}

void Sprite::SetV3V(const float v)
{
    StoreProperty(29, &m_v3v, tNUMBER);

    m_v3v = v;
    m_Id._3 = 1;
}

const float Sprite::GetV4U() const
{
    return m_v4u;
}

void Sprite::SetV4U(const float u)
{
    StoreProperty(30, &m_v4u, tNUMBER);

    m_v4u = u;
    m_Id._3 = 1;
}

const float Sprite::GetV4V() const
{
    return m_v4v;
}

void Sprite::SetV4V(const float v)
{
    StoreProperty(31, &m_v4v, tNUMBER);

    m_v4v = v;
    m_Id._3 = 1;
}

void Sprite::GetColorRGB1(ColorRGB& outColor) const
{
    outColor =
    {
        (float)(m_ColorRGB_1.Red * (1.f / 255.f)),
        (float)(m_ColorRGB_1.Green * (1.f / 255.f)),
        (float)(m_ColorRGB_1.Blue * (1.f / 255.f)),
        (float)(m_ColorRGB_1.Alpha * (1.f / 255.f))
    };
}

void Sprite::SetColorRGB1(const ColorRGB& color)
{
    const ColorRGB currentColor1((float)m_ColorRGB_1.Red * (1.f / 255.f), (float)m_ColorRGB_1.Green * (1.f / 255.f), (float)m_ColorRGB_1.Blue * (1.f / 255.f), (float)m_ColorRGB_1.Alpha * (1.f / 255.f));
    StoreProperty(32, &currentColor1, tVECTOR);

    m_ColorRGB_1.ColorDword = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

const float Sprite::GetColorA1() const
{
    return (float)(m_ColorRGB_1.Alpha * (1.f / 255.f));
}

void Sprite::SetColorA1(const float alpha)
{
    const float currentColor1AlphaValue = (float)(m_ColorRGB_1.Alpha * (1.f / 255.f));
    StoreProperty(33, &currentColor1AlphaValue, tNUMBER);

    m_ColorRGB_1.Alpha = (unsigned char)(alpha * 255.f);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

void Sprite::GetColorRGB2(ColorRGB& outColor) const
{
    outColor =
    {
        (float)(m_ColorRGB_2.Red * (1.f / 255.f)),
        (float)(m_ColorRGB_2.Green * (1.f / 255.f)),
        (float)(m_ColorRGB_2.Blue * (1.f / 255.f)),
        (float)(m_ColorRGB_2.Alpha * (1.f / 255.f))
    };
}

void Sprite::SetColorRGB2(const ColorRGB& color)
{
    const ColorRGB currentColor2((float)m_ColorRGB_2.Red * (1.f / 255.f), (float)m_ColorRGB_2.Green * (1.f / 255.f), (float)m_ColorRGB_2.Blue * (1.f / 255.f), (float)m_ColorRGB_2.Alpha * (1.f / 255.f));
    StoreProperty(34, &currentColor2, tVECTOR);

    m_ColorRGB_2.ColorDword = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

const float Sprite::GetColorA2() const
{
    return (float)(m_ColorRGB_2.Alpha * (1.f / 255.f));
}

void Sprite::SetColorA2(const float alpha)
{
    const float currentColor2AlphaValue = (float)(m_ColorRGB_2.Alpha * (1.f / 255.f));
    StoreProperty(35, &currentColor2AlphaValue, tNUMBER);

    m_ColorRGB_2.Alpha = (unsigned char)(alpha * 255.f);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

void Sprite::GetColorRGB3(ColorRGB& outColor) const
{
    outColor =
    {
        (float)(m_ColorRGB_3.Red * (1.f / 255.f)),
        (float)(m_ColorRGB_3.Green * (1.f / 255.f)),
        (float)(m_ColorRGB_3.Blue * (1.f / 255.f)),
        (float)(m_ColorRGB_3.Alpha * (1.f / 255.f))
    };
}

void Sprite::SetColorRGB3(const ColorRGB& color)
{
    const ColorRGB currentColor3((float)m_ColorRGB_3.Red * (1.f / 255.f), (float)m_ColorRGB_3.Green * (1.f / 255.f), (float)m_ColorRGB_3.Blue * (1.f / 255.f), (float)m_ColorRGB_3.Alpha * (1.f / 255.f));
    StoreProperty(36, &currentColor3, tVECTOR);

    m_ColorRGB_3.ColorDword = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

const float Sprite::GetColorA3() const
{
    return (float)(m_ColorRGB_3.Alpha * (1.f / 255.f));
}

void Sprite::SetColorA3(const float alpha)
{
    const float currentColor3AlphaValue = (float)(m_ColorRGB_3.Alpha * (1.f / 255.f));
    StoreProperty(37, &currentColor3AlphaValue, tNUMBER);

    m_ColorRGB_3.Alpha = (unsigned char)(alpha * 255.f);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

void Sprite::GetColorRGB4(ColorRGB& outColor) const
{
    outColor =
    {
        (float)(m_ColorRGB_4.Red * (1.f / 255.f)),
        (float)(m_ColorRGB_4.Green * (1.f / 255.f)),
        (float)(m_ColorRGB_4.Blue * (1.f / 255.f)),
        (float)(m_ColorRGB_4.Alpha * (1.f / 255.f))
    };
}

void Sprite::SetColorRGB4(const ColorRGB& color)
{
    const ColorRGB currentColor4((float)m_ColorRGB_4.Red * (1.f / 255.f), (float)m_ColorRGB_4.Green * (1.f / 255.f), (float)m_ColorRGB_4.Blue * (1.f / 255.f), (float)m_ColorRGB_4.Alpha * (1.f / 255.f));
    StoreProperty(38, &currentColor4, tVECTOR);

    m_ColorRGB_4.ColorDword = D3DCOLOR_DWORD(color.r, color.g, color.b, color.a);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

const float Sprite::GetColorA4() const
{
    return (float)(m_ColorRGB_4.Alpha * (1.f / 255.f));
}

void Sprite::SetColorA4(const float alpha)
{
    const float currentColor4AlphaValue = (float)(m_ColorRGB_4.Alpha * (1.f / 255.f));
    StoreProperty(39, &currentColor4AlphaValue, tNUMBER);

    m_ColorRGB_4.Alpha = (unsigned char)(alpha * 255.f);
    m_Id._3 = 1;

    m_SpriteFlags.ColorsAllSame =
        m_ColorRGB_1.ColorDword == m_ColorRGB_2.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_3.ColorDword &&
        m_ColorRGB_1.ColorDword == m_ColorRGB_4.ColorDword;
}

const bool Sprite::MouseInputEnabled() const
{
    return m_SpriteState.MouseEventsEnabled;
}

void Sprite::SetMouseInputEnabled(const bool enabled)
{
    m_SpriteState.MouseEventsEnabled = enabled;
}

void Sprite::SetTextureFromSprite(int* args)
{
    if (!args[0])
        return;

    m_Texture.m_AssetPtr = (Asset*)((Sprite*)args[0])->m_Texture.m_AssetPtr;
    m_Id._3 = 1;
}

void Sprite::Register()
{
    tSprite = new EntityType("Sprite");
    tSprite->InheritFrom(tNode);
    tSprite->SetCreator((CREATOR)Create);

    tSprite->RegisterProperty(tSTRING, "texture", (EntityGetterFunction)&GetTexture, (EntitySetterFunction)&SetTexture, "control=resource|type=*.bmp", 10);
    tSprite->RegisterProperty(tVECTOR, "sprite_size", (EntityGetterFunction)&GetSpriteSize, (EntitySetterFunction)&SetSpriteSize, "control=string", 11);
    tSprite->RegisterProperty(tINTEGER, "blendMode", (EntityGetterFunction)&GetBlendMode, (EntitySetterFunction)&SetBlendMode, "control=dropdown|Normal=0|Add=1", 12);
    tSprite->RegisterProperty(tNUMBER, "opacity", (EntityGetterFunction)&GetOpacity, (EntitySetterFunction)&SetOpacity, "control=slider|min=0|max=1", 13);
    tSprite->RegisterProperty(tTRUTH, "is_3d", (EntityGetterFunction)&Is3D, (EntitySetterFunction)&SetIs3D, "control=truth", 14);
    tSprite->RegisterProperty(tTRUTH, "is_screen_relative", (EntityGetterFunction)&IsScreenRelative, (EntitySetterFunction)&SetIsScreenRelative, "control=truth");
    tSprite->RegisterProperty(tTRUTH, "use_virtual_hud", (EntityGetterFunction)&UseVirtualHud, (EntitySetterFunction)&SetUseVirtualHud, "control=truth");
    tSprite->RegisterProperty(tINTEGER, "cullmode", (EntityGetterFunction)&GetCullMode, (EntitySetterFunction)&SetCullMode, "control=dropdown|None=0|Back=1", 15);
    tSprite->RegisterProperty(tINTEGER, "prioritylayer", (EntityGetterFunction)&GetPriorityLayer, (EntitySetterFunction)&SetPriorityLayer, "control=dropdown|Plane0=0|Plane1=1|Plane2=2|Plane3=3|Plane4=4", 16);
    tSprite->RegisterProperty(tTRUTH, "infinite_distance", (EntityGetterFunction)&IsInfiniteDistance, (EntitySetterFunction)&SetIsInfiniteDistance, "control=truth", 17);
    tSprite->RegisterProperty(tINTEGER, "constant_size_mode", (EntityGetterFunction)&GetConstantSizeMode, (EntitySetterFunction)&SetConstantSizeMode, "control=dropdown|Off=0|Mode1=1|Mode2=2", 18);
    tSprite->RegisterProperty(tNUMBER, "const_size_near", (EntityGetterFunction)&GetConstSizeNear, (EntitySetterFunction)&SetConstSizeNear, "control=slider|min=0|max=100", 19);
    tSprite->RegisterProperty(tNUMBER, "const_size_far", (EntityGetterFunction)&GetConstSizeFar, (EntitySetterFunction)&SetConstSizeFar, "control=slider|min=10|max=1000", 20);
    tSprite->RegisterProperty(tNUMBER, "const_size_far_shrink", (EntityGetterFunction)&GetConstSizeFarShrink, (EntitySetterFunction)&SetConstSizeFarShrink, "control=slider|min=0|max=100", 21);
    tSprite->RegisterProperty(tINTEGER, "AxisAlign3d", (EntityGetterFunction)&GetAxisAlign3D, (EntitySetterFunction)&SetAxisAlign3D, "control=dropdown|None=0|X_AXIS=1|Y_AXIS=2|Z_AXIS=3|XY_PLANE=4", 22);
    tSprite->RegisterProperty(tNUMBER, "angle", (EntityGetterFunction)&GetAngle, (EntitySetterFunction)&SetAngle, "control=slider|min=-180|max=180", 23);
    tSprite->RegisterProperty(tNUMBER, "v1u", (EntityGetterFunction)&GetV1U, (EntitySetterFunction)&SetV1U, "control=slider|min=0|max=1", 24);
    tSprite->RegisterProperty(tNUMBER, "v1v", (EntityGetterFunction)&GetV1V, (EntitySetterFunction)&SetV1V, "control=slider|min=0|max=1", 25);
    tSprite->RegisterProperty(tNUMBER, "v2u", (EntityGetterFunction)&GetV2U, (EntitySetterFunction)&SetV2U, "control=slider|min=0|max=1", 26);
    tSprite->RegisterProperty(tNUMBER, "v2v", (EntityGetterFunction)&GetV2V, (EntitySetterFunction)&SetV2V, "control=slider|min=0|max=1", 27);
    tSprite->RegisterProperty(tNUMBER, "v3u", (EntityGetterFunction)&GetV3U, (EntitySetterFunction)&SetV3U, "control=slider|min=0|max=1", 28);
    tSprite->RegisterProperty(tNUMBER, "v3v", (EntityGetterFunction)&GetV3V, (EntitySetterFunction)&SetV3V, "control=slider|min=0|max=1", 29);
    tSprite->RegisterProperty(tNUMBER, "v4u", (EntityGetterFunction)&GetV4U, (EntitySetterFunction)&SetV4U, "control=slider|min=0|max=1", 30);
    tSprite->RegisterProperty(tNUMBER, "v4v", (EntityGetterFunction)&GetV4V, (EntitySetterFunction)&SetV4V, "control=slider|min=0|max=1", 31);
    tSprite->RegisterProperty(tVECTOR, "color_rgb1", (EntityGetterFunction)&GetColorRGB1, (EntitySetterFunction)&SetColorRGB1, "control=colorrgb", 32);
    tSprite->RegisterProperty(tNUMBER, "color_a1", (EntityGetterFunction)&GetColorA1, (EntitySetterFunction)&SetColorA1, "control=slider|min=0|max=1", 33);
    tSprite->RegisterProperty(tVECTOR, "color_rgb2", (EntityGetterFunction)&GetColorRGB2, (EntitySetterFunction)&SetColorRGB2, "control=colorrgb", 34);
    tSprite->RegisterProperty(tNUMBER, "color_a2", (EntityGetterFunction)&GetColorA2, (EntitySetterFunction)&SetColorA2, "control=slider|min=0|max=1", 35);
    tSprite->RegisterProperty(tVECTOR, "color_rgb3", (EntityGetterFunction)&GetColorRGB3, (EntitySetterFunction)&SetColorRGB3, "control=colorrgb", 36);
    tSprite->RegisterProperty(tNUMBER, "color_a3", (EntityGetterFunction)&GetColorA3, (EntitySetterFunction)&SetColorA3, "control=slider|min=0|max=1", 37);
    tSprite->RegisterProperty(tVECTOR, "color_rgb4", (EntityGetterFunction)&GetColorRGB4, (EntitySetterFunction)&SetColorRGB4, "control=colorrgb", 38);
    tSprite->RegisterProperty(tNUMBER, "color_a4", (EntityGetterFunction)&GetColorA4, (EntitySetterFunction)&SetColorA4, "control=slider|min=0|max=1", 39);
    tSprite->RegisterProperty(tTRUTH, "enable_mouse_input", (EntityGetterFunction)&MouseInputEnabled, (EntitySetterFunction)&SetMouseInputEnabled, "control=truth");

    tSprite->RegisterScript("settexturefromsprite(entity)", (EntityFunctionMember)&SetTextureFromSprite);

    tSprite->PropagateProperties();

    SpritesList = new std::vector<Sprite*>;

    OnMouseEnterCommand = GetCommandByName("OnMouseEnter");
    OnMouseExitCommand = GetCommandByName("OnMouseExit");
    OnMouseOverCommand = GetCommandByName("OnMouseOver");
}

Sprite* Sprite::Create(AllocatorIndex)
{
    return new Sprite;
}

void Sprite::TriggerMouseCallbacks()
{
    if (OnMouseEnterCommand == -1)
        OnMouseEnterCommand = GetCommandByName("OnMouseEnter");
    if (OnMouseExitCommand == -1)
        OnMouseExitCommand = GetCommandByName("OnMouseExit");
    if (OnMouseOverCommand == -1)
        OnMouseOverCommand = GetCommandByName("OnMouseOver");

    tagPOINT mousepos = g_GfxInternal_Dx9->m_Windowed ? g_InputMouse->m_WindowedMousePosition : g_InputMouse->m_FullscreenMousePosition;
    ScreenResolution screenres;
    g_GfxInternal->GetScreenResolution(screenres);
    screenres =
    {
     (unsigned int)((float)mousepos.x / ((float)screenres.x * 0.00125f)),
     (unsigned int)((float)mousepos.y / ((float)screenres.y * 0.0016666667f))
    };

    if (!SpritesList->size())
        return;

    for (std::vector<Sprite*>::const_iterator it = SpritesList->cbegin(); it != SpritesList->cend(); ++it)
    {
        if ((*it)->m_SpriteState.MouseEventsEnabled &&
            (*it)->m_SpriteFlags.Is3D == 0 && (((*it)->m_Flags.Disable | (*it)->m_Flags.Invisible) & 1) == 0)
        {
            Vector4f spritepos;
            (*it)->GetPos(spritepos);

            if ((*it)->m_SpriteFlags.IsScreenRelative)
                mousepos = { (LONG)screenres.x, (LONG)screenres.y };

            if (mousepos.x < spritepos.x || mousepos.y < mousepos.y ||
                (mousepos.x + (*it)->m_SpriteSize_X) < mousepos.x || (mousepos.y + (*it)->m_SpriteSize_Y) < mousepos.y)
            {
                if ((*it)->m_SpriteState.MouseEventsEnabled && (*it)->m_SpriteState.MouseOver)
                {
                    (*it)->TriggerGlobalScript(OnMouseExitCommand, 0);
                    (*it)->m_SpriteState.MouseOver = false;
                }
            }
            else
            {
                if ((*it)->m_SpriteState.MouseOver == false)
                {
                    (*it)->TriggerGlobalScript(OnMouseEnterCommand, 0);
                    (*it)->m_SpriteState.MouseOver = true;
                }

                (*it)->TriggerGlobalScript(OnMouseOverCommand, 0);
            }
        }
    }
}