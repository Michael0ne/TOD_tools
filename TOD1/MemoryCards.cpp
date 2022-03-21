#include "MemoryCards.h"
#include "MemoryCard.h"
#include "SavePoint.h"
#include "LogDump.h"
#include "Scene.h"
#include "IntegerType.h"
#include "StringType.h"

EntityType* tMemoryCards = nullptr;

MemoryCards::MemoryCards() : Node(NODE_MASK_EMPTY)
{
    MESSAGE_CLASS_CREATED(MemoryCards);

    for (unsigned int i = 0; i < 4; i++)
        m_MemCardsUniqueId[i] = {};

    m_Ps2SlesLicense = "00000";
    m_Ps2SlusLicense = "00000";
    m_GameName = "GameName";

    m_SaveFileSize = 0;
}

MemoryCards::~MemoryCards()
{
    MESSAGE_CLASS_DESTROYED(MemoryCards);
}

unsigned int MemoryCards::GetLastModifiedTimeAsNumber(unsigned int memcardind, unsigned int slotind) const
{
    String tempStr;
    SavePoint savepoint(MemoryCardInfo[memcardind], MEMCARD_DEFAULT_SAVE_DIR, MakeSaveSlotString(tempStr, slotind).m_Str, SAVEPOINT_SAVE_SIZE);
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
    tMemoryCards->SetCreator((CREATOR)Create);

    tMemoryCards->RegisterProperty(tSTRING, "gamename", (EntityGetterFunction)&GetGamename, 0, 0, 0, (EntitySetterFunction)&SetGamename, 0, 0, 0, "control=string", 0, 0, -1);
    tMemoryCards->RegisterProperty(tSTRING, "ps2sleslicense", (EntityGetterFunction)&GetPs2SlesLicense, 0, 0, 0, (EntitySetterFunction)&SetPs2SlesLicense, 0, 0, 0, "control=string", 0, 0, -1);
    tMemoryCards->RegisterProperty(tSTRING, "ps2sluslicense", (EntityGetterFunction)&GetPs2SlusLicense, 0, 0, 0, (EntitySetterFunction)&SetPs2SlusLicense, 0, 0, 0, "control=string", 0, 0, -1);
    tMemoryCards->RegisterProperty(tINTEGER, "savefilesize", (EntityGetterFunction)&GetSaveFileSize, 0, 0, 0, (EntitySetterFunction)&SetSaveFileSize, 0, 0, 0, nullptr, 0, 0, 10);

    tMemoryCards->RegisterScript("ispresent(integer):truth", (EntityFunctionMember)&IsPresent, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("createsavepoint(integer,integer,entity,integer,entity)", (EntityFunctionMember)&CreateSavePoint, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("restoresavepoint(integer,integer,entity)", (EntityFunctionMember)&RestoreSavePoint, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("loadsavepointsummary(integer,integer,entity)", (EntityFunctionMember)&LoadSavePointSummary, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("deletesavepoint(integer,integer):truth", (EntityFunctionMember)&DeleteSavePoint, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("savepointoperationerror:integer", (EntityFunctionMember)&SavePointOperationError, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("savepointexists(integer,integer):truth", (EntityFunctionMember)&SavePointExists, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("preparecardforsavegames(integer):truth", (EntityFunctionMember)&PrepareCardForSavegames, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("unpreparecardforsavegames(integer):truth", (EntityFunctionMember)&UnPrepareCardForSavegames, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("iscardprepared(integer):truth", (EntityFunctionMember)&IsCardPrepared, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("isformatted(integer):truth", (EntityFunctionMember)&IsFormatted, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("formatcard(integer):truth", (EntityFunctionMember)&FormatCard, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("unformatcard(integer):truth", (EntityFunctionMember)&UnformatCard, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("getfreespace(integer):integer", (EntityFunctionMember)&GetCardFreeSpace, 0, 0, 0, nullptr, nullptr);
    tMemoryCards->RegisterScript("getsavepointsize(integer,integer):integer", (EntityFunctionMember)&GetSavePointSize, 0, 0, 0, nullptr, nullptr);
    // TODO: 'GetLastModifiedTime' methods here.
    tMemoryCards->RegisterScript("hascardchanged(integer):truth", (EntityFunctionMember)&HasCardChanged, 0, 0, 0, nullptr, nullptr);

    tMemoryCards->PropagateProperties();
}

MemoryCards* MemoryCards::Create(AllocatorIndex allocInd)
{
    return new MemoryCards();
}

const char* MemoryCards::GetGamename() const
{
    return m_GameName.m_Str;
}

void MemoryCards::SetGamename(const char* const gamename)
{
    if (strlen(gamename) <= 20)
        m_GameName = gamename;
}

const char* MemoryCards::GetPs2SlesLicense() const
{
    return m_Ps2SlesLicense.m_Str;
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
    return m_Ps2SlusLicense.m_Str;
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
    StoreProperty(10, &m_SaveFileSize, tINTEGER);
}

unsigned int MemoryCards::GetSavePointSize(unsigned int memcardind, unsigned int slotind) const
{
    String tempStr;
    MakeSaveSlotString(tempStr, slotind);
    return MemoryCardInfo[memcardind]->GetSavePointFileSize(MEMCARD_DEFAULT_SAVE_DIR, tempStr.m_Str);
}

void MemoryCards::HasCardChanged(int* args) const
{
    *args = NULL;
}

unsigned int MemoryCards::GetCardFreeSpace_Impl(unsigned int memcardind) const
{
    if (MemoryCardInfo[memcardind]->IsFormatted())
        return File::GetStorageFreeSpace().LowPart;

    LogDump::LogA("Warning: Memory Card not found or not formatted.\n");

    return NULL;
}

void MemoryCards::GetCardFreeSpace(int* args) const
{
    *args = GetCardFreeSpace_Impl(args[1]);
}

void MemoryCards::IsFormatted(int* args) const
{
    *args = MemoryCardInfo[args[1]]->IsFormatted();
}

void MemoryCards::IsCardPrepared(int* args) const
{
    *args = IsPrepared(args[1]);
}

void MemoryCards::IsPresent(int* args) const
{
    if (MemoryCardInfo[args[1]]->m_SaveFolderPath.m_Length)
        *args = MemoryCardInfo[args[1]]->m_Formatted && File::IsDirectoryValid(MemoryCardInfo[args[1]]->m_SaveFolderPath.m_Str);
    else
    {
        LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
        args = false;
    }
}

bool MemoryCards::IsPrepared(unsigned int memcardind) const
{
    if (!MemoryCardInfo[memcardind]->IsSaveDirPresent(MEMCARD_DEFAULT_SAVE_DIR))
        return false;

    if (memcardind != SAVE_SLOT_8 && !MemoryCardInfo[memcardind]->IsSaveFilePresent(MEMCARD_DEFAULT_SAVE_DIR, MEMCARD_DEFAULT_SAVE_DIR))
        return false;
    else
        return true;
}

void MemoryCards::FormatCard(int* args) const
{
    *args = MemoryCardInfo[args[1]]->FormatCard();
}

void MemoryCards::UnformatCard(int* args) const
{
    *args = MemoryCardInfo[args[1]]->UnformatCard();
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
    char slotindstr[12] = {};
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

    if (!MemoryCardInfo[memcardind]->m_SaveFolderPath.m_Length)
    {
        LogDump::LogA("Warning: Emulation dir not set. All operations will be ignored.\n");
        return false;
    }

    if (!MemoryCardInfo[memcardind]->IsFormatted() || !File::IsDirectoryValid(MemoryCardInfo[memcardind]->m_SaveFolderPath.m_Str))
        return false;

    String saveSlotStr;
    if (MakeSaveSlotString(saveSlotStr, slotind).m_Str == nullptr)
        return false;

    return MemoryCardInfo[memcardind]->IsSavePointFileExists(MEMCARD_DEFAULT_SAVE_DIR, saveSlotStr.m_Str);
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

    Utils::UniqueId uniqid;

    if (!savepoint.WriteBufferWithSize((const char*)&uniqid, sizeof(uniqid)))
        return false;

    m_MemCardsUniqueId[memcardind] = uniqid;

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

const char* const MemoryCards::GetSaveDirectory(const unsigned int slot) const
{
    return MEMCARD_DEFAULT_SAVE_DIR;
}

void MemoryCards::RestoreSavePoint(unsigned int memcardind, unsigned int slotind, Node* summarynode)
{
    Scene::SceneInstance->RestoreSavePoint(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode, this);
}

void MemoryCards::CreateSavePoint(unsigned int memcardind, unsigned int slotind, EntityType* textbox, unsigned int, const char* const savedirectory, Node* summarynode) const
{
    Scene::SceneInstance->CreateSavePoint(memcardind, slotind, MEMCARD_DEFAULT_SAVE_DIR, summarynode, m_SaveFileSize);
}