#pragma once
#include "BaseAsset.h"

class AnimationAsset : public Asset
{
protected:
	int                    field_1C;
	int                    field_20;
	int                    field_24;
	int                    field_28;
	std::vector<int>       m_List_1;
	std::vector<int>       m_List_2;
	std::vector<int>       m_List_3;
	int                   *field_5C;
	int                    field_60;
	short                  field_64;
	short                  field_66;

private:
	AnimationAsset();	//	@900080

public:
	virtual Asset*         GetInstancePtr() const override;

	static void            CreateInstance();	//	@900980
	static AnimationAsset* Create();	//	@900EF0
};

ASSERT_CLASS_SIZE(AnimationAsset, 104);