#pragma once
#include "MemoryManager.h"

class SkinnedMeshBuffer
{
protected:
    int field_0;
    int field_4;
    int* field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;

public:
    SkinnedMeshBuffer();
    ~SkinnedMeshBuffer();
};

ASSERT_CLASS_SIZE(SkinnedMeshBuffer, 60);