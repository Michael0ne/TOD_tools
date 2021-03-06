#pragma once
#include "Model.h"

class SkyBox : public Model
{
	//	NOTE: possible name 'Mesh'.
	struct SkyBoxStructUnknown
	{
		void*	field_0;
		unsigned short field_4;
		unsigned int field_8;
		void*	field_C;
		void*	field_10;
		String	field_14;
		float	field_18;
		float	field_1C;
		float	field_20;
		float	field_24;
		float	field_28;
		float	field_2C;
		float	field_30;
		float	field_34;
	};

protected:
	int		field_104;

public:
	SkyBox();	//	NOTE: constructor inlined.

	static	SkyBoxStructUnknown*	_A3E0B0[5];	//	@A3E0B0
};

ASSERT_CLASS_SIZE(SkyBox, 260);