#pragma once
#include "Node.h"

enum LightType
{
	LIGHT_TYPE_1 = 1,
	LIGHT_TYPE_2 = 2
};

struct Light_Properties
{
	friend class Light;
protected:
	int					field_0;
	Vector3f			m_Vec_1;
	Vector3f			m_Vec_2;
	float				m_Range;
	float				m_Brightness;
	int					field_24;

public:
	Light_Properties();	//	@422100
	~Light_Properties();	//	@422190
};

struct LightsListStruct
{
	friend class Light;
protected:
	int					m_LightList[4];
	int*				field_10;
	int					field_14;
	int					field_18;
	int					field_1C;
};

class Light : public Node
{
protected:
	ColorRGB			m_LightColor;
	ColorRGB			m_StaticColor;
	Light_Properties	m_LightProperties;
	unsigned int		m_Flags;
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
	virtual ~Light();	//	@880680
	Light();	//	@87FDF0

	void				AddLightToList(void* list, Light* light);	//	@8812A0
	void				SetLightType(LightType);	//	@880170
	void				SetLightColorRGB(const ColorRGB&);	//	@65AB20
	
	static void			OverrideLights(bool unk);	//	@880DC0
	static void			InitLightsList();	//	@881070
	static void			ClearLightsList();	//	@881260

	static Light*		LightType1;		//	@A3D828
	static Light*		LightType2;		//	@A3D82C
	static unsigned int	TotalLights;	//	@A3D81C
	static LightsListStruct	LightsList;		//	@A3D820
};

extern EntityType*	tLight;	//	@A3D818

ASSERT_CLASS_SIZE(Light, 228);