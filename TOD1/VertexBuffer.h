#pragma once
#include "MemoryManager.h"
#include <map>

struct VertexBuffer
{
public:
    struct VertexDeclaration
    {
        unsigned int m_Stride;
        unsigned int m_FVF;

        VertexDeclaration(unsigned int s, unsigned int f)
            : m_Stride(s), m_FVF(f)
        {};
    };

    unsigned int                m_InitialVerticiesCapacity;
    int                         m_VerticiesTotal;
    int                         m_FVF;
    int                         m_Length;
    char        *m_BufferPtr;
    short                       m_Stride;
    char                        field_16;
    int                         m_FVFIndex;
    int                         m_Flags;
    int                         m_LockMode; //  NOTE: 0 - released, 3 - locked.
    LPDIRECT3DVERTEXBUFFER9     m_Direct3DVertexBuffer;

public:
    VertexBuffer(int FVFindex, int size, int flags); // @464E70
    ~VertexBuffer(); // @465070

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

    char*                       LockAndGetBufferPtr(const int mode); // @464B60
    void                        UnlockBuffer(); //  @464BB0
    int                         SetData(const unsigned int verticies, const void* indata, void* outdata); // @464C00
    void                        CreateDirect3DBuffer(); // @464CC0
    void                        FillFromBufferedData();   //  @464AF0
    char*                       GetBufferPtr() const; //  @464BF0

    static void                 CreateVerticesMap(); // @4651B0
    static void                 DestroyVertexBufferMap();   //  @464D60
    static void                 FillMapFromBuffer();    //  @464DC0

    static const VertexDeclaration VertexDeclarations[]; // @A0ABD0
    static std::map<int, VertexBuffer*>* VertexBufferMap; // @A39F58
};

ASSERT_CLASS_SIZE(VertexBuffer, 40);