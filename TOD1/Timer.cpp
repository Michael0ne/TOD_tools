#include "Timer.h"

bool Timer::QueryingPerformance = true;
bool Timer::MeasuringPerformance = false;
bool Timer::QueryingPerformanceSeconds = true;
bool Timer::MeasuringPerformanceSeconds = false;
LARGE_INTEGER Timer::PerformanceFrequency = LARGE_INTEGER();
LARGE_INTEGER Timer::PerformancyFrequencySeconds = LARGE_INTEGER();
INT64 Timer::ClockCycles = NULL;
INT64 Timer::TimeInitialSeconds = NULL;
INT64 Timer::ClockCyclesInitial = NULL;

DWORD Timer::GetMilliseconds()
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
    }
    else
        return timeGetTime();
}

INT64 Timer::GetSeconds()
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
        return 1000 * (INT64)timeGetTime();

    LARGE_INTEGER freq;
    QueryPerformanceCounter(&freq);

    return freq.QuadPart / PerformancyFrequencySeconds.QuadPart;
}

INT64 Timer::ClockGetCycles()
{
    return ClockCycles;
}

INT64 Timer::ClockGetCyclesMilliseconds()
{
    return ClockCycles / 1000;
}

void Timer::Init()
{
    TimeInitialSeconds = GetSeconds();
    ClockCyclesInitial = __rdtsc();
}

void Timer::Calculate()
{
    ClockCycles = (1000000 * (__rdtsc() - ClockCyclesInitial)) / (GetSeconds() - TimeInitialSeconds);
}