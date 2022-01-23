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
protected:
    int                     field_1C;
    int                     field_20;
    int                     m_LoopMode;
    int                     field_28;
    std::vector<List1Struct>    m_List_1;
    std::vector<int>        m_List_2;
    std::vector<int>        m_List_3;
    int                    *field_5C;   //  NOTE: an array of all pivot's positions. Index = pivot index.
    int                     field_60;
    short                   field_64;
    short                   field_66;

private:
    AnimationAsset(); // @900080

public:
    virtual AssetInstance* GetInstancePtr() const override;

    static void            CreateInstance(); // @900980
    static AnimationAsset* Create(); // @900EF0
};

ASSERT_CLASS_SIZE(AnimationAsset, 104);