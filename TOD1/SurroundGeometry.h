#pragma once
#include "Node.h"
#include <vector>

class FrameBuffer;

class SurroundGeometry : public Node
{
    struct MeshLightingInformation
    {
        std::vector<int32_t>       List1;
        std::vector<int32_t>       List2;
        int32_t                     Enabled;
        int32_t                     field_24;
        int32_t                     field_28;
    };

protected:
    int m_Category;
    std::vector<int>    m_List_1;

    int field_64[320];  //  TODO: this is a struct of something, size is 32 bytes.

    FrameBuffer* m_FrameBuffers[8];
    MeshLightingInformation    m_LightingInformation;
    AssetLoader m_Model[4];
    float m_Properbility1;
    float m_Properbility2;
    float m_Properbility3;
    float m_Properbility4;
    float m_OverallProperbility;
    int m_MaxCount;
    int m_RandomSeed;
    float m_FarDistance;
    float m_CloseDistance;
    float m_DistanceKoefficient;    //  NOTE: koeff = close distance / far distance.
    float m_FarDistanceOriginal;
    float m_FarDistanceKoefficient; //  NOTE: koeff = far distance * distance koefficient.
    float m_Variation;

    bool                SubmitMeshForRender(const uint32_t frameBufferIndex);   //  @8F9CC0

public:
    SurroundGeometry(); // @8F9F10
    virtual             ~SurroundGeometry();    //  @8FA100
    virtual void        Instantiate() override; //  @8F9380
    virtual void        Render() override;  //  @8FB640
    virtual void        DestroyFrameBuffers() override; //  @8F9310

    const int           GetCategory() const;    //  @8F92C0
    void                SetCategory(const int category);    //  @8CC950
    const float         GetFarDistance() const; //  @8F9260
    void                SetFarDistance(const float distance);   //  @8F9270
    const float         GetCloseDistance() const;   //  @8F9230
    void                SetCloseDistance(const float distance); //  @8F9240
    const int           GetMaxCount() const;    //  @8F9290
    void                SetMaxCount(const int count);   //  @8F92A0
    const float         GetVariation() const;   //  @8F9220
    void                SetVariation(const float variation);    //  @8F9200
    const char* const   GetModel1() const;  //  @8F9E30
    void                SetModel1(const char* const model); //  @8FBEB0
    const float         GetProperbility1() const;   //  @8F9160
    void                SetProperbility1(const float properbil);    //  @8F9140
    const char* const   GetModel2() const;  //  @8F9E50
    void                SetModel2(const char* const model); //  @8FBF20
    const float         GetProperbility2() const;   //  @8F9190
    void                SetProperbility2(const float properbil);    //  @8F9170
    const char* const   GetModel3() const;  //  @8F9E70
    void                SetModel3(const char* const model); //  @8FBF90
    const float         GetProperbility3() const;   //  @8F91C0
    void                SetProperbility3(const float properbil);    //  @8F91A0
    const char* const   GetModel4() const;  //  @8F9E90
    void                SetModel4(const char* const model); //  @8FC000
    const float         GetProperbility4() const;   //  @8F91F0
    void                SetProperbility4(const float properbil);    //  @8F91D0
    const float         GetRandomSeed() const;  //  @8F92D0
    void                SetRandomSeed(const float seed);    //  @8F92E0

    void                GenerateRandomSeed(); //  @8F92F0

    static void         Register(); //  @8FC070

    static int          TotalCreated; // @A3E0E0

private:
    static SurroundGeometry*    Create(AllocatorIndex); //  @8FCD40
};

extern EntityType* tSurroundGeometry;  //  @A3E0D8

ASSERT_CLASS_SIZE(SurroundGeometry, 1540);