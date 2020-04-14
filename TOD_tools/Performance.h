#pragma once

#include "stdafx.h"

/*
 *------------------------------------------------------------
 *------------------------------------------------------------
 *------------------ Performance helper ----------------------
 *------------------------------------------------------------
 *--------- Used to determine timings of operations ----------
 *------------------------------------------------------------
 * -----------------------------------------------------------
*/
//	TODO: finish. Game freezes when used.
class Performance
{
private:
	//	>> A35E30
	static LARGE_INTEGER		n64Frequency;
	//	>> A35E38
	static bool			bMeasuringPerformance;
	//	>> A35E39
	static bool			bPerformanceFrequencyAcquired;
	//	>> A35E40
	static INT64		n64ClockCyclesInitial;
	//	>> A35E48
	static LARGE_INTEGER		n64TimeInitial;
	//	>> A35E50
	static LARGE_INTEGER		n64FrequencyFixed;
	//	>> A35E58
	static INT64		n64ClockCycles;

	//	>> A08E90
	static bool			bQueryingPerformance;
	//	>> A08E91
	static bool			bNeedToAcquirePerformanceFrequency;

public:
	//	>> 430570
	static long			GetMilliseconds()
	{
		return (*(long(__cdecl*)(void))0x430570)();
	}
	//	>> 430600
	static long			GetSeconds();
	//	>> 4306A0
	static INT64		ClockGetMilliseconds();
	//	>> 4306B0
	static INT64		ClockGetSeconds();
	//	>> 4306D0
	static void			Init();
	//	>> 4306F0
	static void			Calculate();
};

