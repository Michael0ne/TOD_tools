#include "RealtimeFolder.h"

RealtimeFolder* RealtimeFolder::Instance;
EntityType* tRealtimeFolder;

RealtimeFolder* RealtimeFolder::Create(AllocatorIndex)
{
    return new RealtimeFolder;
}

void RealtimeFolder::Register()
{
    tRealtimeFolder = new EntityType("RealtimeFolder");
    tRealtimeFolder->InheritFrom(tFolder);
    tRealtimeFolder->SetCreator((CREATOR)Create);

    tRealtimeFolder->PropagateProperties();
}