#pragma once
#include "Timer.h"
#include "Sprite.h"
#include "Texture.h"
#include <vector>

#pragma pack(4)
struct ProgressStateInfo
{
    String          m_Status;
    UINT64          m_Time;
    char            field_18;

    ProgressStateInfo()
        : m_Status(), m_Time(NULL), field_18(NULL)
    {};
    ProgressStateInfo(LPCSTR state, UINT64 time, char unk)
        : m_Status(state), m_Time(time), field_18(unk)
    {};
};

#pragma pack(4)
class ProgressBase
{
protected:
    int             field_4;
    std::vector<ProgressStateInfo> m_LoadBarPhases;
    UINT64          m_TotalTimeToLoad;
    unsigned int    field_20;
    int             field_24;
    int             field_28;
    float           field_2C;
    float           field_30;
    int             field_34;
    UINT64          field_38;
    UINT64          m_TimeStart;

public:
    virtual ~ProgressBase(); // @40EC50
    virtual void stub1(const char*); // @40E7D0 NOTE: not used anywhere and actual function has nothing in it.
    virtual void UpdateProgress(float time, bool unk); // @40E7E0

    ProgressBase(UINT64 timeStart); // @40E900
};

ASSERT_CLASS_SIZE(ProgressBase, 72);

class Progress : ProgressBase
{
protected:
    UINT64          field_48;
    UINT64          field_50;
    Sprite         *m_LoadScreenSprite;
    Texture        *m_LoadBarTexture;
    Vector2<float>  m_LoadScreenSpriteResolution;
    bool            m_Enabled;
    int             field_6C;

public:
    virtual ~Progress(); // @40E8A0
    virtual void UpdateProgress(float time, bool unk) override; // @87B830

    Progress();  // @87B720
    
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

    void   Complete(); // @40E790
    void   _40E7F0(unsigned int, UINT64); // @40E7F0
    void   Reset(); // @40E8C0
    void   AddLoadbarPhase(const char* phaseName, UINT64 timeToLoad, bool); // @40E970
    void   Enable(); // @87B5F0
    void   Disable(); // @87B690
    void   SetLoadScreenSprite(Sprite*); // @87B710
    void   UpdateProgressTime(unsigned int, UINT64); // @85D590

    static UINT64 StartTime; // @A3D7D8
};

extern Progress*  g_Progress; // @A3D7D0
extern ProgressBase* g_ProgressBase; // @A35B88

ASSERT_CLASS_SIZE(Progress, 112);