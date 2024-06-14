class AI_track : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("AI_track", "Node", false, false);

		inst->field_60 = _493C40;

		inst->AddMember(GetScriptIdByFullName("AI_Track:Entity"), NULL, NULL);
		inst->AddMember(GetScriptIdByFullName("dummy:truth"), NULL, NULL);
		inst->AddMember(GetScriptIdByFullName("Loop_track:truth"), NULL, NULL);

		inst->AddMethod(487, _493C40, _493CC0);
		inst->AddLocal(_493C40, tEntity);

		inst->CalculateSize();
		return inst;
	};

	static void _493C40(ScriptThread* pThread)
	{
		if (!pThread->m_CurrentStackElement->m_Current)
		{
			pThread->AdjustStackSize(1);

			pThread->m_CurrentStackElement->m_Current = 1;
			pThread->m_ThreadFlags.SleepRealTime = 1;
			pThread->m_ThreadFlags.Suspended = false;
			pThread->GetSceneTime();	//	Useless.
			pThread->m_SleepUntil = -1;

			return;
		}

		if (pThread->m_CurrentStackElement->m_Current - 1 > 1)
		{
			pThread->m_StackSize--;
			pThread->PopFromCallstack();

			return;
		}

		if (!pThread->m_CallStackRef)
		{
			pThread->m_CurrentStackElement->m_Current = 1;
			pThread->m_ThreadFlags.SleepRealTime = 1;
			pThread->m_ThreadFlags.Suspended = false;
			pThread->GetSceneTime();	//	Useless.
			pThread->m_SleepUntil = -1;

			return;
		}

		const auto a = pThread->PopFromStackAndSetCurrent(2);
		if (a == -1)
		{
			pThread->m_StackSize--;
			pThread->PopFromCallstack();

			return;
		}

		if (a != 1)
		{
			pThread->m_CurrentStackElement->m_Current = 1;
			pThread->m_ThreadFlags.SleepRealTime = 1;
			pThread->m_ThreadFlags.Suspended = false;
			pThread->GetSceneTime();	//	Useless.
			pThread->m_SleepUntil = -1;

			return;
		}
	}

	static void _493CC0(ScriptThread* pThread, void* pArgs)
	{
		pThread->DecreaseStateMessageCount();
	}
};

AI_track::Create();