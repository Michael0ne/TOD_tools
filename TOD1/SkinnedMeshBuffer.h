#pragma once
#include "MemoryManager.h"
#include "ModelAsset.h"

class MeshBuffer;
class Mesh;

class SkinnedMeshBuffer
{
    friend class Model;
    friend class ModelAsset;
    friend class Cloth;

    struct TextureRef
    {
        int                 m_Index;
    };

protected:
    MeshBuffer             *m_MeshBuffer;
    Mesh                   *m_Mesh;
    std::vector<TextureRef> m_TextureReferences;
    std::vector<int>        m_TextureSets;
    std::vector<float>      m_EnvMapCoefficients;
    union
    {
        struct
        {
            unsigned        _0 : 1;
            unsigned        _1 : 1;
            unsigned        _2 : 1;
            unsigned        _3 : 1;
            unsigned        _4 : 1;
            unsigned        _5 : 1;
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
    SkinnedMeshBuffer();    //  @85A170
    SkinnedMeshBuffer& operator=(const SkinnedMeshBuffer& rhs);    //  @85A1F0

    ~SkinnedMeshBuffer();   //  @857330
};

ASSERT_CLASS_SIZE(SkinnedMeshBuffer, 60);