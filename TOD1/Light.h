#pragma once
#include "Node.h"

struct Light_Properties
{
    D3DCOLOR            m_Color;
    Vector3f			m_Position;
    Vector3f			m_Direction;
    float				m_Range;
    float				m_Brightness;
    int					m_Flags;

public:
    Light_Properties();	//	@422100
    ~Light_Properties();	//	@422190
};

class Light : public Node
{
    friend class GfxInternal_Dx9;

    enum LightType
    {
        AMBIENT = 1,
        DIRECTIONAL = 2,
        POINT = 3,
    };

    struct LightsList
    {
        std::vector<Light*> m_StaticLights;
        int                 field_10;
        int                 field_14;
        int                 m_StaticLightsTotal;
        char                m_LightsOverride;

        LightsList();   //  @882AB0
    };

protected:
    ColorRGB			m_LightColor;
    ColorRGB			m_StaticColor;
    Light_Properties	m_LightProperties;
    union
    {
        struct
        {
            unsigned    Type : 3;
        }               m_FlagsBits;
        unsigned int    m_Flags;
    }                   m_Flags;
    Vector4f			m_Vec_1;
    Vector4f			m_Vec_2;
    int					field_BC;
    int					field_C0;
    int					field_C4;
    int					field_C8;
    int					field_CC;
    int					field_D0;
    int					field_D4;
    int					field_D8;
    int					field_DC;
    int					field_E0;

public:
    Light();	//	@87FDF0
    virtual ~Light();	//	@880680

    void				AddLightToList(void* list, Light* light);	//	@8812A0
    void				SetLightType(LightType);	//	@880170
    void				SetLightColorRGB(const ColorRGB&);	//	@65AB20
    
    static void			OverrideLights(bool unk);	//	@880DC0
    static void			InitLightsList();	//	@881070
    static void			ClearLightsList();	//	@881260
    static void         Register(); //  @8801F0
    static Light*       Create(AllocatorIndex);   //  @880640

    static Light       *AmbientLight;		//	@A3D828
    static Light       *DirectionalLight;		//	@A3D82C
    static unsigned int	TotalLights;	//	@A3D81C
    static LightsList  *GlobalList;		//	@A3D820
};

extern EntityType*	tLight;	//	@A3D818

ASSERT_CLASS_SIZE(Light, 228);