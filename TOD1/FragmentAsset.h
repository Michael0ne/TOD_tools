#pragma once
#include "BaseAsset.h"

class FragmentAsset : public Asset
{
    struct FragmentInfo
    {
        int field_0;
        int field_4;
        int field_8;
        int field_C;
        int field_10;
        int field_14;

        FragmentInfo(const FragmentInfo& rhs, void (*FindSuitableAllocatorProc)(size_t desiredsize, FragmentAsset*), FragmentAsset* owner); //  @406750
    };
public:
    int                     field_1C;
    int                    *field_20;
    int                     field_24;

private:
    FragmentAsset(char a1); // @85DD80
    virtual ~FragmentAsset();   //  @85E3C0 //  NOTE: another d-tor @85DD60, virtual destructor uses inlined version.

public:
    virtual AssetInstance*  GetInstancePtr() const override;
    virtual void            ApplyAssetData(int*) override;  //  @85DEC0

    static void             CreateInstance(); // @85DFA0
    static FragmentAsset*   Create(); // @85DE30

    void                    ApplyFragmentResource(unsigned int entityId, bool); // @85D990

    static AssetInstance* Instance; // @A3BE58
};

ASSERT_CLASS_SIZE(FragmentAsset, 40);