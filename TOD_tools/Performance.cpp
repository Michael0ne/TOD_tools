#include "Performance.h"

void Performance::Init()
{
	void(__cdecl * _Init)() = (void(__cdecl*)())0x4306D0;

	_Init();
}

void Performance::Calculate()
{
	void(__cdecl * _Calculate)() = (void(__cdecl*)())0x4306F0;

	_Calculate();
}

/*
LARGE_INTEGER	Performance::n64Frequency = *(LARGE_INTEGER*)0xA35E30;
bool	Performance::bMeasuringPerformance = *(bool*)0xA35E38;
bool	Performance::bPerformanceFrequencyAcquired = *(bool*)0xA35E39;
INT64	Performance::n64ClockCyclesInitial = *(INT64*)0xA35E40;
LARGE_INTEGER	Performance::n64TimeInitial = *(LARGE_INTEGER*)0xA35E48;
LARGE_INTEGER	Performance::n64FrequencyFixed = *(LARGE_INTEGER*)0xA35E50;
INT64	Performance::n64ClockCycles = *(INT64*)0xA35E58;
bool	Performance::bQueryingPerformance = *(bool*)0xA08E90;
bool	Performance::bNeedToAcquirePerformanceFrequency = *(bool*)0xA08E91;

//	>> 430570
long Performance::GetMilliseconds()
{
	long systemTimeMs;

	if (Performance::bQueryingPerformance) {
		Performance::bMeasuringPerformance = true;

		if (!QueryPerformanceFrequency(&Performance::n64FrequencyFixed))
			Performance::bMeasuringPerformance = false;

		Performance::n64FrequencyFixed.QuadPart /= 1000i64;
		Performance::bQueryingPerformance = false;
	}

	if (Performance::bMeasuringPerformance) {
		LARGE_INTEGER perfCount;
		QueryPerformanceCounter(&perfCount);
		systemTimeMs = perfCount.QuadPart / Performance::n64FrequencyFixed.QuadPart;
	}
	else
		systemTimeMs = timeGetTime();

	return systemTimeMs;
}

//	>> 430600
long Performance::GetSeconds()
{
	if (Performance::bNeedToAcquirePerformanceFrequency) {
		Performance::bPerformanceFrequencyAcquired = true;
		if (!QueryPerformanceFrequency(&Performance::n64Frequency))
			Performance::bPerformanceFrequencyAcquired = true;
		Performance::n64Frequency.QuadPart = Performance::n64Frequency.QuadPart / 1000000;
		Performance::bNeedToAcquirePerformanceFrequency = false;
	}

	if (!Performance::bPerformanceFrequencyAcquired)
		return 1000 * timeGetTime();

	LARGE_INTEGER PerformanceCount;
	QueryPerformanceCounter(&PerformanceCount);

	return PerformanceCount.QuadPart / Performance::n64Frequency.QuadPart;
}

//	>> 4306A0
INT64 Performance::ClockGetMilliseconds()
{
	return Performance::n64ClockCycles;
}

//	>> 4306B0
INT64 Performance::ClockGetSeconds()
{
	return Performance::n64ClockCycles / 1000;
}

//	>> 4306D0
void Performance::Init()
{
	Performance::n64TimeInitial.QuadPart = Performance::GetSeconds();
	Performance::n64ClockCyclesInitial = __rdtsc();
}

//	>> 4306F0
//	TODO: fix. Because of LARGE_INTEGER types everywhere, we can't just assign them anywhere. Need to work this out.
void Performance::Calculate()
{
	//LARGE_INTEGER timeInitial = Performance::n64TimeInitial;
	//LARGE_INTEGER timeCurrent = Performance::GetSeconds();

	//Performance::n64ClockCycles = (1000000 * (__rdtsc() - Performance::n64ClockCyclesInitial)) / (timeCurrent - timeInitial);
}

inline void PATCH_PERF()
{
	//	Since all methods are static, simple hooks can be implemented.
	
	//	Not really needed, since WinMain is already implemented.
	hook(0x43CB5B, &Performance::Calculate, PATCH_CALL);

	//	Not really needed, since WinMain is already implemented.
	hook(0x43CB4E, &Performance::Init, PATCH_CALL);

	//	Hook ClockGetSeconds function.
	hook(0x420BB8, &Performance::ClockGetSeconds, PATCH_CALL);
	hook(0x420BD9, &Performance::ClockGetSeconds, PATCH_CALL);

	//	Hook ClockGetMilliseconds function.
	hook(0x87B725, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x892EB9, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x897678, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x897868, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x89812E, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x8984A5, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x898C84, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x898CBF, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x898CFA, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x89936C, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x89938F, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x899646, &Performance::ClockGetMilliseconds, PATCH_CALL);
	hook(0x8999C8, &Performance::ClockGetMilliseconds, PATCH_CALL);

	//	Hook GetSeconds function.
	hook(0x4306D0, &Performance::GetSeconds, PATCH_CALL);
	hook(0x4306F2, &Performance::GetSeconds, PATCH_CALL);

	//	Hook GetMilliseconds function.
	hook(0x421355, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x421C1D, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x43E804, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x43E833, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x443F82, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x44469D, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x444A82, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x444AE0, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x444DA1, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x444DD0, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x47B4C9, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x47B783, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x487DF0, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x48C414, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x48C536, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x875A34, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x875CD3, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x875E42, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x87C1FB, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x87C439, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x87C676, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x87C6AA, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x8935F4, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x896FAE, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x89745F, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x8980C9, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x8980E8, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x8983EB, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898606, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898642, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898672, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898A67, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898DEE, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898F96, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x898FDF, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x899429, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x899590, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x899911, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x899AED, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x8D2555, &Performance::GetMilliseconds, PATCH_CALL);
	hook(0x93CE52, &Performance::GetMilliseconds, PATCH_CALL);
}

*/