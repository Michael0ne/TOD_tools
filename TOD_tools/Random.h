#pragma once

#include "stdafx.h"

class Random
{
public:
	static void			Init(int seed);	//	@46C420
	static unsigned int	Integer(int maxVal);	//	@473960
	static float		Float();	//	@46C640

	static int			Values[624];	//	@A3A3A8
	static int			_A0ADC8;	//	@A0ADC8
	static int			_A3AD6C;	//	@A3AD6C
};