#pragma once
#include "EntityType.h"
#include "Defragmentator.h"

class MemoryCards;
class SavePoint;

class Entity
{
public:
    EntityType     *m_ScriptEntity;
    unsigned char   field_8[10];
    short           m_Order;
    union
    {
        struct
        {
            unsigned    HasPosition : 1;
            unsigned    HasQuadTree : 1;
            unsigned    HasFragment : 1;
            unsigned    _3 : 5;
            unsigned    Id : 20;
            unsigned    BlockId : 3;
        };
    }               m_Id;
    int            *m_Parameters; // NOTE: an array of properties values.
    Defragmentator *m_Defragmentator;
    
    struct EntityScriptData
    {
        int         field_0;    //  NOTE: flags of some sort.
        ScriptThread*m_ScriptThread;
        void        (*m_Handler)();
        int         field_C;
        int        *field_10;
        int         field_14;
        int         field_18;
        int        *field_1C;
        int        *field_20;
    }              *field_20;

    int             SaveScriptDataToFile_Impl(MemoryCards* memcard, int memcardindex, int savegameslot, const char* a4); // @86B650
    unsigned char   LoadScriptDataFromFile_Impl(EntityType*, int, int); // @86B8B0

public:
    virtual        ~Entity(); // @86C010
    virtual void    Destroy(); // @86C010

    Entity(); // @86A1D0

    void* operator new (size_t size)
    {
        return MemoryManager::AllocatorsList[DEFAULT]->Allocate(size, NULL, NULL);
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

    void            SetScript(EntityType* script); // @869E20

    const int       SaveScriptData(SavePoint * savefilehelper); // @86B110

    static void     Register(); // @86BC70
    static Entity*  Create(AllocatorIndex); // @86C130
};

extern EntityType* tEntity; // @A3CEE0

ASSERT_CLASS_SIZE(Entity, 36);