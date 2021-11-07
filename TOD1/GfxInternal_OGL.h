#pragma once
#ifdef OPENGL
#include "MemoryManager.h"
#include <glfw>

class GfxInternal_OGL
{
private:
    GLFWwindow* m_Window;

public:
    GfxInternal_OGL(const ScreenResolution& resolution, unsigned int a2, unsigned int a3, unsigned int FSAA, unsigned int a5);
    ~GfxInternal_OGL();

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
};

extern GfxInternal_OGL* g_GfxInternal_OGL;

ASSERT_CLASS_SIZE(GfxInternal_OGL, 0);
#endif