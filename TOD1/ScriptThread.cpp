#include "ScriptThread.h"
#include "Node.h"
#include "Scene.h"

int ScriptThread::CurrentThread;
ScriptThread* ScriptThread::Threads[100];
bool ScriptThread::WarnDelayedException;

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
    m_ThreadFlags._28 = node->m_Flags.m_FlagBits._29;
    m_MethodInfo = nullptr;

    Reset();
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

const int ScriptThread::_48CD00() const
{
    return m_ThreadFlags._28 ? Scene::NextUpdateTime : Scene::GameTimeMs;
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