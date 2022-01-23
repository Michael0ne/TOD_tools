#pragma once
#include "Node.h"

class SignPost;
class EntityType;

class Navigator : public Node
{
protected:
    Vector4f            m_TargetPoint;
    Vector4f            m_NextTargetPoint;
    float               m_f70;
    float               m_f74;
    EntityType         *m_MoveCtrl;
    union
    {
        struct
        {
            unsigned    Active : 1;
            unsigned    Looping : 1;
        };
    }                   m_Flags;
    float               m_MaxLookAhead;
    float               m_f84;
    std::vector<int>    m_PathList;
    int                *field_98;
    std::vector<int>    m_List_2;
    int                 m_PathPoint;
    float               m_PathFraction;
    int                 m_ObstacleCheckInterval;
    int                 m_NextObstacleCheckTime;
    bool                m_ObstacleFound;
    Vector4f            m_ObstacleNormal;
    float               m_ObstacleDist;
    int                 m_PathFinishedEvent;
    int                 m_MoveCtrlCommand;
public:
    Navigator(); // @91DB30

private:
    void      FindNearestSignPost(int* args) const; // @91CD20
    static SignPost* FindNearestSignPost_Impl(const Vector4f& pos, const float radius); // @920CA0
};

ASSERT_CLASS_SIZE(Navigator, 220);