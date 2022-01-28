#pragma once
#include "Node.h"

class CollisionSphere : public Node
{
protected:
    Vector4f m_Extrusion;
    void* field_60;
    float m_Radius;
    union
    {
        struct
        {
            unsigned    Solid : 1;
            unsigned    BulletProof : 1;
            unsigned    Opaque : 1;
            unsigned    CameraBlock : 1;
            unsigned    Trigger : 1;
            unsigned    NavigationBlock : 1;
            unsigned    Physics : 1;
            unsigned    ActivateTriggers : 1;
        };
        unsigned char   m_CollisionFlags;
    }   m_CollisionFlags;

public:
    CollisionSphere(); // @8C47D0

    const float     GetRadius() const;  //  @905830
    void            SetRadius(const float radius);  //  @571980
    const float     GetExtrusion() const;   //  @89A8A0
    void            SetExtrusion(const float extrusion);    //  @8AE730
    const char      GetCollisionFlags() const;  //  @5719F0
    void            SetCollisionFlags(const char flags); //  @5719B0

    static void     Register(); //  @8C48E0
    static CollisionSphere* Create(AllocatorIndex); //  @8C4B00
};

extern EntityType* tCollisionSphere;    //  @A3DF88

ASSERT_CLASS_SIZE(CollisionSphere, 108);