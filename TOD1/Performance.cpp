#include "Performance.h"

bool Performance::QueryingPerformance = true;
bool Performance::MeasuringPerformance = false;
bool Performance::QueryingPerformanceSeconds = true;
bool Performance::MeasuringPerformanceSeconds = false;
LARGE_INTEGER Performance::PerformanceFrequency = LARGE_INTEGER();
LARGE_INTEGER Performance::PerformancyFrequencySeconds = LARGE_INTEGER();
INT64 Performance::ClockCycles = NULL;
INT64 Performance::TimeInitialSeconds = NULL;
INT64 Performance::ClockCyclesInitial = NULL;

DWORD Performance::GetMilliseconds()
{
	if (QueryingPerformance)
	{
		MeasuringPerformance = true;

		if (!QueryPerformanceFrequency(&PerformanceFrequency))
			MeasuringPerformance = false;

		PerformanceFrequency.QuadPart /= 1000;
		QueryingPerformance = false;
	}

	if (MeasuringPerformance)
	{
		LARGE_INTEGER freq;
		QueryPerformanceCounter(&freq);

		return (DWORD)(freq.QuadPart / PerformanceFrequency.QuadPart);
	}else
		return timeGetTime();
}

INT64 Performance::GetSeconds()
{
	if (QueryingPerformanceSeconds)
	{
		MeasuringPerformanceSeconds = true;
		if (!QueryPerformanceFrequency(&PerformancyFrequencySeconds))
			MeasuringPerformanceSeconds = false;

		PerformancyFrequencySeconds.QuadPart /= 1000000;
		QueryingPerformanceSeconds = false;
	}

	if (!MeasuringPerformanceSeconds)
		return 1000 * timeGetTime();

	LARGE_INTEGER freq;
	QueryPerformanceCounter(&freq);

	return freq.QuadPart / PerformancyFrequencySeconds.QuadPart;
}

INT64 Performance::ClockGetCycles()
{
	return ClockCycles;
}

INT64 Performance::ClockGetCyclesMilliseconds()
{
	return ClockCycles / 1000;
}

void Performance::Init()
{
	TimeInitialSeconds = GetSeconds();
	ClockCyclesInitial = __rdtsc();
}

void Performance::Calculate()
{
	ClockCycles = (1000000 * (__rdtsc() - ClockCyclesInitial)) / (GetSeconds() - TimeInitialSeconds);
}