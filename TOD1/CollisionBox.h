#pragma once
#include "Node.h"
#include <vector>

struct CollBoxList
{
    std::vector<int>        field_0;
    int                     field_10;

    #pragma message(TODO_IMPLEMENTATION)
    inline CollBoxList()
    {
        MESSAGE_CLASS_CREATED(CollBoxList);
    }

    inline ~CollBoxList()
    {
        MESSAGE_CLASS_DESTROYED(CollBoxList);
    }
};

class CollisionBox : public Node
{
protected:
    CollBoxList            *m_List;
    float                   m_Width;
    float                   m_Height;
    float                   m_Length;
    union
    {
        struct
        {
            unsigned Solid : 1;
            unsigned BulletProof : 1;
            unsigned Opaque : 1;
            unsigned CameraBlock : 1;
            unsigned Trigger : 1;
            unsigned NavigationBlock : 1;
            unsigned Physics : 1;
            unsigned ActivateTriggers : 1;
        };
        unsigned char       m_Flags;
    }                       m_CollisionFlags;

public:
    CollisionBox(); // @8AEC30
    ~CollisionBox();    //  @8AF0A0

    const float             GetWidth() const;   //  @89A890
    void                    SetWidth(const float width);    //  @8AE710
    const float             GetHeight() const;  //  @89A8A0
    void                    SetHeight(const float height);  //  @8AE730
    const float             GetLength() const;  //  @905800
    void                    SetLength(const float length);  //  @8AE750
    const char              GetCollisionFlags() const;  //  @8AE7B0
    void                    SetCollisionFlags(const char flags);    //  @8AE770

    static void             Register(); //  @8AED20
    static CollisionBox*    Create(AllocatorIndex); //  @8AF070
};

extern EntityType* tCollisionBox;

ASSERT_CLASS_SIZE(CollisionBox, 100);
ASSERT_CLASS_SIZE(CollBoxList, 20);