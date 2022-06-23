class Tutorial_pc_index : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Tutorial_pc_index", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("SignIndex:integer"), "-1", 0);
		inst->AddMember(GetScriptIdByFullName("is_alt_text:truth"), "false", 0);

		inst->CalculateSize();

		return inst;
	};
};

Tutorial_pc_index::Create();