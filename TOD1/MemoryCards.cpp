#include "MemoryCards.h"
#include "MemoryCard.h"
#include "SavePoint.h"
#include "LogDump.h"
#include "Scene.h"

EntityType* tMemoryCards = nullptr;

MemoryCards::MemoryCards() : Node(NODE_MASK_EMPTY)
{
	MESSAGE_CLASS_CREATED(MemoryCards);

	for (unsigned int i = 0; i < 8; i++)
	{
		m_MemCardsUniqueId[i] = new UniqueId;
		m_MemCardsUniqueId[i]->Set();
	}

	m_Ps2SlesLicense = "00000";
	m_Ps2SlusLicense = "00000";
	m_GameName = "GameName";

	m_SaveFileSize = 0;
}

MemoryCards::~MemoryCards()
{
	MESSAGE_CLASS_DESTROYED(MemoryCards);

	for (unsigned int i = 0; i < 8; i++)
		delete m_MemCardsUniqueId[i];
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
	tMemoryCards = new EntityType("MemoryCards");
	tMemoryCards->InheritFrom(tNode);
	tMemoryCards->m_Creator = (void* (*)(AllocatorIndex))Create;

	//	TODO: register a bunch of stuff here.
}

MemoryCards* MemoryCards::Create(AllocatorIndex allocInd)
{
	return new MemoryCards();
}

const char* MemoryCards::GetGamename() const
{
	return m_GameName.m_szString;
}

void MemoryCards::SetGamename(const char* const gamename)
{
	if (strlen(gamename) <= 20)
		m_GameName = gamename;
}

const char* MemoryCards::GetPs2SlesLicense() const
{
	return m_Ps2SlesLicense.m_szString;
}

void MemoryCards::SetPs2SlesLicense(const char* const sleslicense)
{
	if (strlen(sleslicense) != 5)
		return;

	for (unsigned int i = 0; i != 5; i++)
		if (sleslicense[i] < '0' || sleslicense[i] > '9')
			return;

	m_Ps2SlesLicense = sleslicense;
}

const char* MemoryCards::GetPs2SlusLicense() const
{
	return m_Ps2SlusLicense.m_szString;
}

void MemoryCards::SetPs2SlusLicense(const char* const sluslicense)
{
	if (strlen(sluslicense) != 5)
		return;

	for (unsigned int i = 0; i != 5; i++)
		if (sluslicense[i] < '0' || sluslicense[i] > '9')
			return;

	m_Ps2SlusLicense = sluslicense;
}

unsigned int MemoryCards::GetSaveFileSize() const
{
	return m_SaveFileSize;
}

void MemoryCards::SetSaveFileSize(unsigned int savesize)
{
	m_SaveFileSize = savesize;
	SetParam(10, &m_SaveFileSize, tINTEGER);
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
		*args = MemoryCardInfo[args[1]]->m_Formatted && File::IsDirectoryValid(MemoryCardInfo[args[1]]->m_SaveFolderPath.m_szString);
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

void MemoryCards::LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, Node* summarynode) const
{
	Scene::SceneInstance->LoadSavePointSummary(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode);
}

void MemoryCards::DeleteSavePoint(int* args)
{
	*args = DeleteSavePoint_Impl(args[1], args[2]);
}

bool MemoryCards::DeleteSavePoint_Impl(unsigned int memcardind, unsigned int slotind)
{
	char slotindstr[10] = {};
	sprintf(slotindstr, "Slot%02d", slotind);

	return MemoryCardInfo[memcardind]->DeleteSavePointFile(MEMCARD_DEFAULT_SAVE_DIR, slotindstr);
}

void MemoryCards::SavePointOperationError(int* args) const
{
	*args = Scene::SceneInstance->m_SavePointOperationError;
}

bool MemoryCards::SavePointExists_Impl(unsigned int memcardind, unsigned int slotind) const
{
	if (!MemoryCardInfo[memcardind])
		return false;

	if (!MemoryCardInfo[memcardind]->m_SaveFolderPath.m_nLength)
	{
		LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
		return false;
	}

	if (!MemoryCardInfo[memcardind]->IsFormatted() || !File::IsDirectoryValid(MemoryCardInfo[memcardind]->m_SaveFolderPath.m_szString))
		return false;

	String saveSlotStr;
	if (MakeSaveSlotString(saveSlotStr, slotind).m_szString == nullptr)
		return false;

	return MemoryCardInfo[memcardind]->IsSavePointFileExists(MEMCARD_DEFAULT_SAVE_DIR, saveSlotStr.m_szString);
}

void MemoryCards::SavePointExists(int* args) const
{
	*args = SavePointExists_Impl(args[1], args[2]);
}

void MemoryCards::PrepareCardForSavegames(int* args)
{
	*args = PrepareCardForSavegames_Impl(args[1]);
}

bool MemoryCards::PrepareCardForSavegames_Impl(unsigned int memcardind)
{
	if (!MemoryCardInfo[memcardind]->IsSaveDirPresent(MEMCARD_DEFAULT_SAVE_DIR) ||
		!MemoryCardInfo[memcardind]->CreateSaveDirectory(MEMCARD_DEFAULT_SAVE_DIR))
		return false;

	if (memcardind == SAVE_SLOT_8 || MemoryCardInfo[memcardind]->IsSavePointFileExists(MEMCARD_DEFAULT_SAVE_DIR, MEMCARD_DEFAULT_SAVE_DIR))
		return true;

	SavePoint savepoint(MemoryCardInfo[memcardind], MEMCARD_DEFAULT_SAVE_DIR, MEMCARD_DEFAULT_SAVE_DIR, SAVEPOINT_SAVE_SIZE);
	if (!savepoint.Open(STATUS_1))
		return false;

	UniqueId uniqid;
	uniqid.Set();

	if (!savepoint.WriteBufferWithSize((const char*)&uniqid, sizeof(uniqid)))
		return false;

	//	TODO: not sure about that.
	*m_MemCardsUniqueId[memcardind] = uniqid;

	return true;
}

void MemoryCards::UnPrepareCardForSavegames(int* args)
{
	*args = UnPrepareCardForSavegames_Impl(args[1]);
}

#pragma message(TODO_IMPLEMENTATION)
bool MemoryCards::UnPrepareCardForSavegames_Impl(unsigned int memcardind)
{
	return false;
}

void MemoryCards::RestoreSavePoint(unsigned int memcardind, unsigned int slotind, Node* summarynode) const
{
	Scene::SceneInstance->RestoreSavePoint(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode, this);
}

void MemoryCards::CreateSavePoint(unsigned int memcardind, unsigned int slotind, EntityType* textbox, unsigned int, const char* const savedirectory, Node* summarynode) const
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