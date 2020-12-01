#include "Progress.h"

Progress* g_Progress = nullptr;
ProgressBase* g_ProgressBase = nullptr;

//	TODO: implementation!
void Progress::UpdateProgress(float time, bool)
{

}

//	TODO: implementation!
Progress::Progress() : ProgressBase(Performance::ClockGetCycles() / 5)
{
	MESSAGE_CLASS_CREATED(Progress);
}

//	TODO: implementation!
Progress::~Progress()
{
	MESSAGE_CLASS_DESTROYED(Progress);
}

ProgressBase::ProgressBase(INT64 timeStart)
{
	MESSAGE_CLASS_CREATED(ProgressBase);

	m_StatesStringsList = List<String>();
	m_TimeStartHi = (int)timeStart;
	m_TimeStartLo = (int)(timeStart >> 32);

	debug("PROGRESS RESET.\n");

	field_18 = NULL;
	field_1C = NULL;
	field_20 = -1;
	field_30 = NULL;

	m_StatesStringsList.Erase();
}

ProgressBase::~ProgressBase()
{
	MESSAGE_CLASS_DESTROYED(ProgressBase);
}
