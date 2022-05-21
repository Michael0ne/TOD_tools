#include "TrackPoint.h"
#include "NumberType.h"

EntityType* tTrackPoint;

void TrackPoint::Register()
{
    tTrackPoint = new EntityType("TrackPoint");
    tTrackPoint->InheritFrom(tNode);
    tTrackPoint->SetCreator((CREATOR)Create);

    tTrackPoint->RegisterProperty(tNUMBER, "speed", (EntityGetterFunction)&GetSpeed, (EntitySetterFunction)&SetSpeed, "control=string");

    tTrackPoint->PropagateProperties();
}

TrackPoint* TrackPoint::Create(AllocatorIndex)
{
    return new TrackPoint;
}

#pragma message(TODO_IMPLEMENTATION)
char TrackPoint::ProcessCollision(int, int)
{
    return 0;
}

const float TrackPoint::GetSpeed() const
{
    return m_Speed;
}

void TrackPoint::SetSpeed(const float speed)
{
    m_Speed = speed;
}