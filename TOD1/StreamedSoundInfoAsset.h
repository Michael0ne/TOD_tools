#pragma once
#include "BaseAsset.h"

class StreamedSoundInfoAsset : public Asset
{
protected:
    int     field_1C;
    int    *m_MonoStream;
    int     field_24;

private:
    StreamedSoundInfoAsset(); // @85C7E0

public:
    virtual AssetInstance* GetInstancePtr() const override;

    static void             CreateInstance(); // @85C8D0
    static StreamedSoundInfoAsset* Create(); // @85CDA0
};

ASSERT_CLASS_SIZE(StreamedSoundInfoAsset, 40);