#pragma once
#include "Globals.h"
#include "Node.h"

#define MEMCARD_DEFAULT_SAVE_DIR "savegames"

class MemoryCards : public Node
{
protected:
    Utils::UniqueId m_MemCardsUniqueId[4];
    String          m_Ps2SlesLicense;
    String          m_Ps2SlusLicense;
    String          m_GameName;
    unsigned int    m_SaveFileSize;

    String&         MakeSaveSlotString(String& outStr, unsigned int slotind) const; // @926540

    static void     Register(); // @9281B0

private:
    static MemoryCards* Create(AllocatorIndex allocInd); // @928F90

    const char*     GetGamename() const; // @928790
    void            SetGamename(const char* const gamename); // @926510
    const char*     GetPs2SlesLicense() const; // @8A6CB0
    void            SetPs2SlesLicense(const char* const sleslicense); // @9265C0
    const char*     GetPs2SlusLicense() const; // @4A66A0
    void            SetPs2SlusLicense(const char* const sluslicense); // @926640
    unsigned int    GetSaveFileSize() const; // @9262A0
    void            SetSaveFileSize(unsigned int savesize); // @9262C0
    void            IsPresent(int* args) const; // @9287E0
    void            CreateSavePoint(unsigned int memcardind, unsigned int slotind, EntityType* textbox, unsigned int, const char* const savedirectory, Node* summarynode) const; // @926C70
    void            RestoreSavePoint(unsigned int memcardind, unsigned int slotind, Node* summarynode); // @926CE0
    void            LoadSavePointSummary(unsigned int memcardind, unsigned int slotind, Node* summarynode) const; // @926D40
    void            DeleteSavePoint(int* args); // @929260
    bool            DeleteSavePoint_Impl(unsigned int memcardind, unsigned int slotind); // @926B60
    void            SavePointOperationError(int* args) const; // @9287A0
    bool            SavePointExists_Impl(unsigned int memcardind, unsigned int slotind) const; // @926DA0
    void            SavePointExists(int* args) const; // @9291D0
    void            PrepareCardForSavegames(int* args); // @9292A0
    bool            PrepareCardForSavegames_Impl(unsigned int memcardind); // @9271F0
    void            UnPrepareCardForSavegames(int* args); // @9292C0
    bool            UnPrepareCardForSavegames_Impl(unsigned int memcardind); // @927060

    unsigned int    GetSavePointSize(unsigned int memcardind, unsigned int slotind) const; // @926F00
    void            HasCardChanged(int* args) const; // @87AA10
    unsigned int    GetCardFreeSpace_Impl(unsigned int memcardind) const; // @928FD0
    void            GetCardFreeSpace(int* args) const;  //  @929030
    void            IsFormatted(int* args) const; // @9289A0
    void            IsCardPrepared(int* args) const;    //  @929280
    bool            IsPrepared(unsigned int memcardind) const; // @926F90
    void            FormatCard(int* args) const;    //  @9289C0
    void            UnformatCard(int* args) const;  //  @9287C0

public:
    MemoryCards(); // @9263B0
    virtual ~MemoryCards(); //  @928A10

    unsigned int    GetLastModifiedTimeAsNumber(unsigned int memcardind, unsigned int slotind) const; // @928090
};

extern EntityType* tMemoryCards; // @A3E178

ASSERT_CLASS_SIZE(MemoryCards, 164);