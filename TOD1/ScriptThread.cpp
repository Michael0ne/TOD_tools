#include "ScriptThread.h"
#include "Node.h"
#include "Scene.h"

int ScriptThread::CurrentThread;
ScriptThread* ScriptThread::Threads[100];
bool ScriptThread::WarnDelayedException;
int ScriptThread::LatestMethodIndex;
ScriptThread::MethodStruct *ScriptThread::RecentMethodsArray[4];
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
    m_MethodInfo = nullptr;

    return 0;
}

ScriptThread::ScriptThread(Node* node)
{
    MESSAGE_CLASS_CREATED(ScriptThread);

    m_Stack.reserve(9);
    m_CallStack.reserve(9);
    m_ScriptNode = node;
    m_ThreadFlags.SceneTimeSynced = node->m_Flags._29;
    m_MethodInfo = nullptr;

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
        if (m_MethodInfo)
        {
            if (LatestMethodIndex == 4)
            {
                delete m_MethodInfo;
                return;
            }

            RecentMethodsArray[LatestMethodIndex++] = m_MethodInfo;
        }
        m_MethodInfo = nullptr;
    }
}

void ScriptThread::StoreMethodInformation(void(*a1)(ScriptThread*), int a2, void(*a3)(ScriptThread*), Node* a4, Scriptbaked* a5)
{
    if (!m_MethodInfo)
    {
        if (LatestMethodIndex)
            m_MethodInfo = RecentMethodsArray[--LatestMethodIndex];
        else
            m_MethodInfo = new MethodStruct;
    }

    m_MethodInfo->field_0 = a1;
    m_MethodInfo->field_8 = a3;
    m_MethodInfo->field_C = a4;
    m_MethodInfo->field_10 = a5;
    m_MethodInfo->field_4 = a2;
}

#pragma message(TODO_IMPLEMENTATION)
void ScriptThread::Reset()
{
    if (m_CallStack.size())
    {
        m_ThreadFlags.Suspended = false;
        m_ThreadFlags.MarkedForSuspend = false;
    }
}

void ScriptThread::_48E390()
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
            m_ScriptNode->_86B610();
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
            m_ScriptNode->_86B610();
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

#pragma message(TODO_IMPLEMENTATION)
void ScriptThread::_48CE30()
{
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

#pragma message(TODO_IMPLEMENTATION)
void ScriptThread::PushToCallStack(void(*funcPtr)(ScriptThread*), const short argNumber, Node* node, void* dummy)
{
    CallStackElement element;
    element.m_FuncPtr = funcPtr;
    element.m_NodePtr = node;
    element.m_LocalOffset = m_StackSize;
    element.m_Current = NULL;
    element.m_ParameterOffset = m_StackSize - argNumber;
    element.m_OnParameterOffset = -2;
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