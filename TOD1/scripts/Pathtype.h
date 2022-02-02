class Pathtype : public GlobalScript
{
public:
	static GlobalScript* Create()
	{
		static GlobalScript* inst = new GlobalScript("Pathtype", "Node", false, false);

		inst->AddStructElement(GetScriptIdByFullName("PathType:integer"), "0", NULL);

		inst->CalculateSize();
		return inst;
	};
};

Pathtype::Create();