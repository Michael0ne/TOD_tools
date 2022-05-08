class IntroStringDB : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("IntroStringDB", "Node", false, false);
		
		inst->AddMember(GetScriptIdByFullName("PS2_nomemorycard1:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("PS2_nomemorycard2:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("PS2_nomemorycard3:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("PS2_nospace1:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("PS2_nospace2:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("PS2_nospace3:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("select:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("startmessage:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("startmessage_PC:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("startmessage_XBOX:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Xbox_nomemory1:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Xbox_nomemory2:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Xbox_nomemory3:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("xbox_continue:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("xbox_freespace:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("loadmessage:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("DiskErrorMessage_PS2:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("DiskErrorMessage_XBOX:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("NoCtrl_PS2:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("NoCtrl_XBOX:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("License:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("NoCtrl_PS2_NTSC:integer"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	};
};

IntroStringDB::Create();