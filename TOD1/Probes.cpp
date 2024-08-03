#include "Probes.h"

IProbe::IProbe()
{
    MESSAGE_CLASS_CREATED(IProbe);

    field_4 = 0.0;
    field_8 = 0.0;
    field_C = 0.0;
    field_10 = -1.0;
    field_14 = 0.0;
    field_18 = 0.0;
    field_1C = 0.0;
    field_20 |= 0xFFFFFFu;
    field_24 = 0x2007C00;
    field_24 |= 0x2003C00u;
    field_20 &= ~0x2000000u;
}

Probe::Probe(ProbeConstructorType constructorPtr)
{
    MESSAGE_CLASS_CREATED(Probe);

    ProbeConstructor = constructorPtr;
    IndexInList = List.size();

    List.push_back(this);

    ProbePtr = constructorPtr();

    delete ProbePtr;
}
