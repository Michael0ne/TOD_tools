#pragma once
#include "Node.h"

class FrameBuffer;

enum class eAxisAlign
{
    NONE = 0,
    X_AXIS = 1,
    Y_AXIS = 2,
    Z_AXIS = 3,
    XY_PLANE = 4
};

enum class ePlane
{
    PLANE_0 = 0,
    PLANE_1 = 1,
    PLANE_2 = 2,
    PLANE_3 = 3,
    PLANE_4 = 4
};

enum class eCullMode
{
    NONE = 0,
    BACK = 1
};

enum class eBlendMode
{
    NORMAL = 0,
    ADD = 1
};

enum class eSizeMode
{
    OFF = 0,
    MODE_1 = 1,
    MODE_2 = 2
};

class Sprite : public Node
{
    friend class Progress;

    struct ColorBits
    {
        union
        {
            struct
            {
                unsigned char   Red;
                unsigned char   Green;
                unsigned char   Blue;
                unsigned char   Alpha;
            };
            unsigned int        ColorDword;
        };
    };

protected:
    float               m_v1u;
    float               m_v1v;
    float               m_v2u;
    float               m_v2v;
    float               m_v3u;
    float               m_v3v;
    float               m_v4u;
    float               m_v4v;
    ColorBits           m_ColorRGB_1;
    ColorBits           m_ColorRGB_2;
    ColorBits           m_ColorRGB_3;
    ColorBits           m_ColorRGB_4;
    float               m_SpriteSize_X;
    float               m_SpriteSize_Y;
    AssetLoader         m_Texture;
    float               m_Angle;
    FrameBuffer        *m_FrameBuffer;
    char                m_Opacity;
    char                m_ConstSizeNear;
    char                m_ConstSizeFar;
    char                m_ConstSizeFarShrink;
    union
    {
        struct
        {
            ePlane      PriorityLayer : 4;
            unsigned    Is3D : 1;
            unsigned    IsScreenRelative : 1;
            unsigned    UseVirtualHud : 1;
            unsigned    InfiniteDistance : 1;
            eSizeMode   ConstantSizeMode : 2;
            unsigned    ColorsAllSame : 1;
            unsigned    SpriteIndex : 5;
            unsigned    _16 : 1;
            eBlendMode  BlendMode : 2;
            eCullMode   CullMode : 2;
            eAxisAlign  AxisAlign3D : 3;
            unsigned    _24 : 1;
            unsigned    _25 : 1;
            unsigned    _26 : 1;
            unsigned    _27 : 1;
            unsigned    _28 : 1;
            unsigned    _29 : 1;

            unsigned    _30 : 1;
        };
    }                   m_SpriteFlags;
    union
    {
        unsigned char   MouseEventsEnabled;
        unsigned char   MouseOver;
        unsigned char   c;
        unsigned char   d;
    }                   m_SpriteState;

public:
    Sprite(); // @8F6AA0
    virtual ~Sprite();  //  @8F6C30

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    const char* const   GetTexture() const; //  @8F85A0
    void                SetTexture(const char* const texture);  //  @8F7980
    void                GetSpriteSize(Vector4f& outSize) const; //  @5422D0
    void                SetSpriteSize(const Vector4f& size);    //  @8F3AA0
    const eBlendMode    GetBlendMode() const;   //  @8F2470
    void                SetBlendMode(const eBlendMode blendmode);   //  @8F4990
    const float         GetOpacity() const; //  @664C50
    void                SetOpacity(const float opacity);    //  @8F3FA0
    const bool          Is3D() const;   //  @8F23F0
    void                SetIs3D(const bool is3d);   //  @8F3B30
    const bool          IsScreenRelative() const;   //  @8F2400
    void                SetIsScreenRelative(const bool relative);   //  @8F3B90
    const bool          UseVirtualHud() const;  //  @8F2410
    void                SetUseVirtualHud(const bool use);   //  @8F3BC0
    const eCullMode     GetCullMode() const;    //  @8F27A0
    void                SetCullMode(const eCullMode cullmode);  //  @8F48F0
    const ePlane        GetPriorityLayer() const;   //  @8F2790
    void                SetPriorityLayer(const ePlane layer);   //  @8F48A0
    const bool          IsInfiniteDistance() const; //  @8E38B0
    void                SetIsInfiniteDistance(const bool inf);  //  @8F3C50
    const eSizeMode     GetConstantSizeMode() const;    //  @8F2420
    void                SetConstantSizeMode(const eSizeMode sizemode);  //  @8F3BF0
    const float         GetConstSizeNear() const;   //  @8F2430
    void                SetConstSizeNear(const float sizenear); //  @8F3CB0
    const float         GetConstSizeFar() const;    //  @8F2440
    void                SetConstSizeFar(const float sizefar);   //  @8F3CF0
    const float         GetConstSizeFarShrink() const;  //  @8F2460
    void                SetConstSizeFarShrink(const float farshrink);   //  @8F3D30
    const eAxisAlign    GetAxisAlign3D() const; //  @8F2780
    void                SetAxisAlign3D(const eAxisAlign axisalign); //  @8F4840
    const float         GetAngle() const;   //  @5A1D40
    void                SetAngle(const float angle);    //  @8F3D70
    const float         GetV1U() const; //  @89A880
    void                SetV1U(const float u);  //  @8F3DA0
    const float         GetV1V() const; //  @89A890
    void                SetV1V(const float v);  //  @8F3DE0
    const float         GetV2U() const; //  @89A8A0
    void                SetV2U(const float u);  //  @8F3E20
    const float         GetV2V() const; //  @905800
    void                SetV2V(const float v);  //  @8F3E60
    const float         GetV3U() const; //  @8A6D40
    void                SetV3U(const float u);  //  @8F3EA0
    const float         GetV3V() const; //  @905830
    void                SetV3V(const float v);  //  @8F3EE0
    const float         GetV4U() const; //  @905840
    void                SetV4U(const float u);  //  @8F3F20
    const float         GetV4V() const; //  @905850
    void                SetV4V(const float v);  //  @8F3F60
    void                GetColorRGB1(ColorRGB& outColor) const; //  @8F2480
    void                SetColorRGB1(const ColorRGB& color);    //  @8F4000
    const float         GetColorA1() const; //  @8F2700
    void                SetColorA1(const float alpha);  //  @8F45C0
    void                GetColorRGB2(ColorRGB& outColor) const; //  @8F2520
    void                SetColorRGB2(const ColorRGB& color);    //  @8F4170
    const float         GetColorA2() const; //  @8F2720
    void                SetColorA2(const float alpha);  //  @8F4660
    void                GetColorRGB3(ColorRGB& outColor) const; //  @8F25C0
    void                SetColorRGB3(const ColorRGB& color);    //  @8F42E0
    const float         GetColorA3() const; //  @8F2740
    void                SetColorA3(const float alpha);  //  @8F4700
    void                GetColorRGB4(ColorRGB& outColor) const; //  @8F2660
    void                SetColorRGB4(const ColorRGB& color);    //  @8F4450
    const float         GetColorA4() const; //  @8F2760
    void                SetColorA4(const float alpha);  //  @8F47A0
    const bool          MouseInputEnabled() const;  //  @8F27B0
    void                SetMouseInputEnabled(const bool enabled);   //  @8F27C0
    void                SetTextureFromSprite(int* args);    //  @8F8730

    static void         Register(); //  @8F7A60
    static Sprite*      Create(AllocatorIndex);  //  @8F86C0
    static void         TriggerMouseCallbacks(); // @8F4A00

    static std::vector<Sprite*>* SpritesList; // @A3E0D0
    static int          OnMouseEnterCommand; // @A12E90
    static int          OnMouseExitCommand; // @A12E94
    static int          OnMouseOverCommand; // @A12E98
    static short        SpritesTotal;   //  @A3E0CC
};

extern EntityType* tSprite; //  @A3E0C4

ASSERT_CLASS_SIZE(Sprite, 164);