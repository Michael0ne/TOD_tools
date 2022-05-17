#include "AnimationAsset.h"

AssetInstance* AnimationAsset::Instance;

AnimationAsset::AnimationAsset() : Asset(false)
{
    MESSAGE_CLASS_CREATED(AnimationAsset);
}

AnimationAsset::~AnimationAsset()
{
    MESSAGE_CLASS_DESTROYED(AnimationAsset);
}

AssetInstance* AnimationAsset::GetInstancePtr() const
{
    return Instance;
}

void AnimationAsset::CreateInstance()
{
    Instance = new AssetInstance("animation", (CREATOR)Create);

    Instance->m_FileExtensions.push_back("animation");
    Instance->m_VerifyChecksum = true;

    Instance->SetAlignment(16, 1);
    Instance->SetAlignment(16, 2);
    Instance->SetAlignment(16, 0);
}

AnimationAsset* AnimationAsset::Create()
{
    return new AnimationAsset;
}
