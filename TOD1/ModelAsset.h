#pragma once
#include "TextureAsset.h"

class ModelAsset : public Asset
{
    struct TextureReference
    {
        TextureAsset   *m_TextureResource;
        unsigned int    field_4;
        char           *m_TexturePath;
    };

    struct Pivot
    {
        Vector3f        m_Position;
        float           field_C;
        float           field_10;
        float           field_14;
        float           field_18;
        char           *m_Name;
        char           *field_20;
        char           *field_24;
        int             field_28;
        int             field_2C;
        int             field_30;
        char           *field_34;
        int             field_38;
        int             field_3C;
        int             field_40;
        int             field_44;
        char           *field_48;
        int             field_4C;
        int             field_50;
        int             field_54;
        int             m_ParentPivotIndex; //  NOTE: or 'NextPivotIndex'.
        int             field_5C;
        int             field_60;
        float           field_64;
        float           field_68;
        float           field_6C;
        int             field_70;
        int             field_74;
        int             field_78;

    };

protected:
    int                 field_1C;
    std::vector<TextureReference> m_TextureResources;
    std::vector<Pivot>  m_PivotList;
    int                *field_40;
    Vector4f            m_BoundingRadius;
    int                *field_54;
    int                *field_58;
    unsigned char       field_5C;

private:
    ModelAsset();

public:
    virtual AssetInstance* GetInstancePtr() const override;

    static void         CreateInstance();	//	@858210
    static ModelAsset*  Create();	//	@8581F0
};

ASSERT_CLASS_SIZE(ModelAsset, 96);
ASSERT_CLASS_SIZE(ModelAsset::Pivot, 124);