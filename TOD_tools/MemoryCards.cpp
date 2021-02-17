#include "MemoryCards.h"
#include "MemoryCard.h"
#include "SavePoint.h"
#include "LogDump.h"
#include "Scene.h"

ScriptType_Entity* tMemoryCards = nullptr;

MemoryCards::MemoryCards() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(MemoryCards);

	m_UniqueId_0.Set();
	m_UniqueId_1.Set();
	m_UniqueId_2.Set();
	m_UniqueId_3.Set();

	m_Ps2SlesLicense = "00000";
	m_Ps2SlusLicense = "00000";
	m_GameName = "Game";

	m_SaveFileSize = 0;
}

unsigned int MemoryCards::GetLastModifiedTimeAsNumber(unsigned int memcardind, unsigned int slotind) const
{
	String tempStr;
	SavePoint savepoint(MemoryCardInfo[memcardind], MEMCARD_DEFAULT_SAVE_DIR, MakeSaveSlotString(tempStr, slotind).m_szString, SAVEPOINT_SAVE_SIZE);
	time_t ftime = savepoint.GetTime();

	if (!ftime || ftime == -1)
		return NULL;

	tm* filetimestamp = localtime(&ftime);
	return (60 * (filetimestamp->tm_min + 60 * (filetimestamp->tm_hour + 24 * (filetimestamp->tm_mday + 31 * (filetimestamp->tm_mon + 12 * filetimestamp->tm_year)))) + filetimestamp->tm_sec + 1080800896);
}

String& MemoryCards::MakeSaveSlotString(String& outStr, unsigned int slotind) const
{
	char slotstr[10] = {};
	sprintf(slotstr, "Slot%02d", slotind);

	return (outStr = slotstr, outStr);
}

#pragma message(TODO_IMPLEMENTATION)
void MemoryCards::Register()
{
	tMemoryCards = new ScriptType_Entity("MemoryCards");
	tMemoryCards->InheritFrom(tNode);
	tMemoryCards->m_Creator = (void* (*)(AllocatorIndex))Create;

	//	TODO: register a bunch of stuff here.
}

MemoryCards* MemoryCards::Create(AllocatorIndex allocInd)
{
	return new MemoryCards();
}

unsigned int MemoryCards::GetSavePointSize(unsigned int memcardind, unsigned int slotind) const
{
	String tempStr;
	return MemoryCardInfo[memcardind]->GetSavePointFileSize(MEMCARD_DEFAULT_SAVE_DIR, MakeSaveSlotString(tempStr, slotind).m_szString);
}

void MemoryCards::HasCardChanged(int* args) const
{
	*args = NULL;
}

unsigned int MemoryCards::GetCardFreeSpace(unsigned int memcardind) const
{
	if (MemoryCardInfo[memcardind]->IsFormatted())
		return File::GetStorageFreeSpace().LowPart;

	LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
	
	return NULL;
}

void MemoryCards::IsFormatted(int* args) const
{
	*args = MemoryCardInfo[args[1]]->IsFormatted();
}

void MemoryCards::IsPresent(int* args) const
{
	if (MemoryCardInfo[args[1]]->m_SaveFolderPath.m_nLength)
		*args = MemoryCardInfo[args[1]]->m_Formatted && Utils::IsDirectoryValid(MemoryCardInfo[args[1]]->m_SaveFolderPath.m_szString);
	else
	{
		LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
		args = false;
	}
}

bool MemoryCards::IsPrepared(unsigned int memcardind) const
{
	String tempStr;
	if (!MemoryCardInfo[memcardind]->IsSaveDirPresent(MEMCARD_DEFAULT_SAVE_DIR))
		return false;

	if (memcardind != SAVE_SLOT_8 && !MemoryCardInfo[memcardind]->IsSaveFilePresent(MEMCARD_DEFAULT_SAVE_DIR, MEMCARD_DEFAULT_SAVE_DIR))
		return false;
	else
		return true;
}

void MemoryCards::LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, ScriptType_Entity* summarynode) const
{
	Scene::SceneInstance->LoadSavePointSummary(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode);
}

void MemoryCards::RestoreSavePoint(unsigned int memcardind, unsigned int slotind, ScriptType_Entity* summarynode) const
{
	Scene::SceneInstance->RestoreSavePoint(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode, this);
}

void MemoryCards::CreateSavePoint(unsigned int memcardind, unsigned int slotind, ScriptType_Entity* textbox, unsigned int, const char* const savedirectory, ScriptType_Entity* summarynode) const
{
	Scene::SceneInstance->CreateSavePoint(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode, m_SaveFileSize);
}

#pragma message(TODO_IMPLEMENTATION)
void UniqueId::Set()
{
	//	TODO: simplify this?
	FILETIME filetime;
	GetSystemTimeAsFileTime(&filetime);

	m_Time = (filetime.dwLowDateTime + 0x2AC18000) / (filetime.dwHighDateTime + 0xFE624E21);
	m_Rdtsc = (int)__rdtsc();
}