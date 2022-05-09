#pragma once
#include "BestFitAllocator.h"

class ScriptThread;

class DefragmentatorBase
{
public:
    struct Defragmentator_Space
    {
        union
        {
            struct
            {
                unsigned    Occupied : 1;
                unsigned    _1 : 1;
                unsigned    Alignment : 8;
                unsigned    Size : 20;
                unsigned    _30 : 1;
                unsigned    _31 : 1;
            };

            unsigned int    FlagsDw;
        }               m_Flags;
        ScriptThread   *m_ScriptThreadPtr;
        int             field_8;
    };

public:
    Allocator              *m_Allocator;
    Allocator              *m_DefaultSelectedAllocator;
    unsigned int            m_Size;
    Defragmentator_Space   *m_AllocatedSpace;
    int                     field_14;   //  NOTE: same as below, can't decide with name right now.
    int                     m_LatestUsedIndex;
    int                     m_ChunksAvailable;
    char                    field_20;
    int                     m_LastDefragmentationTime;
    int                     m_SpilledAllocs;    //  NOTE: in case we don't have enough space already available, fallback to default allocator and record this happened.
    char                    field_2C;
    int                     field_30;

private:
    void    MarkSpaceAsOccupied(int chunkind); // @47BC70
    void    MarkSpaceAsUnoccupied(int chunkind); // @47BCD0
    void    FreeChunk(int chunkind); // @47BD40

public:
    DefragmentatorBase(BestFitAllocator* allocator, char a2, int size); // @47BBD0
    virtual ~DefragmentatorBase(); // @47BDD0

    virtual int   Allocate(int size, int, int, int, int alignment, char a6); // @47BE60
    virtual int   Reallocate(int chunkind, int a2, const char* const a3, int a4, unsigned int newsize); // @47BF20
    virtual char  CanExpand(const int chunkind, const int newsize); // @47BD20
    virtual void  Deallocate(int chunkind); // @47BDB0
    virtual int   CheckFlag(int chunkind); // @4783F0   //  NOTE: not used, could be a profiler leftover.
    virtual void  stub6();
    virtual void  stub7();  //  NOTE: not used, could be a profiler leftover.
};

ASSERT_CLASS_SIZE(DefragmentatorBase, 52);

class Defragmentator : public DefragmentatorBase
{
    void            Defragment();   //  @47B710
    void            DefragmentMemory(); //  @47B4C0

public:
    inline Defragmentator(BestFitAllocator* bestfitallocator, char unk1, int size) : DefragmentatorBase(bestfitallocator, 1, 10000) // NOTE: inlined @478630
    {
        m_SpilledAllocs = NULL;
        field_2C = NULL;
    }

    virtual int   Allocate(int size, int a2, int a3, int a4, int alignment, char a6) override; // @47AE80
    virtual int   Reallocate(int chunkind, int a2, const char* const a3, int a4, unsigned int newsize) override; // @47AEF0
    virtual void  Deallocate(int chunkind) override; // @47AFE0
    virtual void  stub7() override; //  @47AE70 //  NOTE: not used, could be a profiler leftover.

    void            DefragmentIfNecessary();    //  @47B780
    int             FindScriptThreadSpaceIndex(ScriptThread* scriptThread) const;   //  @47BC10
};

ASSERT_CLASS_SIZE(Defragmentator, 52);
ASSERT_CLASS_SIZE(Defragmentator::Defragmentator_Space, 12);