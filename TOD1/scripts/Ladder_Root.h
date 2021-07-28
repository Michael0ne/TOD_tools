class Ladder_Root : public GlobalScript
{
public:
	static GlobalScript* Create()
	{
		static GlobalScript* inst = new GlobalScript("Ladder_Root", "Group", false, false);

		inst->AddStructElement(GetScriptIdByFullName("top_step:vector"), nullptr, NULL);
		inst->AddStructElement(GetScriptIdByFullName("bottom_step_y:number"), nullptr, NULL);
		inst->AddStructElement(GetScriptIdByFullName("Ladder_pivot:vector"), nullptr, NULL);
		inst->AddStructElement(GetScriptIdByFullName("Ladder_Step_Length:number"), nullptr, NULL);

		inst->CalculateSize();
		return inst;
	}
};

Ladder_Root::Create();