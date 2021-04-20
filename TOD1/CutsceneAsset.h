#pragma once
#include "BaseAsset.h"

class CutsceneAsset : public Asset
{
protected:
	int                   field_1C;
	std::vector<int>      m_List_1;
	std::vector<int>      m_List_2;
	int                   field_40;
	int                   field_44;
	int                   field_48;
	String                m_String_1;
	int                   field_5C;

private:
	CutsceneAsset();	//	@916080

public:
	virtual Asset*        GetInstancePtr() const override;

	static void           CreateInstance();	//	@9164C0
	static CutsceneAsset* Create();	//	@916100
};

ASSERT_CLASS_SIZE(CutsceneAsset, 96);