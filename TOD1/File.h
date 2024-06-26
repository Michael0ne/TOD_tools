#pragma once
#include "StringsPool.h"
#include <vector>

#define FILE_MAX_ZIP_FILES 8
#define FILE_BUFFER_SIZE (1024 * 128)
#define FILE_ZIP_MAGIC_HEADER 0x16ED5B50

class FileBufferImpl
{
public:
    friend class FileBuffer;

    enum FileAttribute
    {
        READONLY = 1,
        HIDDEN = 2,
        ARCHIVE = 4
    };

protected:
    mutable CRITICAL_SECTION m_CriticalSection;
    HANDLE   m_File;
    String   m_WorkingDir;
    String   m_GameWorkingDir;
    int    m_DesiredAccess_1;
    int    m_FilePosition;
    int    m_CreationDisposition;
    int    m_DesiredAccess_2;
    bool   m_CreateIfNotFound;
    bool   m_ExecuteAttribute;
    bool   m_Read;
    char* m_Buffer;
    char* m_BufferBegin;
    char* m_BufferEnd;

protected:
    FileBufferImpl(const char* _filename, int _desiredaccess, bool _createifnotfound); // @438180
    ~FileBufferImpl(); // @436F40

public:
    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void* operator new[](size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
        void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }
    void operator delete[](void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    void   SetFileHandle(HANDLE _hnd); // @436D90
    void   ReleaseFileHandle(); // @436DC0
    void   FlushAndClose(HANDLE); // @436FF0
    HANDLE   CreateFile_(); // @4378D0

protected:
    int    GetPosition() const; // @436DF0
    void   SetExecuteAttr(unsigned char); // @436E10
    void   _436E30(); // @436E30
    void   _436E70(); // @436E70
    void   ReadBlock_Internal(); // @436EA0
    void   ReadBlockFromOffset(); // @436EE0
    int    ReadBlock(); // @4370C0
    char   _WriteBufferBlockAndInsertNewLine(char); // @437170
    int    Read(LPVOID, int); // @437230
    int    WriteBuffer(const char*, int); // @437430
    int    Seek(int); // @437560
    char   WriteFromBufferAndSetToEnd(); // @437690
    char   ReadBlockDecreasePosition(); // @437720
    char   _437790(); // @437790
    void   GetLastErrorMessage(char*); // @437820

    static void  MoveFileWithReplace(const char* const oldname, const char* const newname); // @437E10
    static bool  IsFileExists(const char* const file, const int); // @437E90
    static bool  IsFileValid(const char* const file); // @437F70
    static bool  IsDirectoryValid(const char* const dir); // @437FD0
    static bool  IsFileReadOnly(const char* const file); // @4380C0
    static void  CreateNewDirectory(const char* const dir); // @438120
    static void  RemoveDirectory_(const char* const dir); // @4389D0
    static bool  EnumerateFolderFiles(const char* const dir, std::vector<String>& outFilesList); // @439420
    static void  SetFileAttrib(const char* const file, unsigned int attrib, char unk); // @438310
    static bool  CheckGameFileAttributes(const char* const filename, const FileAttribute mode); // @4383B0

    static FileBufferImpl* ZipFilesArray[FILE_MAX_ZIP_FILES]; // @A35DB8 // NOTE: named so because only zip files accessed through it.
    static bool  GameDiscFound; // @A35E68

public:
    static ULARGE_INTEGER GetStorageFreeSpace(); // @4377B0
    static bool  IsStorageFull(); // @4377E0
    static void  DeleteFileFromGameDir(const char* const filename); // @437DC0
    static void  FindGameDir(); // @439230
    static void  SetWorkingDir(const char* const str); // @438460
    static void  SetGameWorkingDir(const char* const str); // @438560
    static void  GetWorkingDirRelativePath(String& path); // @437A70
    static void  GetGameWorkingDirRelativePath(String& path); // @437B80
    static bool  FindFileRecursive(const char* const dir, const char* const fname, String& outfullpath); // @438660

    static String WorkingDirectory; // @A08FA0
    static String GameWorkingDirectory; // @A08FB0
};

ASSERT_CLASS_SIZE(FileBufferImpl, 92);

class IFile
{
public:
    virtual ~IFile();
    virtual bool WriteBuffers() = 0;
    virtual int  _vsnprintf(IFile*, const char* format, ...);
    virtual int  _scanf(IFile*, const char* format, ...);
    virtual int  WriteFormattedVarlistDataToBuffer(char* _buf, va_list args);
    virtual int  _scanf_impl(char* format, int* outArgs);
    virtual char ReadBlock() = 0;
    virtual int  ReadBlockAndGetPosition() = 0;
    virtual char _WriteBufferBlockAndInsertNewLine(char _newlinesym) = 0;
    virtual int  Read(void* _buffer, unsigned int _numbytestoread) = 0;
    virtual int  WriteBuffer(const char* _buf) = 0;
    virtual int  WriteBufferWithSize(const char* _buf, int _size) = 0;
    virtual int  Seek(int _pos) = 0;
    virtual char _WriteBufferAndSetToStart() = 0;
    virtual char WriteFromBuffer() = 0;
    virtual int  GetPosition() const = 0;
    virtual char ReadBlockDecreasePosition() = 0;
    virtual char ReadIfNotEOF() = 0;
    virtual const char* GetFileName() const;

    IFile(); // NOTE: no actual constructor since it's interface.
};

//  NOTE: actual class path is "libs/adapter/filebuffer.h".
class FileBuffer : public IFile
{
    friend class FileBufferImpl;
protected:
    String   m_FileName;
public:
    FileBufferImpl* m_FileHandle;
protected:
    unsigned char m_FileReadAttribute;
    unsigned char m_ReadFromZip;
    int    m_SeekPosition;
    int    m_OffsetInZip;
    int    m_SizeInZip;
    int    m_ZipSlot;
    bool   m_ExecuteAttribute;
    FileBufferImpl* m_ZipFileHandle;

private:
    static time_t GetFileTimestamp_Impl(const char* path); // @437C90

public:
    virtual ~FileBuffer(); // @417E20
    virtual bool WriteBuffers(); // @417D20
    virtual int  _vsnprintf(FileBuffer*, const char* _format, ...); // @42EFC0
    virtual int  _scanf(FileBuffer*, const char* _format, ...); // @417960
    virtual int  WriteFormattedVarlistDataToBuffer(char* _buf, va_list args); // @42F040
    virtual int  _scanf_impl(char* format, int* outArgs); // @42F0A0
    virtual char ReadBlock(); // @417980
    virtual int  ReadBlockAndGetPosition(); // @419BD0
    virtual char _WriteBufferBlockAndInsertNewLine(char _newlinesym); // @419BF0 // NOTE: this is a thunk function.
    virtual int  Read(void* _buffer, unsigned int _numbytestoread); // @417A30
    virtual int  WriteBuffer(const char* _buf); // @419C10 // NOTE: this is a thunk function.
    virtual int  WriteBufferWithSize(const char* _buf, int _size); // @419C00 // NOTE: this is a thunk function.
    virtual int  Seek(int _pos); // @417B90
    virtual char _WriteBufferAndSetToStart(); // @417BB0
    virtual char WriteFromBuffer(); // @417BD0
    virtual int  GetPosition() const; // @417BF0
    virtual char ReadBlockDecreasePosition(); // @417C50
    virtual char ReadIfNotEOF(); // @417850
    virtual const char* GetFileName() const; // @419C40

    FileBuffer(const char* _filename, int _desiredaccess, bool _createifnotfound); // @418E30

    void* operator new(size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void* operator new[](size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
        void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }
    void operator delete[](void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, false);
        ptr = nullptr;
    }

    unsigned int    GetSize(); // @417C10
    bool            IsFileOpen() const; // @417CF0
    char            ReadString(String& outStr); // @4180C0
    void            SetPosAligned(unsigned char alignind); // @417DF0
    int             ReadIntLittleToBigEndian(); // @417870
    short           ReadShortLittleToBigEndian(); // @4178C0
    HANDLE          GetFileHandle(); // @417FD0

    static void     AddDirectoryMappingsListEntry(const char* str1, const char* str2); // @418F90
    static String&  GetPathFromDirectoryMappings(String& outStr, const char* path); // @41A360
    static void     ExtractFilePath(const char* inFilePath, char* outDirectory, char* outFileName, char* outFileExtension); // @4088E0
    static void     ExtractFileExtension(String& outStr, const char* const path);   //  @409420
    static String*  ExtractFileDir(String& outStr, const char* path); // @409360
    static String*  ExtractFileName(String& outStr, const char* path); // @4093B0
    static void     FindDirectoryMappedFileAndDelete(const char* const filename); // @418810
    static bool     FindFileEverywhere(const char* path, const int); // @4182A0
    static time_t   GetFileTimestamp(const char* filename); // @418460
    static void     OpenZip(const char* const zipName); // @419100
    static void     ReadZipDirectories(const char* fileSystem); // @419550
    static ULARGE_INTEGER GetStorageFreeSpace(); // @417D40
    static bool     IsDirectoryValid(const char* const path); // @418410
    static bool     IsFileValid(const char* const file); // @418B00
    static bool     IsFileReadOnly(const char* const file); // @418550
    static bool     EnumerateFolderFiles(const char* const dir, std::vector<String>& outFilesList); // @418670
    static void     CreateNewDirectory(const char* const dir); // @4186F0
    static void     RemoveDirectory_(const char* const dir); // @418740
    static void     SetFileAttrib(const char* const file, unsigned int attrib, char unk); // @417D50
    static bool     SearchScriptFile(const char* const searchpath, const char* const scriptfilename, String& zipname); // @418210
    static bool     CheckGameFileAttributes(const char* const filename, const FileBufferImpl::FileAttribute mode); // @417D60
    static void     DeleteAllFilesInFolder(const char* const foldername);   //  @418B50

    static unsigned int FilesOpen; // @A35DD8
    static HANDLE       FilesSemaphoreArray[FILE_MAX_ZIP_FILES]; // @A35D98
    static std::vector<StringTuple> DirectoryMappingsList; // @A35DE4
    static unsigned int AlignmentArray[3]; // @9B37E4
};

ASSERT_CLASS_SIZE(FileBuffer, 52);

class SaveFileHelper : public IFile
{
private:
    char           *m_Buffer;
    unsigned int    m_CurrentPos;
    unsigned int    m_BufferSize;

public:
    inline SaveFileHelper()
    {
        MESSAGE_CLASS_CREATED(SaveFileHelper);

        m_Buffer = nullptr;
        m_CurrentPos = NULL;
        m_BufferSize = NULL;
    }

    virtual ~SaveFileHelper();  //  @873A70
    virtual bool WriteBuffers() override;    //  @9293E0
    virtual char ReadBlock() override;   //  @4221A0
    virtual int  ReadBlockAndGetPosition() override; //  @4221C0
    virtual char _WriteBufferBlockAndInsertNewLine(char _newlinesym) override;   //  @4221E0
    virtual int  Read(void* _buffer, unsigned int _numbytestoread) override;  //  @4222B0
    virtual int  WriteBuffer(const char* _buf) override; //  @873AA0
    virtual int  WriteBufferWithSize(const char* _buf, int _size) override;  //  @422220
    virtual int  Seek(int _pos) override;    //  @422280
    virtual char _WriteBufferAndSetToStart() override;   //  @873AD0
    virtual char WriteFromBuffer() override; //  @873AE0
    virtual int  GetPosition() const override; //  @419C40
    virtual char ReadBlockDecreasePosition() override;   //  @4222A0
    virtual char ReadIfNotEOF() override;    //  @873AF0
};

ASSERT_CLASS_SIZE(SaveFileHelper, 16);