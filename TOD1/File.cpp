#include "File.h"
#include "LogDump.h"
#include "ZipArch.h"
#include "Window.h"
#include "ScriptDatabase.h"
#include <map>

unsigned int File::FilesOpen = NULL;
HANDLE File::FilesSemaphoreArray[FILE_MAX_ZIP_FILES];
std::vector<StringTuple> File::DirectoryMappingsList;
unsigned int File::AlignmentArray[3] = { 1, 1, 4 };
FileWrapper* FileWrapper::ZipFilesArray[FILE_MAX_ZIP_FILES] = {};
bool FileWrapper::GameDiscFound = false;
String FileWrapper::WorkingDirectory;
String FileWrapper::GameWorkingDirectory;

bool FileWrapper::IsStorageFull()
{
    ULARGE_INTEGER spaceAvailable = {};
    GetDiskFreeSpaceEx(WorkingDirectory.m_Str, &spaceAvailable, nullptr, nullptr);

    return spaceAvailable.QuadPart == NULL;
}

ULARGE_INTEGER FileWrapper::GetStorageFreeSpace()
{
    ULARGE_INTEGER spaceAvailable;

    GetDiskFreeSpaceEx(WorkingDirectory.m_Str, &spaceAvailable, nullptr, nullptr);
    return spaceAvailable;
}

void FileWrapper::DeleteFileFromGameDir(const char* const filename)
{
    String filepath = filename;
    GetWorkingDirRelativePath(filepath);
    DeleteFile(filepath.m_Str);
}

void FileWrapper::FindGameDir()
{
    char currdir[1024];
    memset(currdir, NULL, sizeof(currdir));

    GetCurrentDirectory(sizeof(currdir), currdir);
    SetWorkingDir(currdir);
    SetGameWorkingDir(currdir);
    GameDiscFound = false;
    SetErrorMode(SEM_FAILCRITICALERRORS);
    char driveLetter = NULL;
    char RootPathName[] = "/:";

    for (int drive = GetLogicalDrives(); drive; ++driveLetter)
    {
        if (drive & 1)
        {
            *(unsigned char*)&RootPathName = 'A' + driveLetter;
#ifdef INCLUDE_FIXES
            if (GetDriveType(RootPathName) == DRIVE_CDROM || GetDriveType(RootPathName) == DRIVE_FIXED)
#else
            if (GetDriveType(RootPathName) == DRIVE_CDROM)
#endif
            {
                char buffer[64];
                strcpy(buffer, RootPathName);
                strcat(buffer, "/Program Files/Eidos/Total Overdose/TotalOverdose.exe");

                LogDump::LogA("Found cd/dvd drive %s - looking for ID file '%s'\n", RootPathName, buffer);

                if (GetFileAttributes(buffer) != -1)
                {
                    memset(buffer, NULL, sizeof(buffer));
                    strcpy(buffer, RootPathName);
                    strcat(buffer, "/Program Files/Eidos/Total Overdose/");

                    SetGameWorkingDir(buffer);
                    GameDiscFound = true;

                    LogDump::LogA("Found game disc.\n");

                    break;
                }
            }
        }
        drive = drive >> 1;
    }

    SetErrorMode(NULL);
}

bool FileWrapper::IsDirectoryValid(const char* const dir)
{
    String dirStr = dir;
    GetWorkingDirRelativePath(dirStr);

    int dirAttributes = GetFileAttributes(dirStr.m_Str);

    if (dirAttributes != INVALID_FILE_ATTRIBUTES)
        return (dirAttributes & FILE_ATTRIBUTE_DIRECTORY) != false;

    if (!GameDiscFound)
        return false;

    dirStr = dir;
    GetGameWorkingDirRelativePath(dirStr);

    dirAttributes = GetFileAttributes(dirStr.m_Str);

    if (dirAttributes != INVALID_FILE_ATTRIBUTES)
        return (dirAttributes & FILE_ATTRIBUTE_DIRECTORY) != false;

    return false;
}

bool FileWrapper::IsFileReadOnly(const char* const file)
{
    String fileStr = file;
    GetWorkingDirRelativePath(fileStr);

    return GetFileAttributes(fileStr.m_Str) & FILE_ATTRIBUTE_READONLY;
}

bool FileWrapper::IsFileValid(const char* const file)
{
    String fileStr = file;
    GetWorkingDirRelativePath(fileStr);

    return GetFileAttributes(fileStr.m_Str) != INVALID_FILE_ATTRIBUTES;
}

void FileWrapper::CreateNewDirectory(const char* const dir)
{
    String dirStr = dir;
    GetWorkingDirRelativePath(dirStr);

    if (!IsDirectoryValid(dirStr.m_Str))
        CreateDirectory(dirStr.m_Str, nullptr);
}

void FileWrapper::RemoveDirectory_(const char* const dir)
{
    String workingDir = dir;
    GetWorkingDirRelativePath(workingDir);

    char* lastslash = strrchr(workingDir.m_Str, '/');
    if (lastslash != nullptr)
        *lastslash = '\0';

    RemoveDirectory(workingDir.m_Str);
}

bool FileWrapper::EnumerateFolderFiles(const char* const dir, std::vector<String>& outFilesList)
{
    String workingDir = dir;
    GetWorkingDirRelativePath(workingDir);

    if (workingDir.m_Str[workingDir.m_Length - 1] != '/')
    {
        LogDump::LogA("'%s' must end with '/'\n", workingDir.m_Str);
        return false;
    }

    if (!FileWrapper::IsDirectoryValid(workingDir.m_Str))
    {
        LogDump::LogA("'%s' is not a valid directory\n", workingDir.m_Str);
        return false;
    }

    String searchMask = workingDir.m_Str;
    searchMask.Append("*.*");

    WIN32_FIND_DATA searchFilesData;
    HANDLE searchHandle = FindFirstFile(searchMask.m_Str, &searchFilesData);

    if (searchHandle == INVALID_HANDLE_VALUE)
        return false;

    do
        outFilesList.push_back(searchFilesData.cFileName);
    while (FindNextFile(searchHandle, &searchFilesData));

    FindClose(searchHandle);
    return true;
}

void FileWrapper::SetFileAttrib(const char* const file, unsigned int attrib, char unk)
{
    String fileStr = file;
    GetWorkingDirRelativePath(fileStr);

    int newAttrib;
    switch (attrib)
    {
    case 1:
        newAttrib = FILE_ATTRIBUTE_READONLY;
        break;
    case 2:
        newAttrib = FILE_ATTRIBUTE_HIDDEN;
        break;
    case 4:
        newAttrib = FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE;
        break;
    default:
        newAttrib = unk;
        break;
    }

    SetFileAttributes(fileStr.m_Str, unk ? (newAttrib | GetFileAttributes(fileStr.m_Str)) : (~newAttrib & GetFileAttributes(fileStr.m_Str)));
}

bool FileWrapper::CheckGameFileAttributes(const char* const filename, const FileAttribute mode)
{
    String gamedirrelativefilepath = filename;
    GetWorkingDirRelativePath(gamedirrelativefilepath);
    DWORD fattrib = GetFileAttributes(gamedirrelativefilepath.m_Str);

    switch (mode)
    {
    case READONLY:
        return fattrib & FILE_ATTRIBUTE_READONLY;
    case HIDDEN:
        return fattrib & FILE_ATTRIBUTE_HIDDEN;
    case ARCHIVE:
        return fattrib & FILE_ATTRIBUTE_ARCHIVE;
    default:
        return false;
    }
}

void FileWrapper::SetExecuteAttr(unsigned char _attr)
{
    EnterCriticalSection(&m_CriticalSection);
    m_ExecuteAttribute = _attr;
    LeaveCriticalSection(&m_CriticalSection);
}

time_t File::GetFileTimestamp_Impl(const char* path)
{
    String workingdir = path;
    WIN32_FIND_DATA findData = {};
    HANDLE findHandle = INVALID_HANDLE_VALUE;
    FILETIME fileTime = {};
    SYSTEMTIME fileTimeSystem = {};
    tm time_ = {};

    findHandle = FindFirstFile(workingdir.m_Str, &findData);
    if (findHandle == INVALID_HANDLE_VALUE && FileWrapper::GameDiscFound)
    {
        workingdir = path;
        FileWrapper::GetGameWorkingDirRelativePath(workingdir);
        findHandle = FindFirstFile(workingdir.m_Str, &findData);
    }

    FileTimeToLocalFileTime(&findData.ftLastWriteTime, &fileTime);
    FileTimeToSystemTime(&fileTime, &fileTimeSystem);
    FindClose(findHandle);

    time_.tm_hour = fileTimeSystem.wHour;
    time_.tm_year = fileTimeSystem.wYear - 1900;
    time_.tm_mday = fileTimeSystem.wDay;
    time_.tm_min = fileTimeSystem.wMinute;
    time_.tm_mon = fileTimeSystem.wMonth - 1;
    time_.tm_isdst = -1;
    time_.tm_sec = fileTimeSystem.wSecond;
    time_.tm_wday = fileTimeSystem.wDayOfWeek;
    time_.tm_yday = NULL;

    return mktime(&time_);
}

bool File::WriteBuffers()
{
    if (m_ReadFromZip)
        return false;

    EnterCriticalSection(&m_FileHandle->m_CriticalSection);

    if (!m_FileHandle->m_Read)
    {
        if (m_FileHandle->m_Buffer != m_FileHandle->m_BufferBegin)
        {
            DWORD numBytesWritten = NULL;
            WriteFile(m_FileHandle->m_File, m_FileHandle->m_Buffer, m_FileHandle->m_BufferBegin - m_FileHandle->m_Buffer, &numBytesWritten, nullptr);
            m_FileHandle->m_BufferEnd = m_FileHandle->m_BufferBegin = m_FileHandle->m_Buffer;
        }
        FlushFileBuffers(m_FileHandle->m_File);
    }

    LeaveCriticalSection(&m_FileHandle->m_CriticalSection);

    return true;
}

int File::_vsnprintf(File* _f, const char* _format, ...)
{
    va_list va;
    char buf[1024] = {};

    va_start(va, _format);
    int written = vsnprintf(buf, sizeof(buf), _format, va);

    for (int ind = 0; ind < written; ++ind)
        _f->_WriteBufferBlockAndInsertNewLine(buf[ind]);

    return written;
}

#pragma message(TODO_IMPLEMENTATION)
int File::_scanf(File* _f, const char* _format, ...)
{
    va_list va;
    va_start(va, _format);

    if (m_ReadFromZip)
        return (int)_f; // FIXME: wtf?
    else
        return _f->_scanf_impl((char*)_format, (int*)&va); // FIXME: wtf?
}

int File::WriteFormattedVarlistDataToBuffer(char* _buf, va_list args)
{
    char buf[1024] = {};

    int written = vsnprintf(buf, sizeof(buf), _buf, args);

    for (int ind = 0; ind < written; ++ind)
        _WriteBufferBlockAndInsertNewLine(buf[ind]);

    return written;
}

#pragma message(TODO_IMPLEMENTATION)
int File::_scanf_impl(char* format, int* outArgs)
{
#ifndef _EXE
    // NOTE: this checks input string for formatting symbol and does something with it. Why is it here?
    return (*(int(__thiscall*)(File*, const char*, int*))0x42F0A0)(this, format, outArgs);
#else
    LogDump::LogA("File::_scanf_impl is not implemented!\n");
    return NULL;
#endif
}

char File::ReadBlock()
{
    if (!m_ReadFromZip)
        return m_FileHandle->ReadBlock();

    if (m_SeekPosition >= m_SizeInZip)
        return -1;

    WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);

    FileWrapper::ZipFilesArray[m_ZipSlot]->SetExecuteAttr(m_ExecuteAttribute);
    FileWrapper::ZipFilesArray[m_ZipSlot]->Seek(m_OffsetInZip + m_SeekPosition);
    char charread = FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlock();
    m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_OffsetInZip;

    ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

    return charread;
}

int File::ReadBlockAndGetPosition()
{
    int bytesread = ReadBlock();
    ReadBlockDecreasePosition();

    return bytesread;
}

char File::_WriteBufferBlockAndInsertNewLine(char _newlinesym)
{
    return m_FileHandle->_WriteBufferBlockAndInsertNewLine(_newlinesym);
}

int File::Read(void* _buffer, unsigned int _numbytestoread)
{
    if (!m_ReadFromZip)
        return m_FileHandle->Read(_buffer, _numbytestoread);

    if (m_SeekPosition >= m_SizeInZip)
        return NULL;

    WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);
    FileWrapper::ZipFilesArray[m_ZipSlot]->Seek(m_SeekPosition + m_OffsetInZip);
    FileWrapper::ZipFilesArray[m_ZipSlot]->SetExecuteAttr(true);

    if (m_ExecuteAttribute)
    {
        int bytesread = FileWrapper::ZipFilesArray[m_ZipSlot]->Read(_buffer, ((m_SeekPosition + _numbytestoread) > m_SizeInZip) ? m_SizeInZip - m_SeekPosition : _numbytestoread);

        m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_OffsetInZip;
        ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

        return bytesread;
    }
    else
    {
        int bytesread = NULL;

        for (; bytesread < _numbytestoread; bytesread++)
        {
            if (FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() >= m_SizeInZip + m_OffsetInZip)
                break;
            char lastchar = FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlock();
            if (lastchar == '\n' ||
                lastchar == '\r')
            {
                char lastchar_2 = FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlock();
                if ((lastchar_2 == '\n' || lastchar_2 == '\r') && lastchar == lastchar_2)
                    FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlockDecreasePosition();
                lastchar = '\n';
            }
            ((char*)(_buffer))[bytesread] = lastchar;
        }

        m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_OffsetInZip;
        ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

        return bytesread;
    }
}

int File::WriteBuffer(const char* _buf)
{
    return m_FileHandle->WriteBuffer(_buf, strlen(_buf));
}

int File::WriteBufferWithSize(const char* _buf, int _size)
{
    return m_FileHandle->WriteBuffer(_buf, _size);
}

int File::Seek(int _pos)
{
    if (!m_ReadFromZip)
        return m_FileHandle->Seek(_pos);

    return m_SeekPosition = _pos;
}

char File::_WriteBufferAndSetToStart()
{
    if (m_ReadFromZip)
    {
        m_SeekPosition = NULL;
        return true;
    }

    EnterCriticalSection(&m_FileHandle->m_CriticalSection);

    if (m_FileHandle->m_File)
    {
        if (m_FileHandle->m_Buffer != m_FileHandle->m_BufferBegin &&
            !m_FileHandle->m_Read)
        {
            DWORD numBytesRead = NULL;
            WriteFile(m_FileHandle->m_File, m_FileHandle->m_Buffer, m_FileHandle->m_BufferBegin - m_FileHandle->m_Buffer, &numBytesRead, nullptr);
            m_FileHandle->m_BufferEnd = m_FileHandle->m_BufferBegin = m_FileHandle->m_Buffer;
        }

        if (m_FileHandle->m_Read)
        {
            if (m_FileHandle->m_BufferBegin != m_FileHandle->m_BufferEnd)
                SetFilePointer(m_FileHandle->m_File, m_FileHandle->m_BufferBegin - m_FileHandle->m_BufferEnd, NULL, FILE_CURRENT);
            m_FileHandle->m_BufferEnd = m_FileHandle->m_BufferBegin = m_FileHandle->m_Buffer;
        }

        m_FileHandle->m_FilePosition = SetFilePointer(m_FileHandle->m_File, NULL, NULL, FILE_BEGIN);
    }

    LeaveCriticalSection(&m_FileHandle->m_CriticalSection);
    return true;
}

char File::WriteFromBuffer()
{
    if (!m_ReadFromZip)
        return m_FileHandle->WriteFromBufferAndSetToEnd();

    m_SeekPosition = m_SizeInZip;

    return true;
}

int File::GetPosition() const
{
    return m_ReadFromZip ? m_SeekPosition : m_FileHandle->GetPosition();
}

char File::ReadBlockDecreasePosition()
{
    if (!m_ReadFromZip)
        return m_FileHandle->ReadBlockDecreasePosition();

    WaitForSingleObject(FilesSemaphoreArray[m_ZipSlot], INFINITE);

    FileWrapper::ZipFilesArray[m_ZipSlot]->Seek(m_SeekPosition + m_OffsetInZip);
    FileWrapper::ZipFilesArray[m_ZipSlot]->SetExecuteAttr(m_ExecuteAttribute);
    FileWrapper::ZipFilesArray[m_ZipSlot]->ReadBlockDecreasePosition();
    m_SeekPosition = FileWrapper::ZipFilesArray[m_ZipSlot]->GetPosition() - m_OffsetInZip;

    ReleaseSemaphore(FilesSemaphoreArray[m_ZipSlot], 1, 0);

    return true;
}

char File::ReadIfNotEOF()
{
    if (ReadBlock() == -1)
        return true;

    ReadBlockDecreasePosition();

    return false;
}

const char* File::GetFileName() const
{
    return m_FileName.m_Str;
}

File::File(const char* _filename, int _desiredaccess, bool _createifnotfound)
{
    String fname;

    GetPathFromDirectoryMappings(fname, _filename);

    m_FileName = fname.m_Str;
    m_FileHandle = nullptr;
    m_ReadFromZip = false;
    m_ZipSlot = -1;
    m_FileReadAttribute = (_desiredaccess >> 7) & 1;
    m_ExecuteAttribute = NULL;
    m_SeekPosition = NULL;
    m_ZipFileHandle = nullptr;
    m_OffsetInZip = NULL;
    m_SizeInZip = NULL;

    ZipArch::FileInfo zipFileInfo;
    if (_desiredaccess & 1 &&
        ZipArch::SlotId > 0 &&
        ZipArch::FindFile(_filename, zipFileInfo, &m_ZipSlot))
    {
        m_ReadFromZip = true;
        m_SizeInZip = zipFileInfo.m_FileSize;
        m_OffsetInZip = zipFileInfo.m_OffsetInArch;
        m_SeekPosition = NULL;
        m_ExecuteAttribute = (_desiredaccess >> 5) & 1;
        m_ZipFileHandle = nullptr;
    }
    else
        m_FileHandle = new FileWrapper(m_FileName.m_Str, _desiredaccess, _createifnotfound);

    if ((m_ReadFromZip && m_OffsetInZip) || (m_FileHandle && m_FileHandle->m_File != NULL))
        ++FilesOpen;

    MESSAGE_CLASS_CREATED(File);
}

File::~File()
{
    MESSAGE_CLASS_DESTROYED(File);

    if (m_ReadFromZip)
        if (m_ZipFileHandle)
            delete m_ZipFileHandle;

    if ((m_ReadFromZip && m_OffsetInZip >= 0) || (m_FileHandle->m_File != nullptr))
        --FilesOpen;

    if (m_FileHandle)
        delete m_FileHandle;
}

unsigned int File::GetSize()
{
    if (m_ReadFromZip)
        return m_SizeInZip;

    unsigned int currPos = m_FileHandle->GetPosition();
    m_FileHandle->WriteFromBufferAndSetToEnd();
    unsigned int fsize = m_FileHandle->GetPosition();
    m_FileHandle->Seek(currPos);

    return fsize;
}

bool File::IsFileOpen() const
{
    return (m_ReadFromZip && m_OffsetInZip >= 0) || m_FileHandle->m_File != nullptr;
}

char File::ReadString(String& outStr)
{
    if (ReadIfNotEOF())
        return false;

    char _charread = ReadBlock();

    if (_charread == -1)
        return true;

    while (true)
    {
        if (_charread == '\n')
        {
            char _endsym = ReadBlock();
            if (_endsym != '\r' && _endsym != -1)
                ReadBlockDecreasePosition();

            return true;
        }

        if (_charread == '\r')
            break;

        char buf[2] = { _charread, NULL };
        outStr.Append(buf);

        _charread = ReadBlock();
        if (_charread == -1)
            return true;
    }

    char _endsym = ReadBlock();
    if (_endsym != '\n' && _endsym != -1)
        ReadBlockDecreasePosition();

    return true;
}

void File::SetPosAligned(unsigned char alignind)
{
    Seek(~(AlignmentArray[alignind] - 1) & (AlignmentArray[alignind] + GetPosition() - 1));
}

int File::ReadIntLittleToBigEndian()
{
    unsigned char b3 = ReadBlock();
    unsigned char b1 = ReadBlock();
    unsigned char b2 = ReadBlock();

    return b3 + ((b1 + ((b2 + ((unsigned char)ReadBlock() << 8)) << 8)) << 8);
}

short File::ReadShortLittleToBigEndian()
{
    return ReadBlock() + ((unsigned char)ReadBlock() << 8);
}

HANDLE File::GetFileHandle()
{
    if (!m_ReadFromZip)
        if (m_FileHandle)
            return m_FileHandle->m_File;
        else
            return NULL;

    if (m_ZipSlot == -1 || m_OffsetInZip == -1 || !FileWrapper::ZipFilesArray[m_ZipSlot])
        return NULL;

    m_ZipFileHandle = new FileWrapper(ZipArch::ZipNames[m_ZipSlot].m_Str, 0x21, true);
    m_ZipFileHandle->Seek(m_OffsetInZip);

    return m_ZipFileHandle->m_File;
}

void File::AddDirectoryMappingsListEntry(const char* str1, const char* str2)
{
    DirectoryMappingsList.push_back({ str1, str2 });
}

String& File::GetPathFromDirectoryMappings(String& outStr, const char* path)
{
    if (DirectoryMappingsList.size() <= NULL)
    {
        outStr = path;
        return outStr;
    }

    const size_t pathstrlen = strlen(path);
    for (std::vector<StringTuple>::iterator it = DirectoryMappingsList.begin(); it != DirectoryMappingsList.end(); ++it)
    {
        if (strncmp(path, it->m_String_1.m_Str, pathstrlen) == NULL)
        {
            outStr = it->m_String_2.m_Str;
            outStr.Append(&path[it->m_String_1.m_Length]);

            return outStr;
        }
    }

    outStr = path;
    return outStr;
}

String* File::ExtractFileDir(String& outStr, const char* path)
{
    char buf[1024] = {};
    char buf_[1024] = {};
    ExtractFilePath(path, buf, buf_, buf_);

    outStr = buf;
    return &outStr;
}

String* File::ExtractFileName(String& outStr, const char* path)
{
    char buf[1024] = {};
    char buf_[1024] = {};

    if (path && *path)
    {
        ExtractFilePath(path, buf_, buf, buf_);
        outStr = buf;
        return &outStr;
    }
    else
        return NULL;
}

void File::FindDirectoryMappedFileAndDelete(const char* const filename)
{
    String tempStr;
    GetPathFromDirectoryMappings(tempStr, filename);
    FileWrapper::DeleteFileFromGameDir(tempStr.m_Str);
}

bool File::FindFileEverywhere(const char* path)
{
    if (!path || *path == NULL)
        return false;

    String pathStr;
    GetPathFromDirectoryMappings(pathStr, path);
    int zipSlot;
    ZipArch::FileInfo zipFileInfo;

    if (ZipArch::SlotId > 0 && ZipArch::FindFile(pathStr.m_Str, zipFileInfo, &zipSlot))
        return true;

    return FileWrapper::IsFileExists(pathStr.m_Str);
}

time_t File::GetFileTimestamp(const char* filename)
{
    String pathStr;
    GetPathFromDirectoryMappings(pathStr, filename);
    int zipslot = NULL;
    ZipArch::FileInfo zipFileInfo;

    if (ZipArch::SlotId <= 0 || !ZipArch::FindFile(pathStr.m_Str, zipFileInfo, &zipslot))
        return GetFileTimestamp_Impl(pathStr.m_Str);
    else
        return GetFileTimestamp_Impl(FileWrapper::ZipFilesArray[0]->m_WorkingDir.m_Str);
}

void File::ExtractFilePath(const char* inFilePath, char* outDirectory, char* outFileName, char* outFileExtension)
{
    if (!inFilePath || !*inFilePath)
        return;

    const unsigned int pathlen = strlen(inFilePath);
    const char* const lastdotpos = strrchr(inFilePath, '.');
    const char* lastslashpos = strrchr(inFilePath, '/');
    if (!lastslashpos)
        lastslashpos = strrchr(inFilePath, '\\');
    if (!lastslashpos)
        return;

    if (lastdotpos)
        strcpy(outFileExtension, lastdotpos + 1);

    strncpy(outFileName, lastslashpos + 1, lastdotpos ? (lastdotpos - lastslashpos - 1) : (inFilePath + pathlen - lastslashpos));
    strncpy(outDirectory, inFilePath, lastslashpos - inFilePath + 1);
}

void File::ExtractFileExtension(String& outStr, const char* const path)
{
    char fdir[1024] = {};
    char fname[128] = {};
    char fext[16] = {};
    ExtractFilePath(path, fdir, fname, fext);

    outStr = fext;
}

FileWrapper::FileWrapper(const char* _filename, int _desiredaccess, bool _createifnotfound)
{
    m_File = NULL;
    m_CreateIfNotFound = _createifnotfound;
    m_DesiredAccess_1 = _desiredaccess;
    m_Buffer = m_BufferBegin = m_BufferEnd = nullptr;
    m_CreationDisposition = NULL;
    m_DesiredAccess_2 = NULL;
    m_ExecuteAttribute = NULL;
    m_FilePosition = NULL;
    m_Read = false;
    m_CriticalSection = {};

    InitializeCriticalSection(&m_CriticalSection);

    if (!_filename)
        return;

    m_WorkingDir = _filename;
    m_GameWorkingDir = _filename;
    GetWorkingDirRelativePath(m_WorkingDir);
    GetGameWorkingDirRelativePath(m_GameWorkingDir);

    if (_desiredaccess & FILE_READ_EA)
        IsFileExists(m_GameWorkingDir.m_Str);

    m_DesiredAccess_2 = NULL;

    if (_desiredaccess & FILE_READ_DATA)
    {
        m_DesiredAccess_2 = 0x80000000;
        m_Read = true;
    }
    else
        if (_desiredaccess & FILE_WRITE_DATA)
        {
            m_DesiredAccess_2 = 0x40000000;
            m_Read = false;
        }

    if (_desiredaccess & FILE_READ_DATA)
        m_CreationDisposition = OPEN_EXISTING;
    else
        if (_desiredaccess & FILE_WRITE_DATA && _desiredaccess & FILE_WRITE_EA)
            m_CreationDisposition = OPEN_ALWAYS;
        else
            m_CreationDisposition = CREATE_ALWAYS;

    m_ExecuteAttribute = (_desiredaccess >> 5) & 1;
    m_FilePosition = NULL;
    m_Buffer = nullptr;

    if (_createifnotfound)
        CreateFile_();

    MESSAGE_CLASS_CREATED(FileWrapper);
}

FileWrapper::~FileWrapper()
{
    if (m_File)
    {
        if (m_Buffer != m_BufferBegin && !m_Read)
        {
            DWORD numBytesWritten = NULL;
            WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, nullptr);
            m_BufferEnd = m_BufferBegin = m_Buffer;
        }
    }

    if (m_Buffer)
        delete[] m_Buffer;

    if (m_File && m_CreateIfNotFound)
        CloseHandle(m_File);

    MESSAGE_CLASS_DESTROYED(FileWrapper);
}

HANDLE FileWrapper::CreateFile_()
{
    EnterCriticalSection(&m_CriticalSection);

    m_File = CreateFile(m_WorkingDir.m_Str, m_DesiredAccess_2, FILE_SHARE_READ, NULL, m_CreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);

    if (m_File == INVALID_HANDLE_VALUE)
    {
        if (m_DesiredAccess_1 & FILE_WRITE_DATA)
        {
            char errbuf[512] = {};
            GetLastErrorMessage(errbuf);
            LogDump::LogA("Could not open file for writing: %s\n", errbuf);

            m_File = nullptr;
            LeaveCriticalSection(&m_CriticalSection);
            return m_File;
        }

        if (FileWrapper::GameDiscFound)
        {
            m_WorkingDir = m_GameWorkingDir;
            m_File = CreateFile(m_WorkingDir.m_Str, m_DesiredAccess_2, FILE_SHARE_READ, NULL, m_CreationDisposition, FILE_FLAG_WRITE_THROUGH, NULL);

            if (m_File == INVALID_HANDLE_VALUE)
            {
                m_File = nullptr;
                LeaveCriticalSection(&m_CriticalSection);
                return m_File;
            }
        }
    }

    if (m_File)
    {
        m_Buffer = new char[FILE_BUFFER_SIZE];
        m_BufferBegin = m_BufferEnd = m_Buffer;
    }

    LeaveCriticalSection(&m_CriticalSection);

    return m_File;
}

int FileWrapper::ReadBlock()
{
    EnterCriticalSection(&m_CriticalSection);

    if (m_BufferBegin == m_BufferEnd &&
        m_Read)
    {
        DWORD numBytesRead = NULL;
        ReadFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesRead, nullptr);
        m_BufferEnd = &m_Buffer[numBytesRead];
        m_BufferBegin = m_Buffer;
    }

    if (m_BufferBegin == m_BufferEnd)
    {
        LeaveCriticalSection(&m_CriticalSection);

        return -1;
    }

    ++m_FilePosition;
    char endsym = *m_BufferBegin;
    m_BufferBegin += 1;

    if (m_ExecuteAttribute || endsym != '\r')
    {
        LeaveCriticalSection(&m_CriticalSection);

        return endsym;
    }
    else
    {
        if (m_BufferBegin == m_BufferEnd)
            ReadBlock_Internal();

        m_BufferBegin++;
        m_FilePosition++;

        LeaveCriticalSection(&m_CriticalSection);

        return '\n';
    }
}

int FileWrapper::GetPosition() const
{
    EnterCriticalSection(&m_CriticalSection);
    int filepos = m_FilePosition;
    LeaveCriticalSection(&m_CriticalSection);

    return filepos;
}

int FileWrapper::Seek(int _pos)
{
    EnterCriticalSection(&m_CriticalSection);

    if (_pos != m_FilePosition)
    {
        if (m_Buffer != m_BufferBegin &&
            !m_Read)
        {
            DWORD numBytesRead = NULL;
            WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesRead, nullptr);
            m_BufferEnd = m_BufferBegin = m_Buffer;
        }

        if (m_Read)
        {
            if (m_BufferBegin != m_BufferEnd)
                SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
            m_BufferEnd = m_BufferBegin = m_Buffer;
        }

        m_FilePosition = SetFilePointer(m_File, _pos, NULL, FILE_BEGIN);
    }

    LeaveCriticalSection(&m_CriticalSection);

    return m_FilePosition;
}

void FileWrapper::GetLastErrorMessage(char* _buf)
{
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, GetLastError(), NULL, _buf, NULL, nullptr);
}

void FileWrapper::MoveFileWithReplace(const char* const oldname, const char* const newname)
{
    MoveFileEx(oldname, newname, MOVEFILE_REPLACE_EXISTING);
}

bool FileWrapper::IsFileExists(const char* const file)
{
    String temp = file;
    FileWrapper::GetWorkingDirRelativePath(temp);

    if (GetFileAttributes(temp.m_Str) == INVALID_FILE_ATTRIBUTES)
    {
        if (!GameDiscFound)
            return false;

        temp = file;
        FileWrapper::GetGameWorkingDirRelativePath(temp);

        if (GetFileAttributes(temp.m_Str) == INVALID_FILE_ATTRIBUTES)
            return false;
    }

    return true;
}

void FileWrapper::ReadBlock_Internal()
{
    if (m_Read)
    {
        DWORD numBytesRead = NULL;
        ReadFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesRead, nullptr);
        m_BufferEnd = &m_Buffer[numBytesRead];
        m_BufferBegin = m_Buffer;
    }
}

void FileWrapper::_436E30()
{
    if (m_Buffer != m_BufferBegin && m_Read)
    {
        DWORD numBytesWritten = NULL;
        WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, NULL);
        m_BufferEnd = m_BufferBegin = m_Buffer;
    }
}

void FileWrapper::_436E70()
{
    if (m_Read)
    {
        if (m_BufferBegin != m_BufferEnd)
            SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
        m_BufferEnd = m_BufferBegin = m_Buffer;
    }
}

void FileWrapper::SetFileHandle(HANDLE _hnd)
{
    EnterCriticalSection(&m_CriticalSection);
    if (!m_CreateIfNotFound)
        m_File = _hnd;
    LeaveCriticalSection(&m_CriticalSection);
}

void FileWrapper::ReleaseFileHandle()
{
    EnterCriticalSection(&m_CriticalSection);
    if (!m_CreateIfNotFound)
        m_File = nullptr;
    LeaveCriticalSection(&m_CriticalSection);
}

void FileWrapper::FlushAndClose(HANDLE)
{
    EnterCriticalSection(&m_CriticalSection);

    if (m_File && m_Buffer != m_BufferBegin)
    {
        DWORD numBytesWritten = NULL;
        WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, nullptr);
        m_BufferEnd = m_BufferBegin = m_Buffer;
    }

    delete m_Buffer;

    if (m_File)
        CloseHandle(m_File);

    LeaveCriticalSection(&m_CriticalSection);
}

char FileWrapper::_WriteBufferBlockAndInsertNewLine(char _sym)
{
    EnterCriticalSection(&m_CriticalSection);

    if (m_BufferBegin - m_Buffer == FILE_BUFFER_SIZE &&
        m_Buffer != m_BufferBegin &&
        !m_Read)
    {
        DWORD numBytesWritten;
        WriteFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesWritten, nullptr);
        m_BufferEnd = m_BufferBegin = m_Buffer;
    }

    if (m_ExecuteAttribute ||
        _sym != '\n')
    {
        *m_BufferBegin = _sym;
        ++m_BufferBegin;
        m_FilePosition++;

        LeaveCriticalSection(&m_CriticalSection);

        return true;
    }
    else
    {
        *m_BufferBegin = '\r';
        m_BufferBegin++;

        if (m_BufferBegin - m_Buffer == FILE_BUFFER_SIZE)
            _436E30();

        *m_BufferBegin = '\n';
        ++m_BufferBegin;
        m_FilePosition += 2;

        LeaveCriticalSection(&m_CriticalSection);

        return true;
    }
}

int FileWrapper::WriteBuffer(const char* _buf, int _len)
{
    EnterCriticalSection(&m_CriticalSection);

    if (_len <= FILE_BUFFER_SIZE)
    {
        if (_len > 0)
        {
            char* _buf_tmp = (char*)_buf;
            for (int ind = _len; ind; ind--)
                _WriteBufferBlockAndInsertNewLine(*_buf_tmp++);
        }
        LeaveCriticalSection(&m_CriticalSection);

        return _len;
    }
    else
    {
        if (m_Buffer != m_BufferBegin && !m_Read)
        {
            DWORD numBytesWritten = NULL;
            WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesWritten, nullptr);
            m_BufferEnd = m_BufferBegin = m_Buffer;
        }

        int ind = NULL;
        char* buf = nullptr;
        if (m_ExecuteAttribute)
        {
            buf = (char*)_buf;
            ind = _len;
        }
        else
        {
            int ind_ = NULL;
            for (buf = (char*)MemoryManager::AllocatorsList[DEFAULT]->Allocate_A(_len * 2, NULL, NULL); ind_ < _len; ++ind_)
            {
                if (_buf[ind_] == '\n')
                {
                    buf[ind++] = '\r';
                    buf[ind] = '\n';
                }
                else
                    buf[ind] = _buf[ind_];

                ++ind;
            }
        }
        DWORD numBytesWritten = NULL;
        WriteFile(m_File, buf, ind, &numBytesWritten, nullptr);
        m_FilePosition += numBytesWritten;

        LeaveCriticalSection(&m_CriticalSection);

        return numBytesWritten;
    }
}

char FileWrapper::ReadBlockDecreasePosition()
{
    EnterCriticalSection(&m_CriticalSection);

    if (m_BufferBegin == m_Buffer)
        ReadBlockFromOffset();
    else
        m_BufferBegin -= 1;

    --m_FilePosition;

    if (*m_BufferBegin == '\n' &&
        !m_ExecuteAttribute)
    {
        if (m_BufferBegin == m_Buffer)
        {
            ReadBlockFromOffset();
            --m_FilePosition;

            LeaveCriticalSection(&m_CriticalSection);
            return true;
        }

        m_BufferBegin -= 1;
        --m_FilePosition;
    }

    LeaveCriticalSection(&m_CriticalSection);
    return true;
}

char FileWrapper::_437790()
{
    if (ReadBlock() == -1)
        return 1;

    ReadBlockDecreasePosition();

    return 0;
}

void FileWrapper::ReadBlockFromOffset()
{
    SetFilePointer(m_File, m_Buffer - m_BufferEnd - 20, NULL, FILE_CURRENT);

    if (m_Read)
    {
        DWORD numBytesRead = NULL;
        ReadFile(m_File, m_Buffer, FILE_BUFFER_SIZE, &numBytesRead, nullptr);
        m_BufferEnd = &m_Buffer[numBytesRead];
        m_BufferBegin = m_Buffer;
    }

    m_BufferBegin = m_Buffer + 19;
}

int FileWrapper::Read(LPVOID _buf, int _numbytestoread)
{
    EnterCriticalSection(&m_CriticalSection);
    if (_numbytestoread <= FILE_BUFFER_SIZE)
    {
        int bytesread = 0;
        if (m_ExecuteAttribute)
        {
            int buffersize = m_BufferEnd - m_BufferBegin;
            if (buffersize >= _numbytestoread)
            {
                memcpy(_buf, m_BufferBegin, _numbytestoread);
                m_FilePosition += _numbytestoread;
                m_BufferBegin = &m_BufferBegin[_numbytestoread];

                LeaveCriticalSection(&m_CriticalSection);
                return _numbytestoread;
            }

            memcpy(_buf, m_BufferBegin, buffersize);
            m_BufferBegin = m_BufferEnd;
            ReadBlock_Internal();

            if (m_BufferBegin == m_BufferEnd)
            {
                m_FilePosition += buffersize;
                LeaveCriticalSection(&m_CriticalSection);

                return buffersize;
            }

            int buffersize_1 = m_BufferEnd - m_BufferBegin;
            if (buffersize_1 >= (_numbytestoread - buffersize))
                buffersize_1 = _numbytestoread - buffersize;

            memcpy((char*)_buf + buffersize, m_BufferBegin, buffersize_1);
            m_BufferBegin = &m_BufferBegin[buffersize_1];
            m_FilePosition += (buffersize_1 + buffersize);
            bytesread = buffersize_1 + buffersize;
        }
        else
        {
            if (_numbytestoread > 0)
            {
                while (true)
                {
                    int readchar = ReadBlock();
                    if (readchar == -1)
                        break;

                    *((char*)_buf + bytesread) = readchar;

                    if (++bytesread >= _numbytestoread)
                    {
                        LeaveCriticalSection(&m_CriticalSection);
                        return bytesread;
                    }
                }
            }
        }

        LeaveCriticalSection(&m_CriticalSection);
        return bytesread;
    }

    if (m_Read)
    {
        if (m_BufferBegin != m_BufferEnd)
            SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, 0, SEEK_CUR);
        m_BufferEnd = m_Buffer;
        m_BufferBegin = m_Buffer;
    }

    DWORD filebytesread = NULL;
    ReadFile(m_File, _buf, _numbytestoread, &filebytesread, nullptr);
    m_FilePosition += filebytesread;

    if (m_ExecuteAttribute)
    {
        LeaveCriticalSection(&m_CriticalSection);
        return filebytesread;
    }

    int bytesread = 0;
    for (unsigned int i = 0; i < filebytesread; ++i, ++bytesread)
    {
        if (!i && *((char*)_buf) == '\n')
        {
            i = 1;
            if (filebytesread <= 1)
                break;
        }

        if (*((char*)_buf + i) == '\r')
        {
            *((char*)_buf + bytesread) = '\n';
            ++i;
        }
        else
            *((char*)_buf + bytesread) = *((char*)_buf + i);
    }

    LeaveCriticalSection(&m_CriticalSection);
    return bytesread;
}

char FileWrapper::WriteFromBufferAndSetToEnd()
{
    EnterCriticalSection(&m_CriticalSection);

    if (m_File)
    {
        if (m_Buffer != m_BufferBegin && !m_Read)
        {
            DWORD numBytesRead = NULL;
            WriteFile(m_File, m_Buffer, m_BufferBegin - m_Buffer, &numBytesRead, nullptr);
            m_BufferEnd = m_Buffer;
            m_BufferBegin = m_Buffer;
        }

        if (m_Read)
        {
            if (m_BufferBegin != m_BufferEnd)
                SetFilePointer(m_File, m_BufferBegin - m_BufferEnd, NULL, FILE_CURRENT);
            m_BufferEnd = m_Buffer;
            m_BufferBegin = m_Buffer;
        }

        m_FilePosition = SetFilePointer(m_File, NULL, NULL, FILE_END);
    }

    LeaveCriticalSection(&m_CriticalSection);

    return true;
}

void File::ReadZipDirectories(const char* fileSystem)
{
    if (!fileSystem || !*fileSystem)
        return;

    int archIndex = 0;
    char zipName[255] = {};

    while (true)
    {
        int nCharIndex = 0;
        do
        {
            char currChar = fileSystem[archIndex];

            if (!currChar || currChar == ' ' || currChar == ',')
                break;

            ++archIndex;
            zipName[nCharIndex++] = currChar;
        } while (nCharIndex < 255);

        zipName[nCharIndex] = 0;

        while (true)
        {
            char currChar = fileSystem[archIndex];

            if (currChar != ' ' && currChar != ',')
                break;

            ++archIndex;
        }

        OpenZip(zipName);

        char zipNameLocalised[32] = {};

        strcpy(zipNameLocalised, Script::LanguageMode.m_Str);
        strcat(zipNameLocalised, "_");
        strcat(zipNameLocalised, zipName);

        OpenZip(zipNameLocalised);

        if (!fileSystem[archIndex])
            break;
    }
}

ULARGE_INTEGER File::GetStorageFreeSpace()
{
    return FileWrapper::GetStorageFreeSpace();
}

bool File::IsDirectoryValid(const char* const path)
{
    String pathStr;
    GetPathFromDirectoryMappings(pathStr, path);

    return FileWrapper::IsDirectoryValid(pathStr.m_Str);
}

bool File::IsFileValid(const char* const file)
{
    String fileStr;
    GetPathFromDirectoryMappings(fileStr, file);
    return FileWrapper::IsFileValid(fileStr.m_Str);
}

bool File::IsFileReadOnly(const char* const file)
{
    String fileStr;
    GetPathFromDirectoryMappings(fileStr, file);

    if (!FindFileEverywhere(fileStr.m_Str))
        return false;

    int zipslot = NULL;
    ZipArch::FileInfo zipFileInfo;
    if (ZipArch::SlotId && ZipArch::FindFile(fileStr.m_Str, zipFileInfo, &zipslot))
        return true;

    return FileWrapper::IsFileReadOnly(fileStr.m_Str);
}

void File::CreateNewDirectory(const char* const dir)
{
    String dirStr;
    GetPathFromDirectoryMappings(dirStr, dir);
    FileWrapper::CreateNewDirectory(dirStr.m_Str);
}

void File::RemoveDirectory_(const char* const dir)
{
    String dirStr;
    GetPathFromDirectoryMappings(dirStr, dir);
    FileWrapper::RemoveDirectory_(dirStr.m_Str);
}

void File::SetFileAttrib(const char* const file, unsigned int attrib, char unk)
{
    FileWrapper::SetFileAttrib(file, attrib, unk);
}

bool File::SearchScriptFile(const char* const searchpath, const char* const scriptfilename, String& zipname)
{
    if (ZipArch::SlotId > 0)
    {
        int slotind;
        ZipArch::FileInfo zipfileinfo;
        char path[512] = {};

        strcpy(path, searchpath);
        strcat(path, scriptfilename);

        if (ZipArch::FindFile(path, zipfileinfo, &slotind))
            return true;
    }

    String mappedpath;
    GetPathFromDirectoryMappings(mappedpath, searchpath);

    return FileWrapper::FindFileRecursive(mappedpath.m_Str, scriptfilename, zipname);
}

bool File::CheckGameFileAttributes(const char* const filename, const FileWrapper::FileAttribute mode)
{
    return FileWrapper::CheckGameFileAttributes(filename, mode);
}

#pragma message(TODO_IMPLEMENTATION)
void File::DeleteAllFilesInFolder(const char* const foldername)
{
    if (FindFileEverywhere(foldername))
    {
        String foldermappedpath;
        GetPathFromDirectoryMappings(foldermappedpath, foldername);
        FileWrapper::DeleteFileFromGameDir(foldermappedpath.m_Str);

        return;
    }
}

bool File::EnumerateFolderFiles(const char* const dir, std::vector<String>& outFilesList)
{
    String dirStr;
    GetPathFromDirectoryMappings(dirStr, dir);
    return FileWrapper::EnumerateFolderFiles(dirStr.m_Str, outFilesList);
}

void File::OpenZip(const char* const zipName)
{
    if (!FileWrapper::IsFileExists(zipName))
        return;

    int slotId = ZipArch::SlotId++;
    LogDump::LogA("Opening zip <%s> into slot %i\n", zipName, slotId);

    // ZipArch::SlotInfo[slotId].field_4 = 15; // TODO: reserve space?
    ZipArch::ZipNames[slotId] = zipName;

    FileWrapper::ZipFilesArray[slotId] = new FileWrapper(zipName, 33, true);
    FileWrapper::ZipFilesArray[slotId]->WriteFromBufferAndSetToEnd();

    if (FileWrapper::ZipFilesArray[slotId]->GetPosition())
    {
        FilesSemaphoreArray[slotId] = CreateSemaphore(NULL, 1, 1, nullptr);
        FileWrapper::ZipFilesArray[slotId]->Seek(FileWrapper::ZipFilesArray[slotId]->GetPosition() - 22);

        unsigned char EndOfCentralDirectoryRecord[20] = {}; // TODO: maybe this can become actual struct from ZIP library.
        bool   ValidZipFile = false;

        FileWrapper::ZipFilesArray[slotId]->Read(EndOfCentralDirectoryRecord, sizeof(EndOfCentralDirectoryRecord));
        if (EndOfCentralDirectoryRecord[0] +
            ((EndOfCentralDirectoryRecord[1] + ((EndOfCentralDirectoryRecord[2] + (EndOfCentralDirectoryRecord[3] << 8)) << 8)) << 8) == FILE_ZIP_MAGIC_HEADER)
            ValidZipFile = true;

        unsigned int FilesInArchive = EndOfCentralDirectoryRecord[10] + (EndOfCentralDirectoryRecord[11] << 8);
        unsigned int CentralDirectorySize = EndOfCentralDirectoryRecord[12] + ((EndOfCentralDirectoryRecord[13] + ((EndOfCentralDirectoryRecord[14] + (EndOfCentralDirectoryRecord[15] << 8)) << 8)) << 8);
        unsigned int CentralDirectoryStartOffset = EndOfCentralDirectoryRecord[16] + ((EndOfCentralDirectoryRecord[17] + ((EndOfCentralDirectoryRecord[18] + (EndOfCentralDirectoryRecord[19] << 8)) << 8)) << 8);

        FileWrapper::ZipFilesArray[slotId]->Seek(CentralDirectoryStartOffset);

        unsigned char* FileHeaderArray = new unsigned char[CentralDirectorySize];
        unsigned char* FileHeaderArrayStartPtr = FileHeaderArray;
        FileWrapper::ZipFilesArray[slotId]->Read(FileHeaderArray, CentralDirectorySize);

        std::map<unsigned int, ZipArch::FileInfo> FilesMap;

        if (FilesInArchive > 0)
        {
            for (unsigned int i = FilesInArchive; i; i--)
            {
                if (ValidZipFile)
                    FileHeaderArray -= 8;

                char   fileNameBuffer[MAX_PATH] = {};
                char* fileNameBufferPtr = &fileNameBuffer[0];
                unsigned int FileSize = FileHeaderArray[24] + ((FileHeaderArray[25] + ((FileHeaderArray[26] + (FileHeaderArray[27] << 8)) << 8)) << 8);
                unsigned int filePathStrlen = FileHeaderArray[28] + (FileHeaderArray[29] << 8);
                unsigned short nextOffset_1 = FileHeaderArray[30] + (FileHeaderArray[31] << 8);
                unsigned short nextOffset_2 = FileHeaderArray[32] + (FileHeaderArray[33] << 8);
                unsigned int nextOffset_3 = FileHeaderArray[42] + ((FileHeaderArray[43] + ((FileHeaderArray[44] + (FileHeaderArray[45] << 8)) << 8)) << 8);

                if (ValidZipFile)
                    FileHeaderArray += 8;

                if (filePathStrlen > NULL)
                {
                    unsigned char* filenameStartOffset = FileHeaderArray + 46;

                    for (unsigned int j = filePathStrlen; j; j--)
                    {
                        if (ValidZipFile)
                            *fileNameBufferPtr++ = (*filenameStartOffset << 2) | (*filenameStartOffset >> 6);
                        else
                            *fileNameBufferPtr++ = *filenameStartOffset;

                        filenameStartOffset++;
                    }
                }

                FileHeaderArray += filePathStrlen + nextOffset_1 + nextOffset_2 + 46;
                unsigned int FileOffset = nextOffset_1 + nextOffset_3 + filePathStrlen + 30;

                String::ConvertBackslashes(fileNameBuffer);
                String::ToLowerCase(fileNameBuffer);

                unsigned int checksum = Utils::CalcCRC32(fileNameBuffer, strlen(fileNameBuffer));
                FilesMap[checksum] = { FileOffset, FileSize };

#if defined INCLUDE_FIXES && defined VERBOSELOG
                // NOTE: this will drastically increase your log size! Use with caution.
                LogDump::LogA("Read \"%s\" (crc: %x, offset: %i, size: %i bytes) from \"%s\"\n", fileNameBuffer, checksum, FileOffset, FileSize, zipName);
#endif
            }
        }

        ZipArch::SlotInfo[slotId] = FilesMap;

        delete[] FileHeaderArrayStartPtr;
    }
}

void FileWrapper::SetWorkingDir(const char* const str)
{
    WorkingDirectory = str;
    WorkingDirectory.ConvertBackslashes();

    if (WorkingDirectory.m_Str[WorkingDirectory.m_Length - 1] != '/')
        WorkingDirectory.Append("/");
}

void FileWrapper::SetGameWorkingDir(const char* const str)
{
    GameWorkingDirectory = str;
    GameWorkingDirectory.ConvertBackslashes();

    if (GameWorkingDirectory.m_Str[GameWorkingDirectory.m_Length - 1] != '/')
        GameWorkingDirectory.Append("/");
}

void FileWrapper::GetWorkingDirRelativePath(String& path)
{
    if (!path.m_Length)
    {
        path = WorkingDirectory;
        return;
    }

    char buffer[1024] = {};
    strcpy(buffer, WorkingDirectory.m_Str);

    if (buffer[WorkingDirectory.m_Length - 1] == '/' && path.m_Str[0] == '/')
        strcat(buffer, (const char*)((int)path.m_Str + 1));
    else
        strcat(buffer, path.m_Str);

    path = buffer;
}

void FileWrapper::GetGameWorkingDirRelativePath(String& path)
{
    if (!path.m_Length)
    {
        path = GameWorkingDirectory;
        return;
    }

    char buffer[1024] = {};
    strcpy(buffer, GameWorkingDirectory.m_Str);

    if (buffer[GameWorkingDirectory.m_Length - 1] == '/' && path.m_Str[0] == '/')
        strcat(buffer, (const char*)((int)path.m_Str + 1));
    else
        strcat(buffer, path.m_Str);

    path = buffer;
}

bool FileWrapper::FindFileRecursive(const char* const dir, const char* const fname, String& outfullpath)
{
    String fpath = dir;
    GetWorkingDirRelativePath(fpath);

    if (!fpath.EndsWith('/'))
    {
        LogDump::LogA("'%s' must end with '/'\n", fpath.m_Str);
        return false;
    }

    if (!IsDirectoryValid(fpath.m_Str))
    {
        LogDump::LogA("'%s' is not a valid directory\n");
        return false;
    }

    char fpathsearch[512] = {};
    strcpy(fpathsearch, fpath.m_Str);
    strcat(fpathsearch, "*.*");

    WIN32_FIND_DATA searchdata;
    HANDLE searchhndl = FindFirstFile(fpathsearch, &searchdata);

    if (searchhndl == INVALID_HANDLE_VALUE)
        return false;

    while (true)
    {
        if (searchdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(searchdata.cFileName, "..") == NULL ||
                strcmp(searchdata.cFileName, ".") == NULL)
            {
                if (!FindNextFile(searchhndl, &searchdata))
                {
                    FindClose(searchhndl);
                    return false;
                }

                continue;
            }

            char incdir[1024] = {};

            strcpy(incdir, dir);
            strcat(incdir, searchdata.cFileName);
            strcat(incdir, "/");

            if (FindFileRecursive(incdir, fname, outfullpath))
            {
                FindClose(searchhndl);
                return true;
            }
        }

        if (_stricmp(searchdata.cFileName, fname) == NULL)
            break;

        if (!FindNextFile(searchhndl, &searchdata))
        {
            FindClose(searchhndl);
            return false;
        }
    }

    outfullpath = dir;
    outfullpath.Append(fname);

    FindClose(searchhndl);
    return true;
}

IFile::~IFile()
{
    MESSAGE_CLASS_DESTROYED(IFile);
}

IFile::IFile()
{
    MESSAGE_CLASS_CREATED(IFile);
}

int IFile::_vsnprintf(IFile* file, const char* format, ...)
{
    char buffer[1024] = {};
    va_list va;

    va_start(va, format);

    int writtensize = vsnprintf(buffer, sizeof(buffer), format, va);
    for (int i = 0; i < writtensize; i++)
        file->_WriteBufferBlockAndInsertNewLine(buffer[i]);

    return writtensize;
}

int IFile::_scanf(IFile* file, const char* format, ...)
{
    va_list va;
    va_start(va, format);

    return file->_scanf_impl((char*)format, (int*)&va);
}

int IFile::WriteFormattedVarlistDataToBuffer(char* _buf, va_list args)
{
    char buffer[1024] = {};
    int writtensize = vsnprintf(buffer, sizeof(buffer), _buf, args);

    for (int i = 0; i < writtensize; i++)
        _WriteBufferBlockAndInsertNewLine(buffer[i]);

    return writtensize;
}

#pragma message(TODO_IMPLEMENTATION)
int IFile::_scanf_impl(char* format, int* outArgs)
{
    return ((int(__thiscall*)(IFile*, char*, int*))0x42F0A0)(this, format, outArgs);
}

const char* IFile::GetFileName() const
{
    return "";
}

SaveFileHelper::~SaveFileHelper()
{
    MESSAGE_CLASS_DESTROYED(SaveFileHelper);

    delete[] m_Buffer;
}

bool SaveFileHelper::WriteBuffers()
{
    return true;
}

char SaveFileHelper::ReadBlock()
{
    if (m_CurrentPos >= m_BufferSize)
        return -1;

    m_CurrentPos++;

    return m_Buffer[m_CurrentPos - 1];
}

int SaveFileHelper::ReadBlockAndGetPosition()
{
    return m_CurrentPos >= m_BufferSize ? -1 : (int)m_Buffer[m_CurrentPos];
}

char SaveFileHelper::_WriteBufferBlockAndInsertNewLine(char _newlinesym)
{
    if (m_CurrentPos + 1 > m_BufferSize)
    {
        m_Buffer = (char*)MemoryManager::Realloc(m_Buffer, m_CurrentPos + 1, false);
        m_BufferSize = m_CurrentPos + 1;
    }

    m_Buffer[m_CurrentPos++] = _newlinesym;

    return true;
}

int SaveFileHelper::Read(void* _buffer, unsigned int _numbytestoread)
{
    unsigned int readbuffsize = m_BufferSize - m_CurrentPos;

    if (_numbytestoread < readbuffsize)
        readbuffsize = _numbytestoread;

    memcpy(_buffer, &m_Buffer[m_CurrentPos], readbuffsize);

    m_CurrentPos += readbuffsize;

    return readbuffsize;
}

int SaveFileHelper::WriteBuffer(const char* _buf)
{
    unsigned int i = 0;

    if (*_buf)
        while (_buf[++i]);

    return WriteBufferWithSize(_buf, i);
}

int SaveFileHelper::WriteBufferWithSize(const char* _buf, int _size)
{
    if (m_CurrentPos + _size > m_BufferSize)
    {
        m_Buffer = (char*)MemoryManager::Realloc(m_Buffer, m_CurrentPos + _size, false);
        m_BufferSize = m_CurrentPos + _size;
    }

    memcpy(&m_Buffer[m_CurrentPos], _buf, _size);

    m_CurrentPos += _size;

    return _size;
}

int SaveFileHelper::Seek(int _pos)
{
    m_CurrentPos = _pos <= m_BufferSize ? _pos : m_BufferSize;

    return NULL;
}

char SaveFileHelper::_WriteBufferAndSetToStart()
{
    m_CurrentPos = NULL;

    return true;
}

char SaveFileHelper::WriteFromBuffer()
{
    m_CurrentPos = m_BufferSize;

    return true;
}

int SaveFileHelper::GetPosition() const
{
    return m_CurrentPos;
}

char SaveFileHelper::ReadBlockDecreasePosition()
{
    if (m_CurrentPos)
        m_CurrentPos--;

    return true;
}

char SaveFileHelper::ReadIfNotEOF()
{
    return m_CurrentPos >= m_BufferSize;
}