#pragma once
#include "MeshBuffer.h"
#include <vector>

class MeshBuffer_Dx9 : public MeshBuffer
{
    struct Skin
    {
        int	field_0;
        int	field_4;
        int	field_8;
        int	field_C;
        int	field_10;
        int	field_14;
        int	field_18;
        int	field_1C;
        int	field_20;
        int	field_24;
        int	field_28;
        int	field_2C;
        int	field_30;
        int	field_34;
        int	field_38;
        int	field_3C;
        int	field_40;
        int	field_44;
        int	field_48;
        int	field_4C;
        int	field_50;
        int	field_54;
        int	field_58;
        int	field_5C;

    };

private:
    std::vector<Skin>	field_44;   //  TODO: i dunno what this is.
    std::vector<int>	field_54;

public:
    MeshBuffer_Dx9(void* a1, int a2);	//	@44BE80
    MeshBuffer_Dx9(const void* mutableBuffer, int a2);	//	@4618D0
    ~MeshBuffer_Dx9();	//	@44BBF0

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

    void                SetMeshDataAsCurrent() const;   //  @462F20
    void                SWSkin();   //  @462690
    char*               LockAndGetVertexBufferPtr(const int mode);  //  @44BCA0
};

ASSERT_CLASS_SIZE(MeshBuffer_Dx9, 100);