#include "SavePoint.h"
#include "LogDump.h"

bool SavePoint::GenerateAndWriteCRC(SavePoint* sp, const int pos)
{
    if (sp->m_Status != STATUS_CLOSED)
        sp->CloseFile();

    if (!sp->Open(STATUS_SUCCESS) || sp->IsFileOpen())
        return false;

    const int fsize = sp->GetPosition();
    sp->RewindFileToBeginning();
    const int checksum = SavePoint::CalculateChecksum(sp, pos);

    LogDump::LogA("checksum: %x\n", checksum);
    LogDump::LogA("sizes: %x:%x\n", fsize, pos);
    sp->CloseFile();

    if (!sp->Open(STATUS_2))
        return false;

    sp->Seek(pos);
    const int writesize = sp->WriteBufferWithSize((const char*)&checksum, sizeof(checksum));
    
    return writesize == sizeof(checksum) && sp->CloseFile();
}

bool SavePoint::VerifyFileChecksum(SavePoint* savepoint)
{
    if (savepoint->m_Status != STATUS_CLOSED)
    {
        if (savepoint->m_SaveFile)
        {
            --OpenFilesCount;
            delete savepoint->m_SaveFile;
        }

        savepoint->m_Status = STATUS_CLOSED;
    }

    if (!savepoint->Open(STATUS_SUCCESS) || savepoint->IsFileOpen())
        return false;

    unsigned int fileSize = savepoint->GetPosition(); // NOTE: why not 'GetSize'?
    if (fileSize > (1024 * 1024))
    {
        LogDump::LogA("VerifyFileChecksum failed because size is too large (file '%s' has size %i)\n", savepoint->GetSaveSlotDir(), fileSize);
        return false;
    }

    if (!savepoint->RewindFileToBeginning())
        return false;

    unsigned int checksum = savepoint->CalculateChecksum(savepoint, fileSize);
    savepoint->Seek(fileSize);

    unsigned int saveChecksum = NULL;
    if (savepoint->Read(&saveChecksum, sizeof(saveChecksum)) != sizeof(saveChecksum))
        return false;

    if (saveChecksum != checksum)
    {
        LogDump::LogA("Checksum mismatch: %x : %x\n", saveChecksum, checksum);
        return 0;
    }

    return savepoint->CloseFile() != false;
}

int SavePoint::OpenFilesCount;

SavePoint::SavePoint(MemoryCard* dirInfo, const char* saveDir, const char* saveSlotId, unsigned int bufferSize)
{
    MESSAGE_CLASS_CREATED(SavePoint);

    m_SaveDir = saveDir;
    m_SlotIdStr = saveSlotId;
    m_SlotDir;
    m_SaveMemoryCard = dirInfo;
    m_Status = STATUS_SUCCESS;
    m_SaveFile = nullptr;
}

time_t SavePoint::GetTime() const
{
    String tempStr;

    if (m_SaveMemoryCard->IsFormatted())
        return FileBuffer::GetFileTimestamp(m_SaveMemoryCard->GetFullSaveFolderPath(tempStr, m_SaveDir.m_Str, m_SlotDir.m_Str).m_Str);
    else
        LogDump::LogA("Warning: Memory Card not found or not formatted.\n");

    return NULL;
}

bool SavePoint::Open(SavePointStatus mode)
{
    if (!m_SaveMemoryCard->IsFormatted())
    {
        LogDump::LogA("Warning: Memory Card not found or not formatted.\n");
        return false;
    }

    if (OpenFilesCount >= SAVEPOINT_MAX_OPEN_FILES)
    {
        LogDump::LogA("Warning: 3 Files already open. Open command ignored.\n");
        return false;
    }

    m_Status = mode;
    String saveSlotPath;
    m_SaveMemoryCard->GetFullSaveFolderPath(saveSlotPath, m_SaveDir.m_Str, m_SlotIdStr.m_Str);
    m_SaveFile = new FileBuffer(saveSlotPath.m_Str, mode ? (mode == STATUS_1 ? 2 | 96 : (mode == STATUS_2 ? 18 | 96 : 1 | 96)) : 1 | 96, true);

    if (m_SaveFile && m_SaveFile->IsFileOpen())
        ++OpenFilesCount;
    else
        m_Status = STATUS_CLOSED;

    return m_Status != STATUS_CLOSED;
}

int SavePoint::CalculateChecksum(SavePoint* savepoint, int pos)
{
    int checksum;
    Utils::crc32_init_default(&checksum);

    if (pos <= NULL)
    {
        Utils::crc32_gen(&checksum);
        return checksum;
    }

    char buffer[1024] = {};
    while (true)
    {
        int readsize = savepoint->Read(buffer, sizeof(buffer));
        if (readsize <= NULL)
        {
            Utils::crc32_gen(&checksum);
            return -1;
        }

        if (readsize > pos)
            readsize = pos;

        pos -= readsize;

        if (!readsize)
            break;

        Utils::generic_crc32(&checksum, buffer, readsize);

        if (pos <= NULL)
        {
            Utils::crc32_gen(&checksum);
            return checksum;
        }
    }

    return NULL;
}

bool SavePoint::CloseFile()
{
    if (m_SaveFile)
    {
        --OpenFilesCount;
        delete m_SaveFile;
    }

    m_Status = STATUS_CLOSED;
    return true;
}

SavePoint::~SavePoint()
{
    MESSAGE_CLASS_DESTROYED(SavePoint);

    if (m_SaveFile) {
        --OpenFilesCount;
        delete m_SaveFile;
        m_SaveFile = nullptr;
    }

    m_Status = STATUS_CLOSED;
}