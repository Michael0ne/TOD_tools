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
    float               m_ProbeLineThickness;
    Node               *m_MoveCtrl;
    union
    {
        struct
        {
            unsigned    Active : 1;
            unsigned    Looping : 1;
        };
    }                   m_Flags;
    float               m_MaxLookAhead;
    float               m_LastMaxLookAhead;
    std::vector<Vector4f>    m_PathList;
    int                *field_98;
    std::vector<Vector4f>    m_Path2List;
    unsigned int        m_PathPoint;
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
    virtual ~Navigator();   //  @91DC80
    virtual void Update();  //  @91EEE0

private:
    bool                UpdateTarget(const float maxLookAhead);    //  @91D9A0
    void                GetNextTarget(Vector4f& position); //  @91D610
    const Vector4f&     CheckObstacle(const Vector4f& position);    //  @91DD40
    bool                ShouldTurn(const Vector4f& position) const; //  @91D3C0
    bool                IsPointOccluded(const Vector4f& pos1, const Vector4f& pos2) const;  //  @91D020

public:
    Node*               GetMoveCtrl() const;    //  @8A6D20
    void                SetMoveCtrl(int* args); //  @55A4C0
    const bool          IsActive() const;   //  @91CCC0
    void                SetIsActive(const bool active); //  @91D250
    const float         GetMaxLookAhead() const;    //  @91D2E0
    void                SetMaxLookAhead(const float lookahead); //  @91D2F0
    const int           GetMoveCtrlCommand() const; //  @91CCD0
    void                SetMoveCtrlCommand(const int command);  //  @55A4E0
    const float         GetObstacleCheckInterval() const;   //  @91CCE0
    void                SetObstacleCheckInterval(const float interval); //  @91CCF0
    void                GetTargetPoint(Vector4f& outPoint) const;   //  @91CC90
    void                SetTargetPoint(const Vector4f& point);  //  @91CFA0
    void                GetNextTargetPoint(Vector4f& outPoint) const;   //  @91CD80
    void                SetNextTargetPoint(const Vector4f& point);  //  @91CDB0
    const bool          IsLooping() const;  //  @91CDE0
    void                SetIsLooping(const bool looping);   //  @91CDF0
    void*               GetPath2();   //  @91E470
    void                SetPath2(void* path2List);  //  @91E570
    const int           GetPathPoint() const;   //  @91CE30
    void                SetPathPoint(const int pathPoint);  //  @91CE40
    const float         GetPathFraction() const;    //  @91CE60
    void                SetPathFraction(const float pathFraction);  //  @91CE70
    const int           GetPathFinishedEvent() const;   //  @91CD70
    void                SetPathFinishedEvent(const int nevent); //  @55A500
    const int           GetNextObstacleCheckTime() const;   //  @91CEA0
    void                SetNextObstacleCheckTime(const int time);   //  @91CEB0
    const bool          IsObstacleFound() const;    //  @91CED0
    void                SetObstacleFound(const bool found); //  @91CEE0
    void                GetObstacleNormal(Vector4f& outNormal) const;   //  @9012E0
    void                SetObstacleNormal(const Vector4f& normal);  //  @91CF20
    const float         GetObstacleDist() const;    //  @91CF60
    void                SetObstacleDist(const float dist);  //  @91CF70

    void                GoToPoint(int* args);   //  @9206A0
    void                AddPointToPath(int* args);  //  @9206F0
    void                SetWayPointPath(int* args); //  @920670
    void                PathFind(int* args);    //  @920A60
    void                FindNearestSignPost(int* args); //  @91CD20

    static void         Register(); //  @91FD90
    static Navigator*   Create(AllocatorIndex); //  @91DD00

    static int          TurnCommand;    //  @A13388
    static int          StopTurningCommand; //  @A1338C
    static int          StopCommand;    //  @A1337C
    static int          StopMovingCommand;  //  @A13380
    static int          StopTurningACommand;    //  @A13384 //  NOTE: yes, these are separate but will share same index.

private:
    void                StoreAndClearProperties();  //  @91E750
    void                GoToPoint_Impl(const Vector4f& point, const int finishedEvent); //  @91E880
    void                AddPointToPath_Impl(const Vector4f& point); //  @91E9F0
    void                AddPointToPathList(const Vector4f& point);  //  @91E820
    void                SetWayPointPath_Impl(Node* waypoint, const bool looping, const bool a3, const int finishedEvent);   //  @91EAD0
    bool                PathFind_Impl(const Vector4f& point, const int finishedEvent);  //  @91F780
    SignPost*           FindNearestSignPost_Impl(const Vector4f& pos, const float radius);  //  @920CA0

    static DataType*    VectorList; //  @A3E140
};

extern EntityType* tNavigator;  //  @A3E13C

ASSERT_CLASS_SIZE(Navigator, 220);