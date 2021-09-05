#pragma once
#include "Node.h"

class DecalManager : public Node
{
protected:
 int field_50;
 int* m_ResourceInfo;
 int field_58;
 char m_BufferSize;
 char m_PerFrameFillSize;
 __int16 field_5E;
 void* field_60;

public:
 DecalManager(); // NOTE: no constructor.
};

ASSERT_CLASS_SIZE(DecalManager, 100);