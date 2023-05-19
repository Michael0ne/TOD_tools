#pragma once
#include "MemoryManager.h"

class EntityType;
class Entity;

class TransactionBuffer
{
    struct BufferList
    {
        int         DataBufferSize;
        int         _f4;
        int         TransactionBufferSize;
        EntityType *ScriptEntity;
        int         GameTimeMs;
        int         RealTimeMs;
    };

public:
    uint32_t       *BufferData;   //  NOTE: This holds all entities information. Probably, it was a struct or two structs.
    uint32_t        ChunksTotal;
    size_t          Size;
    int32_t         (*ReadCompressedBufferData)(char* outBuffer, int size);
    std::list<BufferList>  List_1;
    std::list<BufferList>::iterator* ListHeadPtr;
    int             _f20;

public:
    TransactionBuffer(const uint32_t bufferSize); // @8AADA0
    ~TransactionBuffer(); // @875210

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
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