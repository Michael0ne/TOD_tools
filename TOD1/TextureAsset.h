#pragma once
#include "BaseAsset.h"
#include "Texture.h"

class TextureAsset : public Asset
{
    struct Tex
    {
        String                  field_0;
        String                  field_10;
        int                     field_14;
        int                     field_18;
        int                     field_1C;
        int                     field_20;
        short                   field_24;
        int                     field_28;
        int                     field_2C;
        int                     field_30;
        int                     field_34;
        int                     field_38;
        int                     field_3C;
        int                     field_40;
        int                     field_44;
        int                     field_48;
        int                     field_4C;
        int                     field_50;
        int                     field_54;
        int                     field_58;
        int                     field_5C;
        int                     field_60;
        int                     field_64;
        char                    field_68;
        int                     field_6C;
        int                     field_70;

        ~Tex(); // @853A90
    };

public:
    int                         m_BitsPerPixel;
    Tex                        *m_Texture; // NOTE: this pointer should be aligned by 4 bytes.
    char                        field_24;
    Texture                    *m_Texture_1; // NOTE: this too.
    int                         field_2C;

private:
    TextureAsset(); // NOTE: always inlined.

public:
    virtual                    ~TextureAsset(); // @853690
    virtual AssetInstance*      GetInstancePtr() const override; // @853AE0
    virtual void                stub5(int); // @853720
    virtual void                GetResourcesDir(String& outDir, PlatformId platformId) const override;  //  @853240
    virtual void                ApplyAssetData(CompiledAssetInfo*);   //  @853930
    virtual char                SetResourcePlaceholder();   //  @8537B0

    ScreenResolution&           GetTextureResolution(ScreenResolution& outRes); // @853650

    static void                 CreateInstance(); // @853870
    static TextureAsset*        Create(); // @853830

    static AssetInstance*       Instance; // @A3BE28
};

ASSERT_CLASS_SIZE(TextureAsset, 48);