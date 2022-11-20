#include "AnimationAsset.h"
#include "BuiltinType.h"

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

void AnimationAsset::DestroyResource()
{
    FramesTotal = 0;
}

const char* AnimationAsset::GetBoneName(const uint32_t boneIndex) const
{
    const PivotData* pivotData = (PivotData*)ALIGN_4BYTES(field_5C);
    const uint32_t boneNameIndex = pivotData[boneIndex + 1].Data.BoneIndexOrOffset;
    const bool isBoneIndex = pivotData[boneIndex + 1].Data.IsBoneIndex;
    if (isBoneIndex)
        return Bones[boneNameIndex];
    else
        return (const char* const)(((uint32_t*)&pivotData) + 1) + boneNameIndex;
}

void AnimationAsset::GetPivotOrient(Orientation& orient, const uint32_t pivotIndex, const uint32_t frameNumber) const
{
    orient = BuiltinType::Orient;
    const PivotData* pivotData = (PivotData*)ALIGN_4BYTES(field_5C);
    uint32_t pivotOrientOffset = pivotData[pivotIndex].field_8 & 0xFFFFFF;

    if (!pivotOrientOffset)
        return;

    if (!pivotData[pivotIndex + 1].Data._13)
        pivotOrientOffset += 2 * frameNumber * field_66;

    pivotOrientOffset <<= 1;

    orient.x = (float)(*((uint16_t*)pivotData + pivotOrientOffset + 1)) * 0.000030518509f;
    orient.y = (float)(*((uint16_t*)pivotData + pivotOrientOffset + 3)) * 0.000030518509f;
    orient.z = (float)(*((uint16_t*)pivotData + pivotOrientOffset + 4)) * 0.000030518509f;
    orient.w = (float)(*((uint16_t*)pivotData + pivotOrientOffset + 2)) * 0.000030518509f;
}

const uint32_t AnimationAsset::GetPivotEndFrame(const uint32_t pivotIndex) const
{
    uint32_t frame1 = field_5C[pivotIndex].field_4 & 0xFFFFFF;
    frame1 = frame1 != 0 ? FramesTotal : 0;

    uint32_t frame2 = field_5C[pivotIndex].field_8 & 0xFFFFFF;
    frame2 = frame2 != 0 ? FramesTotal : 0;

    return frame1 <= frame2 ? frame2 : frame1;
}

void AnimationAsset::GetPivotPos(Vector4f& pos, const uint32_t pivotIndex, const uint32_t frameNumber) const
{
    pos = BuiltinType::ZeroVector;
    const PivotData* pivotData = (PivotData*)ALIGN_4BYTES(field_5C);
    uint32_t pivotDataOffset = pivotData[pivotIndex].field_4 & 0xFFFFFF;

    if (!pivotDataOffset)
        return;

    if (!pivotData[pivotIndex + 1].Data._12)
        pivotDataOffset += (frameNumber * field_64 * 2) + (frameNumber * field_64);

    pos.x = *((float*)pivotData + pivotDataOffset);
    pos.y = *((float*)pivotData + pivotDataOffset);
    pos.z = *((float*)pivotData + pivotDataOffset);
    pos.a = 0.f;
}

const float_t AnimationAsset::GetFOVForFrame(const float_t frame) const
{
    if (!FOVList.size())
        return 0.f;

    const int32_t frameNumber = (int32_t)(((float_t)FramesTotal - 1.f) * frame);
    if (frameNumber < FOVList.size())
        return FOVList[frameNumber];

    char errorMessage[32];
    sprintf(errorMessage, "No FOV found for frame (%d)\n", frameNumber);

    return 0.f;
}

void AnimationAsset::GetPivotInformation(const uint32_t pivotIndex, const float_t playPos, Orientation& pivotOrientation, const bool looped, const uint32_t a5, const uint32_t a6) const
{
    uint32_t endFrame, endFrame_1;
    if ((field_5C[pivotIndex].field_4 & 0xFFFFFF) != 0)
        endFrame = FramesTotal;
    else
        endFrame = 0;

    if ((field_5C[pivotIndex].field_8 & 0xFFFFFF) != 0)
        endFrame_1 = FramesTotal;
    else
        endFrame_1 = 0;

    if (endFrame <= endFrame_1)
        endFrame = endFrame_1;

    const float_t startFramePos = ((float_t)endFrame - 1.f) * playPos;
    const uint32_t startFramePosInt = (uint32_t)startFramePos;
    float_t frameDelta = 0.f;

    if ((a6 & (a5 != startFramePosInt)) != 0)
        frameDelta = startFramePos - startFramePosInt;

    uint32_t startFrameNumber, startNextFrameNumber;
    if (looped)
    {
        startFrameNumber = startFramePosInt % endFrame;
        startNextFrameNumber = (startFramePosInt + 1) % endFrame;
    }
    else
    {
        startFrameNumber = startFramePosInt;
        if (startFramePosInt >= endFrame)
            startFrameNumber = endFrame - 1;
        startNextFrameNumber = startFramePosInt + 1;
        if (startNextFrameNumber >= endFrame)
            startNextFrameNumber = endFrame - 1;
    }

    GetPivotInformation_Impl(pivotIndex, startFrameNumber, startNextFrameNumber, frameDelta, pivotOrientation);
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

int32_t AnimationAsset::BoneIndexByName(const char* const boneName)
{
    uint32_t i = 0;
    const size_t bonesTotal = ARRAYSIZE(Bones);

    while (strcmp(Bones[i], boneName) == NULL)
        if (i >= bonesTotal)
            return -1;

    return i;
}