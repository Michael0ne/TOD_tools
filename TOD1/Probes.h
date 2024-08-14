#pragma once
#include "stdafx.h"
#include <vector>

class IProbe
{
private:
    float field_4;
    float field_8;
    float field_C;
    float field_10;
    float field_14;
    float field_18;
    float field_1C;
    int field_20;
    int field_24;
    float field_28;
    float field_2C;
    float field_30;

public:
    IProbe();   //  @8C4CD0

    //  VMT@0x9D0A08
    virtual ~IProbe() = default;
    virtual void    stub2() {};
    virtual void    stub3() {};
    virtual void    stub4() {};
    virtual void    stub5() {};
    virtual void    stub6() {};
    virtual void    stub7() {};
    virtual void    stub8() {};
    virtual void    stub9() {};
    virtual void    GetInstance() {};

    virtual void    InstantiateAsset() {};
};

typedef IProbe* (* ProbeConstructorType)();

class Probe
{
private:
    ProbeConstructorType    ProbeConstructor;
    IProbe*                 ProbePtr;
    size_t                  IndexInList;

public:
    Probe(ProbeConstructorType constructorPtr); //  @8C4D70

    static std::vector<Probe*>  List;   //  @A12580
};