#pragma once
#include "TextureAsset.h"
#include <directxmath\include\DirectXMath.h>

class SkinnedMeshBuffer;

class ModelAsset : public Asset
{
    friend class Model;
    friend class Cloth;

public:
    struct TextureSet
    {
        TextureAsset   *m_TextureResource;
        int             m_Index;
        char           *m_TexturePath;
    };

    struct Mesh
    {
        Vector3f        m_Position;
        Orientation     field_C;
        char           *m_Name;
        char           *field_20;
        std::vector<SkinnedMeshBuffer*> m_SkinnedMeshesList;
        std::vector<int>    field_34;
        std::vector<int>   *field_44;
        std::vector<int>    field_48;
        int             m_ParentPivotIndex; //  NOTE: or 'NextPivotIndex'.
        int             field_5C;
        int             field_60;
        float           field_64;
        float           field_68;
        float           field_6C;
        int             field_70;
        int             field_74;
        int             field_78;

        Mesh(); //  @85A740
        Mesh(const Mesh& rhs);  //  @85A7B0
        ~Mesh();    //  @857820
    };

public:
    int                 field_1C;
    std::vector<TextureSet> m_TextureResources;
    std::vector<Mesh>  m_MeshList;
    int                *field_40;
    Vector4f            m_BoundingRadius;
    int                *field_54;
    int                *field_58;
    unsigned char       field_5C;

private:
    ModelAsset();   //  @858090

    int                 LoadTexture(const char* const texname); //  @8575B0
public:
    virtual AssetInstance* GetInstancePtr() const override;
    virtual char        SetResourcePlaceholder() override;  //  @858380

    void                _856E60();  //  @856E60
    int                 PivotIndexByName(const char* const pivotname) const;    //  @88A060
    void                GetPivotMatrix(DirectX::XMMATRIX& outMat, const int pivotIndex) const;  //  @8543F0
    Vector4f&           GetBoundingRadius(Vector4f& outBoundings) const;    //  @854230

    static void         CreateInstance(); // @858210
    static ModelAsset*  Create(); // @8581F0

    static AssetInstance*   Instance;   //  @A3BE30
    static const int        DefaultSurfacePropFields = 0x2007C00;   //  @A11090
};


ASSERT_CLASS_SIZE(ModelAsset, 96);
ASSERT_CLASS_SIZE(ModelAsset::Mesh, 124);