#include "Track.h"
#include "NumberType.h"
#include "TruthType.h"

EntityType* tTrack;

void Track::Register()
{
 tTrack = new EntityType("Track");
 tTrack->InheritFrom(tNode);
 tTrack->SetCreator((EntityType::CREATOR)Create);

 tTrack->RegisterProperty(tNUMBER, "framerate", &GetFramerate, NULL, NULL, NULL, &SetFramerate, NULL, NULL, NULL, "control=slider|min=0|max=10", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "looping", &GetIsLooping, NULL, NULL, NULL, &SetIsLooping, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "pingpong", &GetIsPingpong, NULL, NULL, NULL, &SetIsPingpong, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "reverse", &GetIsReverse, NULL, NULL, NULL, &SetIsReverse, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "connectends", &GetIsConnectEnds, NULL, NULL, NULL, &SetIsConnectEnds, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "spline", &GetIsSpline, NULL, NULL, NULL, &SetIsSpline, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "customspeed", &GetIsCustomSpeed, NULL, NULL, NULL, &SetIsCustomSpeed, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 tTrack->RegisterProperty(tTRUTH, "showexample", &GetIsShowExample, NULL, NULL, NULL, &SetIsShowExample, NULL, NULL, NULL, "control=truth", NULL, NULL, -1);
 
 tTrack->RegisterScript("updatetracking(entity,number,integer,number):number", &UpdateTracking, NULL, NULL, NULL, nullptr, nullptr);
 tTrack->RegisterScript("getlatestpos:vector", &GetLatestPos, NULL, NULL, NULL, nullptr, nullptr);
 tTrack->RegisterScript("getlatestorient:quaternion", &GetLatestOrient, NULL, NULL, NULL, nullptr, nullptr);
 tTrack->RegisterScript("getlatesttrackpoint:entity", &GetLatestTrackPoint, NULL, NULL, NULL, nullptr, nullptr);
 tTrack->RegisterScript("getlatesttime:number", &GetLatestTime, NULL, NULL, NULL, nullptr, nullptr);
 tTrack->RegisterScript("getnextframenode(entity,integer,truth):entity", &GetNextFrameNode, NULL, NULL, NULL, nullptr, nullptr);
 tTrack->RegisterScript("getlatesttangent:vector", &GetLatestTangent, NULL, NULL, NULL, nullptr, nullptr);

 tTrack->_86E9B0();
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