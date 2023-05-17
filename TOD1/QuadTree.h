#pragma once
#include "MemoryManager.h"
#include "Math.h"
#include "Types.h"

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

class QuadTreeNode
{
private:
    Node*               Owner;
    QuadTreeNode*       Parent;
    QuadTreeNode*       NextSibling;
    QuadTreeNode*       FirstSibling;
    int16_t             _f10[4];
    uint32_t            _f18;
    uint32_t            UserType;
    uint32_t            _f20;
    uint32_t            _f24;
    uint32_t            _f28;
    uint32_t            _f2C;
    uint32_t            _f30;
    QuadTreeNode*       ContactNode;
    uint32_t*           _f38;
    uint8_t             LodThreshold;
    uint8_t             FadeThreshold;
    int16_t             TraverseDistance;
    Vector4f            Bounds;
    uint8_t             field_4D;
    uint8_t             Lod;
    int8_t              LodFade;
    uint8_t             ContactFilter;
    int32_t             LodDistance;

private:
    const uint8_t       SetLodLevel(const Vector4f&, const uint32_t factor);   //  @89FAE0
    void                UpdateSiblings();   //  @89F540
};

ASSERT_CLASS_SIZE(QuadTreeNode, 88);
ASSERT_CLASS_SIZE(QuadTree, 24);
ASSERT_CLASS_SIZE(QuadTreeInfo, 92);