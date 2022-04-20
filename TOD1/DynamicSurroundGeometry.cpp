#include "DynamicSurroundGeometry.h"

EntityType* tDynamicSurroundGeometry;
int DynamicSurroundGeometry::TotalCreated;

#pragma message(TODO_IMPLEMENTATION)
void DynamicSurroundGeometry::Register()
{
    tDynamicSurroundGeometry = new EntityType("DynamicSurroundGeometry");
    tDynamicSurroundGeometry->InheritFrom(tNode);
    tDynamicSurroundGeometry->SetCreator((CREATOR)Create);

    //  ...
}

DynamicSurroundGeometry* DynamicSurroundGeometry::Create(AllocatorIndex)
{
    return new DynamicSurroundGeometry;
}