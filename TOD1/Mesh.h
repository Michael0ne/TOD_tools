#pragma once
#include "Types.h"
#include "MemoryManager.h"
#include <vector>

class Mesh
{
    struct Face
    {
        Vector4f                field_0;
        Vector2f                field_10;
        float                   field_18;
        float                   field_1C;
        Vector4f                field_20;
        ColorRGB                field_30;
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

private:
    std::vector<Face>           m_FacesList;
    std::vector<unsigned short> m_IndiciesList;
    std::vector<int>            field_20;
    std::vector<int>            field_30;
    std::vector<int>            field_40;
    int                         field_50;
    int                         field_54;
    int                         field_58;
    int                         field_5C;
    int                         field_60;
    int                         field_64;
    int                         field_68;

public:
    Mesh(const unsigned int a1, const char a2, const char a3); //  @422330
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

    void                        SetFaceVertexIndex(const unsigned int faceind, const unsigned short vertind); //  @422F60
    void                        AddFace(const unsigned int faceind, const Vector3f& face1, const Vector3f& face2, const Vector2f& texuv);   //  @4230B0
    void                        GetFaceColor(ColorRGB& clr, const unsigned int faceind) const;  //  @4224A0

private:
    void                        AddFace1(const unsigned int faceind, const float x, const float y, const float z);  //  @422D90
    void                        AddFace2(const unsigned int faceind, const float x, const float y, const float z);  //  @422E10
};

ASSERT_CLASS_SIZE(Mesh, 108);