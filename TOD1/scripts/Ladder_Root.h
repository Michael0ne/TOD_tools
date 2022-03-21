class Ladder_Root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Ladder_Root", "Group", false, false);

		inst->AddMember(GetScriptIdByFullName("top_step:vector"), nullptr, NULL);
		inst->AddMember(GetScriptIdByFullName("bottom_step_y:number"), nullptr, NULL);
		inst->AddMember(GetScriptIdByFullName("Ladder_pivot:vector"), nullptr, NULL);
		inst->AddMember(GetScriptIdByFullName("Ladder_Step_Length:number"), nullptr, NULL);

		inst->CalculateSize();
		return inst;
	}
};

Ladder_Root::Create();