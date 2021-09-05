#pragma once
#include "stdafx.h"

#define RANDOM_DEFAULT_SEED 5489

// NOTE: this is Mersenne Twister PRNG.
class Random
{
public:
 static void   Init(int seed); // @46C420
 static unsigned int Integer(int maxVal); // @473960
 static float  Float(); // @46C640
 static void   Generate(); // @46C470

 static int   Values[624]; // @A3A3A8
 static int   Index; // @A0ADC8
 static int   SeedSet; // @A3AD6C
 static int*   LastRandomValue; // @A3AD68
};