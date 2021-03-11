#pragma once
#include "Node.h"

struct ParticleSystem_Struct96
{
	friend class ParticleSystem;
protected:
	char m_FadeInTime;
	char m_FadeOutTime;
	char m_GlobalWindFactor;
	char m_SizeVariation;
	char m_LifeTimeVariation;
	char field_5;
	char field_6;
	char field_7;
	char field_8;
	char m_OpacityVariation;
	char m_TumbleSpeed;
	char m_TumbleAmp;
	char m_Turbulence;
	char field_D;
	char field_E;
	char field_F;
	char m_BirthRateVariation[4];
	char m_BirthDistance[4];
	char m_SpeedVariation;
	char field_19;
	char field_1A;
	char field_1B;
	char m_DirVariation;
	char m_DirScaleX;
	char m_DirScaleY;
	char m_DirScaleZ;
	char m_RotationVariation;
	char field_21;
	char field_22;
	char field_23;
	char m_BirthDistFade[4];
	char m_InheritEmitterSpeed;
	char m_EmitterDelay;
	char m_EmitterFadeThreshold;
	char m_EmitterRadius;
	float m_f2C;
	int m_Color;
	int m_Flags;
	int m_PriorityLayer;
	__int16 m_MaxParticles;
	__int16 field_3E;
	int field_40;
	int* field_44;
	int* field_48;
	int field_4C;
	int field_50;
	int field_54;
	int field_58;
	__int16 field_5C;
	__int16 field_5E;

	ParticleSystem_Struct96();	//	@425470
	void	_4250C0();	//	@4250C0
};

class ParticleSystem : public Node
{
protected:
	int* m_Texture;
	int field_54;
	int* m_ModelRes;
	int field_5C;
	ParticleSystem_Struct96 field_60;
	int field_C0;
	int m_List_1[4];
	int m_List_2[4];
	int field_E4;
	int field_E8;
	int field_EC;
	int m_IndexInSceneList;

public:
	ParticleSystem();	//	@8EB650

	static bool	LodAndFade;	//	@A08944
};

ASSERT_CLASS_SIZE(ParticleSystem, 244);