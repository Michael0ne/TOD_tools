#pragma once
#include "Node.h"
#include <vector>

class FrameBuffer;
class ModelAsset;
class MeshBuffer_Dx9;
class PhysSystem;

class Cloth : public Node
{
protected:
    Vector4f            m_BoundingRadius;
    FrameBuffer        *m_FrameBuffer;
    ModelAsset         *m_ModelAsset;
    int                 field_68;
    MeshBuffer_Dx9     *m_MeshBuffer;
    PhysSystem         *m_PhysSystem;
    float               m_Weight;
    float               m_Damping;
    float               m_WindInfluence;
    float               m_Opacity;
    union
    {
        struct
        {
            unsigned    TextureSet : 7;
            unsigned    Iterations : 7;
            unsigned    AllwaysUpdate : 1;
            unsigned    AddBlend : 1;
        };
    }                   m_Flags;
    std::vector<int>    m_List_1;
    std::vector<int>    m_List_2;
    int                 field_A8;
    int                 field_AC;
    int                 field_B0;

public:
    Cloth();    //  @92A7E0
    virtual ~Cloth();   //  @92A980

    virtual void    Instantiate() override;  //  @929D70
    virtual void    Update() override;  //  @929DD0
    virtual void    Render() override;  //  @92A500

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    const char          GetActiveTextureSet() const;    //  @929D20
    void                SetActiveTextureSet(const char set);    //  @92A490
    const int           GetNumberOfTexturesSets() const;    //  @929E20
    const float         GetOpacity() const; //  @89AD10
    void                SetOpacity(const float opacity);    //  @8D5040
    const bool          ShouldAddBlend() const; //  @929CF0
    void                SetShouldAddBlend(const bool add);  //  @929D00
    const char*         GetModelRes() const;    //  @92B4E0
    void                SetModelRes(const char* modelres);  //  @92AA30
    const float         GetWeight() const;  //  @905860
    void                SetWeight(const float weight);    //  @92B500
    const float         GetWindInfluence() const;   //  @905880
    void                SetWindInfluence(const float influence);    //  @929C50
    const float         GetDamping() const; //  @905870
    void                SetDamping(const float damping);    //  @929C30
    const int           GetIterations() const;  //  @929C90
    void                SetIterations(const char iterations);    //  @929C70
    const float         GetBoundingRadius() const;  //  @905800
    void                SetBoundingRadius(const float radius);  //  @929CA0
    const bool          ShouldAlwaysUpdate() const; //  @929CE0
    void                SetShouldAlwaysUpdate(const bool update);   //  @929CC0
    void                ApplyImpulse(int* args);    //  @92B530

    static void         Register(); //  @92B0C0

private:
    void                ApplyImpulse_Impl(const Vector4f& v1, const Vector4f& v2, const float force);    //  @929E60

    static Cloth*       Create(AllocatorIndex); //  @92B5A0

    static const float  MaximumImpulse; //  @9D722C
};

extern EntityType* tCloth;  //  @A3E180

ASSERT_CLASS_SIZE(Cloth, 180);