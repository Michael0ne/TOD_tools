#pragma once
#include "BaseAsset.h"
#include "Font.h"

class FontAsset : public Asset
{
protected:
    int             field_1C;
    int             field_20;
    Font		   *m_Font;

private:
    inline FontAsset();	//	NOTE: always inlined.

public:
    virtual         ~FontAsset();   //  @
    virtual AssetInstance* GetInstancePtr() const override;
    virtual void    ApplyAssetData(int*) override;  //  @85B560

    static void     CreateInstance();	//	@85B460
    static FontAsset* Create();	//	@85B350
};

ASSERT_CLASS_SIZE(FontAsset, 40);