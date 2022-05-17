#include "StreamedSoundInfoAsset.h"
#include "SoundFile.h"

AssetInstance* StreamedSoundInfoAsset::Instance;

StreamedSoundInfoAsset::StreamedSoundInfoAsset() : Asset(false)
{
    MESSAGE_CLASS_CREATED(StreamedSoundInfoAsset);

    m_StreamBuffer = new SoundFile;
}

AssetInstance* StreamedSoundInfoAsset::GetInstancePtr() const
{
    return Instance;
}

void StreamedSoundInfoAsset::CreateInstance()
{
    Instance = new AssetInstance("streamedsoundinfo", (CREATOR)Create);

    Instance->m_FileExtensions.push_back("stream");
    Instance->m_FileExtensions.push_back("ogg");
    Instance->SetAlignment(16, 1);
    Instance->SetAlignment(16, 2);
    Instance->SetAlignment(16, 0);
    Instance->field_2C = 1;
}

StreamedSoundInfoAsset* StreamedSoundInfoAsset::Create()
{
    return new StreamedSoundInfoAsset;
}

StreamedSoundInfoAsset::~StreamedSoundInfoAsset()
{
    MESSAGE_CLASS_DESTROYED(StreamedSoundInfoAsset);
}