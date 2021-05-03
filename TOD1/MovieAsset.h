#pragma once
#include "BaseAsset.h"

class MovieAsset : public Asset
{
protected:
	int                field_1C;
	int                field_20;
	int                field_24;

private:
	MovieAsset();

public:
	virtual AssetInstance* GetInstancePtr() const override;

	static void        CreateInstance();	//	@85BC70
	static MovieAsset* Create();	//	@85BC40
};

ASSERT_CLASS_SIZE(MovieAsset, 40);