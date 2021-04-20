#pragma once
#include "BaseAsset.h"

class TextureAsset : public Asset
{
	struct Tex
	{
		unsigned int	field_0;
		unsigned int	field_4;
		unsigned short	field_8;
		unsigned char	field_A;
		String			field_E;
		unsigned int	field_12;
		unsigned int	field_16;
		unsigned int	field_1A;
		unsigned int	field_1E;
		unsigned short	field_22;
		unsigned int	field_26;
		unsigned int	field_2A;
		unsigned int	field_2E;
		unsigned int	field_32;
		unsigned int	field_36;
		unsigned int	field_3A;
		unsigned int	field_3E;
		unsigned int	field_42;
	};

protected:
	int             field_1C;
	Tex*            field_20;
	int				field_24;
	int*            m_Texture;
	int             field_2C;

private:
	TextureAsset();	//	NOTE: always inlined.

public:
	virtual ~TextureAsset() = default;
	virtual Asset*  GetInstancePtr() const override;

	void            GetTextureResolution(Vector2<int>& outRes);	//	@853650

	static void     CreateInstance();	//	@853870
	static TextureAsset* Create();	//	@853830
};