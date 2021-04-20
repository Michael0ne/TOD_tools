#pragma once
#include "BaseAsset.h"

class SoundAsset : public Asset
{
protected:
	int                field_1C;
	class StreamBuffer* m_MonoStream;
	int                field_24;

private:
	SoundAsset();	//	@85C3C0

public:
	virtual Asset*     GetInstancePtr() const override;

	static void        CreateInstance();	//	@85C010
	static SoundAsset* Create();	//	@85C430
};

ASSERT_CLASS_SIZE(SoundAsset, 40);