class Gotopath_point : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Gotopath_point", "Node", false, false);

		inst->AddMember(GetScriptIdByFullName("maxlookahead:number"), "2.000000", NULL);
		inst->AddMember(GetScriptIdByFullName("movetype:integer"), "2", 0);

		inst->CalculateSize();
		return inst;
	};
};

Gotopath_point::Create();