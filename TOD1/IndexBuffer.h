#pragma once
#include "MemoryManager.h"
#include <map>

struct IndexBuffer
{
public:
    unsigned int    m_TotalIndicies;
    int             m_PrimitiveTypeIndex;
    D3DPRIMITIVETYPE    m_PrimitiveType;
    char           *m_BufferPtr;
    int             field_14;
    int             m_LockMode;
    int             m_Flags;
    IDirect3DIndexBuffer9* m_Direct3DIndexBuffer;

public:
    IndexBuffer(const unsigned int totalindicies, const unsigned int primitivetypeindex, const unsigned int a3);	//	@460780
    IndexBuffer(IndexBuffer* rhs, const int a2);  //  @460840
    ~IndexBuffer();	//	@460960

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

    char*           LockAndGetBufferPtr(const int mode);	//	@460580
    unsigned short* GetBufferPtr() const;   //  @4605B0
    void            UnlockBuffer(); //  @460590

private:
    void            CreateDirect3DBuffer();	//	@4605F0
    void            DestroyDirect3DBuffer();	//	@4605C0
    void            FillFromBufferedData();	//	@460510
    void            CreateFromAssetData(void* asset);   //  @4609D0

private:
    static const D3DPRIMITIVETYPE		PrimitiveTypes[];	//	@A0AA9C

public:
    static std::map<int, IndexBuffer*>*	IndexBufferMap;	//	@A39F38

    static void     CreateIndexBufferMap();	//	@460AB0
    static void     DestroyIndexBufferMap();	//	@460670
    static void     FillMapFromBuffer();	//	@4606D0
};

ASSERT_CLASS_SIZE(IndexBuffer, 32);