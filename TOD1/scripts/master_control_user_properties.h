class master_control_user_properties : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_user_properties", "Node", false, false);
		
		inst->AddMember(GetScriptIdByFullName("UseForceLOD:truth"), "true", 0);
		inst->AddMember(GetScriptIdByFullName("ForceLODLevel:number"), "0.400000", 0);
		inst->AddMember(GetScriptIdByFullName("ForceLODFade:number"), "0.200000", 0);

		inst->CalculateSize();

		return inst;
	};
};

master_control_user_properties::Create();