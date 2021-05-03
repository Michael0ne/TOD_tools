#pragma once
#include "BaseAsset.h"

class MeshColorAsset : public Asset
{
protected:
	int                    field_1C;
	std::vector<int>       m_List_1;
	std::vector<int>       m_List_2;
	int                   *field_40;
	int                   *field_44;
	int                   *field_48;
	int                   *field_4C;

private:
	MeshColorAsset();	//	@85E7F0

public:
	virtual AssetInstance* GetInstancePtr() const override;

	static void            CreateInstance();	//	@85E970
	static MeshColorAsset* Create();	//	@85E950
};

ASSERT_CLASS_SIZE(MeshColorAsset, 80);