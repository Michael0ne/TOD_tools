#pragma once
#include "BaseAsset.h"

class MovieAsset : public Asset
{
protected:
    int                     field_1C;
    int                     field_20;
    int                     field_24;

private:
    MovieAsset() : Asset(false)
    {
        MESSAGE_CLASS_CREATED(MovieAsset);
    }

public:
    virtual                 ~MovieAsset();
    virtual AssetInstance*  GetInstancePtr() const override;    //  @85BC10
    virtual void            DestroyResource() override; //  @85BC30

    static void             CreateInstance(); // @85BC70
    static MovieAsset*      Create(); // @85BC40

private:
    static AssetInstance*   Instance; // @A3BE4C
};

ASSERT_CLASS_SIZE(MovieAsset, 40);