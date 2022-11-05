#include "AnimationAsset.h"

AssetInstance* AnimationAsset::Instance;
const char* const AnimationAsset::Bones[] =
{
    "GamePivot", "Bip", "Pelvis", "Spine", "Spine1",
    "Spine2", "Spine3", "Neck", "Head", "Chin", "ChinR",
    "ChinL", "Eyelid", "EyeBR", "EyeBL", "EyeR",
    "EyeL", "L Clavicle", "L UpperArm", "L Forearm",
    "L Hand", "L Finger0", "L Finger01", "L Finger1",
    "L Finger11", "L Finger2", "L Finger21", "L ForeTwist",
    "L ForeTwist1", "R Clavicle", "R UpperArm", "R Forearm",
    "R Hand", "R Finger0", "R Finger01", "R Finger1", "R Finger11",
    "R Finger2", "R Finger21", "R ForeTwist", "R ForeTwist1",
    "L Thigh", "L Calf", "L Foot", "L Toe0", "R Thigh", "R Calf",
    "R Foot", "R Toe0"
};

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

const char* AnimationAsset::GetBoneName(const uint32_t boneIndex) const
{
    const PivotData* pivotData = (PivotData*)ALIGN_4BYTES(field_5C);
    const uint32_t boneNameIndex = (uint32_t)pivotData[boneIndex + 1].field_0 & 0xFFF;
    if (pivotData[boneIndex + 1].field_0 & 0x4000)
        return Bones[boneNameIndex];
    else
        return (const char* const)pivotData + boneNameIndex;
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
