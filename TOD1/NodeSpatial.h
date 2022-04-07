#pragma once
#include "Node.h"

class NodeSpatial : public Node
{
private:
    float m_BoundRadius;

public:
    NodeSpatial();
    virtual Vector4f* GetBounds(Vector4f& outBounds) override;         //  @892130

    const float     GetBoundRadius() const; //  @89A880
    void            SetBoundRadius(const float radius);    //  @5C1560

    static void    Register(); // @8921C0
    static NodeSpatial* Create(AllocatorIndex); // @892160
};

extern EntityType* tNodeSpatial; // @A3D888

ASSERT_CLASS_SIZE(NodeSpatial, 84);