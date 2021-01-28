#include "SceneSaveLoad.h"
#include "TransactionBuffer.h"
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
	m_SaveInfo_1.m_TransactionBuffer = new TransactionBuffer(102400);
	
	if (!m_SaveInfo_1.m_TransactionBuffer)
		return false;

	if (!((int*)(m_SaveInfo_1.m_TransactionBuffer->m_pBuffer) + m_SaveInfo_1.m_TransactionBuffer->m_nSize))
	{
		delete m_SaveInfo_1.m_TransactionBuffer;
		return false;
	}

	m_SaveInfo_1.field_20 = ent;

	//	TODO: complete this!
}