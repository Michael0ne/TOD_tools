#pragma once
#include "MemoryManager.h"
#include "Types.h"
#include <vector>

class Entity;
class CollisionProbe;

class CollisionList
{
public:
    Entity         *m_Owner;
    Vector3f        m_Position_1;
    Orientation     m_Orientation;
    Vector3f        m_SafePosition;
    Vector3f        m_Position_3;
    Orientation     m_SafeOrientation;
    Vector4f        m_Unknown_1;
    std::vector<int>   m_List_1;
    std::vector<CollisionProbe*> m_CollisionProbesList;
    short           field_78;
    short           m_GlobalIndex;

public:
    CollisionList(Entity* owner); // @8A2D00
    ~CollisionList();   //  @8A2E60

    void            SetListGlobalIndex(const int index);    //  @89FBE0
    void            CommitCollision();  //  @89FC20
};

ASSERT_CLASS_SIZE(CollisionList, 124);