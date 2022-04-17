#include "WayPoint.h"

EntityType* tWayPoint;

WayPoint::~WayPoint()
{
    MESSAGE_CLASS_DESTROYED(WayPoint);
}

void WayPoint::Register()
{
    tWayPoint = new EntityType("WayPoint");
    tWayPoint->InheritFrom(tNode);
    tWayPoint->SetCreator((CREATOR)Create);

    tWayPoint->PropagateProperties();
}

WayPoint* WayPoint::Create(AllocatorIndex)
{
    return new WayPoint;
}