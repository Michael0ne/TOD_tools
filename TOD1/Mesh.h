#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include <vector>

struct CompiledAssetInfo;

struct MeshFace
{
    Vector4f                m_Position;
    Vector2f                m_TexCoord[2];  //  0 - U, 1 - V.
    Vector4f                m_Normal;
    ColorRGB                m_Color;
    int                     field_40;
    int                     field_44;
    int                     field_48;
    int                     field_4C;
    float                   field_50;
    float                   field_54;
    float                   field_58;
    float                   field_5C;
    int                     field_60;

    MeshFace(); //  @423140
};

//  NOTE: 'GameMeshBuffer'? Most likely since this is created first every time.
class Mesh
{
    friend class MeshBuffer;

protected:
    std::vector<MeshFace>       m_FacesList;
    std::vector<unsigned short> m_IndiciesList;
    std::vector<int>            field_20;   //  NOTE: element size is 64 bytes.
    std::vector<int>            field_30;
    std::vector<int>            field_40;
    union
    {
        struct
        {
            unsigned char       HasDiffuse : 1;
            unsigned char       IsBGRColor : 1;
            unsigned char       _2 : 1;
            unsigned char       TexCoordsLevel : 2;
        };
    }                           m_Flags;
    int                         m_IsTrianglesList;
    int                         field_58;
    Vector4f                    field_5C;   //  NOTE: closest face to the center?

public:
    Mesh(const unsigned int isTrianglesList, const bool hasDiffuse, const char TexCoordsLevel); //  @422330
    Mesh(const Mesh& rhs);  //  @422830
    ~Mesh();    //  @421E30

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, true);
        ptr = nullptr;
    }

    enum TexCoord
    {
        U = 0,
        V = 1
    };

    void                        SetVertexIndex(const unsigned int faceind, const unsigned short vertind); //  @422F60
    void                        AddVertex(const unsigned int faceind, const Vector3f& face, const Vector3f& normal, const Vector2f& texuv);   //  @4230B0
    void                        GetFaceColor(ColorRGB& clr, const unsigned int faceind) const;  //  @4224A0
    const bool                  HasDiffuseFlag() const;  //  @4222F0
    void                        GetFacePositionByIndex(Vector4f& outPos, const unsigned int ind) const; //  @422410
    unsigned short              GetIndiciesListElementByIndex(const unsigned int ind) const;    //  @422400
    void                        GetNormaPositionByIndex(Vector4f& outPos, const unsigned int ind) const;    //  @422440
    void                        GetTexCoordByIndex(Vector2f& outCoords, const unsigned int ind, const TexCoord uv) const; //  @422470

    void                        FixAssetFilePointers(CompiledAssetInfo* assetBuffer);   //  @422790

private:
    void                        SetFacePosition(const unsigned int faceind, const float x, const float y, const float z);  //  @422D90
    void                        SetFaceNormal(const unsigned int faceind, const float x, const float y, const float z);  //  @422E10
};

ASSERT_CLASS_SIZE(Mesh, 108);
ASSERT_CLASS_SIZE(MeshFace, 100);