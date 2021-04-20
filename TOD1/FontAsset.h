#pragma once
#include "BaseAsset.h"

class FontAsset : public Asset
{
protected:
	int				field_1C;
	int				field_20;
	int				field_24;

private:
	inline FontAsset();	//	NOTE: always inlined.

public:
	virtual Asset*  GetInstancePtr() const override;

	static void     CreateInstance();	//	@85B460
	static FontAsset* Create();	//	@85B350
};

ASSERT_CLASS_SIZE(FontAsset, 40);