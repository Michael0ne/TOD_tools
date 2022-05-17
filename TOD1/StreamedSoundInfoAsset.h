#pragma once
#include "BaseAsset.h"

class SoundFile;

class StreamedSoundInfoAsset : public Asset
{
public:
    int         field_1C;
    SoundFile  *m_StreamBuffer;
    int         field_24;

private:
    StreamedSoundInfoAsset(); // @85C7E0

    static AssetInstance* Instance; //  @A3BE54

public:
    virtual ~StreamedSoundInfoAsset();  //  @85CD60
    virtual AssetInstance* GetInstancePtr() const override;

    static void             CreateInstance(); // @85C8D0
    static StreamedSoundInfoAsset* Create(); // @85CDA0
};

ASSERT_CLASS_SIZE(StreamedSoundInfoAsset, 40);