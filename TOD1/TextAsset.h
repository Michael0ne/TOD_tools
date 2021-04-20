#pragma once
#include "BaseAsset.h"

class TextAsset : public Asset
{
protected:
	int               field_1C;
	std::vector<int>  m_List_1;
	std::vector<int>  m_TextIndicies;
	std::vector<int>  m_List_3;
	int               field_50;
	int               field_54;

private:
	TextAsset();

public:
	virtual Asset*    GetInstancePtr() const override;

	static void       CreateInstance();	//	@861CE0
	static TextAsset* Create();	//	@861BD0
};

ASSERT_CLASS_SIZE(TextAsset, 88);