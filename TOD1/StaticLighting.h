#pragma once
#include "Node.h"
#include "MeshColorAsset.h"
#include <vector>

class Scene;

//  NOTE: unused entirely.
class GeometryPhotonTracer
{
private:
    std::vector<int> field_0;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    std::vector<int> field_2C;
    std::vector<int> field_3C;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    std::vector<int> field_5C;
    std::vector<int> field_6C;
    int* field_7C;
    std::vector<int> field_80;
    std::vector<int> field_90;

public:
    GeometryPhotonTracer(const float a1, const int a2); //  @470690
    ~GeometryPhotonTracer();    //  @4707A0

    void    Process();  //  @4709A0
    void    PerformSamplePass1(const int a1, const float a2);   //  @46DD80
    void    PerformSamplePass2(const int a1);   //  @46E2E0
    void    PerformSamplePass3();   //  @470920
};

//  NOTE: this lighting is used on static geometry only (buildings, roads).
class StaticLighting : public Node
{
    enum eLightingMethod
    {
        PHONG = 0,
        PHOTON_MAPPING = 1,
        GEOMETRY_PHOTON_TRACING = 2
    };

    enum eBlockId
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
    eBlockId                m_TargetBlockId;
    eLightingMethod         m_LightingMethod;
    int                     m_SamplesPerVert;
    int                     m_NumPhotons;
    AssetLoader             m_LightingAsset;
    Vector4f                field_84;

public:
    inline StaticLighting() : Node(NODE_MASK_EMPTY)
    {
        MESSAGE_CLASS_CREATED(StaticLighting);

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

    const bool              GetLsep1() const;   //  @42F4F0
    void                    SetLsep1(const int);    //  @883EC0
    const char* const       GetLightingFile() const;    //  @8A9900
    void                    SetLightingFile(const char* const filename);    //  @8A9A70
    const eBlockId          GetTargetBlockId() const;   //  @8A6CE0
    void                    SetTargetBlockId(const eBlockId blockid);   //  @8A6CF0
    const eLightingMethod   GetLightingMethod() const;  //  @8A6D00
    void                    SetLightingMethod(const eLightingMethod method);    //  @8A6D10
    const bool              GetLsep2() const;   //  @42F4F0
    void                    SetLsep2(const int);    //  @883EC0
    const int               GetNumPhotons() const;   //  @8A6D20
    void                    SetNumPhotons(const int numphotons);    //  @8A6D30
    const float             GetChanceOfReflection() const;  //  @905800
    void                    SetChanceOfReflection(const float chance);  //  @8A6D80
    const float             GetGammaCorrection() const; //  @8A6D40
    void                    SetGammaCorrection(const float gammacorr);  //  @8A6DC0
    const float             GetSingleColorMultiplier() const;   //  @905840
    void                    SetSingleColorMultiplier(const float mult); //  @8DDA60
    const bool              GetLsep3() const;   //  @42F4F0
    void                    SetLsep3(const int);    //  @883EC0
    const float             GetMaxSampleRadius() const; //  @89A880
    void                    SetMaxSampleRadius(const float radius); //  @8A6C90
    const int               GetSamplesPerVert() const;  //  @8A6CB0
    void                    SetSamplesPerVert(const int samppervert);   //  @8A6CC0
    const int               GetMinSamplePhotons() const;    //  @900F90
    void                    SetMinSamplePhotons(const int sampphotons); //  @9057D0
    const float             GetMaxLodSampleRadius() const;  //  @89A890
    void                    SetMaxLodSampleRadius(const float sampradius);  //  @8A6CA0
    const bool              GetLsep4() const;   //  @42F4F0
    void                    SetLsep4(const int);    //  @883EC0
    const float             GetCutoffArea() const;  //  @905830
    void                    SetCutoffArea(const float cutoffarea);  //  @8A6CD0
    const bool              GetLsep5() const;   //  @42F4F0
    void                    SetLsep5(const int);    //  @883EC0

    void                    CalculateAll(int* args);    //  @883EC0
    void                    CalculateAnchor(int* args); //  @883EC0
    void                    RedoSampling(int* args);    //  @8A6DB0
    void                    RedoSamplingAnchor(int* args);  //  @8A6DD0
    void                    Save(int* args);    //  @8A6F20
    void                    SaveAs(int* args);   //  @8A6F10

    static void             Register(); //  @8A8E00

private:
    Node*                   _8A6EB0();    //  @8A6EB0
    void                    _8A6E10(Node*, Node*);  //  @8A6E10
    void                    ClearSceneModelsLights(Scene* scene) const;   //  @8A8A40

    static StaticLighting*  Create(AllocatorIndex); // @8A9A30

    static GeometryPhotonTracer*    GeometryPhotonTracer;   //  @A3DDFC
};

extern EntityType* tStaticLighting; //  @A3DDF4

ASSERT_CLASS_SIZE(StaticLighting, 148);
ASSERT_CLASS_SIZE(GeometryPhotonTracer, 160);