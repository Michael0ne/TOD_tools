#pragma once
#include "EntityType.h"
#include "Defragmentator.h"

class MemoryCards;
class SavePoint;

class Entity
{
public:
    struct EntityId
    {
        unsigned    HasPosition : 1;    //  0
        unsigned    HasQuadTree : 1;    //  1
        unsigned    HasFragment : 1;    //  2
        unsigned    _3 : 5;             //  3 - 7
        unsigned    Id : 15;            //  8 - 22
        unsigned    BlockId : 3;        //  23 - 25
    };

    EntityType     *m_ScriptEntity;
    char            m_ScriptSlots[5];       //  NOTE: 5 slots with 8 bytes each - 40 possible script arguments. If bit slot is set, then entity script has this argument used.
    char            m_PropertiesSlots[5];   //  NOTE: same as above, but for properties.
    short           m_Order;
    EntityId        m_Id;
    uint32_t       *m_Parameters; // NOTE: an array of properties values.
    Defragmentator *m_Defragmentator;

    EntityScriptData   *m_ScriptData;

    int             SaveScriptDataToFile_Impl(MemoryCards* memcard, int memcardindex, int savegameslot, const char* a4); // @86B650
    unsigned char   LoadScriptDataFromFile_Impl(MemoryCards* memcard, const int memoryCardIndex, const int saveSlotIndex); // @86B8B0

public:
    Entity(); // @86A1D0

    virtual        ~Entity(); // @86A500
    virtual void    Destroy(); // @86C010

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, __FILE__, __LINE__);
    }
    void operator delete(void* ptr)
    {
        if (ptr)
            MemoryManager::ReleaseMemory(ptr, 0);
        ptr = nullptr;
    }

    int             GetId() const; // @489770

    int             GetScriptPriority() const; // @86C100
    void            SetScriptPriority(const unsigned char priority); // @4A0C40

    void            GetPropertyId(int* args) const; //  @86A110
    void            HasProperty(int* args) const;   //  @86A0E0
    void            HasPropertyId(int* args) const; //  @86A130

    void            SaveScriptDataToFile(int* params); // @86BBC0
    void            LoadScriptDataFromFile(int* params); // @86BC20
    int             ReadScriptDataFromSavePoint(SavePoint* sp, int* const outsize); //  @86B1F0

    void            SetScript(EntityType* script); // @869E20

    const int       SaveScriptDataToSavePoint(SavePoint * savefilehelper); // @86B110
    void            ClearPropertiesSlots(); //  @86A190
    void            ClearScriptsSlots();    //  @86A150

    static void     Register(); // @86BC70
    static Entity*  Create(AllocatorIndex); // @86C130
};

extern EntityType* tEntity; // @A3CEE0

ASSERT_CLASS_SIZE(Entity, 36);