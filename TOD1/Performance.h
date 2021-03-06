#pragma once

#include "stdafx.h"

//	TODO: this could be a simple namespace, since all 'members' are static.
class Performance
{
private:
	static bool				QueryingPerformance;	//	@A08E90
	static bool				MeasuringPerformance;	//	@A35E38
	static bool				QueryingPerformanceSeconds;	//	@A08E91
	static bool				MeasuringPerformanceSeconds;	//	@A35E39
	static LARGE_INTEGER	PerformanceFrequency;	//	@A35E50
	static LARGE_INTEGER	PerformancyFrequencySeconds;	//	@A35E30
	static INT64			ClockCycles;	//	@A35E58
	static INT64			TimeInitialSeconds;	//	@A35E48
	static INT64			ClockCyclesInitial;	//	@A35E40

public:
	static DWORD			GetMilliseconds();	//	@430570
	static INT64			GetSeconds();	//	@430600
	static INT64			ClockGetCycles();	//	@4306A0
	static INT64			ClockGetCyclesMilliseconds();	//	@4306B0
	static void				Init();	//	@4306D0
	static void				Calculate();	//	@4306F0
};

