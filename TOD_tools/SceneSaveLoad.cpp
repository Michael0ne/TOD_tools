#include "SceneSaveLoad.h"
#include "RewindBuffer.h"

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