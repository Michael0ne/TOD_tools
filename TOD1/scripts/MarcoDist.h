class MarcoDist : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("MarcoDist", "Node", false, false);
		
		inst->AddMember(GetScriptIdByFullName("MarcoDist:number"), "10.000000", NULL);
		
		inst->CalculateSize();
		return inst;
	};
};

MarcoDist::Create();