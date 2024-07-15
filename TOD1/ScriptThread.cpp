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
    Reset();

    m_CallStack.resize(buffer[1]);

    auto entityScript = m_ScriptNode->m_ScriptEntity->Script;
    uint32_t* buffDup = (uint32_t*)buffer + 2;

    if (m_CallStack.size() > 0)
    {
        for (uint32_t i = 0; i < m_CallStack.size(); i++)
        {
            auto& callStackElementRef = m_CallStack[i];

            callStackElementRef.m_NodePtr = m_ScriptNode;
            callStackElementRef.m_FuncPtr = entityScript->GetParameterProcedure(buffDup[0]);
            callStackElementRef.m_Current = buffDup[1];
            callStackElementRef.m_ParameterOffset = (uint16_t)(buffDup[2] & 0x00FF);
            callStackElementRef.m_LocalOffset = (uint16_t)( (buffDup[2] & 0xFF00) >> 2 );
            callStackElementRef.m_OnParameterOffset = -2;

            buffDup = buffDup + 3;
        }
    }

    const auto stackSize = buffDup[0];
    m_Stack.resize(stackSize);

    std::vector<DataType*> tempMethodParamsList{};
    buffDup += 1;
    const auto maxCallStackSize = 4 * m_CallStack.size() + 2;
    auto newCallStackSize = maxCallStackSize;

    if (m_CallStack.size() > 0)
    {

        for (uint32_t i = 0; i < m_CallStack.size(); i++)
        {
            tempMethodParamsList.clear();

            entityScript->GetMethodParams(m_CallStack[i].m_FuncPtr, tempMethodParamsList);

            for (uint32_t j = 0; j < tempMethodParamsList.size(); j++)
            {
                const auto allocatedSize = tempMethodParamsList[j]->CopyAndAllocate((const char*)buffDup, (char*)m_Stack[m_StackSize].m_Contents);
                newCallStackSize += allocatedSize;
                buffDup += allocatedSize;
                m_StackSize += tempMethodParamsList[j]->GetSize(nullptr, nullptr);
            }
        }
    }

    m_SleepUntil = buffDup[0];
    m_WaitForFrame = buffDup[1];

    m_ThreadFlags.Sleeping = buffDup[2];
    m_ThreadFlags.SleepRealTime = buffDup[3];
    m_ThreadFlags.Priority = buffDup[4];
    m_ThreadFlags.Suspended = buffDup[5];
    m_ThreadFlags.MarkedForSuspend = false;
    m_StateMessageCount = 0;

    m_CurrentStackElement = m_CallStack.size() ? &m_CallStack[m_CallStack.size() - 1] : nullptr;

    return *(buffDup + 6);
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