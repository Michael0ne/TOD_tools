#include "SceneSaveLoad.h"
#include "TransactionBuffer.h"
#include "StreamedSoundBuffers.h"
#include "SavePoint.h"
#include "LogDump.h"
#include "ScriptDatabase.h"

SceneSaveLoad* g_SceneSaveLoad = nullptr;

SceneSaveLoad::SceneSaveLoad()
{
	MESSAGE_CLASS_CREATED(SceneSaveLoad);

	m_SavedPlayMode = MODE_NONE;
	m_SavePoint = nullptr;
	field_38 = nullptr;
	field_3C = NULL;
	m_SaveInfo_1.m_TransactionBuffer = new TransactionBuffer(0x96000);
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
bool SceneSaveLoad::LoadSaveSummaryToBuffer(SavePoint* savepoint, SaveInfo* saveinfo)
{
	unsigned int saveVersion = NULL;

	if (savepoint->Read(&saveVersion, sizeof(saveVersion)) != sizeof(saveVersion))
		return false;

	if (saveVersion != SAVEPOINT_FILE_VERSION)
	{
		LogDump::LogA("Save point version mismatch (current is %d, file is %d)\n", SAVEPOINT_FILE_VERSION, saveVersion);
		return false;
	}

	unsigned int propertyChecksum = NULL;

	if (savepoint->Read(&propertyChecksum, sizeof(propertyChecksum)) != sizeof(propertyChecksum))
		return false;

	unsigned int propertyChecksumDefined = Script::GetGlobalPropertyListCRC();

	if (propertyChecksum != propertyChecksumDefined)
	{
		LogDump::LogA("Property checksum mismatch (current is %d, saved is %d)\n", propertyChecksumDefined, propertyChecksum);
		return false;
	}

	unsigned int saveEngineVersion = NULL;

	if (savepoint->Read(&saveEngineVersion, sizeof(saveEngineVersion)) != sizeof(saveEngineVersion))
		return false;

	if (saveEngineVersion != Utils::GetEngineVersionBuild())
	{
		LogDump::LogA("Savepoint created with a different engine (Current build version = %d, saved = %d)\n", Utils::GetEngineVersionBuild(), saveEngineVersion);
		return false;
	}

	unsigned int saveHeaderChecksum = NULL;

	if (savepoint->Read(&saveHeaderChecksum, sizeof(saveHeaderChecksum)) != sizeof(saveHeaderChecksum))
		return false;

	unsigned int saveHeaderEndPos = NULL;

	if (savepoint->Read(&saveHeaderEndPos, sizeof(saveHeaderEndPos)) != sizeof(saveHeaderEndPos))
		return false;

	const unsigned int saveCurrPos = savepoint->GetPosition();
	if (saveHeaderChecksum != savepoint->CalculateChecksum(savepoint, saveHeaderEndPos))
	{
		LogDump::LogA("Header checksum failed\n");
		return false;
	}

	savepoint->RewindFileToBeginning();
	savepoint->Seek(saveCurrPos);

	unsigned int saveBuffersCount = NULL;

	if (savepoint->Read(&saveBuffersCount, sizeof(saveBuffersCount)) != sizeof(saveBuffersCount))
		return false;

	if (saveBuffersCount != SAVEPOINT_FILE_BUFFERS)
	{
		LogDump::LogA("Save point number of buffers mismatch\n");
		return false;
	}

	if (savepoint->Read(&saveinfo->field_1C, sizeof(saveinfo->field_1C)) != sizeof(saveinfo->field_1C))
		return false;

	//	TODO: complete this!
	return false;
}

bool SceneSaveLoad::LoadSaveSummary(class SavePoint* savepoint, const class Node* summarynode)
{
	if (!savepoint)
		return false;

	m_SaveInfo_1.m_MemorySummaryNode = (Node*)summarynode;
	return LoadSaveSummaryToBuffer(savepoint, &m_SaveInfo_1);
}

#pragma message(TODO_IMPLEMENTATION)
bool SceneSaveLoad::LoadSavePointData(class SavePoint* sp, ScriptType_Entity* ent, class Node* readFinishedCb)
{
	return false;
}

bool SceneSaveLoad::WriteDummySavePointData(class SavePoint* savepoint, unsigned int pad)
{
	if (savepoint->m_Status != STATUS_CLOSED)
	{
		if (savepoint->m_SaveFile)
		{
			--SavePoint::OpenFilesCount;
			delete savepoint->m_SaveFile;
		}

		savepoint->m_Status = STATUS_CLOSED;
	}

	if (savepoint->Open(STATUS_2))
	{
		savepoint->IsFileOpen();	//	NOTE: why?
		unsigned int filePos = savepoint->GetPosition();
		LogDump::LogA("pad: %x, %x\n", filePos, pad);

		if (filePos <= pad)
		{
			char dummybuffer[9216] = {};
			if (pad - filePos >= sizeof(dummybuffer))
				for (unsigned int i = pad - filePos; i >= sizeof(dummybuffer); i--, filePos += sizeof(dummybuffer))
					savepoint->WriteBufferWithSize(dummybuffer, sizeof(dummybuffer));
			savepoint->WriteBufferWithSize(dummybuffer, pad - filePos);

			return savepoint->CloseFile() != false;
		}
		else
			return true;
	}

	return false;
}