#pragma once
#include "MemoryManager.h"

class Scratchpad
{
private:
    char _pad[1076] = {};
public:
    Scratchpad();
    ~Scratchpad();

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    static void Create();   //  @8C1650
};

extern Scratchpad* g_Scratchpad;

ASSERT_CLASS_SIZE(Scratchpad, 1076);