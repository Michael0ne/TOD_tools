#pragma once

#include "Node.h"

#define STATIC_LIGHTING_CLASS_SIZE 148

enum E_STATICLIGHTING_TARGET_BLOCK_ID
{
	ID_MAIN = 0,
	ID_MAP = 1,
	ID_SUBMAP = 2,
	ID_MISSION = 3,
	ID_CUTSCENE = 4
};

enum E_STATICLIGHTING_LIGHTING_METHOD
{
	METHOD_PHONG = 0,
	METHOD_PHOTON_MAPPING = 1,
	METHOD_GEOMETRY_PHOTON_TRACING = 2
};

class StaticLighting : public Node
{
protected:
	float m_MaxSampleRadius;
	float m_MaxLodSampleRadius;
	int m_MinSamplePhotons;
	float m_ChanceOfReflection;
	float m_GammaCorrection;
	float m_CutoffArea;
	float m_SingleColorMultiplier;
	E_STATICLIGHTING_TARGET_BLOCK_ID m_TargetBlockId;
	E_STATICLIGHTING_LIGHTING_METHOD m_LightingMethod;
	int m_SamplesPerVert;
	int m_NumPhotons;
	int* m_LightingFile;
	int field_80;
	Vector4f m_Vec_1;

public:
	StaticLighting() : Node(NODE_MASK_EMPTY)	//	NOTE: no constructor.
	{
		MESSAGE_CLASS_CREATED(StaticLighting);

		m_LightingFile = nullptr;
		field_80 = 1;
		m_ChanceOfReflection = 0.4f;
		m_LightingMethod = METHOD_PHOTON_MAPPING;
		m_MaxSampleRadius = 10.0f;
		m_MaxLodSampleRadius = 10.0f;
		m_NumPhotons = 1000;
		m_MinSamplePhotons = 100;
		m_Vec_1 = Vector4f();
		m_SamplesPerVert = 1;
		m_GammaCorrection = 1.0f;
		m_TargetBlockId = ID_MAIN;
		m_CutoffArea = 3.0f;
		m_SingleColorMultiplier = 1.0f;
	}
};

static_assert(sizeof(StaticLighting) == STATIC_LIGHTING_CLASS_SIZE, MESSAGE_WRONG_CLASS_SIZE(StaticLighting));