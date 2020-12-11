#include "Random.h"

int Random::Values[] = {};
int Random::_A0ADC8 = 1;
int Random::_A3AD6C;

void Random::Init(int seed)
{
	Values[0] = seed;

	constexpr unsigned int _size = sizeof(Values) / 4;

	for (int ind = 1; ind < _size; ind++)
		Values[ind] = ind + 0x6C078965 * (Values[ind - 1] ^ (Values[ind - 1] >> 30));

	_A0ADC8 = 1;
	_A3AD6C = 1;
}

#pragma message(TODO_IMPLEMENTATION)
unsigned int Random::Integer(int maxVal)
{
	return (*(unsigned int(__cdecl*)(int))0x473960)(maxVal);
}

#pragma message(TODO_IMPLEMENTATION)
float Random::Float()
{
	return (*(float(__cdecl*)())0x46C640)();
}