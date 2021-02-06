#include "Progress.h"
#include "LogDump.h"
#include "GfxInternal_Dx9_Surface.h"

Progress* g_Progress = nullptr;
ProgressBase* g_ProgressBase = nullptr;

UINT64 Progress::StartTime;

Progress::~Progress()
{
	MESSAGE_CLASS_DESTROYED(Progress);

	if (g_ProgressBase == this)
		g_ProgressBase = nullptr;

	//	TODO: this is not in original code, who is responsible to delete texture?
	delete m_LoadBarTexture;
}

#pragma message(TODO_IMPLEMENTATION)
void Progress::UpdateProgress(float time, bool unk)
{
	LogDump::LogA("PROGRESS: %.0f%%\n", time * 100.f);
	
	//	TODO: figure out loading bar sprite dimensions and put them into render buffer, then render.
}

#pragma message(TODO_IMPLEMENTATION)
Progress::Progress() : ProgressBase(Performance::ClockGetCycles() / 5)
{
	MESSAGE_CLASS_CREATED(Progress);

	g_Progress = this;

	m_LoadScreenSprite = nullptr;
	m_Enabled = false;

	GfxInternal_Dx9_Surface* surf = new GfxInternal_Dx9_Surface(8, 8);

	for (unsigned char x = NULL; x < 8; x++)
		for (unsigned char y = NULL; y < 8; y++)
			surf->SetPixelColor(x, y, { 1.f, 1.f, 1.f, 1.f });
	
	//	TODO: one more constructor here, but it's class is not done yet.
	void* unk = new char[20];

	m_LoadBarTexture = new GfxInternal_Dx9_Texture(unk);

	//	FIXME: unk needs class definitions and destructor call here, surf doesn't get deleted here. Where?
	delete[] unk;
	delete surf;
}

void Progress::Complete()
{
	field_2C = field_30;

	stub1("");
	UpdateProgress(field_2C, true);
	UpdateProgress(field_2C, true);

	LogDump::LogA("PROGRESS COMPLETE.\n");
}

void Progress::_40E7F0(unsigned int unk1, UINT64 unk2)
{
	if (field_20 + 1 < m_LoadBarPhases.m_CurrIndex)
	{
		field_20++;
		field_28 = unk1;
		field_2C = field_30;
		field_24 = NULL;
		field_30 += m_LoadBarPhases.m_Elements[field_20]->m_Time / m_TotalTimeToLoad;

		stub1(m_LoadBarPhases.m_Elements[field_20]->m_Status.m_szString);

		field_38 = m_TimeStart + unk2;

		if (m_LoadBarPhases.m_Elements[field_20]->field_18)
		{
			field_48 = m_LoadBarPhases.m_Elements[field_20]->m_Time;
			field_50 = unk2;
		}
		else
			field_48 = field_50 = NULL;
	}
}

void Progress::Reset()
{
	LogDump::LogA("PROGRESS RESET.\n");

	m_TotalTimeToLoad = NULL;
	field_20 = -1;
	field_30 = 0.f;

	m_LoadBarPhases.Erase();
}

void Progress::AddLoadbarPhase(const char* phaseName, UINT64 timeToLoad, bool unk)
{
	ProgressStateInfo stateinfo_(phaseName, timeToLoad, unk);
	m_LoadBarPhases.AddElement(&stateinfo_);
	m_TotalTimeToLoad += timeToLoad;
}

void Progress::Enable()
{
	g_ProgressBase = this;

	if (m_Enabled)
		return;

	LogDump::LogA("PROGRESS ENABLE.\n");

	if (m_LoadScreenSprite)
	{
		Vector4f spriteres;
		m_LoadScreenSprite->GetPos(spriteres);
		m_LoadScreenSpriteResolution.x = spriteres.x;
		m_LoadScreenSpriteResolution.y = spriteres.y;
	}
	else
		m_LoadScreenSpriteResolution = Vector2<float>();

	UpdateProgress(NULL, 1);
	UpdateProgress(NULL, 1);
	
	StartTime = __rdtsc();
	m_Enabled = true;
}

void Progress::Disable()
{
	if (!m_Enabled)
		return;

	LogDump::LogA("PROGRESS DISABLE. Loadtime: %d ms\n", (__rdtsc() - StartTime) / Performance::ClockGetCyclesMilliseconds());

	UpdateProgress(1.0, 1);
	UpdateProgress(1.0, 1);

	g_ProgressBase = nullptr;
	m_Enabled = false;
}

void Progress::SetLoadScreenSprite(Sprite* sprite)
{
	m_LoadScreenSprite = sprite;
}

void Progress::UpdateProgressTime(unsigned int unk, UINT64 time)
{
	field_24 += unk;
	if (time >= field_38 || !m_TimeStart)
	{
		float v1 = (float)(field_24 / field_28);
		float v2 = field_48 <= NULL ? 0.f : ((time - field_50) / field_48);

		if (v2 > v1)
			v1 = v2;
		if (v1 > 1.f)
			v1 = 1.f;

		UpdateProgress(((field_30 - field_2C) * v1) + field_2C, false);

		if (m_TimeStart > NULL && field_38 < time)
			do
			{
				field_38 += m_TimeStart;
			} while (field_38 < time);
	}
}

ProgressBase::~ProgressBase()
{
	MESSAGE_CLASS_DESTROYED(ProgressBase);

	if (g_ProgressBase == this)
		g_ProgressBase = nullptr;
}

void ProgressBase::stub1(const char*)
{
	return;
}

void ProgressBase::UpdateProgress(float time, bool unk)
{
	return;
}

ProgressBase::ProgressBase(UINT64 timeStart)
{
	MESSAGE_CLASS_CREATED(ProgressBase);

	m_LoadBarPhases = List<ProgressStateInfo>(0x8B00);
	m_TimeStart = timeStart;

	LogDump::LogA("PROGRESS RESET.\n");

	m_TotalTimeToLoad = NULL;
	field_20 = -1;
	field_30 = NULL;

	m_LoadBarPhases.Erase();
}