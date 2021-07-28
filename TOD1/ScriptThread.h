#pragma once
#include "ScriptDatabase.h"
#include "Defragmentator.h"

class IScriptThread
{
public:
    virtual						~IScriptThread();
    virtual int					stub1(void*);
};

class ScriptThread : public IScriptThread
{
    struct ThreadList_1
    {
        unsigned int			field_0;
        int*					field_4;	//	NOTE: pointer to another list. Contents vary.
        unsigned int			field_8;
    };

    struct CallStackElement
    {
        class Node* m_NodePtr;
        void					(*m_FuncPtr)(ScriptThread*);
        unsigned int			m_Current;
        unsigned short          m_ParameterOffset;
        unsigned short          m_LocalOffset;
        short                   m_OnParameterOffset;
    };

    struct ThreadCallStack
    {
        unsigned int            field_0;
        CallStackElement       *m_Addresses;
        int                    *field_8;
    };

    struct MethodStruct
    {
        void					(*field_0)(ScriptThread*);
        int						field_4;
        void					(*field_8)(ScriptThread*);
        Node*					field_C;
        GlobalScript*			field_10;
        unsigned int			field_14;
    };
protected:
    Defragmentator*				m_Defragmentator;
    std::vector<ThreadList_1>	m_List_1;	//	NOTE: this is the list with a Defragmentator allocator attached to it.
    Defragmentator*				m_Defragmentator_1;
    std::vector<ThreadCallStack>m_CallStack;
    int							m_SleepUntil;
    int							m_WaitForFrame;
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
            unsigned char       _28 : 1;
            unsigned char       HasScriptNode : 1;
            unsigned char       Priority : 1;
            unsigned char       _31 : 1;
        }                       m_FlagBits;
        unsigned int            m_ThreadFlags;
    }                           m_ThreadFlags;
    class Node*					m_ScriptNode;
    MethodStruct*				field_3C;
    CallStackElement*			m_CurrentStackElement;
public:
    virtual						~ScriptThread();	//	@48ED10
    virtual int					stub1(void*);

    ScriptThread(class Node*);	//	@48EC70

    void                        Reset();    //  @48E930
    void                        _48E390();  //  @48E390 //  NOTE: 'Execute'?
    void                        _48F2E0();  //  @48F2E0 //  NOTE: 'SetSleepTime'?
    void                        DecreaseStateMessageCount();    //  @48CD50
    void                        SetScriptNode(Node* scriptnode);    //  @48CCD0
    void						DumpState(String&);	//	@48D690

    static bool                 IsThreadExists(const ScriptThread* scriptthread);
    static int                  GetCurrentThreadIndex();    //  @48CC40

    static int                  CurrentThread;  //  @A3B758
    static ScriptThread*        Threads[100];   //  @A3B5C8
};