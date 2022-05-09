#pragma once
#include "MemoryManager.h"

class EntityType;
class Entity;

class TransactionBuffer
{
    struct BufferList
    {
        int         m_DataBufferSize;
        int         field_4;
        int         m_TransactionBufferSize;
        EntityType *m_Script;
        int         m_GameTimeMs;
        int         m_RealTimeMs;
    };

public:
    int            *m_Buffer;   //  NOTE: This holds all entities information. Probably, it was a struct or two structs.
    int             m_Chunks;
    int             m_Size;
    int             (*m_ReadCompressedBufferData)(char* outBuffer, int size);
    std::list<BufferList>  m_List_1;
    void           *m_ListHead;
    int             field_20;

public:
    TransactionBuffer(unsigned int bufferSize); // @8AADA0
    ~TransactionBuffer(); // @875210

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    void            _8AA1F0(int** a1); // @8AA1F0
    int             DeleteChangesInBlocks(const int index, const int oldSize, const int blockMask); //  @8AA350

    static float    _A11C90;    //  @A11C90
};

ASSERT_CLASS_SIZE(TransactionBuffer, 36);