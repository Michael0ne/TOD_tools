#pragma once
#include "StringsPool.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

class MeshBuffer
{
protected:
    Mesh           *m_Mesh;
    int          field_4;
    int          field_8;
    VertexBuffer   *m_VertexBuffer;
    IndexBuffer    *m_IndexBuffer;
    String          field_14;
    float           field_24;
    float           field_28;
    float           field_2C;
    float           field_30;   //  NOTE: adjacency information?

    Vector4f        field_34;   //  NOTE: boundaries?

public:
    MeshBuffer(Mesh* mutablemesh, const int); // @4617D0
    ~MeshBuffer(); // @4616F0

    void* operator new(size_t size)
    {
        return MemoryManager::Released ? nullptr : MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::AllocatorsList[DEFAULT]->Free(ptr);
        ptr = nullptr;
    }

private:
    void            FillFromMesh(const Mesh& mutablemesh, const int);   //  @461090
};

ASSERT_CLASS_SIZE(MeshBuffer, 68);