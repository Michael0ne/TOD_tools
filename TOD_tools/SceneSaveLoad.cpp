#include "SceneSaveLoad.h"
#include "RewindBuffer.h"
#include "StreamedSoundBuffers.h"

SceneSaveLoad* g_SceneSaveLoad = nullptr;

#pragma message(TODO_IMPLEMENTATION)
SceneSaveLoad::SceneSaveLoad()
{
	MESSAGE_CLASS_CREATED(SceneSaveLoad);
}

#pragma message(TODO_IMPLEMENTATION)
void SceneSaveLoad::_874940()
{
	//	NOTE: possible name is 'CurrentUndo'.
}

void SceneSaveLoad::ResetSavedPlayMode()
{
	m_SavedPlayMode = MODE_NONE;
}

#pragma message(TODO_IMPLEMENTATION)
bool SceneSaveLoad::LoadSavePointData(class SavePoint* sp, ScriptType_Entity* ent, class Node* readFinishedCb)
{
	g_StreamedSoundBuffers->MeasureWaitForSoftPause();
	m_SaveInfo_1.field_0 = new RewindBuffer(102400);
	
	if (!m_SaveInfo_1.field_0)
		return false;

	if (!((int*)(m_SaveInfo_1.field_0->m_pBuffer) + m_SaveInfo_1.field_0->m_nSize))
	{
		delete m_SaveInfo_1.field_0;
		return false;
	}

	m_SaveInfo_1.field_20 = ent;

	//	TODO: complete this!
}