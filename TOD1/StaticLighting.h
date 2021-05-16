#pragma once
#include "Node.h"
#include "MeshColorAsset.h"

//  NOTE: this lighting is used on static geometry only (buildings, roads).
class StaticLighting : public Node
{
    enum LightingMethod
    {
        PHONG = 0,
        PHOTON_MAPPING = 1,
        GEOMETRY_PHOTON_TRACING = 2
    };

    enum BlockId
    {
        MAIN = 0,
        MAP = 1,
        SUBMAP = 2,
        MISSION = 3,
        CUTSCENE = 4
    };

protected:
    float                   m_MaxSampleRadius;
    float                   m_MaxLodSampleRadius;
    int                     m_MinSamplePhotons;
    float                   m_ChanceOfReflection;
    float                   m_GammaCorrection;
    float                   m_CutoffArea;
    float                   m_SingleColorMultiplier;
    BlockId                 m_TargetBlockId;
    LightingMethod          m_LightingMethod;
    int                     m_SamplesPerVert;
    int                     m_NumPhotons;
    MeshColorAsset         *m_LightingFile;
    int                     field_80;
    Vector4f                field_84;

public:
    inline StaticLighting() : Node(NODE_MASK_EMPTY)
    {
        MESSAGE_CLASS_CREATED(StaticLighting);

        m_LightingFile = nullptr;
        field_80 = 1;
        m_ChanceOfReflection = 0.4f;
        m_LightingMethod = PHOTON_MAPPING;
        m_MaxSampleRadius = 10.0f;
        m_MaxLodSampleRadius = 10.0f;
        m_NumPhotons = 1000;
        m_MinSamplePhotons = 100;
        m_SamplesPerVert = 1;
        m_GammaCorrection = 1;
        m_TargetBlockId = MAIN;
        m_CutoffArea = 3.0f;
        m_SingleColorMultiplier = 1;
    }

    virtual                 ~StaticLighting();  //  @8A8D40
    virtual void            Instantiate();  //  @8A6F30

    static void             Register(); //  @8A8E00

private:
    Node*                   _8A6EB0();    //  @8A6EB0
    void                    _8A6E10(Node*, Node*);  //  @8A6E10

    static StaticLighting*  Create(AllocatorIndex);	//	@8A9A30
};

extern EntityType* tStaticLighting; //  @A3DDF4

ASSERT_CLASS_SIZE(StaticLighting, 148);