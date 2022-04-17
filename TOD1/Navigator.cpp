#include "Navigator.h"
#include "ScriptDatabase.h"
#include "TruthType.h"
#include "NumberType.h"
#include "VectorType.h"
#include "IntegerType.h"

EntityType* tNavigator;
DataType* Navigator::VectorList;

int Navigator::TurnCommand = -1;
int Navigator::StopTurningCommand = -1;
int Navigator::StopCommand = -1;
int Navigator::StopMovingCommand = -1;
int Navigator::StopTurningACommand = -1;

Navigator::Navigator() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(Navigator);

    m_f70 = 0.60000002f;
    m_f74 = 0.5f;
    m_PathFraction = 0.0f;
    m_MoveCtrl = 0;
    m_MoveCtrlCommand = GetCommandByName("command_run");
    m_Flags.Active = false;
    m_Flags.Looping = false;
    m_ObstacleFound = 0;
    m_NextObstacleCheckTime = 0;
    m_MaxLookAhead = 4.0f;
    m_LastMaxLookAhead = 4.0f;
    m_ObstacleCheckInterval = 500;
    m_PathFinishedEvent = -1;
}

Navigator::~Navigator()
{
    MESSAGE_CLASS_DESTROYED(Navigator);
}

Node* Navigator::GetMoveCtrl() const
{
    return m_MoveCtrl;
}

void Navigator::SetMoveCtrl(int* args)
{
    StoreProperty(10, &m_MoveCtrl, tEntity);
    m_MoveCtrl = (Node*)args[0];
}

const bool Navigator::IsActive() const
{
    return m_Flags.Active;
}

void Navigator::SetIsActive(const bool active)
{
    const bool currentActiveStatus = m_Flags.Active;
    StoreProperty(11, &currentActiveStatus, tTRUTH);

    m_Flags.Active = active;

    if (currentActiveStatus == active)
    {
        StoreProperty(22, &m_NextObstacleCheckTime, tINTEGER);
        m_NextObstacleCheckTime = 0;
    }
    else
    {
        if (m_MoveCtrl)
        {
            m_MoveCtrl->TriggerGlobalScript(StopCommand, nullptr);
            m_MoveCtrl->TriggerGlobalScript(StopTurningACommand, nullptr);
        }
    }
}

const float Navigator::GetMaxLookAhead() const
{
    return m_MaxLookAhead;
}

void Navigator::SetMaxLookAhead(const float lookahead)
{
    StoreProperty(12, &m_MaxLookAhead, tNUMBER);
    m_MaxLookAhead = lookahead;
    m_LastMaxLookAhead = lookahead;
}

const int Navigator::GetMoveCtrlCommand() const
{
    return m_MoveCtrlCommand;
}

void Navigator::SetMoveCtrlCommand(const int command)
{
    StoreProperty(13, &m_MoveCtrlCommand, tINTEGER);
    m_MoveCtrlCommand = command;
}

const float Navigator::GetObstacleCheckInterval() const
{
    return m_ObstacleCheckInterval * 0.001f;
}

void Navigator::SetObstacleCheckInterval(const float interval)
{
    StoreProperty(14, &m_ObstacleCheckInterval, tINTEGER);
    m_ObstacleCheckInterval = (int)(interval * 1000.f);
}

void Navigator::GetTargetPoint(Vector4f& outPoint) const
{
    outPoint = m_TargetPoint;
}

void Navigator::SetTargetPoint(const Vector4f& point)
{
    StoreProperty(22, &m_NextObstacleCheckTime, tINTEGER);
    m_NextObstacleCheckTime = 0;

    StoreProperty(15, &m_TargetPoint, tVECTOR);
    m_TargetPoint = point;

    StoreProperty(16, &m_NextTargetPoint, tVECTOR);
    m_NextTargetPoint = point;
}

void Navigator::GetNextTargetPoint(Vector4f& outPoint) const
{
    outPoint = m_NextTargetPoint;
}

void Navigator::SetNextTargetPoint(const Vector4f& point)
{
    StoreProperty(16, &m_NextTargetPoint, tVECTOR);
    m_NextTargetPoint = point;
}

const bool Navigator::IsLooping() const
{
    return m_Flags.Looping;
}

void Navigator::SetIsLooping(const bool looping)
{
    const bool currentLoopingStatus = m_Flags.Looping;
    StoreProperty(17, &currentLoopingStatus, tTRUTH);

    m_Flags.Looping = looping;
}

void* Navigator::GetPath2()
{
    if (m_PathList.size() > m_Path2List.size())
        m_Path2List.resize(m_PathList.size());

    for (unsigned int i = 0; i < m_PathList.size(); ++i)
        m_Path2List[i] = m_PathList[i];

    return (void*)&m_Path2List;
}

void Navigator::SetPath2(void* path2List)
{
    if ((field_A & 4) == 0 || (field_D[2] & 4) == 0)
    {
        const void* currentPath2List = GetPath2();
        StoreProperty(18, &currentPath2List, VectorList);
    }

    std::vector<Vector4f>& path2ListType = *((std::vector<Vector4f>*)path2List);
    m_PathList.resize(path2ListType.size());

    //  TODO: check this! Not sure about what original code does!
    for (unsigned int i = 0; i < path2ListType.size(); ++i)
        m_PathList[i] = path2ListType[i];
}

const int Navigator::GetPathPoint() const
{
    return m_PathPoint;
}

void Navigator::SetPathPoint(const int pathPoint)
{
    StoreProperty(19, &m_PathPoint, tINTEGER);
    m_PathPoint = pathPoint;
}

const float Navigator::GetPathFraction() const
{
    return m_PathFraction;
}

void Navigator::SetPathFraction(const float pathFraction)
{
    StoreProperty(20, &m_PathFraction, tNUMBER);
    m_PathFraction = pathFraction;
}

const int Navigator::GetPathFinishedEvent() const
{
    return m_PathFinishedEvent;
}

void Navigator::SetPathFinishedEvent(const int nevent)
{
    StoreProperty(21, &m_PathFinishedEvent, tINTEGER);
    m_PathFinishedEvent = nevent;
}

const int Navigator::GetNextObstacleCheckTime() const
{
    return m_NextObstacleCheckTime;
}

void Navigator::SetNextObstacleCheckTime(const int time)
{
    StoreProperty(22, &m_NextObstacleCheckTime, tINTEGER);
    m_NextObstacleCheckTime = time;
}

const bool Navigator::IsObstacleFound() const
{
    return m_ObstacleFound;
}

void Navigator::SetObstacleFound(const bool found)
{
    StoreProperty(23, &m_ObstacleFound, tTRUTH);
    m_ObstacleFound = found;
}

void Navigator::GetObstacleNormal(Vector4f& outNormal) const
{
    outNormal = m_ObstacleNormal;
}

void Navigator::SetObstacleNormal(const Vector4f& normal)
{
    StoreProperty(24, &m_ObstacleNormal, tVECTOR);
    m_ObstacleNormal = normal;
}

const float Navigator::GetObstacleDist() const
{
    return m_ObstacleDist;
}

void Navigator::SetObstacleDist(const float dist)
{
    StoreProperty(25, &m_ObstacleDist, tNUMBER);
    m_ObstacleDist = dist;
}

void Navigator::GoToPoint(int* args)
{
    const Vector4f point((float)args[0], (float)args[1], (float)args[2], 0.f);
    const int finevent = args[3];

    GoToPoint_Impl(point, finevent);
}

void Navigator::AddPointToPath(int* args)
{
    const Vector4f point((float)args[0], (float)args[1], (float)args[2], 0);
    AddPointToPath_Impl(point);
}

void Navigator::SetWayPointPath(int* args)
{
    SetWayPointPath_Impl((Node*)args[0], args[1] != false, args[2] != false, args[3]);
}

void Navigator::PathFind(int* args)
{
    const Vector4f point((float)args[1], (float)args[2], (float)args[3], 0);
    const int finishedEvent = args[4];

    args[0] = PathFind_Impl(point, finishedEvent);
}

void Navigator::FindNearestSignPost(int* args)
{
    const Vector4f pos((float)args[1], (float)args[2], (float)args[3], 0);
    const float radius = (float)args[4];

    args[0] = (int)FindNearestSignPost_Impl(pos, radius);
}

void Navigator::Register()
{
    tNavigator = new EntityType("Navigator");
    tNavigator->InheritFrom(tNode);
    tNavigator->SetCreator((CREATOR)Create);

    tNavigator->RegisterProperty(tEntity, "movectrl", (EntityGetterFunction)&GetMoveCtrl, (EntitySetterFunction)&SetMoveCtrl, nullptr, 10);
    tNavigator->RegisterProperty(tTRUTH, "active", (EntityGetterFunction)&IsActive, (EntitySetterFunction)&SetIsActive, nullptr, 11);
    tNavigator->RegisterProperty(tNUMBER, "maxlookahead", (EntityGetterFunction)&GetMaxLookAhead, (EntitySetterFunction)&SetMaxLookAhead, "control=slider|min=0|max=25", 12);
    tNavigator->RegisterProperty(tINTEGER, "movectrlcommand", (EntityGetterFunction)&GetMoveCtrlCommand, (EntitySetterFunction)&SetMoveCtrlCommand, nullptr, 13);
    tNavigator->RegisterProperty(tNUMBER, "obstaclecheckinterval", (EntityGetterFunction)&GetObstacleCheckInterval, (EntitySetterFunction)&SetObstacleCheckInterval, "control=slider|min=0.1|max=2|step=0.1", 14);
    tNavigator->RegisterProperty(tVECTOR, "TargetPoint", (EntityGetterFunction)&GetTargetPoint, (EntitySetterFunction)&SetTargetPoint, nullptr, 15);
    tNavigator->RegisterProperty(tVECTOR, "NextTargetPoint", (EntityGetterFunction)&GetNextTargetPoint, (EntitySetterFunction)&SetNextTargetPoint, nullptr, 16);
    tNavigator->RegisterProperty(tTRUTH, "looping", (EntityGetterFunction)&IsLooping, (EntitySetterFunction)&SetIsLooping, nullptr, 17);
    VectorList = DataType::LoadScript("list(vector)");
    tNavigator->RegisterProperty(VectorList, "Path2", (EntityGetterFunction)&GetPath2, (EntitySetterFunction)&SetPath2, nullptr, 18);
    tNavigator->RegisterProperty(tINTEGER, "PathPoint", (EntityGetterFunction)&GetPathPoint, (EntitySetterFunction)&SetPathPoint, nullptr, 19);
    tNavigator->RegisterProperty(tNUMBER, "PathFraction", (EntityGetterFunction)&GetPathFraction, (EntitySetterFunction)&SetPathFraction, nullptr, 20);
    tNavigator->RegisterProperty(tINTEGER, "PathFinishedEvent", (EntityGetterFunction)&GetPathFinishedEvent, (EntitySetterFunction)&SetPathFinishedEvent, nullptr, 21);
    tNavigator->RegisterProperty(tINTEGER, "NextObstacleCheckTime", (EntityGetterFunction)&GetNextObstacleCheckTime, (EntitySetterFunction)&SetNextObstacleCheckTime, nullptr, 22);
    tNavigator->RegisterProperty(tTRUTH, "ObstacleFound", (EntityGetterFunction)&IsObstacleFound, (EntitySetterFunction)&SetObstacleFound, nullptr, 23);
    tNavigator->RegisterProperty(tVECTOR, "ObstacleNormal", (EntityGetterFunction)&GetObstacleNormal, (EntitySetterFunction)&SetObstacleNormal, nullptr, 24);
    tNavigator->RegisterProperty(tNUMBER, "ObstacleDist", (EntityGetterFunction)&GetObstacleDist, (EntitySetterFunction)&SetObstacleDist, nullptr, 25);

    tNavigator->RegisterScript("gotopoint(vector,integer)", (EntityFunctionMember)&GoToPoint);
    tNavigator->RegisterScript("addpointtopath(vector)", (EntityFunctionMember)&AddPointToPath);
    tNavigator->RegisterScript("setwaypointpath(entity,truth,truth,integer)", (EntityFunctionMember)&SetWayPointPath);
    tNavigator->RegisterScript("pathfind(vector,integer):truth", (EntityFunctionMember)&PathFind);
    tNavigator->RegisterScript("findnearestsignpost(vector,number):entity", (EntityFunctionMember)&FindNearestSignPost);

    TurnCommand = GetCommandId("command_turn(number)", true);
    StopTurningCommand = GetCommandId("command_stop_turning", true);
    GetCommandId("command_run", true);
    StopCommand = GetCommandId("command_stop", true);
    StopMovingCommand = GetCommandId("command_stop_moving", true);
    StopTurningCommand = GetCommandId("command_stop_turning", true);

    tNavigator->PropagateProperties();
}

Navigator* Navigator::Create(AllocatorIndex)
{
    return new Navigator;
}

void Navigator::StoreAndClearProperties()
{
    if ((field_A & 4) == 0 || (field_D[2] & 4) == 0)
    {
        const void* currentPath2 = GetPath2();
        StoreProperty(18, &currentPath2, VectorList);
    }

    m_PathList.clear();

    const bool currentLoopingStatus = m_Flags.Looping;
    StoreProperty(17, &currentLoopingStatus, tTRUTH);
    m_Flags.Looping = false;

    StoreProperty(19, &m_PathPoint, tINTEGER);
    m_PathPoint = 0;

    const int currentPathFraction = 0;
    StoreProperty(20, &currentPathFraction, tNUMBER);
    m_PathFraction = 0.f;

    StoreProperty(21, &m_PathFinishedEvent, tINTEGER);
    m_PathFinishedEvent = -1;
}

void Navigator::GoToPoint_Impl(const Vector4f& point, const int finishedEvent)
{
    StoreProperty(22, &m_NextObstacleCheckTime, tINTEGER);
    m_NextObstacleCheckTime = 0;

    StoreAndClearProperties();

    DirectX::XMMATRIX mat;
    GetWorldMatrix(mat);

    const Vector4f& currentPosition = *(Vector4f*)(&mat.r[3].m128_f32[0]);
    const Vector4f targetPositionRelative(point.x, point.y + (m_f74 + m_f70), point.z, point.a);
    AddPointToPathList(currentPosition);
    AddPointToPathList(targetPositionRelative);

    StoreProperty(21, &m_PathFinishedEvent, tINTEGER);
    m_PathFinishedEvent = finishedEvent;
}

void Navigator::AddPointToPath_Impl(const Vector4f& point)
{
    const Vector4f pointAdjusted(point.x, point.y + (m_f74 + m_f70), point.z, point.a);
    AddPointToPathList(pointAdjusted);
}

void Navigator::AddPointToPathList(const Vector4f& point)
{
    if (!m_PathList.size())
        SetTargetPoint(point);

    if ((field_A & 4) == 0 || (field_D[2] & 4) == 0)
    {
        const void* currentPath2 = GetPath2();
        StoreProperty(18, &currentPath2, VectorList);
    }

    m_PathList.push_back(point);
}

#pragma message(TODO_IMPLEMENTATION)
void Navigator::SetWayPointPath_Impl(Node* waypoint, const bool looping, const bool a3, const int finishedEvent)
{
    StoreProperty(22, &m_NextObstacleCheckTime, tINTEGER);
    m_NextObstacleCheckTime = 0;
    Node* child = waypoint->m_FirstChild;
    std::vector<Vector4f> childrenPosition(100);

    if (child)
    {
        do
        {
            if (child->m_ScriptEntity)
            {
                ;
            }
            child = child->m_NextSibling;
        } while (child);
    }

    if (a3)
    {
        ;
    }
}

#pragma message(TODO_IMPLEMENTATION)
//  NOTE: this code pretty much doesn't work, since 'FindNearestSignPost' method has no code.
bool Navigator::PathFind_Impl(const Vector4f& point, const int finishedEvent)
{
    StoreProperty(22, &m_NextObstacleCheckTime, tINTEGER);
    m_NextObstacleCheckTime = 0;

    DirectX::XMMATRIX mat;
    GetWorldMatrix(mat);
    const Vector4f& startPos = *(Vector4f*)(&mat.r[3].m128_f32[0]);
    const Vector4f endPos(point.x, point.y + (m_f74 + m_f70), point.z, point.a);

    SignPost* startSignPost = FindNearestSignPost_Impl(startPos, 50.f);
    SignPost* endSignPost = FindNearestSignPost_Impl(endPos, 50.f);

    if (startSignPost)
    {
        ;
    }

    LogDump::LogA("navigator: pathfind: cant find signpost near start point!\n");

    if (!endSignPost)
        LogDump::LogA("navigator: pathfind: cant find signpost near target point!\n");

    return false;
}

SignPost* Navigator::FindNearestSignPost_Impl(const Vector4f& pos, const float radius)
{
    //  TODO: no code is left. Maybe look somewhere for implementation?
    return nullptr;
}