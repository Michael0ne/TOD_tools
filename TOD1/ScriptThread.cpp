#include "ScriptThread.h"

int ScriptThread::CurrentThread;
ScriptThread* ScriptThread::Threads[100];

void ScriptThread::DecreaseStateMessageCount()
{
    m_ThreadFlags.m_FlagBits.StateMessageCount--;
}

void ScriptThread::SetScriptNode(Node* scriptnode)
{
    m_ScriptNode = scriptnode;
    m_ThreadFlags.m_FlagBits.HasScriptNode = true;
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