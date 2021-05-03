#pragma once
#include "BaseAsset.h"

class FragmentAsset : public Asset
{
protected:
    int                     field_1C;
    int                    *field_20;
    int                     field_24;

private:
    FragmentAsset(char a1);	//	@85DD80
    virtual ~FragmentAsset();   //  @85E3C0

public:
    virtual AssetInstance*  GetInstancePtr() const override;

    static void             CreateInstance();	//	@85DFA0
    static FragmentAsset*   Create();	//	@85DE30

    void                    ApplyFragmentResource(unsigned int entityId, bool);	//	@85D990

    static AssetInstance*	Instance;	//	@A3BE58
};

ASSERT_CLASS_SIZE(FragmentAsset, 40);