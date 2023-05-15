#pragma once
#include "MemoryManager.h"
#include "Math.h"

class Node;

class QuadTreeInfo
{
    friend class QuadTree;

protected:
    uint8_t*    field_0;
    uint8_t*    field_4;
    uint8_t*    field_8;
    float       field_C;
    float       field_10;
    float       field_14;
    float       field_18;
    float       field_1C;
    int         field_20;
    int         field_24;
    int         field_28;
    int         field_2C;
    int         field_30;
    int         field_34;
    int         field_38;
    int         field_3C;
    int         field_40;
    int         field_44;
    int         field_48;
    int         field_4C;
    int         field_50;
    int         field_54;
    int         field_58;

public:
    QuadTreeInfo(); //  @470DE0

    static QuadTreeInfo* Instance;  //  @A3B57C
};

class QuadTree
{
private:
    int16_t     field_0;
    int32_t     LeafsTotal;
    int32_t     field_8;
    int32_t     field_C;
    int32_t     field_10;
    bool        NoFreeNodes;

public:
    QuadTree(const uint32_t nodes, const uint32_t a2);  //  @89A610
    int16_t     _89A3E0();  //  @89A3E0
    Node*       GetFirstNode() const; //  @488880
    Node*       GetNextNode(const Node* node) const;    //  @488360

    static uint32_t     TreesCreated;   //  @A3DD74
};

ASSERT_CLASS_SIZE(QuadTree, 24);
ASSERT_CLASS_SIZE(QuadTreeInfo, 92);