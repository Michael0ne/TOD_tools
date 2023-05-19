#pragma once
#include "Node.h"

struct Light_Properties
{
    D3DCOLOR                m_Color;
    Vector3f                m_Position;
    Vector3f                m_Direction;
    float                   m_Range;
    float                   m_Brightness;
    union
    {
        struct
        {
            unsigned        LightType : 5;
            unsigned        Enabled : 1;
            unsigned        _6 : 1;
            unsigned        _7 : 1;
            unsigned        _8 : 1;
            unsigned        _9 : 1;
            unsigned        _10 : 1;
            unsigned        _11 : 1;
            unsigned        _12 : 1;
            unsigned        _13 : 1;
            unsigned        _14 : 1;
            unsigned        _15 : 1;
            unsigned        _16 : 1;
            unsigned        _17 : 1;
            unsigned        _18 : 1;
            unsigned        _19 : 1;
            unsigned        _20 : 1;
            unsigned        _21 : 1;
            unsigned        _22 : 1;
            unsigned        _23 : 1;
            unsigned        _24 : 1;
            unsigned        _25 : 1;
            unsigned        _26 : 1;
            unsigned        _27 : 1;
            unsigned        _28 : 1;
            unsigned        _29 : 1;
            unsigned        _30 : 1;
        };
    }                       m_Flags;

public:
    Light_Properties(); // @422100
    ~Light_Properties(); // @422190

    void                SetPosition(const Vector3f& position);  //  @880540
    void                SetDirection(const Vector3f& direction);    //  @8805C0
};

enum class eLightType
{
    NONE = 0,
    AMBIENT = 1,
    DIRECTIONAL = 2,
    POINT = 3
};

class Light : public Node
{
    friend class GfxInternal_Dx9;

    struct LightsList
    {
        std::vector<Light*> m_StaticLights;
        int                *field_10;   //  NOTE: some QuadTree struct?
        int                 field_14;
        int                 m_StaticLightsTotal;
        char                m_LightsOverride;

        LightsList();   //  @882AB0

        void                AddLightToList(Light* light); // @8812A0
        void                RemoveLight(Light* light);  //  @8813C0
        void                _880D90(Node* node);    //  @880D90
        void                SetRange(const Vector4f& lightPos, const float range) const;  //  @880F50
        void                AddStaticLight(Light* light);   //  @89A5D0
        void                RemoveAt(const unsigned int pos);   //  @882330
        void                OverrideLights(const bool overrideLights);  //  @880DC0
    };

protected:
    ColorRGB   m_LightColor;
    ColorRGB   m_StaticColor;
    Light_Properties m_LightProperties;
    union
    {
        struct
        {
            eLightType  Type : 3;
            unsigned    DynamicEmission : 1;
            unsigned    StaticEmission : 1;
            unsigned    NegativeLight : 1;
            unsigned    _6 : 1;
            unsigned    StaticallyLit : 1;
        };
    }                   m_Flags;
    Vector4f   m_Vec_1;
    Vector4f   m_Vec_2;

    short   m_QuadTreeNodeIndex;
    short   field_BE;
    int     field_C0;
    int     field_C4;
    int     field_C8;
    int     field_CC;
    int     field_D0;
    int     field_D4;
    int     field_D8;
    int     field_DC;
    int     field_E0;

public:
    Light(); // @87FDF0
    virtual             ~Light(); // @880680
    virtual void        Destroy() override;    //  @880140
    virtual void        Instantiate() override;  //  @8804C0

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

    const bool          HasDynamicEmission() const; //  @87FDA0
    void                SetHasDynamicEmission(const bool emission); //  @87FF90
    const bool          HasStaticEmission() const;  //  @87FDB0
    void                SetHasStaticEmission(const bool emission);  //  @8800B0
    const eLightType    GetLightType() const;   //  @65AB10
    void                SetLightType(const eLightType ltype); // @880170
    void                GetLightColorRGB(ColorRGB& outColor) const; //  @91CC90
    void                SetLightColorRGB(const ColorRGB& color); // @65AB20
    void                GetStaticColorRGB(ColorRGB& outColor) const;    //  @87FCF0
    void                SetStaticColorRGB(const ColorRGB& color);   //  @87FD30
    const float         GetBrightness() const;  //  @5A1D40
    void                SetBrightness(const float brightness);  //  @728F10
    const float         GetRange() const;   //  @501090
    void                SetRange(const float range);    //  @87FD60
    const bool          IsNegativeLight() const;    //  @87FDC0
    void                SetIsNegativeLight(const bool negative);  //  @87FDD0

    static void         InitLightsList(); // @881070
    static void         ClearLightsList(); // @881260
    static LightsList*  GetGlobalList(); //  @880D80

    static void         Register(); //  @8801F0
    static Light*       Create(AllocatorIndex);   //  @880640

    static Light       *AmbientLight;  // @A3D828
    static Light       *DirectionalLight;  // @A3D82C
    static unsigned int TotalLights; // @A3D81C
    static LightsList  *GlobalList;  // @A3D820
    static int          StaticLights;   //  @A3D824
};

extern EntityType* tLight; // @A3D818

ASSERT_CLASS_SIZE(Light, 228);