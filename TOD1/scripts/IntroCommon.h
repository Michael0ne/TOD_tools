class IntroCommon : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("IntroCommon", "Node", true, false);

		inst->AddMember(GetScriptIdByFullName("SCREEN_SIZE_X:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("SCREEN_SIZE_Y:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("SCREEN_HALF_SIZE_X:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("SCREEN_HALF_SIZE_Y:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("BORDER_PERCENTAGE:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("BORDER_X:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("BORDER_Y:number"), nullptr, 0);
		inst->AddMember(GetScriptIdByFullName("MAIN_NODE:Entity"), nullptr, 0);

		inst->CalculateSize();

		return inst;
	};
};

IntroCommon::Create();