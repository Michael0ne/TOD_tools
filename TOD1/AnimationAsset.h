#pragma once
#include "BaseAsset.h"

class AnimationAsset : public Asset
{
    friend class AnimSlot;

    struct List1Struct
    {
        int                 field_0;
        int                 field_4;
        int                 field_8;
        int                 field_C;
        int                 field_10;
        int                 field_14;
    };

    struct List3Struct
    {
        int                 field_0;
        int                 field_4;
        int                 field_8;
        int                 field_C;
        int                 field_10;
        int                 field_14;
        int                 field_18;
        int                 field_1C;
        int                 field_20;
        int                 field_24;
        int                 field_28;
        int                 field_2C;
        int                 field_30;

    };

    struct PivotData
    {
        union
        {
            struct
            {
                unsigned        BoneIndexOrOffset : 12; //  NOTE: this is also used as offset to the bone name if flag is set.
                unsigned        _12 : 1;
                unsigned        _13 : 1;
                unsigned        IsBoneIndex : 1;    //  NOTE: if flag is set, then BoneIndex is a bone index, otherwise it's an offset to the bone name relative to this position.
            };

            uint32_t        FramesTotal;    //  NOTE: this is used as the first element in this array.
        }                   Data;
        uint32_t            field_4;
        uint32_t            field_8;
    };

protected:
    int                     field_1C;
    int                     field_20;
    int                     m_LoopMode;
    int                     FramesTotal;
    std::vector<List1Struct>    m_List_1;
    std::vector<int>        m_List_2;
    std::vector<List3Struct>        m_List_3;
    PivotData              *field_5C;   //  NOTE: there are exactly Data.FramesTotal frames in this array. The size of this data buffer is (_f60 * 4);
    int                     field_60;
    short                   field_64;
    short                   field_66;

private:
    AnimationAsset(); // @900080

    static AssetInstance*   Instance; //  @A3E0FC
    static const char* const    Bones[];    //  @A12FF0

public:
    virtual ~AnimationAsset();  //  @900F40
    virtual AssetInstance*  GetInstancePtr() const override;

    const char*             GetBoneName(const uint32_t boneIndex) const;    //  @8FFF50
    void                    GetPivotOrient(Orientation& orient, const uint32_t pivotIndex, const uint32_t frameNumber) const; //  @904CC0
    const uint32_t          GetPivotEndFrame(const uint32_t pivotIndex) const;  //  @904BF0
    void                    GetPivotPos(Vector4f& pos, const uint32_t pivotIndex, const uint32_t frameNumber) const;    //  @904C30

    static void             CreateInstance(); // @900980
    static AnimationAsset*  Create(); // @900EF0

    static int32_t          BoneIndexByName(const char* const boneName);    //  @8FFEE0 //  NOTE: unused.
};

ASSERT_CLASS_SIZE(AnimationAsset, 104);