#pragma once
#include "ScriptDatabase.h"
#include "Defragmentator.h"

#include <array>

class Node;
class Scriptbaked;

class IScriptThread
{
public:
    virtual         ~IScriptThread();   //  @48CC00
    virtual int     CopyTo(void*);   //  @47BFD0
};

//  NOTE: possible name 'ScriptInterface'.
class ScriptThread : public IScriptThread
{
    friend class Entity;
    friend class Node;
    friend class Scriptbaked;

    struct StackElement
    {
        unsigned int   field_0;
        int           *m_Contents;
        unsigned int   field_8;
    };

    struct CallStackElement
    {
        Node                   *m_NodePtr;
        void                    (*m_FuncPtr)(ScriptThread*);
        unsigned int            m_Current;
        unsigned short          m_ParameterOffset;
        unsigned short          m_LocalOffset;
        short                   m_OnParameterOffset;
    };

    struct CallStackElements
    {
        int                     field_0;
        CallStackElement        *m_Elements;
    };

    struct MethodStruct
    {
        void                    (*field_0)(ScriptThread*);
        int                     field_4;
        void                    (*field_8)(ScriptThread*);
        Node                   *field_C;
        Scriptbaked            *field_10;
    };

    struct NodeScriptDataInfo
    {
        Defragmentator             *m_Defragmentator;
        EntityScriptData           *m_ScriptData;
    };

protected:
    Defragmentator             *m_StackListAllocator;    //  NOTE: allocator for vector below. Same goes for callstack.
    std::vector<StackElement>   m_Stack;

    Defragmentator             *m_CallStackListAllocator;
    std::vector<CallStackElement>m_CallStack;

    int                         m_SleepUntil;
    int                         m_WaitForFrame;
#ifdef INCLUDE_FIXES
    //  NOTE: this way it looks better & clear, but original code access these from 'threadflags' member.
    unsigned short              m_StackSize;
    unsigned char               m_StateMessageCount;
    union
    {
        struct
        {
            unsigned char       Sleeping : 1;
            unsigned char       SleepRealTime : 1;
            unsigned char       Suspended : 1;
            unsigned char       MarkedForSuspend : 1;
            unsigned char       SceneTimeSynced : 1;
            unsigned char       HasScriptNode : 1;
            unsigned char       Priority : 2;
        };
    }                           m_ThreadFlags;
#else
    union
    {
        struct
        {
            unsigned short      StackSize;
            unsigned char       StateMessageCount : 8;
            unsigned char       Sleeping : 1;   //  24
            unsigned char       SleepRealTime : 1;  //  25
            unsigned char       Suspended : 1;  //  26
            unsigned char       MarkedForSuspend : 1;   //  27
            unsigned char       SceneTimeSynced : 1;    //  28
            unsigned char       HasScriptNode : 1;  //  29
            unsigned char       Priority : 2;   //  30
        };
    }                           m_ThreadFlags;
#endif
    Node                       *m_ScriptNode;
    MethodStruct               *m_CallStackRef;
    CallStackElement           *m_CurrentStackElement;

public:
    virtual                     ~ScriptThread(); // @48ED10
    virtual int                 CopyTo(void*) override;   //  @48F220

    ScriptThread(Node* node); // @48EC70

    void                        PopFromCallstack();  //  @48E470
    const int                   RestoreFromBuffer(int* buffer); //  @48E9E0
    void                        StoreMethodInformation(void (*a1)(ScriptThread*), int a2, void (*a3)(ScriptThread*), Node* a4, Scriptbaked* a5);   //  @48CD70
    void                        Reset();    //  @48E930
    void                        RunActiveThreadFunction();  //  @48E390
    void                        SetSleepTime(const float sleepfor, const bool sleepRealTime);  //  @48F2E0
    const int                   GetSceneTime() const;    //  @48CD00
    void                        DecreaseStateMessageCount();    //  @48CD50
    void                        SetScriptNode(Node* scriptnode);    //  @48CCD0
    void                        DumpState(String& outString) const; // @48D690
    void                        AdjustStackSize(const unsigned int size);   //  @48E6A0
    void                        PushToCallStack( void (*funcPtr)(ScriptThread*), const short argNumber, Node* node, void* dummy);   //  @48E700
    void                        AdjustAndPopStack();    //  @48E630
    int                         WriteScriptInformation(int* outInformation) const;  //  @48CF00
    int                         PopFromStackAndSetCurrent(const int stackIndex);  //  @48E8F0
    void                        RunRestAndReset();  //  @48EDA0
    const bool                  IsReferenced(const bool fixDangling) const; //  @48DFF0

private:
    int                         EnqueueScriptMethod();  //  @48E770
    void                        _48E8A0();  //  @48E8A0

public:
    static void                 ResetCallStack();  //  @48CE30
    static bool                 IsThreadExists(const ScriptThread* scriptthread);
    static int                  GetCurrentThreadIndex();    //  @48CC40

    static int                  CurrentThread;  //  @A3B758
    static ScriptThread*        Threads[100];   //  @A3B5C8
    static bool                 WarnDelayedException;   //  @A3B770
    static int                  CallStackIndex;    //  @A3B76C
    static NodeScriptDataInfo   ScriptDataCache[6];    //  @A3B774
    static MethodStruct        *CallStack[5];  //  @A3B5B4
    static int                  LatestScriptDataCacheIndex; //  @A3B768
    static int                  CurrentLocalOffset; //  @A3B760
    static int                  CurrentParameterOffset; //  @A3B764
    static Node*                CurrentScriptNode;  //  @A3B75C //  NOTE: used as 'this' for TNT scripts.
};

static void DummyFunction(ScriptThread* pThread)
{
    return;
}

static std::array<void (*)(ScriptThread*), 1> StaticScriptFunctions = {
    DummyFunction
};

#define SCRIPT_ENUM_VALUE v 

ASSERT_CLASS_SIZE(ScriptThread, 68);