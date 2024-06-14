#include "ScriptThread.h"
#include "Node.h"
#include "Scene.h"

int ScriptThread::CurrentThread;
ScriptThread* ScriptThread::Threads[100];
bool ScriptThread::WarnDelayedException;
int ScriptThread::CallStackIndex;
ScriptThread::MethodStruct *ScriptThread::CallStack[5];
ScriptThread::NodeScriptDataInfo ScriptThread::ScriptDataCache[6];
int ScriptThread::LatestScriptDataCacheIndex;
int ScriptThread::CurrentLocalOffset;
int ScriptThread::CurrentParameterOffset;
Node* ScriptThread::CurrentScriptNode;

ScriptThread::~ScriptThread()
{
    MESSAGE_CLASS_DESTROYED(ScriptThread);
}

int ScriptThread::CopyTo(void* thread)
{
    memcpy(thread, this, sizeof(ScriptThread));

    m_Stack.clear();
    m_CallStack.clear();
    m_CallStackRef = nullptr;

    return 0;
}

ScriptThread::ScriptThread(Node* node)
{
    MESSAGE_CLASS_CREATED(ScriptThread);

    m_Stack.reserve(9);
    m_CallStack.reserve(9);
    m_ScriptNode = node;
    m_ThreadFlags.SceneTimeSynced = node->m_Flags._29;
    m_CallStackRef = nullptr;

    Reset();
}

void ScriptThread::PopFromCallstack()
{
    m_CallStack.pop_back();
    const size_t callstackSize = m_CallStack.size();
    m_ThreadFlags.HasScriptNode = true;

    if (callstackSize)
    {
        m_CurrentStackElement = &m_CallStack[m_CallStack.size()];
    }
    else
    {
        m_CurrentStackElement = nullptr;
        if (m_CallStackRef)
        {
            if (CallStackIndex == 4)
            {
                delete m_CallStackRef;
                m_CallStackRef = nullptr;
                return;
            }

            CallStack[CallStackIndex++] = m_CallStackRef;
        }
        m_CallStackRef = nullptr;
    }
}

const int ScriptThread::RestoreFromBuffer(int* buffer)
{
    struct BufferStruct
    {
        uint32_t    _f0;
        uint32_t    CallStackSize;
        uint32_t    _f8;
        uint32_t    _fC;
        uint32_t    _f10;
    } *sBuffer = reinterpret_cast<BufferStruct*>(buffer);

    Reset();

    /*v3 = buffer[1];
    if (v3 > this->m_CallStack.m_CurrentIndex && v3 > this->m_CallStack.m_Capacity)
    {
        this->m_CallStack.m_Capacity = v3;
        sub_48EFE0(&this->m_CallStack);
    }

    this->m_CallStack.m_CurrentIndex = v3;
    m_CurrentIndex = this->m_CallStack.m_CurrentIndex;*/
    m_CallStack.resize(sBuffer->CallStackSize);

    /*v5 = 0;
    v6 = buffer + 2;
    m_Script = this->m_ScriptNode->m_ScriptEntity->m_Script;*/
    auto entityScript = m_ScriptNode->m_ScriptEntity->Script;

    //if (m_CurrentIndex > 0)
    if (m_CallStack.size() > 0)
    {
        /*v34 = 0;
        do
        {
            v7 = &this->m_CallStack.m_Elements->m_Elements[v34];
            v7->m_NodePtr = this->m_ScriptNode;
            ParameterProcedure = (void(__cdecl*)(ScriptThread*))ScriptBaked::GetParameterProcedure(m_Script, *v6);
            v9 = (unsigned int*)(v6 + 1);
            v7->m_FuncPtr = ParameterProcedure;
            v10 = *v9++;
            v7->m_Current = v10;
            LOWORD(ParameterProcedure) = *(_WORD*)v9++;
            v7->m_ParameterOffset = (unsigned __int16)ParameterProcedure;
            v7->m_LocalOffset = *(_WORD*)v9;
            v6 = (int*)(v9 + 1);
            v7->m_OnParameterOffset = -2;
            ++v5;
            ++v34;
        } while (v5 < this->m_CallStack.m_CurrentIndex);*/

        for (uint32_t i = 0; i < m_CallStack.size(); i++)
        {
            auto& callStackElementRef = m_CallStack[i];

            callStackElementRef.m_NodePtr = m_ScriptNode;

        }
    }

    v11 = *v6;
    v12 = 4 * this->m_CallStack.m_CurrentIndex + 2;
    v36 = *v6;
    if (*v6 > this->Stack.m_CurrentIndex && v11 > this->Stack.m_Capacity)
    {
        this->Stack.m_Capacity = v11;
        sub_921A90((ListWithAllocator*)&this->Stack);
        v11 = v36;
    }
    this->Stack.m_CurrentIndex = v11;

    v13 = this->m_CallStack.m_CurrentIndex;
    v14 = (const char*)(v6 + 1);
    v15 = v12 + 1;
    Flags = 0x4205;
    Elements = (DataType**)a1;
    v39 = v15;
    list.Elements = (int*)a1;
    list.Size = 0;
    list.Capacity = 100;
    list.Flags = list.Flags & 0xFFF80000 | 0x14205;
    v37 = 0;

    if (v13 > 0)
    {
        v35 = 0;
        do
        {
            m_Elements = this->m_CallStack.m_Elements;
            list.Size = 0;
            ScriptBaked::GetMethodParams(m_Script, m_Elements->m_Elements[v35].m_FuncPtr, &list);
            Elements = (DataType**)list.Elements;
            for (i = 0; i < list.Size; v39 = v15)
            {
                v20 = Elements[i]->lpVtbl->CopyAndAllocate(
                    Elements[i],
                    v14,
                    (char*)&this->Stack.Elements->Contents[(unsigned __int16)this->ThreadFlags]);
                Elements = (DataType**)list.Elements;
                v21 = v39;
                LOWORD(this->ThreadFlags) += *(_WORD*)(list.Elements[i] + 24);
                v15 = v20 + v21;
                v14 += 4 * v20;
                ++i;
            }
            v22 = ++v37 < this->m_CallStack.m_CurrentIndex;
            ++v35;
        } while (v22);
        Flags = list.Flags;
    }

    v23 = *(_DWORD*)v14;
    v24 = (int*)(v14 + 4);
    this->m_SleepUntil = v23;
    v25 = *v24++;
    this->m_WaitForFrame = v25;
    v26 = *v24++;
    v27 = (this->ThreadFlags ^ ((v26 != 0) << 24)) & 0x1000000 ^ this->ThreadFlags;
    this->ThreadFlags = v27;
    v28 = *v24++;
    v29 = v27 ^ (v27 ^ ((v28 != 0) << 25)) & 0x2000000;
    this->ThreadFlags = v29;
    this->ThreadFlags = (*v24 << 30) | v29 & 0x3FFFFFFF;
    BYTE2(this->ThreadFlags) = 0;
    v30 = v15 + 6;
    this->ThreadFlags = this->ThreadFlags & 0xF3FFFFFF | ((v24[1] != 0) << 26);
    v31 = this->m_CallStack.m_CurrentIndex;
    if (v31)
        v32 = &this->m_CallStack.m_Elements->m_Elements[v31 - 1];
    else
        v32 = 0;
    this->m_CurrentStackElement = v32;

    if (Elements)
    {
        if ((Flags & 0x100) != 0)
            Allocators::FreePointerUnaligned(Elements);
    }

    return v30;
}

void ScriptThread::StoreMethodInformation(void(*a1)(ScriptThread*), int a2, void(*a3)(ScriptThread*), Node* a4, Scriptbaked* a5)
{
    if (!m_CallStackRef)
    {
        if (CallStackIndex)
            m_CallStackRef = CallStack[--CallStackIndex];
        else
            m_CallStackRef = new MethodStruct;
    }

    m_CallStackRef->field_0 = a1;
    m_CallStackRef->field_8 = a3;
    m_CallStackRef->field_C = a4;
    m_CallStackRef->field_10 = a5;
    m_CallStackRef->field_4 = a2;
}

void ScriptThread::Reset()
{
    if (m_CallStack.size())
    {
        m_ThreadFlags.Suspended = false;
        m_ThreadFlags.MarkedForSuspend = false;
        _48E8A0();
    }

    if (m_CallStackRef)
    {
        if (CallStackIndex == 4)
            delete m_CallStackRef;
        else
            CallStack[CallStackIndex++] = m_CallStackRef;
    }

    m_CallStackRef = nullptr;
    m_Stack.clear();
    m_CallStack.clear();

    m_StackSize = 0;
    m_ThreadFlags.Sleeping = false;
    m_ThreadFlags.SleepRealTime = false;
    m_StateMessageCount = 0;
    m_ThreadFlags.Suspended = false;
    m_ThreadFlags.MarkedForSuspend = false;
    m_ThreadFlags.Priority = true;

    m_CurrentStackElement = nullptr;
    m_WaitForFrame = -1;
    m_SleepUntil = 0;

    if (m_ScriptNode)
        m_ThreadFlags.HasScriptNode = true;
}

void ScriptThread::RunActiveThreadFunction()
{
    if (m_ThreadFlags.Suspended)
        return;

    if (m_ThreadFlags.Sleeping)
    {
        int nextWakeTimeMs;
        if (m_ThreadFlags.SleepRealTime)
            nextWakeTimeMs = Scene::RealTimeMs;
        else
            if (m_ThreadFlags.SceneTimeSynced)
                nextWakeTimeMs = Scene::NextUpdateTime;
            else
                nextWakeTimeMs = Scene::GameTimeMs;

        if (m_SleepUntil <= nextWakeTimeMs)
        {
            m_CurrentStackElement = &m_CallStack[m_CallStack.size() - 1];
            m_ScriptNode->StoreScriptData();
            m_WaitForFrame = -1;
            m_ThreadFlags.Sleeping = false;
            Threads[CurrentThread++] = this;
            m_CurrentStackElement->m_FuncPtr(this);

            while (m_CallStack.size())
            {
                if (m_ThreadFlags.Sleeping || m_WaitForFrame != -1)
                    break;

                m_CurrentStackElement->m_FuncPtr(this);
            }

            --CurrentThread;
        }
    }
    else
    {
        int nextWakeFrame = m_ThreadFlags.SceneTimeSynced ? Scene::TotalFrames : Scene::NewFrameNumber;
        if (m_WaitForFrame == -1 || nextWakeFrame)
        {
            m_CurrentStackElement = &m_CallStack[m_CallStack.size() - 1];
            m_ScriptNode->StoreScriptData();
            m_WaitForFrame = -1;
            m_ThreadFlags.Sleeping = false;
            Threads[CurrentThread++] = this;
            m_CurrentStackElement->m_FuncPtr(this);

            while (m_CallStack.size())
            {
                if (m_ThreadFlags.Sleeping || m_WaitForFrame != -1)
                    break;

                m_CurrentStackElement->m_FuncPtr(this);
            }

            --CurrentThread;
        }
    }
}

void ScriptThread::SetSleepTime(const float sleepfor, const bool sleepRealTime)
{
    m_ThreadFlags.SleepRealTime = sleepRealTime;
    const int sceneTime = sleepRealTime ? Scene::RealTimeMs : GetSceneTime();

    if ((sleepfor * 1000.0f) >= 0.f)
        m_SleepUntil = sceneTime - (int)(sleepfor * -1000.0f);
    else
        m_SleepUntil = -1;
}

int ScriptThread::EnqueueScriptMethod()
{
    if (!m_CallStackRef)
        return 0;

    m_CurrentStackElement = m_CallStack.size() ? &m_CallStack[m_CallStack.size() - 1] : nullptr;

    if (!m_CallStackRef->field_4 && m_CallStackRef->field_0 == m_CurrentStackElement->m_FuncPtr)
        m_CallStackRef->field_4 = 1;

    if (m_CallStackRef->field_4 == 1)
    {
        if (m_CallStackRef->field_8)
        {
            PushToCallStack(m_CallStackRef->field_8, 0, m_CallStackRef->field_C, m_CallStackRef->field_10);
            m_WaitForFrame = -1;
            m_ThreadFlags.Sleeping = false;

            if (CallStackIndex == 4)
                delete m_CallStackRef;
            else
                CallStack[CallStackIndex++] = m_CallStackRef;

            m_CallStackRef = nullptr;
            return 1;
        }
        else
        {
            if (CallStackIndex == 4)
                delete m_CallStackRef;
            else
                CallStack[CallStackIndex++] = m_CallStackRef;

            m_CallStackRef = nullptr;
            return 0;
        }
    }
    else
    {
        if (m_CallStackRef->field_4 == -1 && m_CallStackRef->field_0 == m_CurrentStackElement->m_FuncPtr)
            m_CallStackRef->field_4 = 1;

        m_ThreadFlags.Sleeping = false;
        m_WaitForFrame = -1;
        return -1;
    }
}

void ScriptThread::_48E8A0()
{
    if (!m_CallStack.size())
        return;

    StoreMethodInformation(nullptr, -1, nullptr, nullptr, nullptr);
    EnqueueScriptMethod();
    RunActiveThreadFunction();

    if (m_StackSize != m_CallStack[0].m_LocalOffset)
        MemoryManager::AllocatorsList[DEFAULT]->CallMethodAtOffset20();
}

void ScriptThread::ResetCallStack()
{
    if (LatestScriptDataCacheIndex)
    {
        do
        {
            Defragmentator* defrag = ScriptDataCache[LatestScriptDataCacheIndex].m_Defragmentator;
            LatestScriptDataCacheIndex--;

            unsigned int chunkind = ((unsigned __int64)(0x2AAAAAAB * ((char*)ScriptDataCache[LatestScriptDataCacheIndex].m_ScriptData - (char*)defrag->m_AllocatedSpace)) >> 32) >> 1;
            defrag->Deallocate(chunkind + (chunkind >> 31));

            ScriptDataCache[LatestScriptDataCacheIndex].m_ScriptData = nullptr;
        } while (LatestScriptDataCacheIndex);
    }

    do
    {
        delete CallStack[CallStackIndex--];
    } while (CallStackIndex);
}

const int ScriptThread::GetSceneTime() const
{
    return m_ThreadFlags.SceneTimeSynced ? Scene::NextUpdateTime : Scene::GameTimeMs;
}

void ScriptThread::DecreaseStateMessageCount()
{
    m_StateMessageCount--;
}

void ScriptThread::SetScriptNode(Node* scriptnode)
{
    m_ScriptNode = scriptnode;
    m_ThreadFlags.HasScriptNode = true;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptThread::DumpState(String& outString) const
{
    char buf[256] = {};
    sprintf(buf, " callstack size %d\n", m_CallStack.size());
    outString = buf;
}

void ScriptThread::AdjustStackSize(const unsigned int size)
{
    m_StackSize += size;
    const unsigned short stacksize = m_StackSize;

    m_Stack.resize(stacksize);

    if (size > 0)
        memset(m_Stack[stacksize - size].m_Contents, NULL, size * 4);   //  NOTE: not sure about this.
}

void ScriptThread::PushToCallStack(void(*funcPtr)(ScriptThread*), const short argNumber, Node* node, void* dummy)
{
    CallStackElement element;
    element.m_FuncPtr = funcPtr;
    element.m_NodePtr = node;
    element.m_LocalOffset = m_StackSize;
    element.m_Current = NULL;
    element.m_ParameterOffset = m_StackSize - argNumber;
    element.m_OnParameterOffset = -2;

    m_CallStack.push_back(element);
    m_CurrentStackElement = &m_CallStack[m_CallStack.size() - 1];
}

void ScriptThread::AdjustAndPopStack()
{
    if (m_StackSize > m_Stack.size())
        m_Stack.resize(m_StackSize);

    m_CurrentStackElement = m_CallStack.size() ? &m_CallStack[m_CallStack.size() - 1] : nullptr;
}

int ScriptThread::WriteScriptInformation(int* outInformation) const
{
    int* outInformationBasePtr = outInformation;
    Scriptbaked* script = m_ScriptNode->m_ScriptEntity->Script;
    outInformation[1] = m_CallStack.size();
    int* stackInfoPtr = &outInformation[2];

    for (size_t i = 0; i < m_CallStack.size(); ++i)
    {
        const CallStackElement& currentCallStackElement = m_CallStack[i];
        *stackInfoPtr++ = script->GetParameterProcedureIndex(currentCallStackElement.m_FuncPtr);
        *stackInfoPtr++ = currentCallStackElement.m_Current;
        *stackInfoPtr++ = currentCallStackElement.m_ParameterOffset;
        *stackInfoPtr++ = currentCallStackElement.m_LocalOffset;
    }

    outInformation = stackInfoPtr;
    *outInformation = m_StackSize;

    using DataTypesList = std::vector<DataType*>;

    //std::vector<DataType*> argumentsList(100);
    DataTypesList argumentsList(100);
    unsigned int stackOffset = 0;
    unsigned int dataOffset = (m_CallStack.size() * 10) + 3;
    if (m_CallStack.size())
    {
        for (size_t i = 0; i < m_CallStack.size(); ++i)
        {
            script->GetMethodParams(m_CallStack[i].m_FuncPtr, argumentsList);
            if (argumentsList.size())
            {
                for (size_t j = 0; j < argumentsList.size(); ++j)
                {
                    int newTypeSize = argumentsList[j]->CopyNoAllocate((const char*)m_Stack[stackOffset].m_Contents, (char*)++outInformation);
                    outInformation += newTypeSize;
                    stackOffset += argumentsList[j]->Size;
                    dataOffset += newTypeSize;
                }
            }
        }
    }

    *outInformation++ = m_SleepUntil;
    *outInformation++ = m_WaitForFrame;
    *outInformation++ = m_ThreadFlags.SleepRealTime;
    *outInformation++ = m_ThreadFlags.Sleeping;
    *outInformation++ = m_ThreadFlags.Priority;
    *outInformation++ = m_ThreadFlags.Suspended;
    dataOffset += 6;
    *outInformationBasePtr = dataOffset;

    return dataOffset;
}

int ScriptThread::PopFromStackAndSetCurrent(const int stackIndex)
{
    if (m_CallStack.size())
    {
        m_CurrentStackElement = &m_CallStack[m_CallStack.size() - 1];
        m_CurrentStackElement->m_Current = stackIndex;
    }
    else
    {
        m_CurrentStackElement = nullptr;
        //  NOTE: null pointer exception will be raised here.
#ifndef INCLUDE_FIXES
        m_CurrentStackElement->m_Current = stackIndex;
#endif
    }

    return EnqueueScriptMethod();
}

void ScriptThread::RunRestAndReset()
{
    while (m_CallStackRef)
    {
        m_ThreadFlags.Sleeping = false;
        m_WaitForFrame = -1;
        RunActiveThreadFunction();
    }

    if (m_ThreadFlags.MarkedForSuspend)
    {
        m_ThreadFlags.MarkedForSuspend = false;
        m_ThreadFlags.Suspended = true;
    }

    if (m_CallStack.size() > 1)
        return;

    m_ScriptNode->SetScriptData(nullptr, nullptr);

    if (LatestScriptDataCacheIndex == 6)
    {
        m_ScriptNode->m_Defragmentator->Deallocate(((char*)m_ScriptNode->m_ScriptData - (char*)m_ScriptNode->m_Defragmentator->m_AllocatedSpace) / 12);
    }
    else
    {
        m_ScriptNode->m_ScriptData->m_ScriptThread->Reset();
        m_ScriptNode->m_ScriptData->m_ScriptThread->m_ScriptNode = nullptr;
        ScriptDataCache[LatestScriptDataCacheIndex].m_ScriptData = m_ScriptNode->m_ScriptData;
        ScriptDataCache[LatestScriptDataCacheIndex].m_Defragmentator = m_ScriptNode->m_Defragmentator;
        LatestScriptDataCacheIndex++;
    }
}

const bool ScriptThread::IsReferenced(const bool fixDangling) const
{
    if (!m_CallStack.size())
        return true;

    size_t i = 0;
    size_t argOffset = 0;
    bool referenceFound = true;
    while (true)
    {
        size_t paramInd = 0;
        std::vector<DataType*> params(100);
        if (m_CallStack[i].m_OnParameterOffset < 0)
        {
            m_CallStack[i].m_NodePtr->m_ScriptEntity->Script->GetMethodParams(m_CallStack[i].m_FuncPtr, params);
            if (params.size())
            {
                do
                {
                    if (!params[paramInd]->IsReferenced(m_Stack[argOffset].m_Contents, fixDangling))
                    {
                        LogDump::LogA("param/local %d, in state/method at 0x%x ", paramInd, m_CallStack[i].m_FuncPtr);
                        referenceFound = false;
                    }

                    argOffset += params[paramInd]->Size;
                } while (paramInd < params.size());
            }
        }

        if (++i >= m_CallStack.size())
            break;
    }

    return referenceFound;
}

bool ScriptThread::IsThreadExists(const ScriptThread* scriptthread)
{
    if (CurrentThread <= NULL)
        return false;

    int i = 0;
    while (Threads[i] != scriptthread)
        if (++i >= CurrentThread)
            return false;

    return true;
}

int ScriptThread::GetCurrentThreadIndex()
{
    return CurrentThread;
}

IScriptThread::~IScriptThread()
{
    MESSAGE_CLASS_DESTROYED(IScriptThread);
}

int IScriptThread::CopyTo(void*)
{
    return 0;
}