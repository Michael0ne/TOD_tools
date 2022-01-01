#include "Random.h"

int Random::Values[] = {};
int Random::Index = 1;
int Random::SeedSet = false;
int* Random::LastRandomValue = nullptr;

void Random::Init(int seed)
{
    Values[0] = seed;

    constexpr unsigned int _size = sizeof(Values) / 4;

    for (int ind = 1; ind < _size; ind++)
        Values[ind] = ind + 0x6C078965 * (Values[ind - 1] ^ (Values[ind - 1] >> 30));

    Index = 1;
    SeedSet = 1;
}

unsigned int Random::Integer(int maxVal)
{
    unsigned int genval = NULL;

    do
    {
        if (!--Index)
            Generate();

        genval = ((((((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^
            (*LastRandomValue >> 11) ^ *LastRandomValue & 0xFFFFDF8C) << 15) ^
            ((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^
            (*LastRandomValue >> 11) ^ *LastRandomValue) >> 18) ^ ((((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^
                (*LastRandomValue >> 11) ^ *LastRandomValue & 0xFFFFDF8C) << 15) ^
            ((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^
            (*LastRandomValue >> 11) ^ *LastRandomValue;
        LastRandomValue++;
    } while (genval >= (maxVal + 1) * (0xFFFFFFFF / (maxVal + 1)));

    return genval % maxVal;
}

float Random::Float()
{
    if (!--Index)
        Generate();

    return ((((((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^ (*LastRandomValue >> 11) ^ *LastRandomValue & 0xFFFFDF8C) << 15) ^
        ((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^ (*LastRandomValue >> 11) ^ *LastRandomValue ^ ((((((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^
            (*LastRandomValue >> 11) ^ *LastRandomValue & 0xFFFFDF8C) << 15) ^ ((((*LastRandomValue >> 11) ^ *LastRandomValue) & 0xFF3A58AD) << 7) ^ (*LastRandomValue >> 11) ^ *LastRandomValue) >> 18)) * 2.32f);
}

void Random::Generate()
{
    if (!SeedSet)
        Init(RANDOM_DEFAULT_SEED);

    int* values = Values;
    Index = 624;
    LastRandomValue = Values;

    for (unsigned int i = 227; i != NULL; i--)
    {
        *values = values[397] ^ ((values[1] & 1) != NULL ? 0x9908B0DF : NULL) ^ ((*values ^ (values[1] ^ *values) & 0x7FFFFFFE) >> 1);
        values++;
    }

    for (unsigned int i = 396; i != NULL; i--)
    {
        *values = *(values - 227) ^ ((values[1] & 1) != NULL ? 0x9908B0DF : NULL) ^ ((*values ^ (values[1] ^ *values) & 0x7FFFFFFE) >> 1);
        values++;
    }

    *values = *(values - 227) ^ ((Values[0] & 1) != NULL ? 0x9908B0DF : NULL) ^ ((*values ^ (Values[0] ^ *values) & 0x7FFFFFFE) >> 1);
}