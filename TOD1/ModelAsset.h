#pragma once
#include "TextureAsset.h"

class ModelAsset : public Asset
{
	struct TextureReference
	{
		TextureAsset*  m_TextureResource;
		unsigned int   field_4;
		char*          m_TexturePath;
	};

protected:
	int                field_1C;
	std::vector<TextureReference> m_TextureResources;
	std::vector<int>   m_PivotList;
	int*               field_40;
	Vector4f           m_BoundingRadius;
	int*               field_54;
	int*               field_58;
	int                field_5C;

private:
	ModelAsset();

public:
	virtual AssetInstance* GetInstancePtr() const override;

	static void        CreateInstance();	//	@858210
	static ModelAsset* Create();	//	@8581F0
};

ASSERT_CLASS_SIZE(ModelAsset, 96);