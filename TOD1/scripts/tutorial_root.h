class tutorial_root : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("tutorial_root", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("TutorialEntryNum:integer"), "0", 0);
		inst->AddMember(GetScriptIdByFullName("CaptionIndex:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("DescriptionIndex:integer"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("Completed:truth"), "false", 0);

		inst->CalculateSize();

		return inst;
	};
};

tutorial_root::Create();