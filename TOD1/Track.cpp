#include "Track.h"
#include "NumberType.h"
#include "TruthType.h"

EntityType* tTrack;

void Track::Register()
{
    tTrack = new EntityType("Track");
    tTrack->InheritFrom(tNode);
    tTrack->SetCreator((CREATOR)Create);

    tTrack->RegisterProperty(tNUMBER, "framerate", (EntityGetterFunction)&GetFramerate, NULL, NULL, NULL, (EntitySetterFunction)&SetFramerate, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "looping", (EntityGetterFunction)&GetIsLooping, NULL, NULL, NULL, (EntitySetterFunction)&SetIsLooping, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "pingpong", (EntityGetterFunction)&GetIsPingpong, NULL, NULL, NULL, (EntitySetterFunction)&SetIsPingpong, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "reverse", (EntityGetterFunction)&GetIsReverse, NULL, NULL, NULL, (EntitySetterFunction)&SetIsReverse, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "connectends", (EntityGetterFunction)&GetIsConnectEnds, NULL, NULL, NULL, (EntitySetterFunction)&SetIsConnectEnds, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "spline", (EntityGetterFunction)&GetIsSpline, NULL, NULL, NULL, (EntitySetterFunction)&SetIsSpline, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "customspeed", (EntityGetterFunction)&GetIsCustomSpeed, NULL, NULL, NULL, (EntitySetterFunction)&SetIsCustomSpeed, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
    tTrack->RegisterProperty(tTRUTH, "showexample", (EntityGetterFunction)&GetIsShowExample, NULL, NULL, NULL, (EntitySetterFunction)&SetIsShowExample, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);

    tTrack->RegisterScript("updatetracking(entity,number,integer,number):number", (EntityFunctionMember)&UpdateTracking, NULL, NULL, NULL, nullptr, nullptr);
    tTrack->RegisterScript("getlatestpos:vector", (EntityFunctionMember)&GetLatestPos, NULL, NULL, NULL, nullptr, nullptr);
    tTrack->RegisterScript("getlatestorient:quaternion", (EntityFunctionMember)&GetLatestOrient, NULL, NULL, NULL, nullptr, nullptr);
    tTrack->RegisterScript("getlatesttrackpoint:entity", (EntityFunctionMember)&GetLatestTrackPoint, NULL, NULL, NULL, nullptr, nullptr);
    tTrack->RegisterScript("getlatesttime:number", (EntityFunctionMember)&GetLatestTime, NULL, NULL, NULL, nullptr, nullptr);
    tTrack->RegisterScript("getnextframenode(entity,integer,truth):entity", (EntityFunctionMember)&GetNextFrameNode, NULL, NULL, NULL, nullptr, nullptr);
    tTrack->RegisterScript("getlatesttangent:vector", (EntityFunctionMember)&GetLatestTangent, NULL, NULL, NULL, nullptr, nullptr);

    tTrack->PropagateProperties();
}

const float Track::GetFramerate() const
{
    return m_Framerate;
}

void Track::SetFramerate(const float fr)
{
    m_Framerate = fr;
}

const bool Track::GetIsLooping() const
{
    return m_TrackFlags.m_FlagBits.Looping;
}

void Track::SetIsLooping(const bool looping)
{
    m_TrackFlags.m_FlagBits.Looping = looping;
}

const bool Track::GetIsPingpong() const
{
    return m_TrackFlags.m_FlagBits.PingPong;
}

void Track::SetIsPingpong(const bool pingpong)
{
    m_TrackFlags.m_FlagBits.PingPong = pingpong;
}

const bool Track::GetIsReverse() const
{
    return m_TrackFlags.m_FlagBits.Reverse;
}

void Track::SetIsReverse(const bool reverse)
{
    m_TrackFlags.m_FlagBits.Reverse = reverse;
}

const bool Track::GetIsConnectEnds() const
{
    return m_TrackFlags.m_FlagBits.ConnectEnds;
}

void Track::SetIsConnectEnds(const bool connectends)
{
    m_TrackFlags.m_FlagBits.ConnectEnds = connectends;
}

const bool Track::GetIsSpline() const
{
    return m_TrackFlags.m_FlagBits.Spline;
}

void Track::SetIsSpline(const bool spline)
{
    m_TrackFlags.m_FlagBits.Spline = spline;
}

const bool Track::GetIsCustomSpeed() const
{
    return m_TrackFlags.m_FlagBits.CustomSpeed;
}

void Track::SetIsCustomSpeed(const bool customspeed)
{
    m_TrackFlags.m_FlagBits.CustomSpeed;
}

const bool Track::GetIsShowExample() const
{
    return m_TrackFlags.m_FlagBits.ShowExample;
}

void Track::SetIsShowExample(const bool showexample)
{
    m_TrackFlags.m_FlagBits.ShowExample;
}

void Track::UpdateTracking(float* args)
{
    *args = UpdateTracking_Impl((Node*)((int)args[1]), (const float)args[2], (const int)args[3], (const float)args[4]);
}

#pragma message(TODO_IMPLEMENTATION)
const float Track::UpdateTracking_Impl(Node* node, const float, const int, const float)
{
    return 0.f;
}

void Track::GetLatestPos(float* args) const
{
    *(Vector4f*)args = m_LatestPos;
}

void Track::GetLatestOrient(float* args) const
{
    *(Orientation*)args = m_LatestOrient;
}

void Track::GetLatestTrackPoint(int* args) const
{
    *args = (int)m_TrackPoint;
}

void Track::GetLatestTime(float* args) const
{
    *args = m_LatestTime;
}

void Track::GetNextFrameNode(int* args) const
{
    *args = (int)GetNextFrameNode_Impl((Node*)args[1], args[2], args[3] != NULL);
}

#pragma message(TODO_IMPLEMENTATION)
Node* Track::GetNextFrameNode_Impl(Node* trackpoint, const int, const bool) const
{
    return nullptr;
}

void Track::GetLatestTangent(float* args) const
{
    *(Vector4f*)args = m_LatestTangent;
}

Track* Track::Create()
{
    return new Track;
}