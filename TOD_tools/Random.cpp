#include "Random.h"

void Random::Init(int seed)
{
	Values[0] = seed;

	for (int ind = 1; ind < sizeof(Values); ind++)
		Values[ind] = ind + 0x6C078965 * (Values[ind - 1] ^ (Values[ind - 1] >> 30));

	_A0ADC8 = 1;
	_A3AD6C = 1;
}

//	TODO: implementation!
unsigned int Random::Integer(int maxVal)
{
	return (*(unsigned int(__cdecl*)(int))0x473960)(maxVal);
}

//	TODO: implementation!
float Random::Float()
{
	return (*(float(__cdecl*)())0x46C640)();
}

int Random::Values[623] = {};
int& Random::_A0ADC8 = *(int*)0xA0ADC8;
int& Random::_A3AD6C = *(int*)0xA3AD6C;