#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include <vector>

class Mesh
{
    friend class MeshBuffer;

    struct Face
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

        Face(); //  @423140
    };

protected:
    std::vector<Face>           m_FacesList;
    std::vector<unsigned short> m_IndiciesList;
    std::vector<int>            field_20;
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
        }                       m_FlagsBits;
        unsigned int            m_Flags;
    }                           m_Flags;
    int                         m_IsTrianglesList;
    int                         field_58;
    int                         field_5C;
    int                         field_60;
    int                         field_64;
    int                         field_68;

public:
    Mesh(const unsigned int a1, const char a2, const char a3); //  @422330
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

    void                        SetFaceVertexIndex(const unsigned int faceind, const unsigned short vertind); //  @422F60
    void                        AddFace(const unsigned int faceind, const Vector3f& face1, const Vector3f& face2, const Vector2f& texuv);   //  @4230B0
    void                        GetFaceColor(ColorRGB& clr, const unsigned int faceind) const;  //  @4224A0
    const bool                  HasDiffuseFlag() const;  //  @4222F0
    void                        GetFacePositionByIndex(Vector4f& outPos, const unsigned int ind) const; //  @422410
    unsigned short              GetIndiciesListElementByIndex(const unsigned int ind) const;    //  @422400
    void                        GetNormaPositionByIndex(Vector4f& outPos, const unsigned int ind) const;    //  @422440
    void                        GetTexCoordByIndex(Vector2f& outCoords, const unsigned int ind, const TexCoord uv) const; //  @422470

private:
    void                        AddFace1(const unsigned int faceind, const float x, const float y, const float z);  //  @422D90
    void                        AddFace2(const unsigned int faceind, const float x, const float y, const float z);  //  @422E10
};

ASSERT_CLASS_SIZE(Mesh, 108);