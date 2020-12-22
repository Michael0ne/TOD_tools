#include "Progress.h"

Progress* g_Progress = nullptr;
ProgressBase* g_ProgressBase = nullptr;

#pragma message(TODO_IMPLEMENTATION)
void Progress::UpdateProgress(float time, bool)
{

}

#pragma message(TODO_IMPLEMENTATION)
Progress::Progress() : ProgressBase(Performance::ClockGetCycles() / 5)
{
	MESSAGE_CLASS_CREATED(Progress);
}

#pragma message(TODO_IMPLEMENTATION)
Progress::~Progress()
{
	MESSAGE_CLASS_DESTROYED(Progress);
}

void Progress::AddLoadbarPhase(const char* phaseName, LARGE_INTEGER timeToLoad, bool)
{
	String phasename(phaseName);
	m_StatesStringsList.AddString(&phasename);
	memcpy(m_TotalTimeToLoad, (const void*)&timeToLoad, sizeof(LARGE_INTEGER));	//	FIXME: this is bullshit.
}

void Progress::SetLoadScreenSprite(class Entity* sprite)
{
	m_LoadScreenSprite = sprite;
}

ProgressBase::ProgressBase(INT64 timeStart)
{
	MESSAGE_CLASS_CREATED(ProgressBase);

	m_StatesStringsList = List<String>();
	m_TimeStartHi = (int)timeStart;
	m_TimeStartLo = (int)(timeStart >> 32);

	debug("PROGRESS RESET.\n");

	m_TotalTimeToLoad[0] = m_TotalTimeToLoad[1] = NULL;
	field_20 = -1;
	field_30 = NULL;

	m_StatesStringsList.Erase();
}

ProgressBase::~ProgressBase()
{
	MESSAGE_CLASS_DESTROYED(ProgressBase);
}
