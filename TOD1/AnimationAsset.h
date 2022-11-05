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

    struct PivotData
    {
        int                 field_0;
        union
        {
            struct
            {
                unsigned    _0 : 11;
                unsigned    Flag : 1;   //  NOTE: this flag is checked.
                unsigned    Flag2 : 1;
                unsigned    Flag3 : 1;
            };
        }                   m_Flags;
        int                 field_8;
    };

protected:
    int                     field_1C;
    int                     field_20;
    int                     m_LoopMode;
    int                     field_28;
    std::vector<List1Struct>    m_List_1;
    std::vector<int>        m_List_2;
    std::vector<int>        m_List_3;
    PivotData              *field_5C;
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

    static void             CreateInstance(); // @900980
    static AnimationAsset*  Create(); // @900EF0
};

ASSERT_CLASS_SIZE(AnimationAsset, 104);