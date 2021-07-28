class MarcoDist : public GlobalScript
{
public:
	static GlobalScript* Create()
	{
		static GlobalScript* inst = new GlobalScript("MarcoDist", "Node", false, false);
		
		inst->AddStructElement(GetScriptIdByFullName("MarcoDist:number"), "10.000000", NULL);
		
		inst->CalculateSize();
		return inst;
	};
};

MarcoDist::Create();