#pragma once
#include "MemoryManager.h"
#include "ModelAsset.h"

class MeshBuffer;
class Mesh;

class SkinnedMeshBuffer
{
    friend class Model;
    friend class ModelAsset;
protected:
    MeshBuffer         *m_MeshBuffer;
    Mesh               *m_Mesh;
    std::vector<ModelAsset::TextureSet>    m_TextureSets;   //  TODO: check if list element type is correct.
    std::vector<int>    field_18;
    std::vector<int>    field_28;
    int                 field_38;

public:
    SkinnedMeshBuffer();    //  @85A170
    SkinnedMeshBuffer(const SkinnedMeshBuffer& rhs);    //  @85A1F0

    ~SkinnedMeshBuffer();   //  @857330
};

ASSERT_CLASS_SIZE(SkinnedMeshBuffer, 60);