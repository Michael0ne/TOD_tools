#pragma once
#include "MeshBuffer.h"
#include <vector>

class MeshBuffer_Dx9 : public MeshBuffer
{
private:
    std::vector<int>	field_44;
    std::vector<int>	field_54;

public:
    MeshBuffer_Dx9(void* a1, int a2);	//	@44BE80
    MeshBuffer_Dx9(const void* mutableBuffer, int a2);	//	@4618D0
    ~MeshBuffer_Dx9();	//	@44BBF0
};

ASSERT_CLASS_SIZE(MeshBuffer_Dx9, 100);